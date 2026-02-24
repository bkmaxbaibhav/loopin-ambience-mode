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
