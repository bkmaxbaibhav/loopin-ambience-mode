#include "BandMapper.h"

VisualParams BandMapper::map(float bass,
                              float mid,
                              float treble,
                              bool isSilent) {
    VisualParams params;
    params.isSilent = isSilent;

    // 1. Map each band to its corresponding edge intensity
    params.bassIntensity   = bass;   // → uIntensityBottom
    params.midIntensity    = mid;    // → uIntensityLeft and uIntensityRight
    params.trebleIntensity = treble; // → uIntensityTop

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
