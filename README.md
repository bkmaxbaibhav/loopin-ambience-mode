# loopin-ambience-mode

A cross-platform desktop application that renders an ambient lighting effect along the screen edges, reacting in real-time to system audio.

![screenshot coming soon](assets/placeholder.png)

## Dependencies

- CMake 3.20+
- PortAudio
- FFTW3
- OpenGL 3.3+
- GLFW3
- GLAD
- nlohmann/json

## Installing Dependencies

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  portaudio19-dev \
  libfftw3-dev \
  libglfw3-dev \
  libgl1-mesa-dev \
  nlohmann-json3-dev \
  libx11-dev \
  libxfixes-dev
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install -y \
  gcc-c++ \
  make \
  cmake \
  portaudio-devel \
  fftw-devel \
  glfw-devel \
  mesa-libGL-devel \
  nlohmann_json-devel \
  libX11-devel \
  libXfixes-devel
```

### macOS
```bash
brew install cmake portaudio fftw glfw3 nlohmann-json
```

### Windows
Use vcpkg to install dependencies:
```bash
vcpkg install portaudio:x64-windows fftw3:x64-windows glfw3:x64-windows nlohmann-json:x64-windows
```

## Build Instructions

### Linux
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Session Roadmap

| Session | Status | Description |
| :--- | :--- | :--- |
| Session 1 | In Progress | Project Scaffold + CMakeLists.txt |
| Session 2 | Pending | Audio Capture (PortAudio) |
| Session 3 | Pending | FFT Processing (FFTW3) |
| Session 4 | Pending | Overlay Window & Shader Loading |
| Session 5 | Pending | Audio-to-Visual Mapping |
| Session 6 | Pending | Advanced Shader Effects & Color Modes |
| Session 7 | Pending | Linux Platform Implementation & Config |
| Session 8 | Pending | Windows Platform Implementation |
| Session 9 | Pending | Global Hotkeys & System Tray |
| Session 10 | Pending | Optimization & Bug Fixes |
| Session 11 | Pending | Final Packaging & Distribution |

## License

MIT
