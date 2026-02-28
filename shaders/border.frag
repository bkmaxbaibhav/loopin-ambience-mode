#version 330 core
out vec4 FragColor;
in vec2 vUV;

uniform vec2  uResolution;
uniform float uEdgeWidth;
uniform float uIntensity;
uniform vec3  uPrimaryColor;

void main() {
    float distLeft   = vUV.x * uResolution.x;
    float distRight  = (1.0 - vUV.x) * uResolution.x;
    float distTop    = vUV.y * uResolution.y;
    float distBottom = (1.0 - vUV.y) * uResolution.y;
    float minDist    = min(min(distLeft, distRight),
                          min(distTop, distBottom));

    if (minDist > uEdgeWidth) discard;

    float glow = exp(-minDist * minDist /
                     (uEdgeWidth * uEdgeWidth * 0.5));

    FragColor = vec4(uPrimaryColor * glow * uIntensity, glow);
}
