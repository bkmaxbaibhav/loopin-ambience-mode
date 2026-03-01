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
uniform int   uColorMode;
uniform float uHue;

void main() {
    float distLeft   = vUV.x * uResolution.x;
    float distRight  = (1.0 - vUV.x) * uResolution.x;
    float distTop    = vUV.y * uResolution.y;
    float distBottom = (1.0 - vUV.y) * uResolution.y;
    float minDist    = min(min(distLeft, distRight),
                          min(distTop, distBottom));

    if (minDist > uEdgeWidth) discard;

    // Ticket 1: Breathing pulse
    float pulse = 0.85 + 0.15 * sin(uTime * 1.5);

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

    float glow = exp(-minDist * minDist / (uEdgeWidth * uEdgeWidth * 0.5));
    float finalGlow = glow * pulse * finalEdgeIntensity * finalEdgeWave * uIntensity;

    FragColor = vec4(uPrimaryColor * finalGlow, finalGlow);
}
