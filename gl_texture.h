// gl_utils.h
#ifndef GL_UTILS_H
#define GL_UTILS_H

#include "doomtype.h"
#include "gldefs.h"

#define GLD_TRANSPARENT 0
#define GLD_COLORED     1

void GL_MakeScreenTexture(patch_t *patch, GLTexData *Tex);
void GL_MakeSpriteTexture(patch_t *patch, GLTexData *Tex, dboolean smooth);
void GL_MakeGreyFontTexture(patch_t *patch, GLTexData *Tex, dboolean smooth);
unsigned int MakeRGBTexture(int dw, int dh);
unsigned int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh);
void ConvertToRawTexture(int x, int y, unsigned char* buff, patch_t* patch);
void ConvertToRawTextureOffset(int x, int y, unsigned char* buff, int bx, int by, int patchnum);
unsigned int GL_LoadFlatTexture(int TexNumb);
unsigned int GL_LoadSkyTop( char *filename );

#endif