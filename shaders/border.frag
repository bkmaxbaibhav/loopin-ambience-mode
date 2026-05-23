#version 330 core
out vec4 FragColor;
in vec2 vUV;

uniform vec2  uResolution;
uniform float uEdgeWidth;
uniform float uIntensity;
uniform vec3  uPrimaryColor;
uniform float uTime;
uniform float uIntensityTop;
uniform float uIntensityBottom;
uniform float uIntensityLeft;
uniform float uIntensityRight;
uniform float uBeat;
uniform int   uColorMode;
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
    float minDist    = min(min(distLeft, distRight),
                          min(distTop, distBottom));

    float beat = clamp(uBeat, 0.0, 1.0);
    float glowRadius = max(uEdgeWidth * (4.0 + beat * 2.2), 36.0);
    if (minDist > glowRadius) discard;

    // Ticket 1: Breathing pulse
    float pulse = 0.82 + 0.18 * sin(uTime * 1.5 + beat * 2.0);

    // Ticket 2: Edge detection
    bool isBottom = (distBottom <= distLeft &&
                     distBottom <= distRight &&
                     distBottom <= distTop);
    bool isTop    = (distTop <= distLeft &&
                     distTop <= distRight &&
                     distTop < distBottom);
    bool isLeft   = (distLeft < distTop &&
                     distLeft < distBottom &&
                     distLeft <= distRight);
    bool isRight  = (distRight < distTop &&
                     distRight < distBottom &&
                     distRight < distLeft);

    float edgeIntensity = isBottom ? uIntensityBottom
                        : isTop    ? uIntensityTop
                        : isLeft   ? uIntensityLeft
                        :            uIntensityRight;

    // Ticket 3: Travel wave animation
    float posBottom = vUV.x;
    float posTop    = vUV.x;
    float posLeft   = vUV.y;
    float posRight  = vUV.y;

    float waveBottom = 0.7 + 0.3 * sin(posBottom * 6.28 + uTime * 2.0);
    float waveTop    = 0.7 + 0.3 * sin(posTop    * 6.28 - uTime * 2.0);
    float waveLeft   = 0.7 + 0.3 * sin(posLeft   * 6.28 + uTime * 1.5);
    float waveRight  = 0.7 + 0.3 * sin(posRight  * 6.28 - uTime * 1.5);

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
    if (distLeft < uEdgeWidth && distBottom < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distBottom / (distLeft + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityLeft, weight);
        finalEdgeWave = mix(waveBottom, waveLeft, weight);
    }
    // Bottom-Right
    else if (distRight < uEdgeWidth && distBottom < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distBottom / (distRight + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityRight, weight);
        finalEdgeWave = mix(waveBottom, waveRight, weight);
    }
    // Top-Left
    else if (distLeft < uEdgeWidth && distTop < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distTop / (distLeft + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityLeft, weight);
        finalEdgeWave = mix(waveTop, waveLeft, weight);
    }
    // Top-Right
    else if (distRight < uEdgeWidth && distTop < uEdgeWidth) {
        float weight = smoothstep(0.0, 1.0, distTop / (distRight + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityRight, weight);
        finalEdgeWave = mix(waveTop, waveRight, weight);
    }

    float core = 1.0 - smoothstep(0.0, uEdgeWidth, minDist);
    float aura = exp(-(minDist * minDist) / (glowRadius * glowRadius * 0.18));
    float glow = max(core, aura * 0.82);
    float audioFloor = mix(0.38, 1.0, clamp(finalEdgeIntensity, 0.0, 1.0));
    float beatExpansion = 1.0 + beat * 0.75;
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

        float spectrumHue = fract(edgePosition * 0.85 + uTime * (0.08 + beat * 0.08) + finalEdgeIntensity * 0.16 + beat * 0.04);
        finalColor = hsv2rgb(spectrumHue, 0.95, 1.0);
    }

    float alpha = clamp(finalGlow * (0.70 + beat * 0.22), 0.0, 0.9);
    FragColor = vec4(finalColor * finalGlow, alpha);
}
