#pragma once
#include <vector>

// Processes raw PCM samples using FFTW3
// Extracts 3 frequency bands:
//   Bass    20–250Hz   → bottom edge
//   Mid     250–4kHz   → left & right edges
//   Treble  4kHz–20kHz → top edge
class FFTProcessor {
public:
    FFTProcessor();
    ~FFTProcessor();

    void process(const std::vector<float>& samples);

    float getBass()   const;
    float getMid()    const;
    float getTreble() const;
    bool  isSilent()  const;

private:
    float bass_   = 0.0f;
    float mid_    = 0.0f;
    float treble_ = 0.0f;
    bool  silent_ = true;
    // fftw_plan plan_ will be added in Session 3
};
