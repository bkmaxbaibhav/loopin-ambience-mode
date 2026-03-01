#include "Overlay.h"
#include <iostream>
#include <vector>
#include <chrono>

Overlay::Overlay()  {}
Overlay::~Overlay() { shutdown(); }

bool Overlay::init() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // 2. Set window hints
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. Get primary monitor dimensions
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor" << std::endl;
        return false;
    }
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    width_ = mode->width;
    height_ = mode->height;

    // 4. Create window
    window_ = glfwCreateWindow(width_, height_, "loopin-ambience-mode", NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // 10. Position window at 0,0
    glfwSetWindowPos(window_, 0, 0);

    // 7. Make OpenGL context current
    glfwMakeContextCurrent(window_);

    // 8. Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // 9. Set viewport
    glViewport(0, 0, width_, height_);

    // TICKET 2: Setup VAO and VBO for fullscreen quad
    float vertices[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // TICKET 3: Load shaders
    if (!shader_.load("shaders/border.vert", "shaders/border.frag")) {
        std::cerr << "Failed to load shaders" << std::endl;
        return false;
    }

    appStart_ = std::chrono::steady_clock::now();

    return true;
}

void Overlay::render(float bass, float mid, float treble) {
    (void)bass;
    (void)mid;
    (void)treble;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_.use();
    shader_.setVec2("uResolution", (float)width_, (float)height_);

    auto now = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(now - appStart_).count();
    shader_.setFloat("uTime", time);

    shader_.setFloat("uEdgeWidth", 12.0f);
    shader_.setFloat("uIntensity", 0.8f);
    shader_.setVec3("uPrimaryColor", 0.48f, 0.18f, 1.0f);

    shader_.setFloat("uIntensityTop", 0.8f);
    shader_.setFloat("uIntensityBottom", 0.8f);
    shader_.setFloat("uIntensityLeft", 0.8f);
    shader_.setFloat("uIntensityRight", 0.8f);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void Overlay::shutdown() {
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (window_) glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Overlay::shouldClose() const {
    if (!window_) return true;
    return glfwWindowShouldClose(window_);
}
