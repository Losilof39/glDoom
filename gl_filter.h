/*
** File: gl_filter.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/31/2023
** Description:
** File created for create a utility that manages the 
** texture and filter used
** License: Public domain/ID License 
*/

#include "gldefs.h"

void GL_GenTextures(unsigned int* texture, GLenum texture_target);

dboolean GL_CreateDefaultTexture(unsigned int texture, GLenum texture_target, int width, int height, const void *pixels);