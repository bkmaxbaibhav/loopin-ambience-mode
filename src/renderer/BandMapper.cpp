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
    float partyGain = partyMode_ ? 1.65f : 1.0f;
    params.bassIntensity   = liftBand(bass,   10.0f * partyGain); // bottom edge
    params.midIntensity    = liftBand(mid,    24.0f * partyGain); // side edges
    params.trebleIntensity = liftBand(treble, 80.0f * partyGain); // top edge
    params.leftIntensity = params.midIntensity;
    params.rightIntensity = params.midIntensity;

    if (surroundSync) {
        float stereoTotal = leftLevel + rightLevel + 0.0001f;
        float leftShare = leftLevel / stereoTotal;
        float rightShare = rightLevel / stereoTotal;
        float stereoEnergy = liftBand((leftLevel + rightLevel) * 0.5f, 18.0f * partyGain);
        params.leftIntensity = std::clamp(stereoEnergy * std::pow(leftShare * 2.0f, 1.25f), 0.0f, 1.0f);
        params.rightIntensity = std::clamp(stereoEnergy * std::pow(rightShare * 2.0f, 1.25f), 0.0f, 1.0f);
    }

    bassFloor_ = 0.985f * bassFloor_ + 0.015f * bass;
    float bassLift = std::max(0.0f, bass - bassFloor_);
    float beatHit = std::clamp(bassLift * (partyMode_ ? 14.0f : 6.0f) + params.bassIntensity * (partyMode_ ? 0.62f : 0.28f), 0.0f, 1.0f);
    beatPulse_ = std::max(beatHit, beatPulse_ * (partyMode_ ? 0.80f : 0.93f));
    params.beatPulse = isSilent ? 0.0f : beatPulse_;

    float broadEnergy = std::clamp(
        params.bassIntensity * 0.55f +
        params.midIntensity * 0.24f +
        params.trebleIntensity * 0.21f,
        0.0f,
        1.0f
    );
    float transient = std::clamp(bassLift * (partyMode_ ? 18.0f : 10.0f), 0.0f, 1.0f);
    float dropHit = std::clamp(transient * 0.62f + beatPulse_ * 0.28f + broadEnergy * 0.26f, 0.0f, 1.0f);
    dropEnergy_ = std::max(dropHit, dropEnergy_ * (partyMode_ ? 0.90f : 0.955f));

    float dropThreshold = partyMode_ ? 0.54f : 0.66f;
    float railThreshold = partyMode_ ? 0.74f : 0.84f;

    // Decrement hold timer every frame regardless of current energy.
    // This ensures the timer always counts down and modes can transition.
    if (dropHold_ > 0.0f) {
        dropHold_ -= 1.0f / 60.0f;
        if (dropHold_ < 0.0f) dropHold_ = 0.0f;
    }

    if (!isSilent && dropEnergy_ > railThreshold) {
        // Neon Rails for very strong drops — always override hold.
        autoVisualMode_ = 5;
        dropHold_ = partyMode_ ? 2.2f : 1.6f;
    } else if (!isSilent && dropEnergy_ > dropThreshold) {
        // Beat Bloom for EDM-style lift/drop — only upgrade, don't extend mode 5.
        if (autoVisualMode_ != 5) {
            autoVisualMode_ = 3;
        }
        if (dropHold_ < (partyMode_ ? 1.8f : 1.2f)) {
            dropHold_ = partyMode_ ? 1.8f : 1.2f;
        }
    } else if (!isSilent && broadEnergy > 0.42f && params.trebleIntensity > params.bassIntensity * 0.72f) {
        // Spectrum Flow for brighter/high-energy sections — only set if no higher mode is held.
        if (autoVisualMode_ != 5 && autoVisualMode_ != 3) {
            autoVisualMode_ = 2;
        }
        // Do NOT reset dropHold_ here — let the existing hold run out naturally.
    } else if (dropHold_ <= 0.0f) {
        // Hold expired and no energy trigger — fall back to Soft Aura.
        autoVisualMode_ = 1;
    }

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
    params.autoVisualMode = autoVisualMode_;

    return params;
}
