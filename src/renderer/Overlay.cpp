#include "Overlay.h"

Overlay::Overlay()  {}
Overlay::~Overlay() { shutdown(); }

bool Overlay::init() {
    // TODO Session 4: create fullscreen GLFW window
    // TODO Session 4: init OpenGL, compile shaders, setup VAO
    return false;
}

void Overlay::render(float bass, float mid, float treble) {
    // TODO Session 5: pass uniforms to shader, draw fullscreen quad
}

void Overlay::shutdown() {
    // TODO Session 4: destroy GLFW window, cleanup OpenGL
}

bool Overlay::shouldClose() const {
    // TODO Session 4: return glfwWindowShouldClose
    return true;
}
