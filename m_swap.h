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
//	Endianess handling, swapping 16bit and 32bit.
//
//-----------------------------------------------------------------------------


#ifndef __M_SWAP__
#define __M_SWAP__

#ifdef __GNUG__
#pragma interface
#endif

#ifdef __linux__
#include <SDL2/SDL_endian.h>
#else
#include <SDL_endian.h> /* Header for handle the endian swapping */
#endif
// Endianess handling.
// WAD files are stored little endian.

/* 
** Andr�:
** Taken from Doom64EX+: 
** Defines for checking the endianness of the system. 
*/

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SYS_LITTLE_ENDIAN
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SYS_BIG_ENDIAN
#endif

#if 0 /* Andr�: Unused now, Now uses the SDL_Swap api for handling the endian support. */
#ifdef __BIG_ENDIAN__
short	SwapSHORT(short);
long long	SwapLONG(long long);
#define DSHORT(x)	((short)SwapSHORT((unsigned short) (x)))
#define DLONG(x)         ((long long)SwapLONG((unsigned long long) (x)))
#else
#define DSHORT(x)	(x)
#define DLONG(x)         (x)
#endif
#else
#if SYS_BIG_ENDIAN
#define DSHORT(x)	((short)SDL_SwapLE64((unsigned short) (x)))
#define DLONG(x)         ((long long)SDL_SwapLE64((unsigned long long) (x)))
#elif SDL_LIL_ENDIAN
#define DSHORT (short)SDL_SwapLE16
#define DLONG (signed long)SDL_SwapLE32
#endif
#endif


#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
