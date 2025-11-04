# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -O0

# Target executable
TARGET = benchmark
DEBUG_TARGET = benchmark_debug

# Source files
SOURCES = SortingAlgorithms.cpp Benchmark.cpp
HEADERS = SortingAlgorithms.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
DEBUG_OBJECTS = $(SOURCES:.cpp=_debug.o)

# Default target
all: $(TARGET)

# Release build
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(DEBUG_OBJECTS)
	@echo "Debug build complete! Run with: ./$(DEBUG_TARGET)"

# Compile source files (release)
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files (debug)
%_debug.o: %.cpp $(HEADERS)
	$(CXX) $(DEBUG_FLAGS) -c $< -o $@

# Run the benchmark
run: $(TARGET)
	./$(TARGET)

# Run with output redirection
run-quiet: $(TARGET)
	./$(TARGET) > benchmark_output.txt
	@echo "Results saved to benchmark_output.txt and benchmark_results.csv"

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(DEBUG_OBJECTS) $(TARGET) $(DEBUG_TARGET)
	@echo "Clean complete!"

# Clean everything including results
clean-all: clean
	rm -f benchmark_results.csv benchmark_output.txt
	@echo "All files cleaned!"

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for this project."
	@echo "Ensure you have g++ with C++17 support installed."

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build the release version"
	@echo "  make debug    - Build the debug version"
	@echo "  make run      - Build and run the benchmark"
	@echo "  make run-quiet- Build and run, save output to file"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make clean-all- Remove build artifacts and results"
	@echo "  make help     - Show this help message"

# Phony targets
.PHONY: all debug run run-quiet clean clean-all install-deps help
