#include "BandMapper.h"
#include <algorithm>
#include <cmath>

VisualParams BandMapper::map(float bass,
                              float mid,
                              float treble,
                              bool isSilent) {
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

    bassFloor_ = 0.985f * bassFloor_ + 0.015f * bass;
    float bassLift = std::max(0.0f, bass - bassFloor_);
    float beatHit = std::clamp(bassLift * 9.0f + params.bassIntensity * 0.45f, 0.0f, 1.0f);
    beatPulse_ = std::max(beatHit, beatPulse_ * 0.86f);
    params.beatPulse = isSilent ? 0.0f : beatPulse_;

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
    smoothHue_ = 0.05f * targetHue + 0.95f * smoothHue_;

    params.hue = smoothHue_;
    params.colorMode = colorMode_;

    return params;
}
