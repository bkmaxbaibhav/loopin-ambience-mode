#version 330 core
// TODO Session 4: edge glow fragment shader
// TODO Session 5: connect band uniforms
// TODO Session 6: implement color modes
uniform float uBass;
uniform float uMid;
uniform float uTreble;
uniform float uTime;
uniform float uIntensity;
uniform float uEdgeWidth;
uniform int   uColorMode;
uniform vec3  uPrimaryColor;
in  vec2 vUV;
out vec4 FragColor;
void main() {
    FragColor = vec4(0.0); // transparent stub
}
