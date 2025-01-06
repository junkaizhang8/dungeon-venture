#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 3) in int a_Selected;

out float v_Selected;

void main()
{
    gl_Position = a_Position;
    v_Selected = float(a_Selected);
}