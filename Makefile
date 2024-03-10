CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS =
TARGET = projectA
CWD = $(shell pwd)

# Directories
SRC_DIR = $(CWD)/src
BIN_DIR = $(CWD)/bin
OBJ_DIR = $(CWD)/obj

EXE = $(BIN_DIR)/$(TARGET)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(EXE)

# Linking step for the executable
$(EXE): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compilation step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up
clean:
	@rm -rf $(OBJ_DIR) $(EXE)
