#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <inttypes.h>
#include <glad/glad.h>

#include "doomtype.h"
#include "r_defs.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"
#include "gldefs.h"
#include "gl_texture.h"

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
   }MY_PAL;

extern MY_PAL  gamepal[256];
extern MY_PAL  statpal[256];

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
   }MY_RGBA;

extern int deathmatch;

static unsigned char         Transparent[512*512];
static unsigned char         TexAlpha[512*512];
extern int            TexWide, TexHigh;
extern GLTexData            SBarTexture;
extern GLTexData      SBarTex[2];
extern unsigned char  TexRaw[512*512];
static GLubyte* TexRGB;
static GLubyte *MipMaps[16][32768];

MY_RGBA StatFrag[40*32], StatArms[40*32], StatBack[4][35*31], *TempTex;

unsigned char patchbuff[320*200];
unsigned char platebuff[320*200];
static unsigned char MsgText[2048];
void lfprintf(char *message, ... );

void GL_StatArmsOvl(MY_RGBA *RGBData);
void  GL_MakeSpriteTexture(patch_t *, GLTexData *Tex, dboolean smooth);
unsigned int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh);

void InsertArmsPlate()
   {
    int y, x, soff, doff;

    soff = 0;
    doff = 104;
    for (y = 0; y < 32; y++)
       {
        for (x = 0; x < 40; x++)
           {
            TexRaw[doff+x] = platebuff[soff+x];
           }
        doff += 256;
        soff += 40;
       }
   }

void GL_CreateStatusBar(patch_t *sbar)
   {
    /*int yoff, y, d, x;

    for (x = 0; x < 256; x++)
       {
        TexAlpha[x] = GLD_COLORED;
        TexRaw[x] = 0;
        patchbuff[x] = 0;
       }

    ConvertToRawTexture( 0, 0, patchbuff, sbar);
    for (y = 0, d = 0; y < 32; y++)
       {
        yoff = y * 320;
        for (x = 0; x < 256; x++, d++)
           {
            TexRaw[d] = patchbuff[yoff+x];
           }
       }
    TexWide = 256;
    TexHigh = 32;
    if (deathmatch == 0)
       {
        InsertArmsPlate();
       }
    SBarTexture[0] = MakeRGBATexture(true, false, 256, 32);

    for (x = 0; x < 64; x++)
       {
        TexAlpha[x] = GLD_COLORED;
        TexRaw[x] = 0;
        patchbuff[x] = 0;
       }

    for (y = 0, d = 0; y < 32; y++)
       {
        yoff = (y * 320)+256;
        for (x = 0; x < 64; x++, d++)
           {
            TexRaw[d] = patchbuff[yoff+x];
           }
       }
    TexWide = 64;
    TexHigh = 32;
    SBarTexture[1] = MakeRGBATexture(true, false, 64, 32);*/

    GL_MakeSpriteTexture(sbar, &SBarTexture, false);

    if (deathmatch == 0)
       {
        GL_StatArmsOvl(StatArms);
       }
   }

void GL_RGBStatFrag(patch_t *sbar)
   {
    int            x, y, z, i, yoff;

    ConvertToRawTexture( 0, 0, platebuff, sbar);
    for (y = 0, i = 0; y < 32; y++)
       {
        yoff = (31-y) * 104;
        for (x = 0, z = 0; x < 40; x++, i++, z++)
           {
            StatFrag[i].r = gammatable[usegamma][statpal[patchbuff[yoff+z]].r];
            StatFrag[i].g = gammatable[usegamma][statpal[patchbuff[yoff+z]].g];
            StatFrag[i].b = gammatable[usegamma][statpal[patchbuff[yoff+z]].b];
            StatFrag[i].a = 255;
           }
       }
   }

void GL_RGBStatArms(patch_t *armsbg)
   {
    int            x, y, i, yoff;

    ConvertToRawTexture( 0, 0, platebuff, armsbg);

    yoff = 31*40;
    for (y = 0, i = 0; y < 32; y++)
       {
        for (x = 0; x < 40; x++, i++)
           {
            StatArms[i].r = gammatable[usegamma][statpal[platebuff[yoff+x]].r];
            StatArms[i].g = gammatable[usegamma][statpal[platebuff[yoff+x]].g];
            StatArms[i].b = gammatable[usegamma][statpal[platebuff[yoff+x]].b];
            StatArms[i].a = 255;
           }
        yoff -= 40;
       }
   }

void GL_StatArmsOvl(MY_RGBA *RGBData)
   {
    /*glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, SBarTexture[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 104, 0, 40, 32, GL_RGBA, GL_UNSIGNED_BYTE, RGBData );
    glDisable(GL_TEXTURE_2D);*/
   }

void GL_RGBStatBack(patch_t *statback, int player)
   {
    int            x, y, z, i, yoff;

    ConvertToRawTexture( 0, 0, patchbuff, statback);
    
    for (y = 0, i = 0; y < statback->height; y++)
       {
        yoff = ((statback->height-1) - y) * statback->width;
        for (x = 0, z = 0; x < statback->width; x++, z++, i++)
           {
            StatBack[player][i].r = gammatable[usegamma][statpal[patchbuff[yoff+z]].r];
            StatBack[player][i].g = gammatable[usegamma][statpal[patchbuff[yoff+z]].g];
            StatBack[player][i].b = gammatable[usegamma][statpal[patchbuff[yoff+z]].b];
            StatBack[player][i].a = 255;
           }
       }
   }
