#pragma once
#include <string>

// All user-configurable settings
// Loaded from the per-user config file at startup.
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
    bool        overlayVisible = true;
    bool        partyMode     = false;
    bool        allMonitors   = false;
    bool        debugMode     = false;
    std::string audioSource   = "";
};

class Config {
public:
    static std::string userConfigPath();
    static AppConfig loadOrCreateUserConfig(const std::string& seedPath);
    static AppConfig load(const std::string& path);
    static void      save(const AppConfig& config,
                          const std::string& path);
private:
    static AppConfig validate(AppConfig config);
};
