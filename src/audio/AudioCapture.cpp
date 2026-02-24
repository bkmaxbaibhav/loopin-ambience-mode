#include "AudioCapture.h"

AudioCapture::AudioCapture() {}
AudioCapture::~AudioCapture() { stop(); }

bool AudioCapture::start() {
    // TODO Session 2: initialize PortAudio loopback stream
    return false;
}

void AudioCapture::stop() {
    // TODO Session 2: stop and close PortAudio stream
}

bool AudioCapture::isRunning() const {
    return running_.load();
}

std::vector<float> AudioCapture::readSamples() {
    // TODO Session 2: read from ring buffer
    return {};
}
