#include "FFTProcessor.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    FFTProcessor processor;
    const int N = 1024;
    const double Fs = 44100.0;
    const double frequency = 100.0; // 100Hz Sine wave

    std::vector<float> samples(N);
    for (int i = 0; i < N; ++i) {
        samples[i] = std::sin(2.0 * M_PI * frequency * i / Fs);
    }

    // Process multiple times to ensure EMA converges
    for (int i = 0; i < 20; ++i) {
        processor.process(samples);
    }

    float bass = processor.getBass();
    float mid = processor.getMid();
    float treble = processor.getTreble();

    std::cout << "Test 100Hz Sine Wave Results:" << std::endl;
    std::cout << "  Bass:   " << bass << std::endl;
    std::cout << "  Mid:    " << mid << std::endl;
    std::cout << "  Treble: " << treble << std::endl;

    bool passed = true;
    if (bass <= 0.1f) {
        std::cerr << "FAIL: Bass should be > 0.1, got " << bass << std::endl;
        passed = false;
    }
    if (mid >= 0.01f) {
        std::cerr << "FAIL: Mid should be < 0.01, got " << mid << std::endl;
        passed = false;
    }
    if (treble >= 0.01f) {
        std::cerr << "FAIL: Treble should be < 0.01, got " << treble << std::endl;
        passed = false;
    }

    if (!passed) {
        return EXIT_FAILURE;
    }

    std::cout << "FFTProcessor synthetic test PASSED!" << std::endl;
    return EXIT_SUCCESS;
}
