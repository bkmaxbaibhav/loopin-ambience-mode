#include "FFTProcessor.h"
#include <cmath>
#include <algorithm>
#include <numeric>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FFTProcessor::FFTProcessor() {
    // FFTW3 Initialization (1024 real input, 513 complex output)
    fftIn_ = fftwf_alloc_real(1024);
    fftOut_ = fftwf_alloc_complex(513);
    plan_ = fftwf_plan_dft_r2c_1d(1024, fftIn_, fftOut_, FFTW_MEASURE);

    // Precompute Hann window
    hannWindow_.resize(1024);
    for (int i = 0; i < 1024; ++i) {
        hannWindow_[i] = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / 1023.0f));
    }

    // Initialize silence detection to "already silent"
    auto now = std::chrono::steady_clock::now();
    lastSoundTime_ = now - std::chrono::seconds(4);
    silent_ = true;
}

FFTProcessor::~FFTProcessor() {
    fftwf_destroy_plan(plan_);
    fftwf_free(fftIn_);
    fftwf_free(fftOut_);
}

void FFTProcessor::process(const std::vector<float>& samples) {
    if (samples.size() < 1024) return;

    // 1. Calculate RMS for silence detection
    float sumSq = 0.0f;
    for (int i = 0; i < 1024; ++i) {
        sumSq += samples[i] * samples[i];
    }
    float rms = std::sqrt(sumSq / 1024.0f);

    auto now = std::chrono::steady_clock::now();
    if (rms >= 0.01f) {
        lastSoundTime_ = now;
        silent_ = false;
    } else {
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSoundTime_).count() >= 3) {
            silent_ = true;
        }
    }

    // 2. Apply Hann window
    for (int i = 0; i < 1024; ++i) {
        fftIn_[i] = samples[i] * hannWindow_[i];
    }

    // 3. Execute FFT
    fftwf_execute(plan_);

    // 4. Compute magnitude spectrum and map to bands
    // Normalized magnitude: |X(k)| / (N/2)
    auto computeBandAverage = [&](int startBin, int endBin) {
        float sum = 0.0f;
        for (int i = startBin; i <= endBin; ++i) {
            float real = fftOut_[i][0];
            float imag = fftOut_[i][1];
            float mag = std::sqrt(real * real + imag * imag) / 512.0f;
            sum += mag;
        }
        return sum / (endBin - startBin + 1);
    };

    float currentBass = computeBandAverage(0, 5);
    float currentMid = computeBandAverage(6, 92);
    float currentTreble = computeBandAverage(93, 464);

    // 5. EMA Smoothing and clamping
    auto applyEMA = [&](float current, float& smoothed) {
        smoothed = alpha_ * current + (1.0f - alpha_) * smoothed;
        smoothed = std::clamp(smoothed, 0.0f, 1.0f);
    };

    applyEMA(currentBass, bass_);
    applyEMA(currentMid, mid_);
    applyEMA(currentTreble, treble_);
}

float FFTProcessor::getBass()    const { return bass_;   }
float FFTProcessor::getMid()     const { return mid_;    }
float FFTProcessor::getTreble()  const { return treble_; }
bool  FFTProcessor::isSilent()   const { return silent_; }
