#include "FFTProcessor.h"

FFTProcessor::FFTProcessor() {}
FFTProcessor::~FFTProcessor() {}

void FFTProcessor::process(const std::vector<float>& samples) {
    // TODO Session 3: apply Hann window, run FFTW3, extract bands
}

float FFTProcessor::getBass()    const { return bass_;   }
float FFTProcessor::getMid()     const { return mid_;    }
float FFTProcessor::getTreble()  const { return treble_; }
bool  FFTProcessor::isSilent()   const { return silent_; }
