#version 460 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_Ortho;

void main()
{
    TexCoords = vec2(vertex.z, 1 - vertex.w);
    gl_Position = u_Ortho * u_Model * vec4(vertex.xy, 0.0, 1.0);
}