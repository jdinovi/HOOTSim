CXX = g++
CXXFLAGS = -g -std=c++17 -Wall --coverage
LDFLAGS = -lyaml-cpp
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
INC_DIRS = -I include # -I /opt/homebrew/Cellar/yaml-cpp/0.8.0/include

# Linking step for src files
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling step for src files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

# Linking step for test files
$(BIN_DIR)/$(TEST_TARGET): $(filter-out $(OBJ_DIR)/simulation.o, $(OBJS)) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling step for test files
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure directories exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(TEST_OBJ_DIR))

.PHONY: build
build:
	@make clean
	@make simulation

.PHONY: simulation
simulation: $(BIN_DIR)/$(TARGET)

.PHONY: test
test: $(BIN_DIR)/$(TEST_TARGET)
	./$(BIN_DIR)/$(TEST_TARGET)

.PHONY: coverage
coverage:
	@make build
	@make test
	@results=""; \
	total_lines=0; \
	covered_lines=0; \
	for filename in $(filter-out $(SRC_DIR)/simulation.cpp, $(SRCS)); do \
		obj_file=$(OBJ_DIR)/$${filename%.cpp}.o; \
		result=$$(gcov --object-directory=$(OBJ_DIR) $${filename} -n | grep -v ".*simulation.*" | grep -v ".*\.h" | grep -A 1 "src"); \
		results+="\n\n$$result"; \
		lines=$$(echo $$result | grep -Eo "[0-9]+$$"); \
		line_pct=$$(echo $$result | grep -Eo '([0-9]+\.[0-9]+)\%' | sed 's/\%//'); \
		total_lines=$$((total_lines + lines)); \
		covered_lines=$$(echo "$$(echo "$$covered_lines + $$line_pct * $$lines / 100" | bc)" | bc); \
	done; \
	echo $$results; \
	echo "\nTotal Covered Lines: $$covered_lines"; \
	echo "Total Lines: $$total_lines"; \
	printf "\nTOTAL COVERAGE: %.0f%%\n" "$$(echo "$$covered_lines * 100 / $$total_lines" | bc)";


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(BIN_DIR)
