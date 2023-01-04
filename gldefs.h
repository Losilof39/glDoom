// GLDefs.h

#include "doomtype.h"

#ifndef GLDEFS
#define GLDEFS

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
   }GLPixelRGBA;

typedef struct
   {
    int   TexName;
    float Width;   // Actual texture width
    float Height;  // Actual texture height
    float XDisp;   // Texture coordinate displacement from right side
    float YDisp;   // Texture coordinate displacement from bottom
    float LeftOff;
    float TopOff;
    float glWidth;
    float glHeight;
    int   Translucent;
    int   Intensity;
    dboolean Permanent;   // Is this sprite just for a level?
   }GLTexData;

typedef struct
   {
    GLPixelRGBA *Pixels;  // Array of pixels
    float        Width;   // Actual texture width
    float        Height;  // Actual texture height
    float        LeftOff; // Left offset of pixels
    float        TopOff;  // Top offset of pixels
   }GLPixelmap;

#endif
