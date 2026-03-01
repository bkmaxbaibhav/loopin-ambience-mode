#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

static ColorRGB hexToRGB(const std::string& hex) {
    std::string s = hex;
    if (!s.empty() && s[0] == '#') s = s.substr(1);

    if (s.length() != 6) return {1.0f, 1.0f, 1.0f};

    unsigned int r, g, b;
    std::stringstream ssR, ssG, ssB;
    ssR << std::hex << s.substr(0, 2);
    ssG << std::hex << s.substr(2, 2);
    ssB << std::hex << s.substr(4, 2);
    ssR >> r; ssG >> g; ssB >> b;

    return {r / 255.0f, g / 255.0f, b / 255.0f};
}

AppConfig Config::load(const std::string& path) {
    AppConfig config;
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cerr << "Could not open config file: " << path << ". Using defaults." << std::endl;
        return config;
    }

    try {
        json data = json::parse(f);

        if (data.contains("intensity")) config.intensity = data["intensity"];
        if (data.contains("edge_width")) config.edgeWidth = data["edge_width"];
        if (data.contains("fps_cap")) config.fpsCap = data["fps_cap"];

        if (data.contains("color_mode")) {
            config.colorModeStr = data["color_mode"];
            if (config.colorModeStr == "static") config.colorMode = 0;
            else if (config.colorModeStr == "reactive") config.colorMode = 1;
            else if (config.colorModeStr == "spectrum") config.colorMode = 2;
            else {
                config.colorMode = 1; // fallback
                config.colorModeStr = "reactive";
            }
        }

        if (data.contains("primary_color")) {
            config.primaryColor = data["primary_color"];
            config.primaryColorRGB = hexToRGB(config.primaryColor);
        }

        if (data.contains("focus_mode")) config.focusMode = data["focus_mode"];
        if (data.contains("autostart")) config.autostart = data["autostart"];
        if (data.contains("all_monitors")) config.allMonitors = data["all_monitors"];

    } catch (const std::exception& e) {
        std::cerr << "Error parsing config: " << e.what() << ". Using defaults." << std::endl;
    }

    return config;
}

void Config::save(const AppConfig& config,
                  const std::string& path) {
    // TODO Session 7: serialize AppConfig to JSON file
}
