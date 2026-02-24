#include "audio/AudioCapture.h"
#include "audio/FFTProcessor.h"
#include "renderer/Overlay.h"
#include "renderer/BandMapper.h"
#include "settings/Config.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>
#include <thread>
#include <iomanip>

int main() {
    std::cout << "loopin-ambience-mode starting..." << std::endl;

    // TODO Session 2: AudioCapture init
    // SESSION 2 TEST — remove in Session 5
    AudioCapture audioCapture;
    if (!audioCapture.start()) {
        std::cerr << "Failed to start audio capture." << std::endl;
        return 1;
    }

    std::cout << "SESSION 2 TEST: Capturing audio for 3 seconds..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
        std::vector<float> samples = audioCapture.readSamples();

        float sumSq = 0.0f;
        for (float s : samples) {
            sumSq += s * s;
        }
        float rms = std::sqrt(sumSq / samples.size());

        std::cout << "RMS: " << std::fixed << std::setprecision(4) << rms << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    audioCapture.stop();
    std::cout << "SESSION 2 TEST: Done." << std::endl;
    // SESSION 2 TEST — remove in Session 5

    // TODO Session 3: FFTProcessor init
    // TODO Session 4: Overlay init
    // TODO Session 5: Connect audio -> FFT -> BandMapper -> Overlay

    return 0;
}
