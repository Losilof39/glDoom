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

#include "doomtype.h"

/*
** BMP File header structure definition cleanuped from Windows Api.
** Main Changes:
** Removed typedefs of the windows api 
** Fixed some wrong types used on the structures
*/

typedef struct {
    unsigned short  type;
    size_t size;
    unsigned short  reserved1;
    unsigned short  reserved2;
    size_t offBits;
} bitmapfile_t;

typedef struct {
    size_t size;
    long  width;
    long  height;
    unsigned short  planes;
    unsigned short  bitCount;
    size_t compression;
    size_t sizeImage;
    long  xPelsPerMeter;
    long  yPelsPerMeter;
    size_t clrUsed;
    size_t clrImportant;
} bitmapinfo_t;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char reserved;
} rgbquad_t;

#ifdef __cplusplus
extern "C"
{
#endif

void D_LoadBmp(dbyte* rgb, const char *filename, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* __D_BITMAP__ */