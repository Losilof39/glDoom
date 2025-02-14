/*
** File: d_bitmap.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/30/2023
** Description:
** File created for add separately the bmp loader/parser from gl_texture.c
** License: Public domain/ID License 
*/

#include "doomtype.h"
#include "d_bitmap.h"
#include <stdlib.h>

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

void D_LoadBmp(dbyte* rgb, const char* filename, int width, int height)
{
    FILE *fn;
    int s, d;
    unsigned char* texels;
    bitmapfile_t bmfh;
    bitmapinfo_t bmi;
    rgbquad_t* bmpPalette;

    fn = fopen(filename, "rb");

    fread(&bmfh, sizeof(bitmapfile_t), 1, fn);
    fread(&bmi, sizeof(bitmapinfo_t), 1, fn);

    bmi.width = width;
    bmi.height = height;

    bmpPalette = (rgbquad_t*)malloc(sizeof(rgbquad_t) * 256);
    fread(bmpPalette, sizeof(rgbquad_t) * 256, 1, fn);

    texels = (unsigned char*)malloc(width * height);
    fseek(fn, bmfh.offBits, SEEK_SET);
    fread(texels, (width * height), 1, fn);

    fclose(fn);

    rgb = (dbyte*)malloc(width * (height * 3));
    for (s = 0, d = 0; s < (width * height); s++)
    {
        rgb[d++] = bmpPalette[texels[s]].red;
        rgb[d++] = bmpPalette[texels[s]].green;
        rgb[d++] = bmpPalette[texels[s]].blue;
    }

    free(bmpPalette);
    free(texels);
}