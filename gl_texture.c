#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <inttypes.h>
#include <fcntl.h>
#include <glad/glad.h>
#include "doomdata.h"
#include "r_defs.h"
#include "gldefs.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"

#define GLD_TRANSPARENT 0
#define GLD_COLORED     1

void lfprintf(char *message, ... );

unsigned int MakeRGBTexture(int dw, int dh);
unsigned int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh);
unsigned int MakeGreyTexture(dboolean clamp, dboolean smooth, int dw, int dh);
void V_DrawPatchBuff(int, int, unsigned char *, patch_t *);

unsigned char  PointPattern[16*16] = {   0,  0,  0,  0,  0,  0,  0, 31, 31,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0, 31, 63, 63, 31,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0, 31, 63, 91, 91, 63, 31,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0, 47, 63, 91,127,127, 91, 63, 47,  0,  0,  0,  0,
                                         0,  0,  0, 47, 79, 91,127,159,159,127, 91, 79, 47,  0,  0,  0,
                                         0,  0, 31, 63,107,143,159,191,191,159,143,107, 63, 31,  0,  0,
                                         0, 31, 63, 91,127,159,191,223,223,191,159,127, 91, 63, 31,  0,
                                        31, 63, 91,127,159,191,223,255,255,223,191,159,127, 91, 63, 31,
                                        31, 63, 91,127,159,191,223,255,255,223,191,159,127, 91, 63, 31,
                                         0, 31, 63, 91,127,159,191,223,223,191,159,127, 91, 63, 31,  0,
                                         0,  0, 31, 63,107,143,159,191,191,159,143,107, 63, 31,  0,  0,
                                         0,  0,  0, 47, 79, 91,127,159,159,127, 91, 79, 47,  0,  0,  0,
                                         0,  0,  0,  0, 47, 63, 91,127,127, 91, 63, 47,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0, 31, 63, 91, 91, 63, 31,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0, 31, 63, 63, 31,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0, 31, 31,  0,  0,  0,  0,  0,  0,  0 };


//unsigned char         PointPattern[16*16];

unsigned char         TexRaw[512*512];
unsigned char         SkyRaw[1024*128];
unsigned char         Transparent[512*512];
unsigned char         TexAlpha[512*512];
unsigned char         RawBuff[512*512];
int                   TexWide, TexHigh;
GLubyte              *TexRGB, *MipMaps[16][32768];
dboolean              TexTransparent = false;
extern dboolean       GL_3Dlabs;

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
   }MY_PAL;

extern MY_PAL  gamepal[256];
MY_PAL         statpal[256];

DW_TexList     TexList[1024];

extern int gl_alphatest;
extern int gl_widetex;
extern int gl_core;

// A single patch from a texture definition,
//  basically a rectangular area within
//  the texture rectangle.
typedef struct
{
    // Block origin (allways UL),
    // which has allready accounted
    // for the internal origin of the patch.
    int		originx;	
    int		originy;
    int		patch;
} texpatch_t;

// A maptexturedef_t describes a rectangular texture,
//  which is composed of one or more mappatch_t structures
//  that arrange graphic patches.
typedef struct
{
    // Keep name for switch changing, etc.
    char	name[8];		
    short	width;
    short	height;
    
    // All the patches[patchcount]
    //  are drawn back to front into the cached texture.
    short	patchcount;
    texpatch_t	patches[1];		
    
} texture_t;

void InitGLPalette(int red, int green, int blue)
   {
    int i;

    for (i = 0; i < 256; i++)
       {
        statpal[i].r = red;
        statpal[i].g = green;
        statpal[i].b = blue;
       }
   }


int CreateColorMap(int red, int green, int blue)
   {
    int             TempTexName;
    unsigned char   TexData[4];

    TexWide = 1;
    TexHigh = 1;

    TexData[0] = red;
    TexData[1] = green;
    TexData[2] = blue;
    TexData[3] = 255;

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWide, TexHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexData);

    glBindTexture(GL_TEXTURE_2D, 0);
    return(TempTexName);
   }

int CreatePointLightMap(int red, int green, int blue, dboolean alphaonly)
   {
    int             r, d, t, TempTexName;

    TexWide = 16;
    TexHigh = 16;

    TexRGB =  (GLubyte *)malloc(TexWide*(TexHigh*4));

    t = (TexHigh - 1);
    for (r = 0, d = 0; r < (TexWide*TexHigh); r++, d += 4)
       {
        if (!alphaonly)
           {
            //TexRGB[d]   = (unsigned char)(((float)PointPattern[r] / 255.0f) * red);
            //TexRGB[d+1] = (unsigned char)(((float)PointPattern[r] / 255.0f) * green);
            //TexRGB[d+2] = (unsigned char)(((float)PointPattern[r] / 255.0f) * blue);
            if (PointPattern[r] == 0)
               {
                TexRGB[d+3] = 0;
               }
            else
               {
                TexRGB[d]   = 255;
                TexRGB[d+1] = 255;
                TexRGB[d+2] = 255;
                TexRGB[d+3] = 255;
               }
           }
        else
           {
            TexRGB[d]   = 0;
            TexRGB[d+1] = 0;
            TexRGB[d+2] = 0;
            TexRGB[d+3] = PointPattern[r];
           }
       }

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWide, TexHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexRGB);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(TexRGB);
    return(TempTexName);
   }

void GeneratePointPattern()
   {
    int i, x, base, color;

    return;

    memset(PointPattern, 0, 256);
    base = 63;
    for (i = 0; i < 8; i++)
       {
        for (x = (7 - i); x < 8; x++)
           {
            color = base - ((8 - x) * 32);
            if (color < 0)
                color = 0;
            PointPattern[(i*16)+x] = color;
            PointPattern[((i*16)+15)-x] = color;
            PointPattern[((15-i)*16)+x] = color;
            PointPattern[(((15-i)*16)+15)-x] = color;
           }
        base += 32;
       }
   }

extern texture_t**	textures;

int GL_LoadSkyTop( char *filename )
   {
    unsigned int TempTexName;
    int fn, s, d;
    unsigned char   *texels;
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmi;
    RGBQUAD         *bmpPalette;

    fn = Open(filename, O_RDONLY | O_BINARY );

    Read(fn, &bmfh, sizeof(BITMAPFILEHEADER));
    Read(fn, &bmi, sizeof(BITMAPINFOHEADER));

    TexWide = bmi.biWidth;
    TexHigh = bmi.biHeight;

    bmpPalette = (RGBQUAD *)malloc(sizeof(RGBQUAD)*256);
    Read(fn, bmpPalette, (sizeof(RGBQUAD)*256));

    texels = (unsigned char *)malloc(TexWide*TexHigh);
    LSeek(fn, bmfh.bfOffBits, SEEK_SET);
    Read(fn, texels, (TexWide*TexHigh));
    Close(fn);

    TexRGB =  (GLubyte *)malloc(TexWide*(TexHigh*3));
    for (s = 0, d = 0; s < (TexWide*TexHigh); s++)
       {
        TexRGB[d++] = bmpPalette[texels[s]].rgbRed;
        TexRGB[d++] = bmpPalette[texels[s]].rgbGreen;
        TexRGB[d++] = bmpPalette[texels[s]].rgbBlue;
       }

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexWide, TexHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, TexRGB);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(TexRGB);
    free(texels);
    free(bmpPalette);
    return(TempTexName);
   }

int GL_LoadTexture(int TexNumb)
   {
    int            x, n, r, wide, s, d, glw,w;
    int            TempTexNumb = 0, iPower;
    short          sx, sy, fields;
    int            px, py;

    if (TexList[TexNumb].DWide > 256)
       {
        return 0;
       }

    TexWide = sx = TexList[TexNumb].DWide;
    TexHigh = sy = TexList[TexNumb].DHigh;
    for (x = 0, iPower = 2; ((x < 8) && (TexWide > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexWide) && (iPower <= 256))
       TexList[TexNumb].GLWide = iPower;
    for (x = 0, iPower = 2; ((x < 8) && (TexHigh > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexHigh) && (iPower <= 256))
       TexList[TexNumb].GLHigh = iPower;
    wide = 1;
    while (TexList[TexNumb].GLHigh > ((TexList[TexNumb].GLWide * wide) * 8))
       {
        wide *= 2;
       }
    TexList[TexNumb].GLWide *= wide;
    // Don't think we need this for height...
    while (TexList[TexNumb].GLWide > (TexList[TexNumb].GLHigh * 8))
    {
        TexList[TexNumb].GLHigh = TexList[TexNumb].GLHigh * 2;
    }
    TexWide = TexList[TexNumb].GLWide;
    TexHigh = TexList[TexNumb].GLHigh;
    for (n = 0; n < (TexWide*TexHigh); n++)
       {
        TexRaw[n] = 110;
        Transparent[n] = GLD_TRANSPARENT;
       }

    fields = textures[TexList[TexNumb].Number]->patchcount;
    for (n = 0; n < fields; n++)
       {
        px = textures[TexList[TexNumb].Number]->patches[n].originx;
        py = textures[TexList[TexNumb].Number]->patches[n].originy;
        V_DrawPatchOffsetBuff( px,py, TexRaw,
                               TexWide, TexHigh,
                               textures[TexList[TexNumb].Number]->patches[n].patch);
       }
    if (wide > 1)
       {
        s = 0;
        d = TexList[TexNumb].DWide;
        glw = TexWide;
        w = TexList[TexNumb].DWide;
        for (r = 0; r < TexHigh; r++)
           {
            memcpy(&TexRaw[d], &TexRaw[s], w);
            memcpy(&Transparent[d], &Transparent[s], w);
            s += glw;
            d += glw;
           }
        TexList[TexNumb].DWide *= 2;
       }
    memcpy(TexAlpha, Transparent, (TexWide*TexHigh));
    if (gl_alphatest == 1)
       {
        TempTexNumb = MakeRGBATexture(false, true, TexList[TexNumb].DWide, TexList[TexNumb].DHigh);
       }
    else
       {
        TempTexNumb = MakeRGBATexture(false, false, TexList[TexNumb].DWide, TexList[TexNumb].DHigh);
       }
    return(TempTexNumb);
   }

int GL_LoadSkyTexture(int TexNumb, int *SkyTex)
   {
    int            x, n, r, s, d, glw,w;
    int            TempTexNumb = 0, iPower;
    short          sx, sy, fields;
    int            px, py, part, parts;

    if ((TexList[TexNumb].DWide > 1024) || (TexList[TexNumb].DHigh > 1024))
       {
        SkyTex[0] = 0;
        SkyTex[1] = 0;
        SkyTex[2] = 0;
        SkyTex[3] = 0;
        return 1;
       }

    TexWide = sx = TexList[TexNumb].DWide;
    TexHigh = sy = TexList[TexNumb].DHigh;
    for (x = 0, iPower = 2; ((x < 9) && (TexWide > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexWide) && (iPower <= 1024))
       TexList[TexNumb].GLWide = iPower;
    for (x = 0, iPower = 2; ((x < 8) && (TexHigh > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexHigh) && (iPower <= 256))
       TexList[TexNumb].GLHigh = iPower;
    TexWide = TexList[TexNumb].GLWide;
    TexHigh = TexList[TexNumb].GLHigh;

    parts = TexWide / 256;

    memset(SkyRaw, 0, TexWide*TexHigh);

    fields = textures[TexList[TexNumb].Number]->patchcount;
    for (n = 0; n < fields; n++)
       {
        px = textures[TexList[TexNumb].Number]->patches[n].originx;
        py = textures[TexList[TexNumb].Number]->patches[n].originy;
        V_DrawPatchOffsetBuff( px,py, SkyRaw,
                               //TexList[TexNumb].DWide, TexList[TexNumb].DHigh,
                               TexWide, TexHigh,
                               textures[TexList[TexNumb].Number]->patches[n].patch);
       }

    TexWide = 256;
    for (part = 0; part < parts; part++)
       {
        s = part * 256;
        d = 0;
        glw = 256;
        w = TexList[TexNumb].DWide;
        memset(TexRaw, 0, 256*w);
        for (r = 0; r < TexHigh; r++)
           {
            memcpy(&TexRaw[d], &SkyRaw[s], glw);
            s += w;
            d += glw;
           }
        TempTexNumb = MakeRGBTexture(256, TexList[TexNumb].DHigh);
        SkyTex[part] = TempTexNumb;
       }
    while(part < 4)
       {
        SkyTex[part] = SkyTex[0];
        part++;
       }
    return(part);
   }

extern int firstflat;

int GL_LoadFlatTexture(int TexNumb)
   {
    unsigned int            TempTexNumb;
    int n;
    unsigned char *lump, *pixels;

    lump = W_CacheLumpNum(firstflat+TexList[TexNumb].Number, PU_CACHE);

    TexWide = TexList[TexNumb].GLWide = TexList[TexNumb].DWide;
    TexHigh = TexList[TexNumb].GLHigh = TexList[TexNumb].DHigh;
    
    pixels = &lump[0];

    for (n = 0; n < TexList[TexNumb].GLWide*TexList[TexNumb].GLHigh; n++)
       {
        TexRaw[n] = pixels[n];
        TexAlpha[n] = GLD_COLORED;
       }
    TempTexNumb = MakeRGBATexture(false, false, TexList[TexNumb].DWide, TexList[TexNumb].DHigh);
    return( TempTexNumb );
   }

int Translucent = false;

//
// GL_MakeGreyFontTexture
// Masks a column based masked pic into an OpenGL texture
//
void GL_MakeGreyFontTexture(patch_t *Sprite, GLTexData *Tex, dboolean smooth)
   {
    static int      x, ixsize, iysize;
    unsigned short *tshort;

    int		       i, j, n;
    int            iOffSet, iPower;

    int            TempTexNumb;
    int            iGLWide, iGLHigh;

    float coords[] = { 0.0f, 1.0f, 0.0f, Tex->YDisp, Tex->XDisp, Tex->YDisp, Tex->XDisp, 1.0f };

    tshort = (unsigned short *)Sprite;
    ixsize = *tshort;

    tshort++;

    iysize = *tshort;

    TexWide = ixsize;
    TexHigh = iysize;
    for (x = 0, iPower = 2; ((x < 8) && (TexWide > iPower)); x++)
         iPower = iPower * 2;
    if ((iPower >= TexWide) && (iPower <= 512))
        iGLWide = iPower;
    for (x = 0, iPower = 2; ((x < 8) && (TexHigh > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexHigh) && (iPower <= 512))
        iGLHigh = iPower;

    while ((iGLWide / iGLHigh) > 8)
       iGLHigh *= 2;
    while ((iGLHigh / iGLWide) > 8)
       iGLWide *= 2;
    TexWide = iGLWide;
    TexHigh = iGLHigh;

    for (n = 0; n < ixsize*iysize; n++)
       {
        RawBuff[n] = 0;
        Transparent[n] = GLD_TRANSPARENT;
       }
    V_DrawPatchBuff(0, 0, RawBuff, Sprite);
    for (n = 0; n < (TexWide*TexHigh); n++)
       {
        TexRaw[n] = 0;
        TexAlpha[n] = GLD_TRANSPARENT;
       }
    for (i = 0, n = 0; i < iysize; i++)
       {
        iOffSet = i * TexWide;
        for (j = 0; j < ixsize; j++, n++)
           {
            TexRaw[iOffSet+j] = RawBuff[n];
            TexAlpha[iOffSet+j] = Transparent[n];
           }
       }

    if (gl_alphatest == 1)
        Translucent = Tex->Translucent;
    else
        Translucent = 0;

    if (gl_alphatest == 1)
       {
        TempTexNumb = MakeGreyTexture(true, smooth, ixsize, iysize);
       }
    else
       {
        TempTexNumb = MakeGreyTexture(true, false, ixsize, iysize);
       }
    Tex->TexName = TempTexNumb;
    Tex->Width = (float)Sprite->width;
    Tex->Height = (float)Sprite->height;
    Tex->XDisp = (float)Sprite->width / (float)TexWide;
    Tex->YDisp = 1.0f - ((float)Sprite->height / (float)TexHigh);
    Tex->LeftOff = (float)Sprite->leftoffset;
    Tex->TopOff = (float)Sprite->topoffset;
    Tex->glWidth = (float)TexWide;
    Tex->glHeight = (float)TexHigh;
    Translucent = false;

    if (gl_core)
    {
        glGenBuffers(1, &Tex->texVBO);
        glBindBuffer(GL_ARRAY_BUFFER, Tex->texVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
   }

//
// GL_MakeSpriteTexture
// Masks a column based masked pic into an OpenGL texture
//
void GL_MakeSpriteTexture(patch_t *Sprite, GLTexData *Tex, dboolean smooth)
   {
    static int      x, ixsize, iysize, cwidth;
    unsigned short *tshort;

    int		       i, n;
    int            iOffSet, iPower;

    int            TempTexNumb;
    int            iGLWide, iGLHigh;

    if (Sprite == NULL)
        return 0;

    //ixsize = Sprite->width-Sprite->leftoffset;
    //iysize = Sprite->height-Sprite->topoffset;

    tshort = (unsigned short *)Sprite;
    ixsize = *tshort;
    tshort++;
    iysize = *tshort;

    if (ixsize > 256)
       {
        lfprintf("\ntexture size %dx%d", ixsize, iysize);
       }

    //ixsize = Sprite->width;
    //iysize = Sprite->height;

    TexWide = ixsize;
    TexHigh = iysize;
    for (x = 0, iPower = 2; ((x < 8) && (TexWide > iPower)); x++)
         iPower = iPower * 2;
    if ((iPower >= TexWide) && (iPower <= 512))
        iGLWide = iPower;
    for (x = 0, iPower = 2; ((x < 8) && (TexHigh > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexHigh) && (iPower <= 512))
        iGLHigh = iPower;

    while ((iGLWide / iGLHigh) > 8)
       iGLHigh *= 2;
    while ((iGLHigh / iGLWide) > 8)
       iGLWide *= 2;
    TexWide = iGLWide;
    TexHigh = iGLHigh;

    for (n = 0; n < ixsize*iysize; n++)
       {
        RawBuff[n] = 0;
        Transparent[n] = GLD_TRANSPARENT;
       }
    V_DrawPatchBuff(0, 0, RawBuff, Sprite);
    for (n = 0; n < (TexWide*TexHigh); n++)
       {
        TexRaw[n] = 0;
        TexAlpha[n] = GLD_TRANSPARENT;
       }
    if ((ixsize > 256) && (!gl_widetex))
       {
        TexWide = 256;
        cwidth = 256;
       }
    else
       {
        cwidth = ixsize;
       }
    iOffSet = 0;
    for (i = 0, n = 0; i < iysize; i++)
       {
        memcpy(&TexRaw[iOffSet], &RawBuff[n], cwidth);
        memcpy(&TexAlpha[iOffSet], &Transparent[n], cwidth);
        n += ixsize;
        iOffSet += TexWide;
       }

    if (gl_alphatest == 1)
        Translucent = Tex->Translucent;
    else
        Translucent = 0;

    if (gl_alphatest == 1)
       {
        //TempTexNumb = MakeRGBATexture(true, smooth, ixsize, iysize);
        TempTexNumb = MakeRGBATexture(true, smooth, cwidth, iysize);
       }
    else
       {
        //TempTexNumb = MakeRGBATexture(true, false, ixsize, iysize);
        TempTexNumb = MakeRGBATexture(true, smooth, cwidth, iysize);
       }
    Tex->TexName = TempTexNumb;
    //Tex->Width = (float)Sprite->width;
    Tex->Width = (float)cwidth;
    Tex->Height = (float)Sprite->height;
    //Tex->XDisp = (float)Sprite->width / (float)TexWide;
    Tex->XDisp = (float)cwidth / (float)TexWide;
    Tex->YDisp = 1.0f - ((float)Sprite->height / (float)TexHigh);
    Tex->LeftOff = (float)Sprite->leftoffset;
    Tex->TopOff = (float)Sprite->topoffset;
    Tex->glWidth = (float)TexWide;
    Tex->glHeight = (float)TexHigh;
    Translucent = false;

    float coords[] = { 0.0f, 1.0f, 0.0f, Tex->YDisp, Tex->XDisp, Tex->YDisp, Tex->XDisp, 1.0f };

    if (gl_core)
    {
        glGenBuffers(1, &Tex->texVBO);
        glBindBuffer(GL_ARRAY_BUFFER, Tex->texVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
   }

// GL_MakeWideSpriteTexture
// Masks a column based masked pic into OpenGL textures
//
int GL_MakeWideSpriteTexture(patch_t *Screen, GLTexData *Tex)
   {
    static int      x, ixsize, iysize;
    unsigned short *tshort;
    int		       i, d, s;
    int            TempTexNumb;
    int            iGLWide, iGLHigh, iPower;

    tshort = (unsigned short *)Screen;
    ixsize = *tshort;
    tshort++;
    //ixsize = Screen->width;
    iysize = *tshort;
    //iysize = Screen->height;

    TexHigh = iysize;

    for (x = 0, iPower = 2; ((x < 8) && (TexHigh > iPower)); x++)
       iPower = iPower * 2;
    if ((iPower >= TexHigh) && (iPower <= 512))
        iGLHigh = iPower;

    TexHigh = iGLHigh;
    TexWide = 256;

    memset(RawBuff, 0, ixsize*iysize);
    V_DrawPatchBuff(0, 0, RawBuff, Screen);
    memset(TexRaw, 0, 256*iysize);
    memset(TexRaw, GLD_TRANSPARENT, 256*iysize);
    for (i = 0, d = 0, s = 0; i < iysize; i++)
       {
        memcpy(&TexRaw[d], &RawBuff[s], 256);
        memcpy(&TexAlpha[d], &Transparent[s], 256);
        d += 256;
        s += ixsize;
       }

    TexWide = 256;

    if (gl_alphatest == 1)
       {
        TempTexNumb = MakeRGBATexture(true, true, 256, TexHigh);
       }
    else
       {
        TempTexNumb = MakeRGBATexture(true, false, 256, TexHigh);
       }
    Tex[0].TexName = TempTexNumb;
    Tex[0].Width = (float)TexWide;
    Tex[0].Height = (float)iysize;
    Tex[0].XDisp = 1.0f;
    Tex[0].YDisp = 1.0f - ((float)iysize/(float)TexWide);
    Tex[0].LeftOff = 0.0f;
    Tex[0].TopOff = 0.0f;
    Tex[0].glWidth = 256.0f;
    Tex[0].glHeight = (float)TexHigh;

    TexWide = ixsize - 256;

    for (x = 0, iPower = 2; ((x < 8) && (TexWide > iPower)); x++)
         iPower = iPower * 2;
    if ((iPower >= TexWide) && (iPower <= 512))
        iGLWide = iPower;

    memset(TexRaw, GLD_TRANSPARENT, 256*iysize);
    memset(TexRaw, 0, TexWide*TexHigh);
    for (i = 0, d = 0, s = 256; i < iysize; i++)
       {
        memcpy(&TexRaw[d], &RawBuff[s], ixsize-256);
        memcpy(&TexAlpha[d], &Transparent[s], ixsize-256);
        d += TexWide;
        s += ixsize;
       }

    if (gl_alphatest == 1)
       {
        TempTexNumb = MakeRGBATexture(true, true, TexWide, TexHigh);
       }
    else
       {
        TempTexNumb = MakeRGBATexture(true, false, TexWide, TexHigh);
       }
    Tex[1].TexName = TempTexNumb;
    Tex[1].Width = (float)ixsize-256;
    Tex[1].Height = (float)iysize;
    Tex[1].XDisp = (float)(ixsize-256)/(float)TexWide;
    Tex[1].YDisp = 1.0f - ((float)iysize/(float)TexHigh);
    Tex[1].LeftOff = 0.0f;
    Tex[1].TopOff = 0.0f;
    Tex[1].glWidth = (float)TexWide;
    Tex[1].glHeight = (float)TexHigh;

    return TempTexNumb;
   }

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
   }rgba_t;


void AntiAlias(rgba_t *inbuff, rgba_t *outbuff, int w, int h)
   {
    int     x, y, s, d, n;
    short   mr, mg, mb;

    for (y = 0, d = 0, s = 0; y < h; y++)
       {
        for (x = 0; x < w; x++, s++, d++)
           {
            if (inbuff[s].a != 0)
               {
                outbuff[s].r = inbuff[s].r;
                outbuff[s].g = inbuff[s].g;
                outbuff[s].b = inbuff[s].b;
                outbuff[s].a = inbuff[s].a;
               }
            else
               {
                n = 0;
                mr = 0;
                mg = 0;
                mb = 0;
                if (y > 0)
                   {
                    if (x < (w-1))
                       {
                        if (inbuff[s-(w-1)].a != 0)
                           {
                            mr += inbuff[s-(w-1)].r;
                            mg += inbuff[s-(w-1)].g;
                            mb += inbuff[s-(w-1)].b;
                            n++;
                           }
                       }
                    if (inbuff[s-w].a != 0)
                       {
                        mr += inbuff[s-w].r;
                        mg += inbuff[s-w].g;
                        mb += inbuff[s-w].b;
                        n++;
                       }
                    if (x > 0)
                       {
                        if (inbuff[s-(w+1)].a != 0)
                           {
                            mr += inbuff[s-(w+1)].r;
                            mg += inbuff[s-(w+1)].g;
                            mb += inbuff[s-(w+1)].b;
                            n++;
                           }
                       }
                   }
                if (x > 0)
                   {
                    if (inbuff[s-1].a != 0)
                       {
                        mr += inbuff[s-1].r;
                        mg += inbuff[s-1].g;
                        mb += inbuff[s-1].b;
                        n++;
                       }
                   }
                if (x < (w-1))
                   {
                    if (inbuff[s+1].a != 0)
                       {
                        mr += inbuff[s+1].r;
                        mg += inbuff[s+1].g;
                        mb += inbuff[s+1].b;
                        n++;
                       }
                   }
                if (y < (h-1))
                   {
                    if (x < (w-1))
                       {
                        if (inbuff[s+(w+1)].a != 0)
                           {
                            mr += inbuff[s+(w+1)].r;
                            mg += inbuff[s+(w+1)].g;
                            mb += inbuff[s+(w+1)].b;
                            n++;
                           }
                       }
                    if (inbuff[s+w].a != 0)
                       {
                        mr += inbuff[s+w].r;
                        mg += inbuff[s+w].g;
                        mb += inbuff[s+w].b;
                        n++;
                       }
                    if (x > 0)
                       {
                        if (inbuff[s+(w-1)].a != 0)
                           {
                            mr += inbuff[s+(w-1)].r;
                            mg += inbuff[s+(w-1)].g;
                            mb += inbuff[s+(w-1)].b;
                            n++;
                           }
                       }
                   }
                if (n > 0)
                   {
                    outbuff[s].r = mr / n;
                    outbuff[s].g = mg / n;
                    outbuff[s].b = mb / n;
                   }
                else
                   {
                    outbuff[s].r = 0;
                    outbuff[s].g = 0;
                    outbuff[s].b = 0;
                   }
                outbuff[s].a = inbuff[s].a;
               }
           }
       }
   }

//
// GL_MakeScreenTexture
// Draws a column based full-screen pic into OpenGL textures
//
void GL_MakeScreenTexture(patch_t *Screen, GLTexData *Tex)
   {
    static int     x, ixsize, iysize;
    unsigned short *tshort;
    int		       i, d, s;
    int            TempTexNumb;

    tshort = (unsigned short *)Screen;
    ixsize = *tshort;
    tshort++;
    iysize = *tshort;

    if ((ixsize != 320) || (iysize != 200))
       {
        // This is not a full screen image...
        return 0;
       }

    memset(RawBuff, 0, ixsize*iysize);
    V_DrawPatchBuff(0, 0, RawBuff, Screen);
    memset(TexRaw, 0, 256*200);
    for (i = 0, d = 0, s = 0; i < 200; i++)
       {
        memcpy(&TexRaw[d], &RawBuff[s], 256);
        d += 256;
        s += 320;
       }

    TexWide = 256;
    TexHigh = 256;

    TempTexNumb = MakeRGBTexture(256, 256);
    Tex[0].TexName = TempTexNumb;
    Tex[0].Width = (float)256;
    Tex[0].Height = (float)200;
    Tex[0].XDisp = 1.0f;
    Tex[0].YDisp = 1.0f - (200.0f/256.0f);
    Tex[0].LeftOff = 0.0f;
    Tex[0].TopOff = 0.0f;
    Tex[0].glWidth = 256.0f;
    Tex[0].glHeight = 256.0f;

    float coords[] = { 0.0f, 1.0f, 0.0f, Tex[0].YDisp, Tex[0].XDisp, Tex[0].YDisp, Tex[0].XDisp, 1.0f };

    if (gl_core)
    {
        glGenBuffers(1, &Tex->texVBO);
        glBindBuffer(GL_ARRAY_BUFFER, Tex->texVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    memset(TexRaw, 0, 64*200);
    for (i = 0, d = 0, s = 256; i < 200; i++)
       {
        memcpy(&TexRaw[d], &RawBuff[s], 64);
        d += 64;
        s += 320;
       }

    TexWide = 64;
    TexHigh = 256;

    TempTexNumb = MakeRGBTexture(64, 256);
    Tex[1].TexName = TempTexNumb;
    Tex[1].Width = 64.0f;
    Tex[1].Height = 200.0f;
    Tex[1].XDisp = 1.0f;
    Tex[1].YDisp = 1.0f-(200.0f/256.0f);
    Tex[1].LeftOff = 0.0f;
    Tex[1].TopOff = 0.0f;
    Tex[1].glWidth = 64.0f;
    Tex[1].glHeight = 256.0f;
    
    coords[3] = Tex[1].YDisp;
    coords[4] = Tex[1].XDisp;
    coords[5] = Tex[1].YDisp;
    coords[6] = Tex[1].XDisp;

    if (gl_core)
    {
        glGenBuffers(1, &Tex->texVBO2);
        glBindBuffer(GL_ARRAY_BUFFER, Tex->texVBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

   }

unsigned int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh)
   {
    int             r, c, d, h, t, m, n;
    unsigned int TempTexName;
    unsigned char  *TexAa;
    TexRGB =  (GLubyte *)malloc(TexWide*(TexHigh*4));

    TexTransparent = false;
    t = (TexHigh - 1);
    for (r = 0; r < TexHigh; r++)
       {
        h = t - r;
        m = (r * TexWide);
        for (c = 0; c < TexWide; c++)
           {
            n = m + c;
            d = ((h * TexWide) * 4) + (c * 4);
            TexRGB[d]   = statpal[TexRaw[n]].r;
            TexRGB[d+1] = statpal[TexRaw[n]].g;
            TexRGB[d+2] = statpal[TexRaw[n]].b;
            if (TexAlpha[n] == GLD_COLORED)
               {
                if (Translucent > 0)
                    TexRGB[d+3] = Translucent;
                else
                    TexRGB[d+3] = 255;
               }
            else
               {
                TexRGB[d+3] = 0;
                if ((c < dw) && (r < dh))
                   {
                    TexTransparent = true;
                   }
               }
           }
       }

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);
    if ((smooth == true) && (TexTransparent == true))
       {
        TexAa =  (GLubyte *)malloc(TexWide*(TexHigh*4));
        for (r = 0; r < (TexWide*TexHigh*4); r++)
           TexAa[r] = TexRGB[r];
        AntiAlias( (rgba_t *)TexAa, (rgba_t *)TexRGB, TexWide, TexHigh );
        free(TexAa);
       }
    if (clamp == true)
       {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       }
    else
       {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWide, TexHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexRGB);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(TexRGB);
    return(TempTexName);
   }

unsigned int MakeGreyTexture(dboolean clamp, dboolean smooth, int dw, int dh)
   {
    int             r, c, d, h, t, m, n;
    unsigned int TempTexName;
    unsigned char  *TexAa, tcolor;
    TexRGB =  (GLubyte *)malloc(TexWide*(TexHigh*4));

    TexTransparent = false;
    t = (TexHigh - 1);
    for (r = 0; r < TexHigh; r++)
       {
        h = t - r;
        m = (r * TexWide);
        for (c = 0; c < TexWide; c++)
           {
            n = m + c;
            d = ((h * TexWide) * 4) + (c * 4);
            tcolor = statpal[TexRaw[n]].r;
            if (statpal[TexRaw[n]].g > tcolor)
               {
                tcolor = statpal[TexRaw[n]].g;
               }
            if (statpal[TexRaw[n]].b > tcolor)
               {
                tcolor = statpal[TexRaw[n]].b;
               }
            TexRGB[d]   = tcolor;
            TexRGB[d+1] = tcolor;
            TexRGB[d+2] = tcolor;
            if (TexAlpha[n] == GLD_COLORED)
               {
                if (Translucent > 0)
                    TexRGB[d+3] = Translucent;
                else
                    TexRGB[d+3] = 255;
               }
            else
               {
                TexRGB[d+3] = 0;
                if ((c < dw) && (r < dh))
                   {
                    TexTransparent = true;
                   }
               }
           }
       }

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);

    if ((smooth == true) && (TexTransparent == true))
       {
        TexAa =  (GLubyte *)malloc(TexWide*(TexHigh*4));
        for (r = 0; r < (TexWide*TexHigh*4); r++)
           TexAa[r] = TexRGB[r];
        AntiAlias( (rgba_t *)TexAa, (rgba_t *)TexRGB, TexWide, TexHigh );
        free(TexAa);
       }
    if (clamp == true)
       {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       }
    else
       {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
       }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWide, TexHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexRGB);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(TexRGB);
    return(TempTexName);
   }

unsigned int MakeRGBTexture(int dw, int dh)
   {
    int             r, c, d, h, t, m, n;
    unsigned int TempTexName;
    TexRGB =  (GLubyte *)malloc(TexWide*(TexHigh*3));

    TexTransparent = false;
    t = (TexHigh - 1);
    for (r = 0; r < TexHigh; r++)
       {
        h = t - r;
        m = (r * TexWide);
        for (c = 0; c < TexWide; c++)
           {
            n = m + c;
            d = ((h * TexWide) * 3) + (c * 3);
            TexRGB[d]   = statpal[TexRaw[n]].r;
            TexRGB[d+1] = statpal[TexRaw[n]].g;
            TexRGB[d+2] = statpal[TexRaw[n]].b;
           }
       }

    glGenTextures(1, &TempTexName);
    glBindTexture(GL_TEXTURE_2D, TempTexName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Define the 2D texture image.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TexWide, TexHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, TexRGB);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(TexRGB);
    return(TempTexName);
   }

