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

    // SESSION 3 TEST — remove in Session 5
    AudioCapture audioCapture;
    FFTProcessor fftProcessor;
    if (!audioCapture.start()) {
        std::cerr << "Failed to start audio capture." << std::endl;
        return 1;
    }

    std::cout << "SESSION 3 TEST: Processing audio for 5 seconds..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(5)) {
        std::vector<float> samples = audioCapture.readSamples();
        fftProcessor.process(samples);

        std::cout << std::fixed << std::setprecision(3)
                  << "Bass: " << fftProcessor.getBass()
                  << " | Mid: " << fftProcessor.getMid()
                  << " | Treble: " << fftProcessor.getTreble()
                  << " | Silent: " << (fftProcessor.isSilent() ? "Yes" : "No")
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    audioCapture.stop();
    std::cout << "SESSION 3 TEST: Done." << std::endl;
    // SESSION 3 TEST — remove in Session 5

    // TODO Session 4: Overlay init
    // TODO Session 5: Connect audio -> FFT -> BandMapper -> Overlay

    return 0;
}
