#include "Overlay.h"
#include "../utils.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

Overlay::Overlay()  {}
Overlay::~Overlay() { shutdown(); }

bool Overlay::init() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwInitialized_ = true;

    // 2. Set window hints
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
#ifdef GLFW_MOUSE_PASSTHROUGH
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
#endif
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
    std::cout << "[AMBIENCE] Overlay size: " << width_ << "x" << height_ << std::endl;

    // 4. Create window
    window_ = glfwCreateWindow(width_, height_, "loopin-ambience-mode", NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // 10. Position window at 0,0
    glfwSetWindowPos(window_, 0, 0);
    glfwShowWindow(window_);

    // 7. Make OpenGL context current
    glfwMakeContextCurrent(window_);

    // 8. Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // 9. Set viewport
    glViewport(0, 0, width_, height_);

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

    std::string vertPath = "shaders/border.vert";
    std::string fragPath = "shaders/border.frag";

    if (!fs::exists(vertPath)) {
        vertPath = "/usr/share/loopin-ambience-mode/shaders/border.vert";
        fragPath = "/usr/share/loopin-ambience-mode/shaders/border.frag";
    }

    if (!shader_.load(vertPath, fragPath)) {
        std::cerr << "Failed to load shaders" << std::endl;
        return false;
    }

    appStart_ = std::chrono::steady_clock::now();

    return true;
}

void Overlay::render(const VisualParams& params) {
    // Audio silence fade
    if (params.isSilent) {
        silenceFade_ -= 0.01f;
        if (silenceFade_ < 0.0f) silenceFade_ = 0.0f;
    } else {
        silenceFade_ += 0.05f;
        if (silenceFade_ > 1.0f) silenceFade_ = 1.0f;
    }

    // Manual visibility toggle fade
    float fadeInSpeed  = 0.05f; // ~20 frames
    float fadeOutSpeed = 0.03f; // ~33 frames

    if (visibilityFade_ < targetAlpha_) {
        visibilityFade_ = std::min(targetAlpha_, visibilityFade_ + fadeInSpeed);
    } else if (visibilityFade_ > targetAlpha_) {
        visibilityFade_ = std::max(targetAlpha_, visibilityFade_ - fadeOutSpeed);
    }

    float combinedFade = silenceFade_ * visibilityFade_;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_.use();
    shader_.setVec2("uResolution", (float)width_, (float)height_);

    auto now = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(now - appStart_).count();
    shader_.setFloat("uTime", time);

    shader_.setFloat("uEdgeWidth", edgeWidth_);
    shader_.setFloat("uIntensity", intensity_ * combinedFade);
    shader_.setFloat("uContrast", contrast_);
    shader_.setVec3("uPrimaryColor", primaryColor_[0], primaryColor_[1], primaryColor_[2]);

    shader_.setFloat("uIntensityTop",    params.trebleIntensity * combinedFade);
    shader_.setFloat("uIntensityBottom", params.bassIntensity   * combinedFade);
    shader_.setFloat("uIntensityLeft",   params.midIntensity    * combinedFade);
    shader_.setFloat("uIntensityRight",  params.midIntensity    * combinedFade);
    shader_.setFloat("uBeat", params.beatPulse * combinedFade);
    shader_.setFloat("uGenreWarmth", params.genreWarmth);
    shader_.setFloat("uGenrePresence", params.genrePresence);
    shader_.setFloat("uGenreConfidence", params.genreConfidence);

    shader_.setInt("uColorMode", params.colorMode);
    shader_.setInt("uVisualMode", visualMode_);
    shader_.setInt("uSideMask", sideMask_);
    shader_.setFloat("uHue", params.hue);

    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void Overlay::setConfig(const AppConfig& config) {
    edgeWidth_ = (float)config.edgeWidth;
    intensity_ = config.intensity;
    contrast_ = config.contrast;
    sideMask_ = (config.sideTop ? 1 : 0)
              | (config.sideRight ? 2 : 0)
              | (config.sideBottom ? 4 : 0)
              | (config.sideLeft ? 8 : 0);
    if (sideMask_ == 0) sideMask_ = 15;

    ColorRGB rgb = hexToRGB(config.primaryColor);
    primaryColor_[0] = rgb.r;
    primaryColor_[1] = rgb.g;
    primaryColor_[2] = rgb.b;

    colorMode_ = colorModeFromString(config.colorMode);
    visualMode_ = visualModeFromString(config.visualMode);
    fpsCap_ = config.fpsCap;
    frameTarget_ = std::chrono::microseconds(1000000 / config.fpsCap);
}

void Overlay::setTargetVisibility(bool visible) {
    targetAlpha_ = visible ? 1.0f : 0.0f;
}

void Overlay::shutdown() {
    if (VAO_) {
        glDeleteVertexArrays(1, &VAO_);
        VAO_ = 0;
    }
    if (VBO_) {
        glDeleteBuffers(1, &VBO_);
        VBO_ = 0;
    }
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    if (glfwInitialized_) {
        glfwTerminate();
        glfwInitialized_ = false;
    }
}

bool Overlay::shouldClose() const {
    if (!window_) return true;
    return glfwWindowShouldClose(window_);
}
