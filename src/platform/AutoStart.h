#pragma once
#include <string>
#include <filesystem>

class AutoStart {
public:
    static bool enable(const std::string& execPath);
    static bool disable();
    static bool isEnabled();

private:
    static std::string getDesktopFilePath();
};
