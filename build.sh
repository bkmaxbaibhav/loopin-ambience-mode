#!/bin/bash
set -e

# Build script for loopin-ambience-mode
echo "Building loopin-ambience-mode..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir -p build
fi

cd build

# Run CMake configuration
echo "Running CMake configuration..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building project..."
make -j$(nproc)

echo "Build complete! Executable: $(pwd)/loopin-ambience-mode"
