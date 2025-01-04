#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 3) in float a_LineWeight;

out vec4 v_Color;
out float v_LineWeight;

void main()
{
    gl_Position = a_Position;
    v_Color = a_Color;
    v_LineWeight = a_LineWeight;
}