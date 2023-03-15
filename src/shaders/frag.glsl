#version 330 core

out vec4 FragColor;

uniform bool edges;

void main(){
    FragColor = edges ? vec4(vec3(0.0), 1.0) : vec4(1.0);
}