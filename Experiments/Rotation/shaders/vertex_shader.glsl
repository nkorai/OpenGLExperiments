#version 460 core

in vec3 vp;
uniform mat4 u_camera;

void main(){
    gl_Position = vec4(vp, 1.0);
}