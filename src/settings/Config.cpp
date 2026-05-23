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
    config.contrast     = j.value("contrast",     1.0f);
    config.edgeWidth    = j.value("edge_width",   12);
    config.fpsCap       = j.value("fps_cap",      60);
    config.colorMode    = j.value("color_mode",   "reactive");
    config.visualMode   = j.value("visual_mode",  "auto");
    config.primaryColor = j.value("primary_color","#7B2FFF");
    config.sideTop      = j.value("side_top",     true);
    config.sideRight    = j.value("side_right",   true);
    config.sideBottom   = j.value("side_bottom",  true);
    config.sideLeft     = j.value("side_left",    true);
    config.focusMode    = j.value("focus_mode",   true);
    config.autostart    = j.value("autostart",    false);
    config.allMonitors  = j.value("all_monitors", false);
    config.debugMode    = j.value("debug_mode",   false);

    config = validate(config);

    std::cout << "[AMBIENCE] Config loaded — "
              << "intensity: " << config.intensity << ", "
              << "contrast: " << config.contrast << ", "
              << "edge_width: " << config.edgeWidth << ", "
              << "color_mode: " << config.colorMode << ", "
              << "visual_mode: " << config.visualMode << ", "
              << "fps_cap: " << config.fpsCap << std::endl;

    return config;
}

AppConfig Config::validate(AppConfig config) {
    config.intensity = std::clamp(config.intensity, 0.0f, 1.0f);
    config.contrast = std::clamp(config.contrast, 0.55f, 1.6f);
    config.edgeWidth = std::clamp(config.edgeWidth, 4, 64);
    config.fpsCap = std::clamp(config.fpsCap, 15, 240);

    if (config.colorMode != "static" && config.colorMode != "reactive" && config.colorMode != "spectrum") {
        std::cout << "[AMBIENCE] Invalid color_mode — using reactive" << std::endl;
        config.colorMode = "reactive";
    }

    if (config.visualMode != "auto" &&
        config.visualMode != "soft_aura" &&
        config.visualMode != "spectrum_flow" &&
        config.visualMode != "beat_bloom" &&
        config.visualMode != "corner_hits") {
        std::cout << "[AMBIENCE] Invalid visual_mode — using auto" << std::endl;
        config.visualMode = "auto";
    }

    if (!config.sideTop && !config.sideRight && !config.sideBottom && !config.sideLeft) {
        std::cout << "[AMBIENCE] No sides enabled — enabling all sides" << std::endl;
        config.sideTop = true;
        config.sideRight = true;
        config.sideBottom = true;
        config.sideLeft = true;
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
    j["contrast"]      = config.contrast;
    j["edge_width"]    = config.edgeWidth;
    j["fps_cap"]       = config.fpsCap;
    j["color_mode"]    = config.colorMode;
    j["visual_mode"]   = config.visualMode;
    j["primary_color"] = config.primaryColor;
    j["side_top"]      = config.sideTop;
    j["side_right"]    = config.sideRight;
    j["side_bottom"]   = config.sideBottom;
    j["side_left"]     = config.sideLeft;
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
