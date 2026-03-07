#include "settings/Config.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

int main() {
    std::string configPath = "config/default.json";
    fs::create_directories("config");

    // Create initial config
    {
        std::ofstream f(configPath);
        f << "{\"intensity\": 0.8, \"edge_width\": 12, \"fps_cap\": 60, \"color_mode\": \"reactive\"}";
    }

    AppConfig config = Config::load(configPath);
    auto lastWriteTime = fs::last_write_time(configPath);
    auto lastConfigCheck = std::chrono::steady_clock::now();

    std::cout << "Starting hot-reload test loop..." << std::endl;

    int iterations = 0;
    bool reloaded = false;

    while (iterations < 20) {
        auto now = std::chrono::steady_clock::now();

        if (iterations == 5) {
            std::cout << "Modifying config file..." << std::endl;
            std::ofstream f(configPath);
            f << "{\"intensity\": 0.9, \"edge_width\": 15, \"fps_cap\": 30, \"color_mode\": \"static\"}";
            f.close();
            // Ensure filesystem registers the change
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (now - lastConfigCheck >= std::chrono::seconds(1)) { // Faster check for test
            lastConfigCheck = now;
            try {
                auto currentWriteTime = fs::last_write_time(configPath);
                if (currentWriteTime != lastWriteTime) {
                    lastWriteTime = currentWriteTime;
                    AppConfig newConfig = Config::load(configPath);

                    if (!(newConfig.intensity == 0.8f &&
                          newConfig.edgeWidth == 12 &&
                          newConfig.colorMode == "reactive" &&
                          newConfig.fpsCap == 60)) {

                        config = newConfig;
                        std::cout << "[AMBIENCE] Config hot-reloaded" << std::endl;
                        reloaded = true;
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "Reload error: " << e.what() << std::endl;
            }
        }

        if (reloaded && config.intensity == 0.9f) {
            std::cout << "Hot-reload successful and verified!" << std::endl;
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        iterations++;
    }

    std::cout << "Hot-reload test timed out or failed." << std::endl;
    return 1;
}
