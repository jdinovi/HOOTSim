CXX = g++
CXXFLAGS = -std=c++11 -Wall --coverage
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = test
TEST_OBJ_DIR = obj_test
TARGET = HOOTSim
TEST_TARGET = test_HOOTSim

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Test files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Include directories
INC_DIRS = -I $(INC_DIR)

# Linking step for src files
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling step for src files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

# Linking step for test files
$(BIN_DIR)/$(TEST_TARGET): $(filter-out $(OBJ_DIR)/simulation.o, $(OBJS)) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling step for test files
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(TEST_OBJ_DIR))

.PHONY: all
all: $(BIN_DIR)/$(TARGET)

.PHONY: test
test: $(BIN_DIR)/$(TEST_TARGET)

.PHONY: coverage
coverage:
	@make clean
	@make
	@make test
	@for filename in $(SRCS); do \
		obj_file=$(OBJ_DIR)/$${filename%.cpp}.o; \
		gcov --object-directory=$(OBJ_DIR) $${filename} -n | grep -v ".*simulation.*" | grep -v ".*\.h" | grep -A 1 "src"; \
		echo "\n"; \
	done

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(BIN_DIR)
