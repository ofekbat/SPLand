# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

# Define the name of the output binary
TARGET = simulation

# Define the source, include, and build directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Find all source files in the source directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Define the object files to be created in the build directory
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I$(INC_DIR) -Wall -g
CXXFLAGS = -std=c++11 -Iinclude -Wall -g

# Default target: build the executable
all: $(TARGET)

# Build the executable by linking all object files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to build object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up all generated files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
