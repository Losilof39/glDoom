#version 460 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 ortho;
uniform mat4 model;

void main()
{
    TexCoords = vec2(-vertex.z, 1 - vertex.w);
    gl_Position = ortho * model * vec4(vertex.xy, 0.0, 1.0);
}