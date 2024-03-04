/*
** File: d_bitmap.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/30/2023
** Description:
** File created for add separately the bmp loader/parser from gl_texture.c
** License: Public domain/ID License 
*/

#include "d_bitmap.h"
#include <stdlib.h>

void D_LoadBmp(dbyte* rgb, const char* filename, int width, int height)
{
    int fn, s, d;
    unsigned char* texels;
    bitmapfile_t bmfh;
    bitmapinfo_t bmi;
    rgbquad_t* bmpPalette;

    fn = Open(filename, O_RDONLY | O_BINARY);

    Read(fn, &bmfh, sizeof(bitmapfile_t));
    Read(fn, &bmi, sizeof(bitmapfile_t));

    bmi.width = width;
    bmi.height = height;

    bmpPalette = (rgbquad_t*)malloc(sizeof(rgbquad_t) * 256);
    Read(fn, bmpPalette, (sizeof(rgbquad_t) * 256));

    texels = (unsigned char*)malloc(width * height);
    LSeek(fn, (long)bmfh.offBits, SEEK_SET);
    Read(fn, texels, (width * height));
    Close(fn);

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