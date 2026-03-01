#pragma once

// Maps raw FFT band values to visual parameters
// for the border shader uniforms
struct VisualParams {
    float bassIntensity   = 0.0f;  // → uBass uniform
    float midIntensity    = 0.0f;  // → uMid uniform
    float trebleIntensity = 0.0f;  // → uTreble uniform
    float hue             = 0.0f;  // → reactive color hue
    int   colorMode       = 0;     // 0=static 1=reactive 2=spectrum
    bool  isSilent        = true;
};

class BandMapper {
public:
    VisualParams map(float bass, float mid, float treble, bool isSilent = false);
};
