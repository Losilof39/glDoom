// Emacs style mode select   -*- C -*- 
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
#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif
#include <inttypes.h>
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
    long  biWidth;
    long  biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    long  biXPelsPerMeter;
    long  biYPelsPerMeter;
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
    long left;
    long top;
    long right;
    long bottom;
} RECT;
#endif

#ifndef O_BINARY 
#define O_BINARY 0
#endif

// Fixed to use builtin bool type with C++.
#ifdef __cplusplus
typedef bool dboolean;
#else
#define false 0
#define true 1
typedef int dboolean;
#endif

#if defined(_MSC_VER) && !defined(ANSI_C)
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strupr _strupr
#define strlwr _strlwr
#endif

#if defined(_MSC_VER)
#define Open(filename, openflag, ...) _open(filename, openflag)
#define Close(filehandle) _close(filehandle)
#define Read(filehandle, dstbuf, maxcharcount) _read(filehandle, dstbuf, maxcharcount)
#define LSeek(filehandle, offset, origin) _lseek(filehandle, offset, origin)
#define Write(filehandle, buf, maxcharcount) _write(filehandle, buf, maxcharcount)
#define Access(filename, accessmode) _access(filename, accessmode)
#define Getcwd(dstbuf, size_in_bytes) _getcwd(dstbuf, size_in_bytes)
#define Tell(FileHandle) _tell(FileHandle)
#else
#define Open(filename, openflag, ...) open(filename, openflag)
#define Close(filehandle) close(filehandle)
#define Read(filehandle, dstbuf, maxcharcount) read(filehandle, dstbuf, maxcharcount)
#define LSeek(filehandle, offset, origin) lseek(filehandle, offset, origin)
#define Write(filehandle, buf, maxcharcount) write(filehandle, buf, maxcharcount)
#define Access(filename, accessmode) access(filename, accessmode)
#define Getcwd(dstbuf, size_in_bytes) getcwd(dstbuf, size_in_bytes)
#define Tell(FileHandle) tell(FileHandle)
#endif

#ifdef _WIN64
#define GetTicks SDL_GetTicks64
#else
#define GetTicks SDL_GetTicks
#endif

#define arrlen(array) (sizeof(array) / sizeof(*array))

// Predefined with some OS.
#ifdef IMPL
#define DMINCHAR    128
#define DMAXCHAR    127
#define DMINSHORT    (-32768)
#define DMAXSHORT      32767
#define DMININT     (-2147483647 - 1)
#define DMAXINT       2147483647
#define DMINLONG    (-2147483647L - 1)
#define DMAXLONG      2147483647L
#else
#define DMAXCHAR CHAR_MAX
#define DMAXSHORT SHRT_MAX
#define DMAXINT INT_MAX
#define DMAXLONG LONG_MAX
#define DMINCHAR CHAR_MIN
#define DMINSHORT SHRT_MIN
#define DMININT INT_MIN
#define DMINLONG LONG_MIN
#endif

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
