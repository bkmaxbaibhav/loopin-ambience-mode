# Project: loopin-ambience-mode
## What is this app?

Loopin-ambience-mode is a cross-platform desktop plugin (Linux & Windows)
that renders an ambient lighting effect along the screen edges — inspired
by Android's Edge Lighting / Ambient Display border glow feature.

The screen borders (all 4 edges) light up and animate in real-time,
reacting to the amplitude and frequency of the system's audio output.
The effect lives only at the very periphery of the display (~10–15px inset),
making it non-distracting — visible in peripheral vision but never
interrupting focus.

The core problem it solves: people who enjoy ambient/event lighting
(like Philips Hue, or phone edge lighting) but work at a desktop and
miss out on that experience during music, videos, or notifications.

---

## Core Concept

- Capture system audio OUTPUT (loopback) in real-time
- Run FFT (Fast Fourier Transform) to extract frequency bands:
    - Bass (20–250Hz)     → bottom edge pulse
    - Mids (250–4kHz)     → left & right edge wave
    - Treble (4kHz–20kHz) → top edge shimmer
- Map amplitude + frequency data to an animated border glow overlay
- The overlay is a borderless, always-on-top, click-through
  transparent window sitting over the entire screen
- Color, intensity, and animation speed react to the audio in real-time

---

## Tech Stack

- Language: C++17
- Build System: CMake (3.20+)
- Audio Capture: PortAudio (cross-platform)
    - Linux backend: PipeWire / PulseAudio loopback
    - Windows backend: WASAPI loopback
- FFT: FFTW3 (fastest FFT in the west)
- Rendering: OpenGL 3.3 core profile + GLFW + GLAD
- Shader Language: GLSL
- Overlay Window:
    - Linux: X11 override-redirect window + XFixes for click-through
             OR Wayland wlr-layer-shell protocol
    - Windows: Win32 layered window
               (WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST)
- System Tray:
    - Linux: libappindicator or ayatana-appindicator
    - Windows: Win32 Shell_NotifyIcon
- Settings: INI or JSON config file (nlohmann/json for parsing)
- Hotkeys:
    - Linux: XGrabKey (X11) or libinput
    - Windows: RegisterHotKey Win32 API

---

## Project Structure

vibe-border-lighting/
├── CMakeLists.txt
├── config/
│   └── default.json            # Default user settings
├── src/
│   ├── main.cpp                # Entry point, init & main loop
│   ├── audio/
│   │   ├── AudioCapture.h      
│   │   ├── AudioCapture.cpp    # PortAudio loopback capture
│   │   ├── FFTProcessor.h      
│   │   └── FFTProcessor.cpp    # FFTW3 FFT + band extraction
│   ├── renderer/
│   │   ├── Overlay.h           
│   │   ├── Overlay.cpp         # OpenGL overlay window management
│   │   ├── Shader.h            
│   │   ├── Shader.cpp          # GLSL shader loader & compiler
│   │   ├── border.vert         # Vertex shader
│   │   ├── border.frag         # Fragment shader (glow effect)
│   │   └── BandMapper.h/cpp    # Maps FFT bands → visual parameters
│   ├── platform/
│   │   ├── Platform.h          # Abstract platform interface
│   │   ├── LinuxPlatform.cpp   # X11 / Wayland window setup
│   │   └── WindowsPlatform.cpp # Win32 layered window setup
│   └── settings/
│       ├── Config.h            
│       └── Config.cpp          # Load/save JSON config
├── shaders/
│   ├── border.vert
│   └── border.frag
└── assets/
    └── icon.png                # Tray icon

---

## Key Behaviors

1. OVERLAY WINDOW must be:
   - Always on top of all other windows
   - Fully transparent in the center (only edges visible)
   - Click-through (mouse events pass to windows underneath)
   - Frameless / no title bar / no taskbar entry

2. AUDIO REACTIVITY:
   - Target 60fps render loop (use glfwSwapInterval or manual 
     frame timing)
   - FFTW3 plan: FFTW_R2C with window size 1024 or 2048 samples
   - Apply Hann window function before FFT to reduce spectral leakage
   - Smooth amplitude with exponential moving average (alpha ~0.3)
     to prevent harsh flickering:
       smoothed = alpha * raw + (1 - alpha) * smoothed
   - Silence detection: if RMS < 0.01 for 3 consecutive seconds,
     fade effect out gracefully

3. EDGE MAPPING:
   - Each edge animates independently
   - Glow travels/pulses along edge length, not just uniform brighten
   - Support 3 color modes:
       a. Static — user defined hex color
       b. Reactive — hue shifts based on dominant frequency
       c. Spectrum — rainbow, each frequency band maps to a hue

4. OPENGL RENDERING APPROACH:
   - Create a fullscreen quad VAO
   - In the fragment shader, discard fragments that are not 
     within edge_width pixels of the screen border
   - Pass band amplitudes as uniforms:
       uniform float uBass;
       uniform float uMid;
       uniform float uTreble;
       uniform float uTime;
       uniform int   uColorMode;
       uniform vec3  uPrimaryColor;
       uniform float uEdgeWidth;
       uniform float uIntensity;

5. SETTINGS (stored in JSON):
   {
     "intensity": 0.8,
     "edge_width": 12,
     "fps_cap": 60,
     "color_mode": "reactive",
     "primary_color": "#7B2FFF",
     "focus_mode": true,
     "autostart": false
   }

6. SYSTEM TRAY MENU:
   - Toggle effect on/off
   - Open settings window
   - Quit application

7. GLOBAL HOTKEY: Ctrl+Alt+V → toggle effect on/off

---

## Platform Notes

### Linux
- Detect X11 or Wayland at runtime using environment variable
  (check WAYLAND_DISPLAY first, fallback to DISPLAY)
- X11 path:
    - Create override-redirect window (bypass window manager)
    - Use XShapeCombineRectangles + XFixesCreateRegion 
      for click-through input region
    - Set _NET_WM_STATE_ABOVE for always-on-top
- Wayland path:
    - Use zwlr_layer_shell_v1 protocol (overlay layer)
    - Set input region to empty for click-through
    - Requires compositor support (GNOME/KDE/Sway/Hyprland)

### Windows
- Create Win32 window with extended styles:
    WS_EX_LAYERED | WS_EX_TRANSPARENT | 
    WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW
- Use SetLayeredWindowAttributes or UpdateLayeredWindow
  for alpha blending
- WASAPI loopback:
    - CoInitialize → IMMDeviceEnumerator → GetDefaultAudioEndpoint
    - IAudioClient with AUDCLNT_STREAMFLAGS_LOOPBACK
    - Capture from render endpoint in loopback mode

---

## Build Instructions to Generate

Please generate a complete CMakeLists.txt that:
- Finds and links: PortAudio, FFTW3, OpenGL, GLFW, GLAD, 
  nlohmann_json
- Conditionally compiles LinuxPlatform.cpp or WindowsPlatform.cpp
  based on CMAKE_SYSTEM_NAME
- Sets C++17 standard
- Copies shaders and assets to build directory post-build

---

## Priority Build Order

1. AudioCapture.cpp — PortAudio loopback capture, callback-based
2. FFTProcessor.cpp — FFTW3 plan, Hann window, band extraction, 
   smoothing
3. Overlay.cpp — Basic OpenGL window rendering a static 
   colored border
4. BandMapper.cpp — Connect FFT output to OpenGL uniforms
5. border.frag — Animated glow shader reacting to band uniforms
6. LinuxPlatform.cpp / WindowsPlatform.cpp — Click-through, 
   always-on-top, overlay window
7. Config.cpp — JSON settings load/save
8. Tray icon + hotkey registration
9. CMake packaging + installers

---

## Code Style & Standards

- C++17 throughout — use std::filesystem, std::optional, 
  structured bindings where appropriate
- RAII everywhere — wrap PortAudio streams, FFTW plans, 
  OpenGL objects in classes with proper destructors
- No raw owning pointers — prefer std::unique_ptr / std::shared_ptr
- Separate audio capture thread from render thread — use 
  std::atomic<float> or lock-free ring buffer for data passing
- Comment all FFT math, shader uniforms, and platform-specific 
  window code thoroughly
- Each module (audio, renderer, platform, settings) must be 
  independently compilable and testable

---

## Non-Goals (out of scope for MVP)

- macOS support (future)
- Notification-triggered lighting (future)
- Hardware LED sync e.g. Razer Chroma, Govee (future)
- Mobile companion app (future)
