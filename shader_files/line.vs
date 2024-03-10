#version 460 core
layout (location = 0) in vec2 vertex;

uniform mat4 u_Model;
uniform mat4 u_Ortho;

void main()
{
    gl_Position = u_Ortho * vec4(vertex, 0.0, 1.0);
}