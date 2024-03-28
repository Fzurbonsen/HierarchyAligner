CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS =
TARGET = projectA
TEST_TARGET = test
CWD = $(shell pwd)

# Directories
SRC_DIR = $(CWD)/src
TEST_DIR = $(CWD)/tests
BIN_DIR = $(CWD)/bin
OBJ_DIR = $(CWD)/obj
INC_DIR = $(CWD)/include

# Executables
EXE = $(BIN_DIR)/$(TARGET)
TEST_EXE = $(BIN_DIR)/$(TEST_TARGET)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter-out $(SRC_DIR)/main.cpp, $(SRCS)))
MAIN_OBJ = $(OBJ_DIR)/main.o
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))

# Header files
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)
TEST_HEADERS = $(wildcard $(TEST_DIR)/*.hpp)

# Default target
all: copy_headers $(EXE)

tests: copy_test_headers $(TEST_EXE) run_tests

# Linking step for the executable
$(EXE): $(OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ -I$(INC_DIR)

$(TEST_EXE): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ -I$(INC_DIR)

# Compilation step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

copy_test_headers: copy_headers
	@mkdir -p $(INC_DIR)
	cp $(TEST_HEADERS) $(INC_DIR)

copy_headers:
	@mkdir -p $(INC_DIR)
	cp $(HEADERS) $(INC_DIR)

run_tests:
	$(shell $(TEST_EXE))

# Clean up
clean:
	@rm -rf $(OBJ_DIR) $(INC_DIR) $(BIN_DIR)

.PHONY: all clean copy_headers