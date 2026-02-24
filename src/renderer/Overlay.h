#pragma once

// Manages the fullscreen borderless OpenGL overlay window
// Window properties:
//   - Always on top
//   - Click-through (transparent to mouse input)
//   - Frameless, no title bar, no taskbar entry
//   - Alpha-blended center (only edges visible)
class Overlay {
public:
    Overlay();
    ~Overlay();

    bool init();
    void render(float bass, float mid, float treble);
    void shutdown();
    bool shouldClose() const;

private:
    // GLFWwindow* window_ will be added in Session 4
};
