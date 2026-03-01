#ifdef __linux__
#include "platform/LinuxPlatform.h"
#endif
#include "renderer/Overlay.h"
#include "audio/AudioCapture.h"
#include "audio/FFTProcessor.h"
#include "renderer/BandMapper.h"
#include "settings/Config.h"
#include "utils.h"
#include <iostream>
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

int main() {
    signal(SIGINT, signalHandler);

    std::cout << "loopin-ambience-mode starting..." << std::endl;

    // a. Load config
    std::string configPath = "config/default.json";
    AppConfig config = Config::load(configPath);
    if (getenv("AMBIENCE_DEBUG")) {
        config.debugMode = true;
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

#ifdef __linux__
    LinuxPlatform platform;
    platform.init(overlay.getWindowHandle());
#endif

    // e. Create BandMapper
    BandMapper mapper(colorModeFromString(config.colorMode));

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

                    // Safety heuristic: skip reload if it looks like mid-save defaults
                    if (!(newConfig.intensity == 0.8f &&
                          newConfig.edgeWidth == 12 &&
                          newConfig.colorMode == "reactive" &&
                          newConfig.fpsCap == 60)) {

                        config = newConfig;
                        if (getenv("AMBIENCE_DEBUG")) config.debugMode = true;

                        overlay.setConfig(config);
                        mapper.setColorMode(colorModeFromString(config.colorMode));

                        std::cout << "[AMBIENCE] Config hot-reloaded" << std::endl;
                    }
                }
            } catch (...) {}
        }

        // f. Run the main loop
        auto samples = capture.readSamples();
        fft.process(samples);

        auto params = mapper.map(
            fft.getBass(),
            fft.getMid(),
            fft.getTreble(),
            fft.isSilent()
        );

        overlay.render(params);

        if (config.debugMode) {
            auto debugNow = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(debugNow - lastDebugTime).count() >= 500) {
                std::cout << "[AMBIENCE] bass=" << fft.getBass()
                          << " mid=" << fft.getMid()
                          << " treble=" << fft.getTreble()
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
