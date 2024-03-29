#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <map>
#include <math.h>
#include <fstream>
#include <filesystem>
#include <utility>
#include <algorithm>
#include <yaml-cpp/yaml.h>

#include "../include/environment.h"
#include "../include/body.h"
#include "../include/particle.h"
#include "../include/octree.h"


namespace fs = std::filesystem;
float G = 6.6743e-11;
#include "../include/statistics.h"

char* REPOPATH_ = std::getenv("HOOTSIM_PATH");
std::string REPOPATH = REPOPATH_ == nullptr ? "." : std::string(REPOPATH_);


// Load config file to a hashmap. These files are of the form (key: param, value: the distribution parameters).
std::map<std::string, std::map<std::string, std::string>> loadConfig(const std::string& fileName) {

    std::map<std::string, std::map<std::string, std::string>> configMap;  // a 2D hashmap of the configuration
    std::string fullPath = std::string(REPOPATH) + "/configs/" + fileName;  // the full path to the configfile

    try {

        // Get file and parse like a map
        YAML::Node config = YAML::LoadFile(fullPath);
        if (config.IsMap()) {

            // Fill in 2D hashmap
            for (const auto& element : config) {
                std::string outerKey = element.first.as<std::string>();
                const YAML::Node& innerNode = element.second;

                if (innerNode.IsMap()) { // If there is a 2nd dim for the specific element
                    std::map<std::string, std::string> innerMap;
                    for (const auto& innerElement : innerNode) {
                        std::string innerKey = innerElement.first.as<std::string>();
                        std::string innerValue = innerElement.second.as<std::string>();
                        innerMap[innerKey] = innerValue;
                    }
                    configMap[outerKey] = innerMap;
                }
            }
        }
    } catch (const YAML::BadFile& e) {  // fail to open
        std::cerr << "Failed to open YAML file: " << fullPath << std::endl;
        throw;
    } catch (const YAML::Exception& e) {  // issue parsing
        std::cerr << "YAML parsing error: " << e.what() << std::endl;
        throw;
    }

    // Check if the map contains the key "dist" is assigned for each parameter
    for (auto const& [property, propertyDist] : configMap) {
        if (property != "global") {
            if (propertyDist.find("dist") == propertyDist.end()) {
                throw std::runtime_error("Key 'dist' not found in configuration for property " + property + ".");
            }
        }
    }

    return configMap;
}


// Get the largest number from a vector of filenames
inline int getLargestLabelNumber(const std::vector<std::string>& filenames, const std::string logFilePrefix = "run") {
    int maxNumber = -1;

    for (const std::string& filename : filenames) {
        size_t pos = filename.find(logFilePrefix);
        if (pos != std::string::npos) {
            // Assuming "run" is followed immediately by the number and then ".csv"
            size_t start = pos + 3; // Start of the number (after "run")
            size_t end = filename.rfind(".csv"); // Find the start of ".csv"

            if (end != std::string::npos && end > start) {
                std::string numberPart = filename.substr(start, end - start);
                try {
                    int number = std::stoi(numberPart);
                    maxNumber = std::max(maxNumber, number);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << " for " << filename << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << " for " << filename << std::endl;
                }
            }
        }
    }

    return maxNumber;
}

float getEuclidianDistance(std::array<float, 3> coords1, std::array<float, 3> coords2) {
    return sqrt(pow(coords1[0] - coords2[0], 2) + pow(coords1[1] - coords2[1], 2) + pow(coords1[2] - coords2[2], 2));
}

std::array<float, 2> defaultXCoords = {0, 0};
std::array<float, 2> defaultYCoords = {0, 0};
std::array<float, 2> defaultZCoords = {0, 0};

template <typename T>
GravitationalEnvironment<T>::GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log, std::string logFilePrefix, std::string forceAlgorithm)
    : particlePtrs(particlePtrs), log(log), time(0), nParticles(particlePtrs.size()), envOctree(defaultXCoords, defaultYCoords, defaultZCoords, true) {  
    // Determine which algorithm to use
    if (forceAlgorithm == "pair-wise") {
        getForces = std::bind(&GravitationalEnvironment::getForcesPairWise, this, std::placeholders::_1);
    } else {
        getForces = std::bind(&GravitationalEnvironment::getForcesBarnesHut, this, std::placeholders::_1);
    }

    // Create a log file if we want one
    if (log == true) {
        
        // Get a vector of the filenames in the data directory
        std::vector<std::string> lastLogFileNames;
        const char* repoPath = std::getenv("HOOTSIM_PATH");
        std::string dataPath = repoPath == nullptr ? "./data" : std::string(repoPath) + "/data";
        if (!fs::exists(dataPath)) {
            fs::create_directory(dataPath);
            std::cout << "data directory created successfully.\n";
        } else {
            std::cout << "data directory already exists.\n";
        }
        for (const auto& entry : fs::directory_iterator(dataPath)) {
            if (fs::is_regular_file(entry.status())) {
                lastLogFileNames.push_back(entry.path().filename().string());
            }
        }

        // Get the largest log file number and create new log file
        int lastLogNum = getLargestLabelNumber(lastLogFileNames, logFilePrefix);
        logFileName = dataPath + "/" + logFilePrefix + std::to_string(lastLogNum + 1) + ".csv";
        }
    }

// Constructor for config files
template <typename T>
GravitationalEnvironment<T>::GravitationalEnvironment(const std::string configFileName, const bool log, std::string logFilePrefix, std::string forceAlgorithm)
    : log(log), time(0), envOctree(defaultXCoords, defaultYCoords, defaultZCoords, true) {
    // Determine which algorithm to use
    if (forceAlgorithm == "pair-wise") {
        getForces = std::bind(&GravitationalEnvironment::getForcesPairWise, this, std::placeholders::_1);
    } else {
        getForces = std::bind(&GravitationalEnvironment::getForcesBarnesHut, this, std::placeholders::_1);
    }

    // Get particles
    loadParticlesFromConfig(configFileName);

    // Declare the number of particles
    nParticles = particlePtrs.size();

    // Create a log file if we want one
    if (log == true) {
        
        // Get a vector of the filenames in the data directory
        std::vector<std::string> lastLogFileNames;
        std::string dataPath = REPOPATH + "/data";
        for (const auto& entry : fs::directory_iterator(dataPath)) {
            if (fs::is_regular_file(entry.status())) {
                lastLogFileNames.push_back(entry.path().filename().string());
            }
        }

        // Get the largest log file number and create new log file
        int lastLogNum = getLargestLabelNumber(lastLogFileNames, logFilePrefix);
        logFileName = dataPath + "/" + logFilePrefix + std::to_string(lastLogNum + 1) + ".csv";
    }

}


// Load a full environment from the configuration file
template <typename T>
void GravitationalEnvironment<T>::loadParticlesFromConfig(const std::string configFileName) {

    // Get configuration map
    std::map<std::string, std::map<std::string, std::string>> configMap = loadConfig(configFileName);

    // Grab the gloabl config params for the environment
    std::map<std::string, std::string> globalConfigMap = configMap.at("global");
    int nParticles = std::stoi(globalConfigMap.at("nParticles"));

    // Generate distributions for each param
    std::map<std::string, std::vector<float>> envParams;

    // Iterate through the configuration and sample particles according to config prescirption
    for (auto const& [property, propertyDist] : configMap) {

        if (property != "global") { // not the global configuarion field

            // The type of distribution for the property
            std::string distType = propertyDist.at("dist");

            // Sample from the distribution for the property
            if (distType == "constant") { // Constant dist
                envParams[property] = std::vector<float> (nParticles, std::stod(propertyDist.at("val")));
            } else if (distType == "normal") { // Normal dist
                float mu = std::stod(propertyDist.at("mu"));
                float sigma = std::stod(propertyDist.at("sigma"));
                std::normal_distribution<> normalDist(mu, sigma);
                envParams[property] = sampleFromDistribution(nParticles, normalDist);
            }  else if (distType == "uniform") { // Uniform dist
                float min = std::stod(propertyDist.at("min"));
                float max = std::stod(propertyDist.at("max"));
                std::uniform_real_distribution<> uniformDist(min, max);
                envParams[property] = sampleFromDistribution(nParticles, uniformDist);
            } else {
                throw std::invalid_argument("Property " + property + " has an invalid distribution.");
            }
        }
    }

    // Declare particle pointer vector
    std::vector<std::array<float, 3>> positions(nParticles);
    std::vector<std::array<float, 3>> velocities(nParticles);
    for (int i = 0; i < nParticles; i++) {

        // Populate positions and velocities
        positions[i] = {envParams.at("x")[i], envParams.at("y")[i], envParams.at("z")[i]};
        velocities[i] = {envParams.at("vx")[i], envParams.at("vy")[i], envParams.at("vz")[i]};
        float mass = envParams.at("mass")[i];

        // Create Particle instance with pointers to elements in the positions and velocities vectors
        particlePtrs.push_back(std::make_shared<T>(&positions[i], &velocities[i], mass));
    }
}


template <typename T>
// Get the forces in the environment
std::vector<std::array<float, 3>> GravitationalEnvironment<T>::getForcesPairWise(const float timestep) {
    // A vector to hold the forces on each particle
    std::vector<std::array<float, 3>> forces(nParticles);

    // Iterate through and find each source contribution
    float prop_to_force;  // Gmm
    float r_dep; // rhat // r^2
    for (int i = 0; i < nParticles; i++) {
        for (int j = i + 1; j < nParticles; j++) {

            // Only calculate Gmm
            prop_to_force = G * particlePtrs[i]->mass * particlePtrs[j]->mass;

            for (int k = 0; k < 3; k++) {

                // r-dependence
                if (particlePtrs[i]->position[k] == particlePtrs[j]->position[k]){
                    r_dep = 0;
                } else {
                    r_dep = (particlePtrs[i]->position[k] - particlePtrs[j]->position[k]) / abs(pow(particlePtrs[i]->position[k] - particlePtrs[j]->position[k], 3));
                }

                // Update forces (opposite and equal)
                forces[i][k] = -1 * prop_to_force * r_dep;
                forces[j][k] = prop_to_force * r_dep;
            }
        }
    }

    return forces;
}

// Calculate the net force on objPtr, using currOctPtr to navigate the tree (i.e. current node in the tree)
template <typename T>
std::array<float, 3> GravitationalEnvironment<T>::calculateForceBarnesHut(std::shared_ptr<T> objPtr, std::shared_ptr<Octree<T>> currOctPtr, std::array<float, 3> netForce, float theta) {
    // If current node is a nullptr, return netForce
    if (currOctPtr == nullptr) {
        return netForce;
    }
    
    // If current node is an external node and the object node is not the current node, calculate the force
    if (!(currOctPtr->internal) && (currOctPtr->objPtrs[0] != objPtr)) {
        // Calculate force of the external node, and add it to net force and return        
        float prop_to_force = G * objPtr->mass * currOctPtr->objPtrs[0]->mass;
        float r_dep;

        for (int k = 0; k < 3; k++) {
            // r-dependence
            if (objPtr->position[k] == currOctPtr->objPtrs[0]->position[k]){
                r_dep = 0;
            } else {
                r_dep = (currOctPtr->objPtrs[0]->position[k] - objPtr->position[k]) / abs(pow(currOctPtr->objPtrs[0]->position[k] - objPtr->position[k], 3));
            }

            // Update forces
            netForce[k] += prop_to_force * r_dep;
            return netForce;
        }
    }

    // Calculate width of region
    float s = getEuclidianDistance({currOctPtr->xCoords[0], currOctPtr->yCoords[0], currOctPtr->zCoords[0]}, {currOctPtr->xCoords[1], currOctPtr->yCoords[1], currOctPtr->zCoords[1]});

    // Calculate distance between currPtr and objPtr
    float d = getEuclidianDistance(currOctPtr->centerOfMass, objPtr->position);

    // Calculate s / d
    float ratio = s / d;

    // If ratio s / d is < theta, treat the node as a single body and calculate force from currPtr on objPtr; return netForce plus recursive call
    if (ratio < theta) {
        float prop_to_force = G * objPtr->mass * (currOctPtr->totalMass);
        float r_dep;

        for (int k = 0; k < 3; k++) {
            // r-dependence
            if (objPtr->position[k] == currOctPtr->centerOfMass[k]){
                r_dep = 0;
            } else {
                r_dep = (currOctPtr->centerOfMass[k] - objPtr->position[k]) / abs(pow(currOctPtr->centerOfMass[k] - objPtr->position[k], 3));
            }

            // Update forces
            netForce[k] += prop_to_force * r_dep;
        }
        return netForce;
    } else { // Else, recursive call of calculateForceBarnesHut on all children and add them together to return sum of recursive calls
        std::array<float, 3> totalNetForces;

        std::array<float, 3> child0Force = calculateForceBarnesHut(objPtr, currOctPtr->child0, {0, 0, 0}, theta);
        std::array<float, 3> child1Force = calculateForceBarnesHut(objPtr, currOctPtr->child1, {0, 0, 0}, theta);
        std::array<float, 3> child2Force = calculateForceBarnesHut(objPtr, currOctPtr->child2, {0, 0, 0}, theta);
        std::array<float, 3> child3Force = calculateForceBarnesHut(objPtr, currOctPtr->child3, {0, 0, 0}, theta);
        std::array<float, 3> child4Force = calculateForceBarnesHut(objPtr, currOctPtr->child4, {0, 0, 0}, theta);
        std::array<float, 3> child5Force = calculateForceBarnesHut(objPtr, currOctPtr->child5, {0, 0, 0}, theta);
        std::array<float, 3> child6Force = calculateForceBarnesHut(objPtr, currOctPtr->child6, {0, 0, 0}, theta);
        std::array<float, 3> child7Force = calculateForceBarnesHut(objPtr, currOctPtr->child7, {0, 0, 0}, theta);

        for (int i = 0; i < 3; i++) {
            totalNetForces[i] = child0Force[i] + child1Force[i] + child2Force[i] + child3Force[i] + child4Force[i] + child5Force[i] + child6Force[i] + child7Force[i];
        }
        return totalNetForces;
    }
}

template <typename T>
std::vector<std::array<float, 3>> GravitationalEnvironment<T>::getForcesBarnesHut(const float timestep) {

    // Clear the Octree
    envOctree.clearOctree();

    // Get the extreme coordinate locations
    std::array<float, 2> extremeXCoords = {static_cast<float>(particlePtrs[0]->position[0]), static_cast<float>(particlePtrs[0]->position[0])};
    std::array<float, 2> extremeYCoords = {static_cast<float>(particlePtrs[0]->position[1]), static_cast<float>(particlePtrs[0]->position[1])};
    std::array<float, 2> extremeZCoords = {static_cast<float>(particlePtrs[0]->position[2]), static_cast<float>(particlePtrs[0]->position[2])};

    for (int i = 0; i < nParticles; i++) {
        extremeXCoords[0] = std::min(extremeXCoords[0], static_cast<float>(particlePtrs[i]->position[0]));
        extremeXCoords[1] = std::max(extremeXCoords[1], static_cast<float>(particlePtrs[i]->position[0]));
        
        extremeYCoords[0] = std::min(extremeYCoords[0], static_cast<float>(particlePtrs[i]->position[1]));
        extremeYCoords[1] = std::max(extremeYCoords[1], static_cast<float>(particlePtrs[i]->position[1]));
        
        extremeZCoords[0] = std::min(extremeZCoords[0], static_cast<float>(particlePtrs[i]->position[2]));
        extremeZCoords[1] = std::max(extremeZCoords[1], static_cast<float>(particlePtrs[i]->position[2]));
    }

    // Update the coordiantes of the octree
    envOctree.updateCoords(extremeXCoords, extremeYCoords, extremeZCoords);

    // Build the Octree
    envOctree.build(particlePtrs);

    // Calculate the forces
    std::vector<std::array<float, 3>> forces(nParticles); // Vector to hold the forces
    float theta = 0.5;
    std::shared_ptr<Octree<T>> octreePtr = std::make_shared<Octree<T>>(envOctree);
    for (int i = 0; i < nParticles; i++) {
        // Need to implement walking down the tree
        forces[i] = calculateForceBarnesHut(particlePtrs[i], octreePtr, {0, 0, 0}, theta);
    }
    return forces;
}
    

template <typename T>
// Update each particle in the environment
void GravitationalEnvironment<T>::updateAll(const std::vector<std::array<float, 3>>& forces, const float timestep) {
    for (int i = 0; i < nParticles; i++){
        particlePtrs[i]->update(&(forces[i]), timestep);
    }
}

template <typename T>
// Take a step
void GravitationalEnvironment<T>::step(const float timestep) {

    // Get the forces and upate everything
    std::vector<std::array<float, 3>> forces = getForces(timestep);
    updateAll(forces, timestep);

    // Update time
    time += timestep;
}

// Get log file header
template <typename T>
std::string GravitationalEnvironment<T>::getLogHeader() const {
    std::string header = "Time,";

    // Add header entries for each particle
    for (int i=0; i < nParticles; i++) {
        header += "mass" + std::to_string(i) + ",";
        header += "x" + std::to_string(i) + ",";
        header += "y" + std::to_string(i) + ",";
        header += "z" + std::to_string(i) + ",";
        header += "vx" + std::to_string(i) + ",";
        header += "vy" + std::to_string(i) + ",";
        if (i == nParticles - 1){
            header += "vz" + std::to_string(i);
        } else {
            header += "vz" + std::to_string(i) + ",";
        }
    }
    return header + "\n";
}

template <typename T>
// Get the row of the logging csv
std::string GravitationalEnvironment<T>::getStepLog() const {
    
    // Iterate through the particles and append the data to the logging string
    std::string logLine = "";
    for (auto partPrt : particlePtrs) {

        // Append mass
        logLine += std::to_string(partPrt->mass) + ",";

        // Append position
        logLine += std::to_string(partPrt->position[0]) + "," + std::to_string(partPrt->position[1]) + "," + std::to_string(partPrt->position[2]) + ",";

        // Append velocity
        logLine += std::to_string(partPrt->velocity[0]) + "," + std::to_string(partPrt->velocity[1]) + "," + std::to_string(partPrt->velocity[2]) + ",";
    }

    return logLine;
}

template <typename T>
// Run a simulation
void GravitationalEnvironment<T>::simulate(const float duration, const float timestep) {
    std::string logStr = getLogHeader();
    std::cout << getLogHeader() + "\n";

    // Get number of timesteps and take steps iteratively
    float nTimesteps = duration / timestep;
    for (int i = 0; i < nTimesteps; i++) {

        // If logging, append to log string
        if (log == true) {
            logStr += std::to_string(i * timestep);
            logStr += getStepLog() + "\n";
        }
        std::cout << i * timestep << ",\t" << getStepLog() << "\n";

        // Take a step
        step(timestep);
    }
    // end state
    if (log == true) {
        logStr += std::to_string(nTimesteps * timestep) + ",";
        logStr += getStepLog() + "\n";
    }
    std::cout << nTimesteps * timestep << ",\t"  << getStepLog() << "\n";


    // If logging, open file and write to it
    if (log == true) {
        std::ofstream logFile(logFileName);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open the file: " << logFileName << std::endl;
        } else {
            logFile << logStr;
            std::cout << "Successfully logged to " + logFileName + "\n";
        }
        logFile.close();
    }
}

template <typename T>
// Reset the environment
void GravitationalEnvironment<T>::reset() {
    time = 0;
}

// Define classes for both 'Particle' and 'Body'
template class GravitationalEnvironment<Particle>;
template class GravitationalEnvironment<Body>;
