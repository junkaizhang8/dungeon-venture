#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 3) in int a_Selected;

out vec4 v_Color;

const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 selectedColor = vec4(0.0, 1.0, 0.0, 1.0);

void main()
{
    gl_Position = a_Position;
    v_Color = (a_Selected != 0) ? selectedColor : color;
}