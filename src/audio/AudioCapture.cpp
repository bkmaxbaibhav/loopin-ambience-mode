#include "AudioCapture.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <random>
#include <chrono>

#ifdef _WIN32
#include <pa_win_wasapi.h>
#endif

AudioCapture::AudioCapture() {
    ringBuffer_.resize(RING_BUFFER_SIZE, 0.0f);
}

AudioCapture::~AudioCapture() {
    stop();
}

int AudioCapture::paCallback(const void* inputBuffer, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData) {
    auto* self = static_cast<AudioCapture*>(userData);
    if (inputBuffer && self->running_.load()) {
        self->pushSamples(static_cast<const float*>(inputBuffer), framesPerBuffer);
    }
    return paContinue;
}

void AudioCapture::pushSamples(const float* input, unsigned long frames) {
    std::lock_guard<std::mutex> lock(bufferMutex_);
    for (unsigned long i = 0; i < frames; ++i) {
        // Stereo to mono mixdown (assuming 2 channels)
        float mono = (input[i * 2] + input[i * 2 + 1]) * 0.5f;

        ringBuffer_[writePos_] = mono;
        writePos_ = (writePos_ + 1) % RING_BUFFER_SIZE;

        if (count_ < RING_BUFFER_SIZE) {
            count_++;
        } else {
            // Overwrite policy: advance readPos to keep up with writePos
            readPos_ = (readPos_ + 1) % RING_BUFFER_SIZE;
        }
    }
}

bool AudioCapture::start() {
    if (running_.load()) return true;

    PaError err = Pa_Initialize();
    bool paInitialized = (err == paNoError);
    int inputDevice = -1;

    if (paInitialized) {
        int numDevices = Pa_GetDeviceCount();

#ifdef _WIN32
        inputDevice = Pa_GetDefaultOutputDevice();
        if (inputDevice != paNoDevice) {
            static PaWasapiStreamInfo wasapiInfo;
            wasapiInfo.size = sizeof(PaWasapiStreamInfo);
            wasapiInfo.hostApiType = paWinWasapi;
            wasapiInfo.version = 1;
            wasapiInfo.flags = paWinWasapiLoopback;

            PaStreamParameters inputParameters;
            inputParameters.device = inputDevice;
            inputParameters.channelCount = 2;
            inputParameters.sampleFormat = paFloat32;
            inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDevice)->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = &wasapiInfo;

            err = Pa_OpenStream(&stream_, &inputParameters, nullptr, SAMPLE_RATE,
                                paFramesPerBufferUnspecified, paNoFlag, paCallback, this);
        }
#else
        for (int i = 0; i < numDevices; ++i) {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
            if (info->maxInputChannels > 0) {
                std::string name = info->name;
                if (name.find("monitor") != std::string::npos) {
                    inputDevice = i;
                    break;
                }
            }
        }

        if (inputDevice == -1) {
            inputDevice = Pa_GetDefaultInputDevice();
        }

        if (inputDevice != -1) {
            PaStreamParameters inputParameters;
            inputParameters.device = inputDevice;
            inputParameters.channelCount = 2;
            inputParameters.sampleFormat = paFloat32;
            inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDevice)->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = nullptr;

            err = Pa_OpenStream(&stream_, &inputParameters, nullptr, SAMPLE_RATE,
                                paFramesPerBufferUnspecified, paNoFlag, paCallback, this);
        }
#endif

        if (stream_) {
            err = Pa_StartStream(stream_);
            if (err == paNoError) {
                running_.store(true);
                return true;
            }
        }
    }

    // Fallback to simulation if no audio device is available
    std::cout << "Starting audio capture in SIMULATION mode (no hardware device found)." << std::endl;
    running_.store(true);
    simulationThread_ = std::thread([this]() {
        std::mt19937 gen(42);
        std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
        while (running_.load()) {
            std::vector<float> mockBuffer(128 * 2); // 128 frames, stereo
            for (auto& s : mockBuffer) s = dist(gen);
            pushSamples(mockBuffer.data(), 128);
            std::this_thread::sleep_for(std::chrono::milliseconds(2)); // ~64000 samples/sec, close enough to 44.1k
        }
    });

    return true;
}

void AudioCapture::stop() {
    if (!running_.load()) return;

    running_.store(false);
    if (simulationThread_.joinable()) {
        simulationThread_.join();
    }
    if (stream_) {
        Pa_StopStream(stream_);
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    Pa_Terminate();
}

bool AudioCapture::isRunning() const {
    return running_.load();
}

std::vector<float> AudioCapture::readSamples() {
    std::vector<float> samples(READ_SIZE, 0.0f);
    std::lock_guard<std::mutex> lock(bufferMutex_);

    size_t toRead = std::min(count_, READ_SIZE);
    for (size_t i = 0; i < toRead; ++i) {
        samples[i] = ringBuffer_[readPos_];
        readPos_ = (readPos_ + 1) % RING_BUFFER_SIZE;
    }
    count_ -= toRead;

    return samples;
}
