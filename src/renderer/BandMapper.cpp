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

    // 2. Compute dominant frequency band
    if (bass > mid && bass > treble) {
        params.colorMode = 0;
    } else if (mid > bass && mid > treble) {
        params.colorMode = 1;
    } else if (treble >= bass && treble >= mid) {
        params.colorMode = 2;
    }

    // 3. Compute a combined hue value based on weighted mix
    // bass -> red (0.0), mid -> green (0.33), treble -> blue (0.66)
    params.hue = (bass * 0.0f + mid * 0.33f + treble * 0.66f);

    // Ensure hue is between 0.0 and 1.0 (though with 0.0, 0.33, 0.66 weights it should be if inputs are <= 1.0 and we normalize if needed,
    // but the ticket says bass*0.0 + mid*0.33 + treble*0.66. If all are 1.0, it's 0.99.
    if (params.hue > 1.0f) params.hue = 1.0f;

    return params;
}
