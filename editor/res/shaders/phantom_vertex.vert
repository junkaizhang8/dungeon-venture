#version 410 core

layout(location = 0) in vec4 a_Position;

out vec4 v_Color;

uniform mat4 u_VP;

const float pointSize = 20.0;

void main()
{
    gl_Position = u_VP * a_Position;
    gl_PointSize = pointSize;
    v_Color = vec4(0.7, 0.7, 0.7, 0.8);
}