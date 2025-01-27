#version 410 core

layout(location = 0) in vec4 a_Position;
layout(location = 3) in int a_Selected;

out vec4 v_Color;

uniform mat4 u_VP;

const float pointSize = 20.0;

const vec4 color = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 selectedColor = vec4(0.0, 1.0, 0.0, 1.0);

void main()
{
    gl_Position = u_VP * a_Position;
    gl_PointSize = pointSize;
    v_Color = (a_Selected != 0) ? selectedColor : color;
}