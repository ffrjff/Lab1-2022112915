# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Source files and output
SRC = main.cpp graph.cpp text.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = program

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program with input file
run: $(TARGET)
	./$(TARGET) $(FILE)

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean run
