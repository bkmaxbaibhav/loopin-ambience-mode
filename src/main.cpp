#include "renderer/Overlay.h"
#ifdef __linux__
#include "platform/LinuxPlatform.h"
#include "platform/Tray.h"
#include "platform/Hotkey.h"
#include "platform/AutoStart.h"
#include <X11/Xlib.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#endif
#include "audio/AudioCapture.h"
#include "audio/FFTProcessor.h"
#include "renderer/BandMapper.h"
#include "settings/Config.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>
#include <filesystem>

namespace fs = std::filesystem;

std::atomic<bool> running{ true };

void signalHandler(int signum) {
    (void)signum;
    running = false;
}

int main(int argc, char** argv) {
    signal(SIGINT, signalHandler);

    std::cout << "loopin-ambience-mode starting..." << std::endl;

    // a. Load config
    std::string configPath = "config/default.json";
    if (!fs::exists(configPath)) {
        configPath = "/etc/loopin-ambience-mode/default.json";
    }
    AppConfig config = Config::load(configPath);
    if (getenv("AMBIENCE_DEBUG")) {
        config.debugMode = true;
    }
    bool visualTestMode = getenv("AMBIENCE_VISUAL_TEST") != nullptr;
    if (visualTestMode) {
        config.intensity = 1.0f;
        config.edgeWidth = 32;
        config.colorMode = "spectrum";
        std::cout << "[AMBIENCE] Visual test mode enabled" << std::endl;
    }

    // Auto-start check
    try {
        std::string execPath = fs::canonical(argv[0]).string();
        if (config.autostart && !AutoStart::isEnabled()) {
            AutoStart::enable(execPath);
        } else if (!config.autostart && AutoStart::isEnabled()) {
            AutoStart::disable();
        }
    } catch (...) {
        std::cerr << "Warning: Could not handle auto-start configuration" << std::endl;
    }

    fs::file_time_type lastWriteTime;
    try {
        lastWriteTime = fs::last_write_time(configPath);
    } catch (...) {
        std::cerr << "Warning: Could not get last write time for " << configPath << std::endl;
    }

    // b. Initialize audio
    AudioCapture capture;
    if (!capture.start()) {
        std::cerr << "Failed to start audio capture" << std::endl;
        return 1;
    }

    // c. Initialize FFT
    FFTProcessor fft;

    // d. Initialize renderer
    Overlay overlay;
    if (!overlay.init()) {
        std::cerr << "Failed to init overlay" << std::endl;
        return 1;
    }
    overlay.setConfig(config);

    // e. Create BandMapper
    BandMapper mapper(colorModeFromString(config.colorMode));

#ifdef __linux__
    LinuxPlatform platform;
    platform.init(overlay.getWindowHandle());

    Tray tray;
    tray.init("loopin-ambience-mode"); // Use icon name for system theme or path
    tray.syncConfig(config);

    Hotkey hotkey;
    // For hotkey we need native X11 handles if available
    Display* xDisp = glfwGetX11Display();
    if (xDisp) {
        hotkey.init(xDisp, DefaultRootWindow(xDisp));
    } else {
        std::cerr << "[AMBIENCE] Could not get X11 display"
                  << " — hotkey disabled" << std::endl;
    }

    bool overlayVisible = true;
    auto toggleOverlay = [&]() {
        overlayVisible = !overlayVisible;
        tray.setVisible(overlayVisible);
        overlay.setTargetVisibility(overlayVisible);
        std::cout << "[AMBIENCE] Overlay " << (overlayVisible ? "shown" : "hidden") << std::endl;
    };

    tray.setOnToggle(toggleOverlay);
    tray.setOnQuit([&]() { running = false; });
    auto applyMenuConfig = [&]() {
        config = Config::load(configPath);
        overlay.setConfig(config);
        mapper.setColorMode(colorModeFromString(config.colorMode));
        tray.syncConfig(config);
    };
    auto saveMenuConfig = [&]() {
        Config::save(config, configPath);
        applyMenuConfig();
    };
    tray.setOnColorMode([&](const std::string& mode) {
        config.colorMode = mode;
        saveMenuConfig();
    });
    tray.setOnVisualMode([&](const std::string& mode) {
        config.visualMode = mode;
        saveMenuConfig();
    });
    tray.setOnSideToggle([&](const std::string& side, bool enabled) {
        if (side == "top") config.sideTop = enabled;
        else if (side == "right") config.sideRight = enabled;
        else if (side == "bottom") config.sideBottom = enabled;
        else if (side == "left") config.sideLeft = enabled;

        if (!config.sideTop && !config.sideRight && !config.sideBottom && !config.sideLeft) {
            if (side == "top") config.sideTop = true;
            else if (side == "right") config.sideRight = true;
            else if (side == "bottom") config.sideBottom = true;
            else if (side == "left") config.sideLeft = true;
        }

        saveMenuConfig();
    });
    tray.setOnEdgeWidth([&](int width) {
        config.edgeWidth = width;
        saveMenuConfig();
    });
    tray.setOnBrightness([&](float brightness) {
        config.intensity = brightness;
        saveMenuConfig();
    });
    tray.setOnContrast([&](float contrast) {
        config.contrast = contrast;
        saveMenuConfig();
    });
    hotkey.setOnToggle(toggleOverlay);
#endif

    std::cout << "Starting render loop..." << std::endl;

    auto lastDebugTime = std::chrono::steady_clock::now();
    auto lastConfigCheck = std::chrono::steady_clock::now();

    while (running && !overlay.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();

        // Hot Reload logic
        auto now = std::chrono::steady_clock::now();
        if (now - lastConfigCheck >= std::chrono::seconds(2)) {
            lastConfigCheck = now;
            try {
                auto currentWriteTime = fs::last_write_time(configPath);
                if (currentWriteTime != lastWriteTime) {
                    lastWriteTime = currentWriteTime;
                    AppConfig newConfig = Config::load(configPath);

                    config = newConfig;
                    if (getenv("AMBIENCE_DEBUG")) config.debugMode = true;
                    if (visualTestMode) {
                        config.intensity = 1.0f;
                        config.edgeWidth = 32;
                        config.colorMode = "spectrum";
                    }

                    overlay.setConfig(config);
                    mapper.setColorMode(colorModeFromString(config.colorMode));

                    // Handle autostart toggle on hot reload
                    std::string execPath = fs::canonical(argv[0]).string();
                    if (config.autostart && !AutoStart::isEnabled()) {
                        AutoStart::enable(execPath);
                    } else if (!config.autostart && AutoStart::isEnabled()) {
                        AutoStart::disable();
                    }

                    std::cout << "[AMBIENCE] Config hot-reloaded" << std::endl;
                }
            } catch (...) {}
        }

#ifdef __linux__
        tray.update();
        hotkey.poll();
#endif

        // f. Run the main loop
        auto samples = capture.readSamples();
        fft.process(samples);

        auto params = mapper.map(
            fft.getBass(),
            fft.getMid(),
            fft.getTreble(),
            fft.isSilent()
        );
        if (visualTestMode) {
            params.bassIntensity = 1.0f;
            params.midIntensity = 1.0f;
            params.trebleIntensity = 1.0f;
            params.beatPulse = 0.8f + 0.2f * std::sin(
                std::chrono::duration<float>(
                    std::chrono::steady_clock::now().time_since_epoch()
                ).count() * 4.0f
            );
            params.isSilent = false;
            params.colorMode = 2;
        }

        overlay.render(params);

        if (config.debugMode) {
            auto debugNow = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(debugNow - lastDebugTime).count() >= 500) {
                std::cout << "[AMBIENCE] bass=" << fft.getBass()
                          << " mid=" << fft.getMid()
                          << " treble=" << fft.getTreble()
                          << " beat=" << params.beatPulse
                          << " genre_conf=" << params.genreConfidence
                          << " silent=" << (fft.isSilent() ? "true" : "false")
                          << std::endl;
                lastDebugTime = debugNow;
            }
        }

        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed  = frameEnd - frameStart;
        auto target   = overlay.getFrameTarget();

        if (elapsed < target) {
            std::this_thread::sleep_for(target - elapsed);
        }
    }

    // g. Cleanup
    overlay.shutdown();
    capture.stop();

    std::cout << "loopin-ambience-mode shutdown." << std::endl;

    return 0;
}
