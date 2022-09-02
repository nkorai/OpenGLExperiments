#version 460 core

layout (location = 0) in vec3 aPos;
out vec3 oPos;

void main(){
    oPos.xyz  = aPos.xyz; // ADD THIS LINE
    gl_Position = vec4(aPos.xyz, 1.0);
}