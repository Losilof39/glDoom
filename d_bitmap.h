/*
** File: d_bitmap.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/30/2023
** Description:
** File created for add separately the bmp loader/parser from gl_texture.c
** License: Public domain/ID License 
*/

#ifndef __D_BITMAP__
#define __D_BITMAP__

#ifdef __cplusplus
extern "C"
{
#endif

void D_LoadBmp(dbyte* rgb, const char *filename, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* __D_BITMAP__ */