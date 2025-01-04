#version 410 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 v_Color[];
out vec4 g_Color;

uniform float u_LineWeight;
uniform mat4 u_VP;

void main()
{
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec4 dir = normalize(p2 - p1);
    vec4 normal = vec4(-dir.y, dir.x, 0.0, 0.0);

    float r = u_LineWeight / 2;

    vec4 offset = normal * r;

    gl_Position = u_VP * (p1 + offset);
    g_Color = v_Color[0];
    EmitVertex();
    gl_Position = u_VP * (p1 - offset);
    g_Color = v_Color[0];
    EmitVertex();
    gl_Position = u_VP * (p2 + offset);
    g_Color = v_Color[0];
    EmitVertex();
    gl_Position = u_VP * (p2 - offset);
    g_Color = v_Color[0];
    EmitVertex();

    EndPrimitive();
}