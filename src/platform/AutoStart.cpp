#include "AutoStart.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

std::string AutoStart::getDesktopFilePath() {
    const char* home = getenv("HOME");
    if (!home) return "";
    return std::string(home) + "/.config/autostart/loopin-ambience-mode.desktop";
}

bool AutoStart::enable(const std::string& execPath) {
    std::string path = getDesktopFilePath();
    if (path.empty()) return false;

    namespace fs = std::filesystem;
    try {
        fs::path dir = fs::path(path).parent_path();
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }

        std::ofstream file(path);
        if (!file.is_open()) return false;

        file << "[Desktop Entry]\n"
             << "Type=Application\n"
             << "Name=Loopin Ambience Mode\n"
             << "Comment=Audio reactive screen border\n"
             << "Exec=" << execPath << "\n"
             << "Icon=loopin-ambience-mode\n"
             << "Hidden=false\n"
             << "NoDisplay=false\n"
             << "X-GNOME-Autostart-enabled=true\n";

        std::cout << "[AMBIENCE] Auto-start enabled" << std::endl;
        return true;
    } catch (...) {
        return false;
    }
}

bool AutoStart::disable() {
    std::string path = getDesktopFilePath();
    if (path.empty()) return false;

    if (std::filesystem::exists(path)) {
        if (std::filesystem::remove(path)) {
            std::cout << "[AMBIENCE] Auto-start disabled" << std::endl;
            return true;
        }
    }
    return false;
}

bool AutoStart::isEnabled() {
    std::string path = getDesktopFilePath();
    if (path.empty()) return false;
    return std::filesystem::exists(path);
}
