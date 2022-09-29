#version 460 core

in vec3 vp;
uniform vec2 u_resolution;

uniform vec2 u_offsets[100];

void main(){
    vec2 offset = u_offsets[gl_InstanceID];
    gl_Position = vec4(vp.x + offset.x, vp.y + offset.y, vp.z, 1.0);
}