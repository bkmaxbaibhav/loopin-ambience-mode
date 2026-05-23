#include "AudioCapture.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <random>
#include <chrono>
#include <array>

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
        // Mixdown to mono based on channel count
        float mono = (channels_ == 2)
            ? (input[i * 2] + input[i * 2 + 1]) * 0.5f
            : input[i];

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

#ifdef __linux__
std::string AudioCapture::findPulseMonitorSource() const {
    if (const char* source = getenv("AMBIENCE_PULSE_SOURCE")) {
        if (strlen(source) > 0) return source;
    }

    FILE* pipe = popen("pactl list short sources", "r");
    if (!pipe) return "";

    std::array<char, 512> buffer{};
    std::string selected;
    while (fgets(buffer.data(), buffer.size(), pipe)) {
        std::istringstream line(buffer.data());
        std::string index;
        std::string name;
        line >> index >> name;
        if (name.find(".monitor") != std::string::npos) {
            selected = name;
            break;
        }
    }

    pclose(pipe);
    return selected;
}

bool AudioCapture::startPulseMonitor() {
    std::string source = findPulseMonitorSource();
    if (source.empty()) return false;

    for (char c : source) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) ||
              c == '_' || c == '-' || c == '.')) {
            std::cerr << "[AMBIENCE] Ignoring unsafe PulseAudio source name: "
                      << source << std::endl;
            return false;
        }
    }

    std::string command =
        "parec --device=" + source +
        " --format=s16le --rate=44100 --channels=2 2>/dev/null";

    pulsePipe_ = popen(command.c_str(), "r");
    if (!pulsePipe_) return false;

    channels_ = 2;
    running_.store(true);
    std::cout << "[AMBIENCE] PulseAudio monitor source: " << source << std::endl;

    pulseThread_ = std::thread([this]() {
        std::array<int16_t, 256 * 2> raw{};
        std::vector<float> samples(raw.size(), 0.0f);

        while (running_.load()) {
            size_t read = fread(raw.data(), sizeof(int16_t), raw.size(), pulsePipe_);
            if (read == 0) break;

            for (size_t i = 0; i < read; ++i) {
                samples[i] = static_cast<float>(raw[i]) / 32768.0f;
            }

            pushSamples(samples.data(), static_cast<unsigned long>(read / channels_));
        }

        running_.store(false);
    });

    return true;
}
#endif

bool AudioCapture::start() {
    if (running_.load()) return true;

#ifdef __linux__
    if (!getenv("AMBIENCE_DISABLE_PULSE") && startPulseMonitor()) {
        return true;
    }
#endif

    PaError err = Pa_Initialize();
    int inputDevice = -1;

    if (err == paNoError) {
        paInitialized_ = true;
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
            const PaDeviceInfo* info = Pa_GetDeviceInfo(inputDevice);
            channels_ = std::min(info->maxInputChannels, 2);
            if (channels_ < 1) channels_ = 1;
            inputParameters.channelCount = channels_;
            inputParameters.sampleFormat = paFloat32;
            inputParameters.suggestedLatency = info->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = &wasapiInfo;

            err = Pa_OpenStream(&stream_, &inputParameters, nullptr, SAMPLE_RATE,
                                paFramesPerBufferUnspecified, paNoFlag, paCallback, this);
        }
#else
        if (getenv("AMBIENCE_LIST_AUDIO")) {
            std::cout << "[AMBIENCE] Available audio input devices:" << std::endl;
            for (int i = 0; i < numDevices; ++i) {
                const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
                if (!info || info->maxInputChannels <= 0) continue;
                std::cout << "  [" << i << "] "
                          << (info->name ? info->name : "unknown")
                          << " channels=" << info->maxInputChannels << std::endl;
            }
        }

        const char* requestedDevice = getenv("AMBIENCE_AUDIO_DEVICE");
        if (requestedDevice && strlen(requestedDevice) > 0) {
            std::string requested = requestedDevice;
            std::transform(requested.begin(), requested.end(), requested.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            for (int i = 0; i < numDevices; ++i) {
                const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
                if (!info || info->maxInputChannels <= 0) continue;

                std::string name = info->name ? info->name : "";
                std::string lowerName = name;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                if (lowerName.find(requested) != std::string::npos) {
                    inputDevice = i;
                    break;
                }
            }
        }

        for (int i = 0; inputDevice == -1 && i < numDevices; ++i) {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
            if (info && info->maxInputChannels > 0) {
                std::string name = info->name ? info->name : "";
                std::string lowerName = name;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                               [](unsigned char c) { return std::tolower(c); });

                if (lowerName.find("monitor") != std::string::npos) {
                    inputDevice = i;
                    break;
                }
            }
        }

        if (inputDevice == -1 && getenv("AMBIENCE_ALLOW_MIC")) {
            inputDevice = Pa_GetDefaultInputDevice();
        }

        if (inputDevice != -1) {
            PaStreamParameters inputParameters;
            inputParameters.device = inputDevice;
            const PaDeviceInfo* info = Pa_GetDeviceInfo(inputDevice);
            channels_ = std::min(info->maxInputChannels, 2);
            if (channels_ < 1) channels_ = 1;
            inputParameters.channelCount = channels_;
            inputParameters.sampleFormat = paFloat32;
            inputParameters.suggestedLatency = info->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = nullptr;

            err = Pa_OpenStream(&stream_, &inputParameters, nullptr, SAMPLE_RATE,
                                paFramesPerBufferUnspecified, paNoFlag, paCallback, this);
            if (err == paNoError) {
                std::cout << "[AMBIENCE] Audio capture device: "
                          << (info->name ? info->name : "unknown") << std::endl;
            } else {
                std::cerr << "[AMBIENCE] Failed to open audio capture device: "
                          << Pa_GetErrorText(err) << std::endl;
                stream_ = nullptr;
            }
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
    std::cout << "[AMBIENCE] WARNING: No audio loopback device found." << std::endl;
    std::cout << "[AMBIENCE] Running in SIMULATION MODE — border will animate with fake data." << std::endl;
    std::cout << "[AMBIENCE] On Linux ensure PulseAudio or PipeWire monitor device is available." << std::endl;
    std::cout << "[AMBIENCE] Run: pactl list sources | grep monitor" << std::endl;

    running_.store(true);
    channels_ = 2; // Simulation uses stereo mixdown in this implementation
    simulationThread_ = std::thread([this]() {
        std::mt19937 gen(42);
        std::uniform_real_distribution<float> dist(-0.03f, 0.03f);
        int simCount = 0;
        while (running_.load()) {
            std::vector<float> mockBuffer(128 * 2); // 128 frames, stereo
            for (auto& s : mockBuffer) s = dist(gen);
            pushSamples(mockBuffer.data(), 128);

            if (++simCount % 5000 == 0) {
                std::cout << "[AMBIENCE] Still in simulation mode" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(2)); // ~64000 samples/sec, close enough to 44.1k
        }
    });

    return true;
}

void AudioCapture::stop() {
    if (!running_.load()) return;

    running_.store(false);
    if (pulsePipe_) {
        pclose(pulsePipe_);
        pulsePipe_ = nullptr;
    }
    if (pulseThread_.joinable()) {
        pulseThread_.join();
    }
    if (simulationThread_.joinable()) {
        simulationThread_.join();
    }
    if (stream_) {
        Pa_StopStream(stream_);
        Pa_CloseStream(stream_);
        stream_ = nullptr;
    }
    if (paInitialized_) {
        Pa_Terminate();
        paInitialized_ = false;
    }
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
