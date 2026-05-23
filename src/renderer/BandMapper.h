#pragma once

// Maps raw FFT band values to visual parameters
// for the border shader uniforms
struct VisualParams {
    float bassIntensity   = 0.0f;  // → uBass uniform
    float midIntensity    = 0.0f;  // → uMid uniform
    float trebleIntensity = 0.0f;  // → uTreble uniform
    float beatPulse       = 0.0f;  // short kick/beat transient
    float hue             = 0.0f;  // → reactive color hue
    int   colorMode       = 0;     // 0=static 1=reactive 2=spectrum
    bool  isSilent        = true;
};

class BandMapper {
public:
    BandMapper(int colorMode = 1) : colorMode_(colorMode) {}
    VisualParams map(float bass, float mid, float treble, bool isSilent = false);
    void setColorMode(int mode) { colorMode_ = mode; }

private:
    int colorMode_ = 1;
    float smoothHue_ = 0.0f;
    float bassFloor_ = 0.0f;
    float beatPulse_ = 0.0f;
};
