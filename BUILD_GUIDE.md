# Build Instructions for loopin-ambience-mode

## Project Overview
This is a C++ audio visualization application using CMake build system on Ubuntu 24.04 LTS.

## Required Dependencies (Verified from CMakeLists.txt)
- CMake 3.20+
- C++17 compiler (GCC/Clang)
- PortAudio development libraries (portaudio-2.0)
- FFTW3 (single precision: fftw3f)
- OpenGL 3.3+
- GLFW3
- nlohmann/json
- X11 and XFixes libraries (for Linux)

## Build Steps

### 1. Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  libportaudio2 libportaudio-dev \
  libfftw3-dev libfftw3-single3 \
  libglfw3 libglfw3-dev \
  libgl1-mesa-dev \
  nlohmann-json3-dev \
  libx11-dev \
  libxfixes-dev
```

### 2. Configure Build
```bash
cd /workspaces/loopin-ambience-mode
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 3. Compile
```bash
make -j$(nproc)
```

### 4. Run Tests (Optional)
```bash
./loopin-ambience-fft-test
```

### 5. Run Application
```bash
./loopin-ambience-mode
```

## Build Output
- `build/loopin-ambience-mode` - Main application executable
- `build/loopin-ambience-fft-test` - FFT processor test executable
- Shaders and config will be copied to build directory

## Project Structure
```
src/
  ├── main.cpp           - Entry point
  ├── audio/             - Audio capture and FFT processing
  ├── renderer/          - OpenGL rendering components
  ├── platform/          - Platform-specific window management
  └── settings/          - Configuration handling
```

## Verified Source Files
All source files have been verified and are complete:
- ✓ AudioCapture (with PortAudio integration)
- ✓ FFTProcessor (with FFTW3)
- ✓ Overlay (placeholder for Session 4)
- ✓ Shader (placeholder for Session 4)
- ✓ BandMapper (placeholder for Session 5)
- ✓ Config (placeholder for Session 7)
- ✓ Platform implementations (LinuxPlatform, WindowsPlatform)

## Current Development Status
According to README.md, this is Session 1 (Project Scaffold + CMakeLists.txt)
- Session 1: ✓ Project scaffold complete
- Session 2+: Pending implementations (but compilable)

## Notes
The project is currently set up to run in SIMULATION mode if no audio device is available, making it testable in container environments.
