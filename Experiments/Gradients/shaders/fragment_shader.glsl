#version 460 core

out vec4 fragColor;

vec4 firstColor = vec4(1.0, 0.0, 0.0, 1.0); // red
vec4 middleColor = vec4(0.0, 1.0, 0.0, 1.0); // green
vec4 endColor = vec4(0.0, 0.0, 1.0, 1.0); // blue

uniform vec2 u_resolution;

void main()
{
    vec2 st = gl_FragCoord.xy / u_resolution;
    fragColor = vec4(st.x, st.y, 0.0, 1.0);
}