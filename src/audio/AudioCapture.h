#pragma once
#include <atomic>
#include <cstdio>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <portaudio.h>

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
    float getLeftLevel() const;
    float getRightLevel() const;

private:
    static int paCallback(const void* inputBuffer, void* outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void* userData);

    void pushSamples(const float* input, unsigned long frames);
#ifdef __linux__
    bool startPulseMonitor();
    std::string findPulseMonitorSource() const;
#endif

    std::atomic<bool> running_{ false };
    PaStream* stream_{ nullptr };
    bool paInitialized_ = false;
    FILE* pulsePipe_{ nullptr };
    int channels_ = 2;
    std::thread simulationThread_;
    std::thread pulseThread_;

    // Ring Buffer settings
    static constexpr size_t RING_BUFFER_SIZE = 8192;
    static constexpr size_t READ_SIZE = 1024;
    static constexpr double SAMPLE_RATE = 44100.0;

    std::vector<float> ringBuffer_;
    float leftLevel_{ 0.0f };
    float rightLevel_{ 0.0f };
    size_t writePos_{ 0 };
    size_t readPos_{ 0 };
    size_t count_{ 0 };
    mutable std::mutex bufferMutex_;
};
