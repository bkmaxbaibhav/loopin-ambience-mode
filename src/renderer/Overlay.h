#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <chrono>

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
    GLFWwindow* window_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    Shader shader_;
    std::chrono::steady_clock::time_point appStart_;
};
