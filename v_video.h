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
//	Gamma correction LUT.
//	Functions to draw patches (by post) directly to screen.
//	Functions to blit a block to the screen.
//
//-----------------------------------------------------------------------------


#ifndef __V_VIDEO__
#define __V_VIDEO__

#include "doomtype.h"

#include "doomdef.h"

// Needed because we are refering to patches.
#include "r_data.h"

#include "mathlib.h"

//
// VIDEO
//

#define CENTERY			(SCREENHEIGHT/2)


// Screen 0 is the screen updated by I_Update screen.
// Screen 1 is an extra buffer.



extern	byte *screens[5];

extern  int	dirtybox[4];

extern	byte	gammatable[5][256];
extern	int	usegamma;

// symbolic indices into color translation table pointer array
typedef enum
{
  CR_BRICK,   //0
  CR_TAN,     //1
  CR_GRAY,    //2
  CR_GREEN,   //3
  CR_BROWN,   //4
  CR_GOLD,    //5
  CR_RED,     //6
  CR_BLUE,    //7
  CR_ORANGE,  //8
  CR_YELLOW,  //9
  CR_LIMIT    //10 //jff 2/27/98 added for range check
} crange_idx_e;
//jff 1/16/98 end palette color range additions


// Allocates buffer screens, call before R_Init.
void V_Init (void);


void
V_CopyRect
( int		srcx,
  int		srcy,
  int		srcscrn,
  int		width,
  int		height,
  int		destx,
  int		desty,
  int		destscrn );

void
V_DrawPatchOffsetBuff
( int		     x,
  int		     y,
  unsigned char *buff,
  int            bx,  // buffer width
  int            by,  // buffer height -- probably not necessary
  int            patchnum);

void
V_DrawPatchBuff
( int		x,
  int		y,
  unsigned char *buff,
  patch_t*	patch);

void
V_DrawPatch
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch);

void
V_DrawPatchDirect
( int		x,
  int		y,
  int		scrn,
  patch_t*	patch );


// Draw a linear block of pixels into the view buffer.
void
V_DrawBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		src );

// Reads a linear block of pixels into the view buffer.
void
V_GetBlock
( int		x,
  int		y,
  int		scrn,
  int		width,
  int		height,
  byte*		dest );


void
V_MarkRect
( int		x,
  int		y,
  int		width,
  int		height );

// New data structure for video control
// This data is "generic" in that no specific
// renderer is assumed.  It does, however try
// to handle all necessary data for a true 3D
// rendering sub-system.

typedef struct
   {
    int        width;
    int        height;
    int        bpp;
    dboolean        fullscreen;
    int        allowsoft;
    float      nearclip;
    float      farclip;
    float      fov;
    float      fovy;
    float      hFov;
    float      vFov;
    ml_vec3_t  frustum[5];
    ml_vec3_t  normals[5];
    ml_vert3_t origin;
    ml_vert3_t nearplane[4];
    ml_vert3_t farplane[4];
	dboolean   wide;
   }video_t;

#define DEF_WIDTH     640
#define DEF_HEIGHT    480
#define DEF_COLORB     32

#define FOV_MAX   160.0f
#define FOV_MIN   10.0f
#define FOV_DEF   90.0f

#define LCLIP     0
#define RCLIP     1
#define BCLIP     2
#define TCLIP     3

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
