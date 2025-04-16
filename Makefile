# Compiler and flags
CXX = g++
# Use C++11 standard, enable warnings
CXXFLAGS = -std=c++11 -Wall -Wextra -I. # -I. includes current directory for headers

# Target executable name
TARGET = calculator

# Source files
SRCS = main.cpp string_calculator.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Default target: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete: $(TARGET)"

# Rule to compile source files into object files
%.o: %.cpp string_calculator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned build files."

# Phony targets (targets that aren't actual files)
.PHONY: all clean