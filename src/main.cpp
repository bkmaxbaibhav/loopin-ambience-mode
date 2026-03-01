#include "renderer/Overlay.h"
#include "audio/AudioCapture.h"
#include "audio/FFTProcessor.h"
#include "renderer/BandMapper.h"
#include "settings/Config.h"
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

    // e. Create BandMapper
    BandMapper mapper(config.colorMode);

    std::cout << "Starting 60fps render loop..." << std::endl;

    auto lastDebugTime = std::chrono::steady_clock::now();
    auto lastConfigCheck = std::chrono::steady_clock::now();

    while (running && !overlay.shouldClose()) {
        auto frameStart = std::chrono::steady_clock::now();

        // Hot Reload (Ticket 4, Step 4)
        if (std::chrono::duration_cast<std::chrono::seconds>(frameStart - lastConfigCheck).count() >= 2) {
            lastConfigCheck = frameStart;
            try {
                auto currentWriteTime = fs::last_write_time(configPath);
                if (currentWriteTime != lastWriteTime) {
                    lastWriteTime = currentWriteTime;
                    config = Config::load(configPath);
                    if (getenv("AMBIENCE_DEBUG")) config.debugMode = true;

                    overlay.setConfig(config);
                    mapper.setColorMode(config.colorMode);

                    std::cout << "[AMBIENCE] Config reloaded — color_mode: " << config.colorModeStr << std::endl;
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
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDebugTime).count() >= 500) {
                std::cout << "[AMBIENCE] bass=" << fft.getBass()
                          << " mid=" << fft.getMid()
                          << " treble=" << fft.getTreble()
                          << " silent=" << (fft.isSilent() ? "true" : "false")
                          << std::endl;
                lastDebugTime = now;
            }
        }

        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed  = frameEnd - frameStart;
        auto target   = std::chrono::microseconds(16667); // ~60fps

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
