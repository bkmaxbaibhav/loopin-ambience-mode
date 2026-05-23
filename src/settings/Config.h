#pragma once
#include <string>

// All user-configurable settings
// Loaded from config/default.json at startup
struct AppConfig {
    float       intensity     = 0.8f;
    float       contrast      = 1.0f;
    int         edgeWidth     = 12;
    int         fpsCap        = 60;
    std::string colorMode     = "reactive";
    std::string visualMode    = "auto";
    std::string primaryColor  = "#7B2FFF";
    bool        sideTop       = true;
    bool        sideRight     = true;
    bool        sideBottom    = true;
    bool        sideLeft      = true;
    bool        surroundSync  = false;
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
private:
    static AppConfig validate(AppConfig config);
};
