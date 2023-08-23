#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 ortho;

void main()
{
	gl_Position = ortho * vec4(aPos, 1.0);
	texCoords = aTexCoords;
}