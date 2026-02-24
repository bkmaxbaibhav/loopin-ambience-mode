#pragma once
#include <string>

// All user-configurable settings
// Loaded from config/default.json at startup
struct AppConfig {
    float       intensity     = 0.8f;
    int         edgeWidth     = 12;
    int         fpsCap        = 60;
    std::string colorMode     = "reactive";
    std::string primaryColor  = "#7B2FFF";
    bool        focusMode     = true;
    bool        autostart     = false;
    bool        allMonitors   = false;
};

class Config {
public:
    static AppConfig load(const std::string& path);
    static void      save(const AppConfig& config,
                          const std::string& path);
};
