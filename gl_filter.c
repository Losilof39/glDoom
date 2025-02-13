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

static dboolean tex_copy = false;
static dboolean rgba_tex = false;

static dboolean ActivateRGBA()
{
	return (rgba_tex) ? GL_RGBA : GL_RGB;
}

void GL_GenTextures(unsigned int* texture, GLenum texture_target)
{
	glGenTextures(1, texture);
	glBindTexture(texture_target, *texture);
}

dboolean GL_CreateDefaultTexture(unsigned int texture, GLenum texture_target, int width, int height, const void* pixels)
{
	GL_GenTextures(&texture, texture_target);
	glTexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/* Define the 2D texture image. */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); /* Force 4 bytes aligment */
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	glTexImage2D(texture_target, 0, ActivateRGBA(), width, height, 0, ActivateRGBA(), GL_UNSIGNED_BYTE, pixels);

	if(tex_copy)
	{
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
		tex_copy = false;
	}

	glBindTexture(texture_target, 0);

	return false;
}

