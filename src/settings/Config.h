#pragma once
#include <string>

struct ColorRGB {
    float r, g, b;
};

// All user-configurable settings
// Loaded from config/default.json at startup
struct AppConfig {
    float       intensity     = 0.8f;
    int         edgeWidth     = 12;
    int         fpsCap        = 60;
    int         colorMode     = 1; // 0=static 1=reactive 2=spectrum
    std::string colorModeStr  = "reactive";
    std::string primaryColor  = "#7B2FFF";
    ColorRGB    primaryColorRGB = {0.482f, 0.184f, 1.0f};
    bool        focusMode     = true;
    bool        autostart     = false;
    bool        allMonitors   = false;
    bool        debugMode     = false;
};

class Config {
public:
    static AppConfig load(const std::string& path);
    static void      save(const AppConfig& config,
                          const std::string& path);
};
