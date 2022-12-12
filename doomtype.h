// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Simple basic typedefs, isolated here to make it easier
//	 separating modules.
//    
//-----------------------------------------------------------------------------

//#ifdef WIN32
//#include <windows.h>
//#endif

#include <stdio.h>

#ifndef __DOOMTYPE__
#define __DOOMTYPE__

#ifndef __BYTEBOOL__
#define __BYTEBOOL__
// Fixed to use builtin bool type with C++.
#ifdef __cplusplus
typedef bool dboolean;
#else
typedef enum { false, true } dboolean;
#endif
typedef unsigned char byte;

      typedef short        SHORT;
      typedef int          LONG;
      typedef unsigned int DWORD;
      typedef long long    DLONG;
      typedef unsigned short WORD;
      typedef unsigned char BYTE;
#endif

// win32 structs manually defined to be more portable
typedef struct BITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1;
    WORD  bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

typedef struct RECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
static long filelength_(handle) { fseek(handle, 0L, SEEK_END); long sz = ftell(handle); fseek(handle, 0L, SEEK_SET); return sz; }

//#define strcasecmp strcmp
//#define strncasecmp strncmp

// Predefined with some OS.
#ifdef LINUX
#include <values.h>
#else
#ifndef MAXCHAR
#define MAXCHAR		((char)0x7f)
#endif
#ifndef MAXSHORT
#define MAXSHORT	((short)0x7fff)
#endif

// Max pos 32-bit int.
#ifndef MAXINT
#define MAXINT		((int)0x7fffffff)	
#endif
#ifndef MAXLONG
#define MAXLONG		((long long)0x7fffffff)
#endif
#ifndef MINCHAR
#define MINCHAR		((char)0x80)
#endif
#ifndef MINSHORT
#define MINSHORT	((short)0x8000)
#endif

// Max negative 32-bit integer.
#ifndef MININT
#define MININT		((int)0x80000000)	
#endif
#ifndef MINLONG
#define MINLONG		((long long)0x80000000)
#endif
#endif


#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
