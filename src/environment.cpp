#include <iostream>
#include <array>
#include <vector>
#include <math.h>
#include <fstream>
#include <filesystem>

#include "../include/environment.h"
#include "../include/body.h"
#include "../include/particle.h"


namespace fs = std::filesystem;
double G = 6.6743e-11;


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

template <typename T>
GravitationalEnvironment<T>::GravitationalEnvironment(const std::vector<std::shared_ptr<T>>& particlePtrs, const bool log, std::string logFilePrefix)
    : particlePtrs(particlePtrs), log(log), time(0), nParticles(particlePtrs.size()) {

        // Create a log file if we want one
        if (log == true) {
            
            // Get a vector of the filenames in the data directory
            std::vector<std::string> lastLogFileNames;
            const char* repoPath = std::getenv("HOOTSIM_PATH");
            std::string dataPath = std::string(repoPath) + "/data";
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


template <typename T>
// Get the forces in the environment
std::vector<std::array<double, 3>> GravitationalEnvironment<T>::getForces(const double timestep) {
    // A vector to hold the forces on each particle
    std::vector<std::array<double, 3>> forces(nParticles);

    // Iterate through and find each source contribution
    double prop_to_force;  // Gmm
    double r_dep; // rhat // r^2
    for (int i = 0; i < nParticles; i++) {
        for (int j = i + 1; j < nParticles; j++) {

            // Only calculate Gmm
            prop_to_force = G * particlePtrs[i]->mass * particlePtrs[j]->mass;

            for (int k = 0; k < 3; k++) {

                // r-dependence
                if (particlePtrs[i]->position[k] == particlePtrs[j]->position[k]){
                    r_dep = 0;
                } else {
                    r_dep = (particlePtrs[i]->position[k] - particlePtrs[j]->position[k]) / pow(particlePtrs[i]->position[k] - particlePtrs[j]->position[k], 3);
                }

                // Update forces (opposite and equal)
                forces[i][k] = prop_to_force * r_dep;
                forces[j][k] = -1 * prop_to_force * r_dep;
            }
        }
    }

    return forces;
}

template <typename T>
// Update each particle in the environment
void GravitationalEnvironment<T>::updateAll(const std::vector<std::array<double, 3>>& forces, const double timestep) {
    for (int i = 0; i < nParticles; i++){
        particlePtrs[i]->update(&(forces[i]), timestep);
    }
}

template <typename T>
// Take a step
void GravitationalEnvironment<T>::step(const double timestep) {

    // Get the forces and upate everything
    std::vector<std::array<double, 3>> forces = getForces(timestep);
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
void GravitationalEnvironment<T>::simulate(const double duration, const double timestep) {
    std::string logStr = getLogHeader();
    std::cout << getLogHeader() + "\n";

    // Get number of timesteps and take steps iteratively
    double nTimesteps = duration / timestep;
    for (int i = 0; i < nTimesteps; i++) {

        // If logging, append to log string
        if (log == true) {
            logStr += std::to_string(i * timestep) + ",";
            logStr += getStepLog() + "\n";
        }
        std::cout << i * timestep << ",\t" << getStepLog() << "\n";

        // Take a step
        step(timestep);
    }
    // end state
    if (log == true) {
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
            std::cout << "Successfully logged to " + logFileName;
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
