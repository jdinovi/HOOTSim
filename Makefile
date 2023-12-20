CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TARGET = HOOTSim

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Include directories
INC_DIRS = -I $(INC_DIR)

# Linking step
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
