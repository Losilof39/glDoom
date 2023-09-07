#version 330 core

uniform mat4 ortho;
uniform mat4 model;

void main()
{
	gl_Position = ortho * model * vec4(aPos, 1.0);
}