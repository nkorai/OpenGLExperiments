#version 460 core

in vec3 vp;

uniform vec2 u_resolution;
uniform mat4 u_transform;
uniform vec2 u_offsets[100];

void main(){
    vec2 offset = u_offsets[gl_InstanceID];
    gl_Position = u_transform * vec4(vp.x + offset.x, vp.y + offset.y, vp.z, 1.0);
}