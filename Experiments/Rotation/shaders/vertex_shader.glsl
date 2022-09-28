#version 460 core

in vec3 vp;
uniform vec2 u_resolution;
uniform mat4 u_transform;

void main(){
    gl_Position = u_transform * vec4(vp, 1.0);
}