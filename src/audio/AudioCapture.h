#pragma once
#include <atomic>
#include <vector>

// Captures system audio output (loopback) via PortAudio
// Linux:   PipeWire / PulseAudio loopback device
// Windows: WASAPI loopback (render endpoint)
class AudioCapture {
public:
    AudioCapture();
    ~AudioCapture();

    bool start();
    void stop();
    bool isRunning() const;
    std::vector<float> readSamples();

private:
    std::atomic<bool> running_{ false };
    // PaStream* stream_ will be added in Session 2
};
