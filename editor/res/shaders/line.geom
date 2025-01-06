#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in float v_Selected[];
out vec4 g_Color;

uniform float u_LineWeight;
uniform mat4 u_VP;

const float epsilon = 0.0001;

const vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 selectedColor = vec4(0.0, 1.0, 0.0, 1.0);

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec4 dir = normalize(p2 - p1);
    vec4 normal = vec4(-dir.y, dir.x, 0.0, 0.0);

    float r = u_LineWeight / 2;

    vec4 offset = normal * r;

    float p1Selected = v_Selected[0];
    float p2Selected = v_Selected[1];
    vec4 lineColor =
        (p1Selected > epsilon && p2Selected > epsilon) ? selectedColor : color;

    gl_Position = u_VP * (p1 + offset);
    g_Color = lineColor;
    EmitVertex();
    gl_Position = u_VP * (p1 - offset);
    g_Color = lineColor;
    EmitVertex();
    gl_Position = u_VP * (p2 + offset);
    g_Color = lineColor;
    EmitVertex();
    gl_Position = u_VP * (p2 - offset);
    g_Color = lineColor;
    EmitVertex();

    EndPrimitive();
}