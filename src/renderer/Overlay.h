#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "BandMapper.h"
#include "../settings/Config.h"
#include <chrono>

// Manages the fullscreen borderless OpenGL overlay window
class Overlay {
public:
    Overlay();
    ~Overlay();

    bool init();
    void render(const VisualParams& params);
    void setConfig(const AppConfig& config);
    void shutdown();
    bool shouldClose() const;
    void setTargetVisibility(bool visible);
    GLFWwindow* getWindowHandle() const { return window_; }
    std::chrono::microseconds getFrameTarget() const { return frameTarget_; }

private:
    GLFWwindow* window_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    Shader shader_;
    std::chrono::steady_clock::time_point appStart_;
    float silenceFade_ = 1.0f;
    float visibilityFade_ = 1.0f;
    float targetAlpha_ = 1.0f;

    // Config stored for rendering
    float primaryColor_[3] = {0.482f, 0.184f, 1.0f};
    float edgeWidth_ = 12.0f;
    float intensity_ = 0.8f;
    int colorMode_ = 1;
    int fpsCap_ = 60;
    std::chrono::microseconds frameTarget_{16667};
};
