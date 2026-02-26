#pragma once
#include <vector>
#include <fftw3.h>
#include <chrono>

// Processes raw PCM samples using FFTW3
// Extracts 3 frequency bands:
//   Bass    (bins 0–5)
//   Mid     (bins 6–92)
//   Treble  (bins 93–464)
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

    // FFTW3 members
    fftwf_plan plan_;
    float* fftIn_;
    fftwf_complex* fftOut_;

    // Hann window
    std::vector<float> hannWindow_;

    // EMA smoothing
    const float alpha_ = 0.3f;

    // Silence detection
    std::chrono::steady_clock::time_point lastSoundTime_;
};
