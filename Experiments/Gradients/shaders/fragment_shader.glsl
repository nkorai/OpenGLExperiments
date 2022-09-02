#version 460 core

in vec3 oPos;
out vec4 fragColor;

uniform vec3 u_outputColor = vec3(0.3, 0.3, 0.8);
uniform vec2 u_lightPosition = vec2(0.5, 0.5);

void main(){
  float intensity =  1 / length(oPos.xy - u_lightPosition);
  vec4 col = vec4(u_outputColor, 1.0f);
  fragColor = col * intensity; 
} 