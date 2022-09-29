#version 460 core

out vec4 fragColor;

vec4 firstColor = vec4(1.0, 0.0, 0.0, 1.0); // red
vec4 middleColor = vec4(0.0, 1.0, 0.0, 1.0); // green
vec4 endColor = vec4(0.0, 0.0, 1.0, 1.0); // blue

uniform vec2 u_resolution;

void main()
{
    vec2 st = gl_FragCoord.xy / u_resolution;
    float h = 0.5;

    vec4 startColor = mix(firstColor, middleColor, (st.x / h));
    vec4 endColor = mix(middleColor, endColor, (st.x - h)/(1.0 - h));
    fragColor = mix(startColor, endColor, step(h, st.x));
}