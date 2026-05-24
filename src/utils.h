#pragma once
#include <string>
#include <sstream>

struct ColorRGB {
    float r, g, b;
};

inline int colorModeFromString(const std::string& mode) {
    if (mode == "static")   return 0;
    if (mode == "reactive") return 1;
    if (mode == "spectrum") return 2;
    if (mode == "auto")     return 3;
    return 1; // Default to reactive
}

inline int visualModeFromString(const std::string& mode) {
    if (mode == "auto")          return 0;
    if (mode == "soft_aura")     return 1;
    if (mode == "spectrum_flow") return 2;
    if (mode == "beat_bloom")    return 3;
    if (mode == "corner_hits")   return 4;
    if (mode == "neon_rails")    return 5;
    return 0;
}

inline ColorRGB hexToRGB(const std::string& hex) {
    std::string s = hex;
    if (!s.empty() && s[0] == '#') s = s.substr(1);

    if (s.length() != 6) return {0.482f, 0.184f, 1.0f}; // Default #7B2FFF

    unsigned int r, g, b;
    std::stringstream ssR, ssG, ssB;
    ssR << std::hex << s.substr(0, 2);
    ssG << std::hex << s.substr(2, 2);
    ssB << std::hex << s.substr(4, 2);

    if (!(ssR >> r)) r = 123;
    if (!(ssG >> g)) g = 47;
    if (!(ssB >> b)) b = 255;

    return {r / 255.0f, g / 255.0f, b / 255.0f};
}
