#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

AppConfig Config::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "[AMBIENCE] Config file not found at " << path << ". Using default values" << std::endl;
        return validate(AppConfig{});
    }

    json j = json::parse(file, nullptr, false);
    if (j.is_discarded()) {
        std::cout << "[AMBIENCE] Config file is invalid JSON. Using default values" << std::endl;
        return validate(AppConfig{});
    }

    AppConfig config;
    config.intensity    = j.value("intensity",    0.8f);
    config.edgeWidth    = j.value("edge_width",   12);
    config.fpsCap       = j.value("fps_cap",      60);
    config.colorMode    = j.value("color_mode",   "reactive");
    config.primaryColor = j.value("primary_color","#7B2FFF");
    config.focusMode    = j.value("focus_mode",   true);
    config.autostart    = j.value("autostart",    false);
    config.allMonitors  = j.value("all_monitors", false);
    config.debugMode    = j.value("debug_mode",   false);

    config = validate(config);

    std::cout << "[AMBIENCE] Config loaded — "
              << "intensity: " << config.intensity << ", "
              << "edge_width: " << config.edgeWidth << ", "
              << "color_mode: " << config.colorMode << ", "
              << "fps_cap: " << config.fpsCap << std::endl;

    return config;
}

AppConfig Config::validate(AppConfig config) {
    config.intensity = std::clamp(config.intensity, 0.0f, 1.0f);
    config.edgeWidth = std::clamp(config.edgeWidth, 4, 64);
    config.fpsCap = std::clamp(config.fpsCap, 15, 240);

    if (config.colorMode != "static" && config.colorMode != "reactive" && config.colorMode != "spectrum") {
        std::cout << "[AMBIENCE] Invalid color_mode — using reactive" << std::endl;
        config.colorMode = "reactive";
    }

    bool validColor = true;
    if (config.primaryColor.length() != 7 || config.primaryColor[0] != '#') {
        validColor = false;
    } else {
        for (int i = 1; i < 7; ++i) {
            if (!isxdigit(config.primaryColor[i])) {
                validColor = false;
                break;
            }
        }
    }

    if (!validColor) {
        std::cout << "[AMBIENCE] Invalid primary_color — using #7B2FFF" << std::endl;
        config.primaryColor = "#7B2FFF";
    }

    return config;
}

void Config::save(const AppConfig& config, const std::string& path) {
    json j;
    j["intensity"]     = config.intensity;
    j["edge_width"]    = config.edgeWidth;
    j["fps_cap"]       = config.fpsCap;
    j["color_mode"]    = config.colorMode;
    j["primary_color"] = config.primaryColor;
    j["focus_mode"]    = config.focusMode;
    j["autostart"]     = config.autostart;
    j["all_monitors"]  = config.allMonitors;
    j["debug_mode"]    = config.debugMode;

    std::ofstream file(path);
    if (!file.is_open()) {
        std::cout << "[AMBIENCE] Failed to save config to " << path << std::endl;
        return;
    }

    file << j.dump(2);
    std::cout << "[AMBIENCE] Config saved to " << path << std::endl;
}
