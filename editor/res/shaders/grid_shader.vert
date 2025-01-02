#version 330 core

layout(location = 0) in vec4 aPosition;

uniform mat4 u_MVP;

void main() { gl_Position = u_MVP * aPosition; }