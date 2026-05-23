#include "BandMapper.h"
#include <algorithm>
#include <cmath>

namespace {
struct GenreSignature {
    float bassMin, bassMax;
    float trebleMin, trebleMax;
    float midMin, midMax;
    float warmthMin, warmthMax;
    float presenceMin, presenceMax;
    float hueBias;
    float warmth;
    float presence;
};

constexpr GenreSignature kGenres[] = {
    // Neutral fallback.
    {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 8.0f, 0.0f, 3.0f, 0.72f, 1.0f, 1.0f},
    // EDM / House
    {0.45f, 0.60f, 0.15f, 0.25f, 0.15f, 0.25f, 2.5f, 4.0f, 0.3f, 0.5f, 0.58f, 0.7f, 0.9f},
    // Lo-fi / Chill
    {0.30f, 0.45f, 0.08f, 0.15f, 0.25f, 0.35f, 3.0f, 5.0f, 0.6f, 0.9f, 0.08f, 1.25f, 0.75f},
    // Rock / Metal
    {0.25f, 0.40f, 0.20f, 0.35f, 0.25f, 0.40f, 1.2f, 2.0f, 0.8f, 1.2f, 0.96f, 1.1f, 1.15f},
    // Jazz
    {0.20f, 0.35f, 0.20f, 0.30f, 0.30f, 0.45f, 1.0f, 1.8f, 1.0f, 1.5f, 0.47f, 1.05f, 1.25f},
    // Classical
    {0.10f, 0.25f, 0.25f, 0.40f, 0.35f, 0.50f, 0.6f, 1.2f, 1.5f, 2.5f, 0.13f, 0.85f, 1.45f},
    // Hip Hop / Trap
    {0.50f, 0.65f, 0.10f, 0.20f, 0.15f, 0.25f, 4.0f, 6.0f, 0.3f, 0.5f, 0.78f, 1.35f, 0.75f},
    // Acoustic
    {0.15f, 0.30f, 0.20f, 0.35f, 0.35f, 0.50f, 0.8f, 1.5f, 1.2f, 2.0f, 0.10f, 0.95f, 1.3f},
    // Pop
    {0.30f, 0.45f, 0.20f, 0.30f, 0.25f, 0.40f, 1.5f, 2.5f, 0.7f, 1.1f, 0.88f, 1.0f, 1.05f}
};

float rangeDistance(float value, float minValue, float maxValue) {
    if (value >= minValue && value <= maxValue) return 0.0f;
    return value < minValue ? minValue - value : value - maxValue;
}

float scoreGenre(const GenreSignature& genre,
                 float bassRatio,
                 float trebleRatio,
                 float midRatio,
                 float warmth,
                 float presence) {
    float distance =
        rangeDistance(bassRatio, genre.bassMin, genre.bassMax) * 2.2f +
        rangeDistance(trebleRatio, genre.trebleMin, genre.trebleMax) * 1.7f +
        rangeDistance(midRatio, genre.midMin, genre.midMax) * 1.5f +
        rangeDistance(warmth, genre.warmthMin, genre.warmthMax) * 0.14f +
        rangeDistance(presence, genre.presenceMin, genre.presenceMax) * 0.32f;

    return std::clamp(1.0f - distance, 0.0f, 1.0f);
}
}

VisualParams BandMapper::map(float bass,
                              float mid,
                              float treble,
                              bool isSilent,
                              float leftLevel,
                              float rightLevel,
                              bool surroundSync) {
    VisualParams params;
    params.isSilent = isSilent;

    auto liftBand = [](float value, float gain) {
        return std::clamp(std::pow(std::max(0.0f, value * gain), 0.62f), 0.0f, 1.0f);
    };

    // PortAudio/Pulse monitor FFT values are small in normal desktop playback,
    // so compress the range before sending it to the visual layer.
    params.bassIntensity   = liftBand(bass,   10.0f); // bottom edge
    params.midIntensity    = liftBand(mid,    24.0f); // side edges
    params.trebleIntensity = liftBand(treble, 80.0f); // top edge
    params.leftIntensity = params.midIntensity;
    params.rightIntensity = params.midIntensity;

    if (surroundSync) {
        float stereoTotal = leftLevel + rightLevel + 0.0001f;
        float leftShare = leftLevel / stereoTotal;
        float rightShare = rightLevel / stereoTotal;
        float stereoEnergy = liftBand((leftLevel + rightLevel) * 0.5f, 18.0f);
        params.leftIntensity = std::clamp(stereoEnergy * std::pow(leftShare * 2.0f, 1.25f), 0.0f, 1.0f);
        params.rightIntensity = std::clamp(stereoEnergy * std::pow(rightShare * 2.0f, 1.25f), 0.0f, 1.0f);
    }

    bassFloor_ = 0.985f * bassFloor_ + 0.015f * bass;
    float bassLift = std::max(0.0f, bass - bassFloor_);
    float beatHit = std::clamp(bassLift * 9.0f + params.bassIntensity * 0.45f, 0.0f, 1.0f);
    beatPulse_ = std::max(beatHit, beatPulse_ * 0.86f);
    params.beatPulse = isSilent ? 0.0f : beatPulse_;

    avgBass_ = 0.992f * avgBass_ + 0.008f * bass;
    avgMid_ = 0.992f * avgMid_ + 0.008f * mid;
    avgTreble_ = 0.992f * avgTreble_ + 0.008f * treble;

    float total = avgBass_ + avgMid_ + avgTreble_ + 0.0001f;
    float bassRatio = avgBass_ / total;
    float midRatio = avgMid_ / total;
    float trebleRatio = avgTreble_ / total;
    float warmth = avgBass_ / (avgTreble_ + 0.0001f);
    float presence = avgTreble_ / (avgBass_ + 0.0001f);

    int bestGenre = detectedGenre_;
    float bestScore = 0.0f;
    for (int i = 1; i < static_cast<int>(sizeof(kGenres) / sizeof(kGenres[0])); ++i) {
        float score = scoreGenre(kGenres[i], bassRatio, trebleRatio, midRatio, warmth, presence);
        if (score > bestScore) {
            bestScore = score;
            bestGenre = i;
        }
    }

    if (bestScore >= 0.62f) {
        if (bestGenre == candidateGenre_) {
            candidateHold_ += 1.0f / 60.0f;
        } else {
            candidateGenre_ = bestGenre;
            candidateHold_ = 0.0f;
        }

        if (candidateHold_ >= 5.0f || detectedGenre_ == 0) {
            detectedGenre_ = candidateGenre_;
            genreConfidence_ = 0.96f * genreConfidence_ + 0.04f * bestScore;
        }
    } else {
        genreConfidence_ *= 0.995f;
    }

    // 2. Compute dominant frequency band for Reactive Mode target hue
    float targetHue = 0.0f;
    if (bass > mid && bass > treble) {
        targetHue = 0.0f;  // warm red / orange
    } else if (mid > bass && mid > treble) {
        targetHue = 0.33f; // green / cyan
    } else if (treble >= bass && treble >= mid) {
        targetHue = 0.66f; // blue / violet
    }

    // 3. Apply EMA smoothing (Ticket 2, Step 5)
    // smoothHue_ = 0.05 * targetHue + 0.95 * smoothHue_
    const GenreSignature& genre = kGenres[detectedGenre_];
    float genreInfluence = std::clamp(genreConfidence_, 0.0f, 0.75f);
    targetHue = (1.0f - genreInfluence) * targetHue + genreInfluence * genre.hueBias;
    smoothHue_ = 0.035f * targetHue + 0.965f * smoothHue_;

    params.hue = smoothHue_;
    params.genreWarmth = genre.warmth;
    params.genrePresence = genre.presence;
    params.genreConfidence = genreConfidence_;
    params.colorMode = colorMode_;

    return params;
}
