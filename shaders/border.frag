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
uniform float uBeatTime;
uniform float uGenreWarmth;
uniform float uGenrePresence;
uniform float uGenreConfidence;
uniform int   uColorMode;
uniform int   uVisualMode;
uniform int   uSideMask;
uniform int   uPartyMode;
uniform float uHue;
uniform float uModeBlend;

// ---------- helpers ----------------------------------------------------------

vec3 hsv2rgb(float h, float s, float v) {
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + K.xyz) * 6.0 - K.www);
    return v * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), s);
}

float hash(uint n) {
    n = (n ^ 61u) ^ (n >> 16u);
    n *= 186u;
    n = n ^ (n >> 4u);
    n *= 2246822519u;
    n = n ^ (n >> 15u);
    return float(n) / 4294967296.0;
}

// ---------- color mode -------------------------------------------------------

vec3 computeColor(float edgePos, float edgeIntens, float beat, float party, float genreMix) {
    if (uColorMode == 0) {
        return uPrimaryColor;
    } else if (uColorMode == 1) {
        return hsv2rgb(uHue, 0.72 + beat * (0.12 + party * 0.10), 1.0);
    } else if (uColorMode == 3) {
        float flow = edgePos * (0.34 + party * 0.12)
                   + uTime * (0.012 + party * 0.028)
                   + edgeIntens * (0.10 + party * 0.08)
                   + beat * (0.055 + party * 0.045);
        float wShift = (uGenreWarmth   - 1.0) * 0.018 * genreMix;
        float pShift = (uGenrePresence - 1.0) * 0.035 * genreMix;
        float h = fract(uHue * 0.42 + flow + pShift - wShift);
        float s = clamp(mix(0.60 + party * 0.12, 0.84 + party * 0.12, beat)
                      * mix(1.0, 0.98 + uGenrePresence * 0.03, genreMix), 0.0, 1.0);
        return hsv2rgb(h, s, 1.0);
    } else {
        float spd = (0.055 + beat * 0.04) * mix(1.0, uGenrePresence, genreMix * 0.18);
        float h   = fract(edgePos * 0.72 + uTime * spd + edgeIntens * 0.12 + beat * 0.025);
        h = fract(h - (uGenreWarmth - 1.0) * 0.025 * genreMix);
        float s = clamp(mix(0.62 + party * 0.10, 0.82 + party * 0.14, beat)
                      * mix(1.0, 0.93 + uGenrePresence * 0.05, genreMix), 0.0, 1.0);
        return hsv2rgb(h, s, 1.0);
    }
}

// ---------- perimeter parameterisation helper --------------------------------
// Maps the current fragment (given edge flags + UV) to t in [0,1) going
// clockwise: bottom-left=0, bottom-right=0.25, top-right=0.5, top-left=0.75
float perimT(bool isBottom, bool isRight, bool isTop, bool isLeft, vec2 uv) {
    if (isBottom) return uv.x * 0.25;
    if (isRight)  return 0.25 + (1.0 - uv.y) * 0.25;
    if (isTop)    return 0.50 + (1.0 - uv.x) * 0.25;
    return              0.75 + uv.y * 0.25;
}

// ---------- main -------------------------------------------------------------

void main() {
    float distLeft   = vUV.x * uResolution.x;
    float distRight  = (1.0 - vUV.x) * uResolution.x;
    float distTop    = vUV.y * uResolution.y;
    float distBottom = (1.0 - vUV.y) * uResolution.y;

    bool enableTop    = (uSideMask & 1) != 0;
    bool enableRight  = (uSideMask & 2) != 0;
    bool enableBottom = (uSideMask & 4) != 0;
    bool enableLeft   = (uSideMask & 8) != 0;

    float big = 1000000.0;
    float activeTop    = enableTop    ? distTop    : big;
    float activeRight  = enableRight  ? distRight  : big;
    float activeBottom = enableBottom ? distBottom : big;
    float activeLeft   = enableLeft   ? distLeft   : big;
    float minDist = min(min(activeLeft, activeRight), min(activeTop, activeBottom));

    float beat  = clamp(uBeat, 0.0, 1.0);
    float party = uPartyMode == 1 ? 1.0 : 0.0;
    int   mode  = uVisualMode == 0 ? 1 : uVisualMode;

    float genreMix   = clamp(uGenreConfidence, 0.0, 0.8);
    float comfort    = mode == 1 ? 1.0 : 0.0;
    float bloomScale = (mode == 3 ? 1.0 : 0.65) * mix(1.0, uGenreWarmth, genreMix * 0.18);

    // Laser/streak modes need a wider glow radius
    float laserExtra = (mode == 5 || mode == 6 || mode == 7) ? 1.8 : 1.0;
    float glowRadius = max(uEdgeWidth * (4.2 + beat * 1.45 * bloomScale + party * 1.55)
                           * laserExtra, 40.0 + party * 32.0);
    if (minDist > glowRadius) discard;

    float pulse = 0.92 + 0.08 * sin(uTime * mix(0.55, 1.05, party)
                                   + beat * mix(0.55, 1.25, party));

    bool isBottom = (activeBottom <= activeLeft && activeBottom <= activeRight && activeBottom <= activeTop);
    bool isTop    = (activeTop    <= activeLeft && activeTop    <= activeRight && activeTop    <  activeBottom);
    bool isLeft   = (activeLeft   <  activeTop  && activeLeft   <  activeBottom && activeLeft  <= activeRight);
    bool isRight  = (activeRight  <  activeTop  && activeRight  <  activeBottom && activeRight <  activeLeft);

    float edgeIntensity = isBottom ? uIntensityBottom
                        : isTop    ? uIntensityTop
                        : isLeft   ? uIntensityLeft
                        :            uIntensityRight;

    float edgePos = isBottom ? vUV.x
                  : isTop    ? vUV.x
                  : isLeft   ? vUV.y
                  :            vUV.y;

    // Travel wave (modes 1-4)
    float flowSpeed = mode == 2 ? mix(1.15, 2.0, party) : mix(0.55, 1.0, party);
    float waveDepth = mix(0.16, 0.045, comfort) + party * 0.05;
    float waveBottom = (1.0 - waveDepth) + waveDepth * sin(vUV.x * 6.28 + uTime * 2.0 * flowSpeed);
    float waveTop    = (1.0 - waveDepth) + waveDepth * sin(vUV.x * 6.28 - uTime * 2.0 * flowSpeed);
    float waveLeft   = (1.0 - waveDepth) + waveDepth * sin(vUV.y * 6.28 + uTime * 1.5 * flowSpeed);
    float waveRight  = (1.0 - waveDepth) + waveDepth * sin(vUV.y * 6.28 - uTime * 1.5 * flowSpeed);
    float edgeWave   = isBottom ? waveBottom : isTop ? waveTop : isLeft ? waveLeft : waveRight;

    // Corner blending
    float finalEdgeIntensity = edgeIntensity;
    float finalEdgeWave      = edgeWave;

    if (enableLeft && enableBottom && distLeft < uEdgeWidth && distBottom < uEdgeWidth) {
        float w = smoothstep(0.0, 1.0, distBottom / (distLeft + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityLeft, w);
        finalEdgeWave      = mix(waveBottom, waveLeft, w);
    } else if (enableRight && enableBottom && distRight < uEdgeWidth && distBottom < uEdgeWidth) {
        float w = smoothstep(0.0, 1.0, distBottom / (distRight + distBottom + 0.0001));
        finalEdgeIntensity = mix(uIntensityBottom, uIntensityRight, w);
        finalEdgeWave      = mix(waveBottom, waveRight, w);
    } else if (enableLeft && enableTop && distLeft < uEdgeWidth && distTop < uEdgeWidth) {
        float w = smoothstep(0.0, 1.0, distTop / (distLeft + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityLeft, w);
        finalEdgeWave      = mix(waveTop, waveLeft, w);
    } else if (enableRight && enableTop && distRight < uEdgeWidth && distTop < uEdgeWidth) {
        float w = smoothstep(0.0, 1.0, distTop / (distRight + distTop + 0.0001));
        finalEdgeIntensity = mix(uIntensityTop, uIntensityRight, w);
        finalEdgeWave      = mix(waveTop, waveRight, w);
    }

    // Base glow shape
    float core = 1.0 - smoothstep(0.0, uEdgeWidth, minDist);
    float aura = exp(-(minDist * minDist) / (glowRadius * glowRadius * 0.18));
    float glow = max(core, aura * 0.82);

    float cornerBoost = 1.0;
    if (mode == 1 || mode == 4) {
        float cd = min(
            min(length(vec2(distLeft, distTop)),    length(vec2(distRight, distTop))),
            min(length(vec2(distLeft, distBottom)), length(vec2(distRight, distBottom)))
        );
        cornerBoost += (mode == 4 ? 0.42 * beat : 0.16)
                     * exp(-(cd * cd) / (glowRadius * glowRadius * 0.65));
    }

    float shapedIntensity = pow(clamp(finalEdgeIntensity, 0.0, 1.0), 1.0 / max(uContrast, 0.01));
    float audioFloor    = mix(0.34 + party * 0.22, 0.92 + party * 0.16, shapedIntensity);
    float beatExpansion = 1.0 + beat * mix(0.24, 0.94, party) * bloomScale;
    float daylightLift  = 1.0 + party * (0.45 + shapedIntensity * 0.55);
    float finalGlow     = glow * pulse * audioFloor * finalEdgeWave
                        * beatExpansion * daylightLift * uIntensity;

    vec3 finalColor = computeColor(edgePos, finalEdgeIntensity, beat, party, genreMix);

    // ==========================================================================
    // MODE 5: Neon Lightning
    // Two glowing beams travel anticlockwise around the border, 180° apart.
    // Smooth, soothing, constant speed — NOT synced to music at all.
    // Tail trails behind the head in the direction of travel.
    // ==========================================================================
    if (mode == 5) {
        float W     = uResolution.x;
        float H     = uResolution.y;
        float perim = 2.0 * (W + H);

        // Constant slow anticlockwise speed — no beat, no party influence
        float speed = 0.018;

        // Two beams 180° apart, both anticlockwise
        // perimT goes clockwise, so anticlockwise = decreasing t
        float beamA = fract(0.0 - speed * uTime);   // beam 1
        float beamB = fract(0.5 - speed * uTime);   // beam 2, opposite side

        // Head size and tail length
        float headR  = 5.0 + uEdgeWidth * 0.15;     // head radius along edge (px)
        float tailPx = perim * 0.10;                 // tail length: 10% of perimeter
        float perpR  = 8.0 + uEdgeWidth * 0.2;      // glow width perpendicular to edge

        float laserGlow  = 0.0;
        vec3  laserColor = finalColor;

        // Fragment's perimeter position [0,1) clockwise
        float fragT = perimT(isBottom, isRight, isTop, isLeft, vUV);

        // Helper macro: compute one beam's contribution
        // For anticlockwise travel, the tail is at HIGHER t values (clockwise side of head)
        for (int b = 0; b < 2; b++) {
            float beamT = (b == 0) ? beamA : beamB;

            // Signed delta: positive = fragment is clockwise-ahead of beam
            float dt = fragT - beamT;
            if (dt >  0.5) dt -= 1.0;
            if (dt < -0.5) dt += 1.0;

            // Convert to pixels along perimeter
            float dtPx = dt * perim;

            // Head: tight gaussian centered on beam
            float headG = exp(-dtPx * dtPx / (headR * headR * 0.5))
                        * exp(-minDist * minDist / (perpR * perpR * 0.5));

            // Tail: anticlockwise means beam moves toward lower t,
            // so tail is at dtPx > 0 (clockwise side = behind the anticlockwise head)
            float tailG = 0.0;
            if (dtPx > 0.0 && dtPx < tailPx) {
                float t = dtPx / tailPx;
                tailG = exp(-t * t * 2.5) * (1.0 - t * t)
                      * exp(-minDist * minDist / (perpR * perpR * 0.7));
            }

            // Constant brightness — no music sync
            laserGlow += headG * 3.5 + tailG * 1.4;

            // White-hot head, color tail
            vec3 tailCol = computeColor(fragT, finalEdgeIntensity, 0.0, 0.0, 0.0);
            vec3 col = mix(tailCol, vec3(1.0), clamp(headG * 2.0, 0.0, 1.0));
            laserColor = mix(laserColor, col,
                             clamp(headG * 3.0 + tailG * 1.2, 0.0, 1.0));
        }

        laserGlow = clamp(laserGlow, 0.0, 4.0);

        // Dark background — only the beam is visible
        finalGlow  = laserGlow * uIntensity * 0.9;
        finalColor = mix(finalColor, laserColor, clamp(laserGlow * 0.85, 0.0, 1.0));
    }

    // ==========================================================================
    // MODE 6: Firefly Drift
    // Soft slow-moving glowing orbs that float along the border in both
    // directions. Each orb breathes at its own rate. Beat multiplies brightness.
    // Gentle fairy-light / festival string feel.
    // ==========================================================================
    if (mode == 6) {
        float W     = uResolution.x;
        float H     = uResolution.y;
        float perim = 2.0 * (W + H);

        float fragT = perimT(isBottom, isRight, isTop, isLeft, vUV);

        float laserGlow  = 0.0;
        vec3  laserColor = finalColor;

        int N = 18 + int(party * 10.0);
        for (int i = 0; i < N; i++) {
            uint  seed   = uint(i) * 3571u + 6197u;
            float rndPos = hash(seed);
            float rndSpd = hash(seed + 1u) * 0.012 + 0.005;
            float rndHue = hash(seed + 2u);
            float rndSz  = hash(seed + 3u) * 0.5 + 0.6;
            float rndDir = hash(seed + 4u) > 0.5 ? 1.0 : -1.0;

            float ballT = fract(rndPos + rndDir * rndSpd * uTime);

            float dt   = abs(fragT - ballT);
            dt = min(dt, 1.0 - dt);
            float dtPx = dt * perim;

            float ballR   = max(uEdgeWidth * 0.55, 7.0) * rndSz;
            float contrib = exp(-dtPx * dtPx / (ballR * ballR * 0.6));

            float breathPhase = hash(seed + 5u) * 6.28;
            float breath = 0.65 + 0.35 * sin(uTime * (0.8 + hash(seed + 6u) * 0.6) + breathPhase);
            float bright = breath * (0.6 + beat * 0.35 + party * 0.2);

            laserGlow += contrib * bright;

            vec3 flyCol = hsv2rgb(fract(uHue + rndHue * 0.5), 0.78, 1.0);
            laserColor  = mix(laserColor, flyCol, clamp(contrib * bright, 0.0, 1.0));
        }

        laserColor = mix(laserColor, vec3(1.0), clamp(laserGlow * 0.3, 0.0, 0.6));
        laserGlow  = clamp(laserGlow, 0.0, 2.2);

        finalGlow  *= 0.35;
        finalGlow  += laserGlow * uIntensity * (0.75 + party * 0.35);
        finalColor  = mix(finalColor, laserColor, clamp(laserGlow * 0.65, 0.0, 1.0));
    }

    // ==========================================================================
    // MODE 7: Comet Shower
    // Multiple fast laser beams with long tails, all travelling in the same
    // clockwise direction but at different speeds and starting positions.
    // High-energy festival effect — like a shower of comets.
    // ==========================================================================
    if (mode == 7) {
        float W     = uResolution.x;
        float H     = uResolution.y;
        float perim = 2.0 * (W + H);

        float fragT = perimT(isBottom, isRight, isTop, isLeft, vUV);

        float laserGlow  = 0.0;
        vec3  laserColor = finalColor;

        int N = 5 + int(party * 4.0);
        for (int i = 0; i < N; i++) {
            uint  seed   = uint(i) * 7919u + 2311u;
            float rndPos = hash(seed);
            float rndSpd = hash(seed + 1u) * 0.12 + 0.10;
            float rndHue = hash(seed + 2u);
            float rndDir = hash(seed + 3u) > 0.4 ? 1.0 : -1.0;

            float speed = (rndSpd + beat * 0.15 + party * 0.08);
            float headT = fract(rndPos + rndDir * speed * uTime);

            float dt   = (fragT - headT) * rndDir;
            if (dt >  0.5) dt -= 1.0;
            if (dt < -0.5) dt += 1.0;
            float dtPx = dt * perim;

            float headR    = max(uEdgeWidth * 0.55, 7.0);
            float headG    = exp(-dtPx * dtPx / (headR * headR * 0.3));

            float tailPx   = perim * (0.15 + beat * 0.06 + party * 0.04);
            float tailG    = 0.0;
            if (dtPx > 0.0 && dtPx < tailPx) {
                float t = dtPx / tailPx;
                tailG = exp(-t * t * 3.5) * (1.0 - t * t);
            }

            float bright = (0.9 + beat * 0.45 + party * 0.3);
            laserGlow += (headG * 2.2 + tailG * 0.8) * bright;

            vec3 cometCol = hsv2rgb(fract(uHue + rndHue * 0.4), 0.88, 1.0);
            vec3 col      = mix(cometCol, vec3(1.0), clamp(headG * 1.8, 0.0, 1.0));
            laserColor    = mix(laserColor, col,
                                clamp((headG * 2.0 + tailG * 0.7) * bright * 0.65, 0.0, 1.0));
        }

        laserGlow = clamp(laserGlow, 0.0, 3.5);

        finalGlow  *= 0.15;
        finalGlow  += laserGlow * uIntensity * (1.0 + party * 0.5);
        finalColor  = mix(finalColor, laserColor, clamp(laserGlow * 0.8, 0.0, 1.0));
    }

    // ---------- final composite ----------------------------------------------
    finalGlow *= cornerBoost;
    finalGlow *= mix(0.55, 1.0, uModeBlend);
    float alpha = clamp(finalGlow * (0.58 + beat * 0.18 + party * 0.18), 0.0, 0.78 + party * 0.18);
    FragColor = vec4(finalColor * finalGlow, alpha);
}
