CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS =
TARGET = projectA
CWD = $(shell pwd)

# Directories
SRC_DIR = $(CWD)/src
BIN_DIR = $(CWD)/bin
OBJ_DIR = $(CWD)/obj
INC_DIR = $(CWD)/include

EXE = $(BIN_DIR)/$(TARGET)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Header files
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)

# Default target
all: copy_headers $(EXE)

# Linking step for the executable
$(EXE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ -I$(INC_DIR)

# Compilation step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I$(INC_DIR)

copy_headers:
	@mkdir -p $(INC_DIR)
	cp $(HEADERS) $(INC_DIR)

# Clean up
clean:
	@rm -rf $(OBJ_DIR) $(INC_DIR) $(EXE)

.PHONY: all clean copy_headers