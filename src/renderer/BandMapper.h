#pragma once

// Maps raw FFT band values to visual parameters
// for the border shader uniforms
struct VisualParams {
    float bassIntensity   = 0.0f;  // → uBass uniform
    float midIntensity    = 0.0f;  // → uMid uniform
    float trebleIntensity = 0.0f;  // → uTreble uniform
    float leftIntensity   = 0.0f;
    float rightIntensity  = 0.0f;
    float beatPulse       = 0.0f;  // short kick/beat transient
    float genreWarmth     = 1.0f;  // stable palette warmth bias
    float genrePresence   = 1.0f;  // stable upper-mid/treble presence bias
    float genreConfidence = 0.0f;
    float hue             = 0.0f;  // → reactive color hue
    int   colorMode       = 0;     // 0=static 1=reactive 2=spectrum 3=auto
    int   autoVisualMode  = 1;     // shader visual mode chosen only when config visual_mode=auto
    bool  isSilent        = true;
};

class BandMapper {
public:
    BandMapper(int colorMode = 1) : colorMode_(colorMode) {}
    VisualParams map(float bass,
                     float mid,
                     float treble,
                     bool isSilent = false,
                     float leftLevel = 0.0f,
                     float rightLevel = 0.0f,
                     bool surroundSync = false);
    void setColorMode(int mode) { colorMode_ = mode; }
    void setPartyMode(bool enabled) { partyMode_ = enabled; }

private:
    int colorMode_ = 1;
    bool partyMode_ = false;
    int detectedGenre_ = 0;
    int candidateGenre_ = 0;
    float candidateHold_ = 0.0f;
    float genreConfidence_ = 0.0f;
    float smoothHue_ = 0.0f;
    float bassFloor_ = 0.0f;
    float avgBass_ = 0.0f;
    float avgMid_ = 0.0f;
    float avgTreble_ = 0.0f;
    float beatPulse_ = 0.0f;
    float dropEnergy_ = 0.0f;
    float dropHold_ = 0.0f;
    int autoVisualMode_ = 1;
    float peakVolume_ = 0.01f;
};
