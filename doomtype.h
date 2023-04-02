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

#ifndef __DOOMTYPE__
#define __DOOMTYPE__

#include <stdio.h>
#ifdef ANSI_C
#include "doomlib.h"
#endif
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>


#ifndef MAX
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifdef _MSC_VER
#include <rpc.h>
#include <rpcndr.h>
#endif

#ifndef _MSC_VER
typedef unsigned char byte;
typedef short        SHORT;
typedef long          LONG;
typedef unsigned short WORD;

// win32 structs manually defined to be more portable
typedef struct BITMAPFILEHEADER {
    uint16_t  bfType;
    uint32_t bfSize;
    uint16_t  bfReserved1;
    uint16_t  bfReserved2;
    uint32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER {
    uint32_t biSize;
    LONG  biWidth;
    LONG  biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    byte rgbBlue;
    byte rgbGreen;
    byte rgbRed;
    byte rgbReserved;
} RGBQUAD;

typedef struct RECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

#define O_BINARY 0
#endif

// Fixed to use builtin bool type with C++.
#ifdef __cplusplus
typedef bool dboolean;
#else
typedef enum
{
    false,
    true
} dboolean;
#endif


static long filelength_(handle) { fseek(handle, 0L, SEEK_END); long sz = ftell(handle); fseek(handle, 0L, SEEK_SET); return sz; }

#ifndef ANSI_C
#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif
#endif

#if defined(_MSC_VER)
#define Open(filename, openflag, ...) _open(filename, openflag, __VA_ARGS__)
#define Close(filehandle) _close(filehandle)
#define Read(filehandle, dstbuf, maxcharcount) _read(filehandle, dstbuf, maxcharcount)
#define LSeek(filehandle, offset, origin) _lseek(filehandle, offset, origin)
#define Write(filehandle, buf, maxcharcount) _write(filehandle, buf, maxcharcount)
#define Access(filename, accessmode) _access(filename, accessmode)
#else
#define Open(filename, openflag, ...) open(filename, openflag)
#define Close(filehandle) close(filehandle)
#define Read(filehandle, dstbuf, maxcharcount) read(filehandle, dstbuf, maxcharcount)
#define LSeek(filehandle, offset, origin) lseek(filehandle, offset, origin)
#define Write(filehandle, buf, maxcharcount) write(filehandle, buf, maxcharcount)
#define Access(filename, accessmode) access(filename, accessmode)
#endif

#define arrlen(array) (sizeof(array) / sizeof(*array))

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
