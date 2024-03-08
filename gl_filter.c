/*
** File: gl_filter.c
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/31/2023
** Description:
** File created for create a utility that manages the
** texture and filter used
** License: Public domain/ID License
*/

#include "gl_filter.h"

void GL_GenTextures(unsigned int* texture, int texture_size, gltexture_t texture_target)
{
	glGenTextures(texture_size, texture);
	switch (texture_target)
	{
		case gl_texture_1d:
			glBindTexture(GL_TEXTURE_1D, *texture);
			break;
		case gl_texture_2d:
			glBindTexture(GL_TEXTURE_2D, *texture);
			break;
		case gl_texture_width:
			glBindTexture(GL_TEXTURE_WIDTH, *texture);
			break;
		case gl_texture_height:
			glBindTexture(GL_TEXTURE_HEIGHT, *texture);
			break;
		case gl_texture_border_color:
			glBindTexture(GL_TEXTURE_BORDER_COLOR, *texture);
			break;
		default:
			printf("Unknown texture choosen, Please select the available texture!");
			break;
	}
}

dboolean GL_CreateDefaultTexture(unsigned int texture, gltexture_t texture_target, int width, int height, const void* pixels, dboolean enable_tex, dboolean copy, dboolean use_rgba, dboolean activated)
{
	if(activated)
	{
		if (enable_tex && activated)
		{
			glEnable(texture_target);
		}

		GL_GenTextures(&texture, sizeof(texture), texture_target);
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		/* Define the 2D texture image. */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); /* Force 4 bytes aligment */
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		if (use_rgba == true)
		{
			glTexImage2D(texture_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		}
		else
		{
			glTexImage2D(texture_target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		}

		if(copy)
		{
			glCopyTexSubImage2D(gl_texture_2d, 0, 0, 0, 0, 0, width, height);
		}

		return activated;
	}
	else
	{
		printf("Failed to activate the texture");
		return activated;
	}
	return activated;
}

void GL_CreateClampTexture(gltexture_t texture_target, int width, int height, const void* pixels, dboolean activate_edge_clamp)
{
	if(activate_edge_clamp == true)
	{
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* Define the 2D texture image. */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); /* Force 4 bytes aligment */
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	glTexImage2D(texture_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(texture_target, 0);
}