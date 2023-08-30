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
#else
typedef unsigned char byte;
typedef short        SHORT;
typedef long          LONG;
typedef unsigned short WORD;
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
