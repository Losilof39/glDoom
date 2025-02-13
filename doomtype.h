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
#include <limits.h>

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

typedef unsigned char dbyte;

//#define ENABLE_GLCONSOLE 

#ifndef O_BINARY 
#define O_BINARY 0
#endif

// Fixed to use builtin bool type with C++.
#ifdef __cplusplus
typedef bool dboolean;
#else
#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

typedef int dboolean;
#endif

#if defined(_MSC_VER) && !defined(ANSI_C)
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define strupr _strupr
#define strlwr _strlwr
#endif

#if defined(_MSC_VER)
#define Open _open
#define Close _close
#define Read _read
#define LSeek _lseek
#define Write _write
#define Access _access
#define Getcwd _getcwd
#define Tell _tell
#else
#define Open open
#define Close close
#define Read read
#define LSeek lseek
#define Write write
#define Access access
#define Getcwd getcwd
#define Tell tell
#endif

#ifdef _WIN64
#define GetTicks SDL_GetTicks64
#else
#define GetTicks SDL_GetTicks
#endif

#define arrlen(array) (sizeof(array) / sizeof(*array))

// Predefined with some OS.
#ifndef CHAR_MIN
#define CHAR_MIN 128
#endif

#ifndef CHAR_MAX
#define CHAR_MAX 128
#endif

#ifndef SHRT_MIN
#define SHRT_MIN (-32768)
#endif

#ifndef SHRT_MAX
#define SHRT_MAX      32767
#endif

#ifndef INT_MIN
#define INT_MIN (-2147483647 - 1)
#endif

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef LONG_MIN
#define LONG_MIN (-2147483647L - 1)
#endif

#ifndef LONG_MAX
#define LONG_MAX 2147483647L
#endif

#define DMAXCHAR CHAR_MAX
#define DMAXSHORT SHRT_MAX
#define DMAXINT INT_MAX
#define DMAXLONG LONG_MAX
#define DMINCHAR CHAR_MIN
#define DMINSHORT SHRT_MIN
#define DMININT INT_MIN
#define DMINLONG LONG_MIN

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
