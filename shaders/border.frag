#version 330 core
out vec4 FragColor;
in vec2 vUV;

uniform vec2  uResolution;
uniform float uEdgeWidth;
uniform float uIntensity;
uniform float uContrast;
uniform vec3  uPrimaryColor;
uniform float uTime;
uniform float uIntensityTop;
uniform float uIntensityBottom;
uniform float uIntensityLeft;
uniform float uIntensityRight;
uniform float uBeat;
uniform float uGenreWarmth;
uniform float uGenrePresence;
uniform float uGenreConfidence;
uniform int   uColorMode;
uniform int   uVisualMode;
uniform int   uSideMask;
uniform float uHue;

vec3 hsv2rgb(float h, float s, float v) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + K.xyz) * 6.0 - K.www);
    return v * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), s);
}

void main() {
    float distLeft   = vUV.x * uResolution.x;
    float distRight  = (1.0 - vUV.x) * uResolution.x;
    float distTop    = vUV.y * uResolution.y;
    float distBottom = (1.0 - vUV.y) * uResolution.y;
    bool enableTop = (uSideMask & 1) != 0;
    bool enableRight = (uSideMask & 2) != 0;
    bool enableBottom = (uSideMask & 4) != 0;
    bool enableLeft = (uSideMask & 8) != 0;

    float disabledDist = 1000000.0;
    float activeTop = enableTop ? distTop : disabledDist;
    float activeRight = enableRight ? distRight : disabledDist;
    float activeBottom = enableBottom ? distBottom : disabledDist;
    float activeLeft = enableLeft ? distLeft : disabledDist;
    float minDist = min(min(activeLeft, activeRight),
                        min(activeTop, activeBottom));

    float beat = clamp(uBeat, 0.0, 1.0);
    int mode = uVisualMode;
    if (mode == 0) {
        mode = beat > 0.55 ? 3 : 1;
    }

    float genreMix = clamp(uGenreConfidence, 0.0, 0.8);
    float comfort = mode == 1 ? 1.0 : 0.0;
    float bloomScale = (mode == 3 ? 1.0 : 0.65) * mix(1.0, uGenreWarmth, genreMix * 0.18);
    float glowRadius = max(uEdgeWidth * (4.2 + beat * 1.45 * bloomScale), 40.0);
    if (minDist > glowRadius) discard;

    // Comfort-first breath layer: slow, peripheral-safe brightness motion.
    float pulse = 0.88 + 0.12 * sin(uTime * 1.15 + beat * 1.35);

    // Ticket 2: Edge detection
    bool isBottom = (activeBottom <= activeLeft &&
                     activeBottom <= activeRight &&
                     activeBottom <= activeTop);
    bool isTop    = (activeTop <= activeLeft &&
                     activeTop <= activeRight &&
                     activeTop < activeBottom);
    bool isLeft   = (activeLeft < activeTop &&
                     activeLeft < activeBottom &&
                     activeLeft <= activeRight);
    bool isRight  = (activeRight < activeTop &&
                     activeRight < activeBottom &&
                     activeRight < activeLeft);

    float edgeIntensity = isBottom ? uIntensityBottom
                        : isTop    ? uIntensityTop
                        : isLeft   ? uIntensityLeft
                        :            uIntensityRight;

    // Ticket 3: Travel wave animation
    float posBottom = vUV.x;
    float posTop    = vUV.x;
    float posLeft   = vUV.y;
    float posRight  = vUV.y;

    float flowSpeed = mode == 2 ? 2.0 : 1.0;
    float waveDepth = mix(0.22, 0.08, comfort);
    float waveBottom = (1.0 - waveDepth) + waveDepth * sin(posBottom * 6.28 + uTime * 2.0 * flowSpeed);
    float waveTop    = (1.0 - waveDepth) + waveDepth * sin(posTop    * 6.28 - uTime * 2.0 * flowSpeed);
    float waveLeft   = (1.0 - waveDepth) + waveDepth * sin(posLeft   * 6.28 + uTime * 1.5 * flowSpeed);
    float waveRight  = (1.0 - waveDepth) + waveDepth * sin(posRight  * 6.28 - uTime * 1.5 * flowSpeed);

    float edgeWave = isBottom ? waveBottom
                   : isTop    ? waveTop
                   : isLeft   ? waveLeft
                   :            waveRight;

    // Ticket 4: Soft corner blending
    float blendBottomLeft  = smoothstep(0.0, uEdgeWidth, distLeft) * smoothstep(0.0, uEdgeWidth, distBottom);
    float blendBottomRight = smoothstep(0.0, uEdgeWidth, distRight) * smoothstep(0.0, uEdgeWidth, distBottom);
    float blendTopLeft     = smoothstep(0.0, uEdgeWidth, distLeft) * smoothstep(0.0, uEdgeWidth, distTop);
    float blendTopRight    = smoothstep(0.0, uEdgeWidth, distRight) * smoothstep(0.0, uEdgeWidth, distTop);

    // Re-calculating intensities and waves for corners to avoid seams
    // Actually Ticket 4 says to use smoothstep to blend where two edge values meet.
    // If we just use the edge detection, it's a hard cut at 45 degrees.
    // To blend, we can interpolate between the adjacent edges near corners.

    float finalEdgeIntensity = edgeIntensity;
    float finalEdgeWave = edgeWave;

    // For simplicity and to match Ticket 4 requirements while keeping Ticket 2/3 logic:
    // If we are near a corner, we blend.

    // Bottom-Left corner
    if (enableLeft && enableBottom && distLeft < uEdgeWidth && distBottom < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distBottom / (distLeft + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityLeft, weight);
        finalEdgeWave = mix(waveBottom, waveLeft, weight);
    }
    // Bottom-Right
    else if (enableRight && enableBottom && distRight < uEdgeWidth && distBottom < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distBottom / (distRight + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityRight, weight);
        finalEdgeWave = mix(waveBottom, waveRight, weight);
    }
    // Top-Left
    else if (enableLeft && enableTop && distLeft < uEdgeWidth && distTop < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distTop / (distLeft + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityLeft, weight);
        finalEdgeWave = mix(waveTop, waveLeft, weight);
    }
    // Top-Right
    else if (enableRight && enableTop && distRight < uEdgeWidth && distTop < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distTop / (distRight + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityRight, weight);
        finalEdgeWave = mix(waveTop, waveRight, weight);
    }

    float core = 1.0 - smoothstep(0.0, uEdgeWidth, minDist);
    float aura = exp(-(minDist * minDist) / (glowRadius * glowRadius * 0.18));
    float glow = max(core, aura * 0.82);
    float cornerBoost = 1.0;
    if (mode == 1 || mode == 4) {
        float cornerDist = min(
            min(length(vec2(distLeft, distTop)), length(vec2(distRight, distTop))),
            min(length(vec2(distLeft, distBottom)), length(vec2(distRight, distBottom)))
        );
        cornerBoost += (mode == 4 ? 0.42 * beat : 0.16) * exp(-(cornerDist * cornerDist) / (glowRadius * glowRadius * 0.65));
    }

    float shapedIntensity = pow(clamp(finalEdgeIntensity, 0.0, 1.0), 1.0 / max(uContrast, 0.01));
    float audioFloor = mix(0.34, 0.92, shapedIntensity);
    float beatExpansion = 1.0 + beat * 0.52 * bloomScale;
    float finalGlow = glow * pulse * audioFloor * finalEdgeWave * beatExpansion * uIntensity;

    vec3 finalColor;
    if (uColorMode == 0) {
        // Static Mode
        finalColor = uPrimaryColor;
    } else if (uColorMode == 1) {
        // Reactive Hue Mode
        finalColor = hsv2rgb(uHue, 1.0, 1.0);
    } else {
        // Spectrum Rainbow Mode (Ticket 3)
        // edgePosition calculation (from Session 4b)
        float edgePosition;
        if (isBottom) edgePosition = vUV.x;
        else if (isTop) edgePosition = vUV.x;
        else if (isLeft) edgePosition = vUV.y;
        else edgePosition = vUV.y;

        float hueSpeed = (mode == 1 ? 0.018 : 0.055 + beat * 0.04) * mix(1.0, uGenrePresence, genreMix * 0.18);
        float spectrumHue = fract(edgePosition * 0.72 + uTime * hueSpeed + finalEdgeIntensity * 0.12 + beat * 0.025);
        float warmShift = (uGenreWarmth - 1.0) * 0.025 * genreMix;
        spectrumHue = fract(spectrumHue - warmShift);
        float saturation = mix(0.62, 0.82, beat) * mix(1.0, 0.93 + uGenrePresence * 0.05, genreMix);
        finalColor = hsv2rgb(spectrumHue, saturation, 1.0);
    }

    finalGlow *= cornerBoost;
    float alpha = clamp(finalGlow * (0.58 + beat * 0.18), 0.0, 0.78);
    FragColor = vec4(finalColor * finalGlow, alpha);
}
