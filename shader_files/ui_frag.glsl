#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D texture;

void main()
{
	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);//texture(texture, texCoords);
}

//vec4(0.0f, 1.0f, 0.0f, 1.0f)