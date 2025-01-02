#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * aPosition;
    vColor = aColor;
}