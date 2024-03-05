// gl_texture.h
#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "doomtype.h"
#include "gldefs.h"

#define GLD_TRANSPARENT 0
#define GLD_COLORED     1

int GL_MakeScreenTexture(patch_t *patch, GLTexData *Tex);
int GL_MakeSpriteTexture(patch_t *patch, GLTexData *Tex, dboolean smooth);
int GL_MakeGreyFontTexture(patch_t *patch, GLTexData *Tex, dboolean smooth);
unsigned int MakeRGBTexture(int dw, int dh);
unsigned int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh);
void ConvertToRawTexture(int x, int y, unsigned char* buff, patch_t* patch);
void ConvertToRawTextureOffset(int x, int y, unsigned char* buff, int bx, int by, int patchnum);
int GL_LoadFlatTexture(int TexNumb);
int GL_LoadSkyTop( char *filename );

#endif