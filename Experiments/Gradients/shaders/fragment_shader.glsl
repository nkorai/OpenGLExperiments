#version 460 core

in vec3 oPos;
out vec4 fragColor;

vec4 firstColor = vec4(1.0, 0.0, 0.0, 1.0); // red
vec4 middleColor = vec4(0.0, 1.0, 0.0, 1.0); // green
vec4 endColor = vec4(0.0, 0.0, 1.0, 1.0); // blue

uniform vec2 resolution = vec2(640, 480);

void main()
{
    vec2 xy = gl_FragCoord.xy / resolution;
    float h = 0.5;
    vec4 col = mix(mix(firstColor, middleColor, (xy.x / h)), mix(middleColor, endColor, (xy.x - h)/(1.0 - h)), step(h, xy.x));
    fragColor = col;
}