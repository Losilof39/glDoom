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
//  all external data is defined here
//  most of the data is loaded into different structures at run time
//  some internal structures shared by many modules are here
//
//-----------------------------------------------------------------------------

#ifndef __DOOMDATA__
#define __DOOMDATA__

// The most basic types we use, portability.
#include "doomtype.h"

// Some global defines, that configure the game.
#include "doomdef.h"



//
// Map level types.
// The following data structures define the persistent format
// used in the lumps of the WAD files.
//

// Lump order in a map WAD: each map needs a couple of lumps
// to provide a complete scene geometry description.
enum
{
  ML_LABEL,		// A separator, name, ExMx or MAPxx
  ML_THINGS,		// Monsters, items..
  ML_LINEDEFS,		// LineDefs, from editing
  ML_SIDEDEFS,		// SideDefs, from editing
  ML_VERTEXES,		// Vertices, edited and BSP splits generated
  ML_SEGS,		// LineSegs, from LineDefs split by BSP
  ML_SSECTORS,		// SubSectors, list of LineSegs
  ML_NODES,		// BSP nodes
  ML_SECTORS,		// Sectors, from editing
  ML_REJECT,		// LUT, sector-sector visibility	
  ML_BLOCKMAP		// LUT, motion clipping, walls/grid element
};


// A single Vertex.
typedef struct
{
  short		x;
  short		y;
} mapvertex_t;


// A SideDef, defining the visual appearance of a wall,
// by setting textures and offsets.
typedef struct
{
  short		textureoffset;
  short		rowoffset;
  char		toptexture[8];
  char		bottomtexture[8];
  char		midtexture[8];
  // Front sector, towards viewer.
  short		sector;
} mapsidedef_t;



// A LineDef, as used for editing, and as input
// to the BSP builder.
typedef struct
{
  short		v1;
  short		v2;
  short		flags;
  short		special;
  short		tag;
  // sidenum[1] will be -1 if one sided
  short		sidenum[2];		
} maplinedef_t;


//
// LineDef attributes.
//

// Solid, is an obstacle.
#define ML_BLOCKING		1

// Blocks monsters only.
#define ML_BLOCKMONSTERS	2

// Backside will not be present at all
//  if not two sided.
#define ML_TWOSIDED		4

// If a texture is pegged, the texture will have
// the end exposed to air held constant at the
// top or bottom of the texture (stairs or pulled
// down things) and will move with a height change
// of one of the neighbor sectors.
// Unpegged textures allways have the first row of
// the texture at the top pixel of the line for both
// top and bottom textures (use next to windows).

// upper texture unpegged
#define ML_DONTPEGTOP		8

// lower texture unpegged
#define ML_DONTPEGBOTTOM	16	

// In AutoMap: don't map as two sided: IT'S A SECRET!
#define ML_SECRET		32

// Sound rendering: don't let sound cross two of these.
#define ML_SOUNDBLOCK		64

// Don't draw on the automap at all.
#define ML_DONTDRAW		128

// Set if already seen, thus drawn in automap.
#define ML_MAPPED		256

#define DW_UPPER  0x0001
#define DW_MIDDLE 0x0002
#define DW_LOWER  0x0004

#define DW_UPUNPEG  0x0008
#define DW_LWUNPEG  0x0010
#define DW_NOTSEEN  0x0080

// Sector definition, from editing.
typedef	struct
{
  short		floorheight;
  short		ceilingheight;
  char		floorpic[8];
  char		ceilingpic[8];
  short		lightlevel;
  short		special;
  short		tag;
} mapsector_t;

// SubSector, as generated by BSP.
typedef struct
{
  short		numsegs;
  // Index of first one, segs are stored sequentially.
  short		firstseg;	
} mapsubsector_t;


// LineSeg, generated by splitting LineDefs
// using partition lines selected by BSP builder.
typedef struct
{
  short		v1;
  short		v2;
  short		angle;		
  short		linedef;
  short		side;
  short		offset;
} mapseg_t;



// BSP node structure.

// Indicate a leaf.
#define	NF_SUBSECTOR	0x8000

typedef struct
{
  // Partition line from (x,y) to x+dx,y+dy)
  short		x;
  short		y;
  short		dx;
  short		dy;

  // Bounding box for each child,
  // clip against view frustum.
  short		bbox[2][4];

  // If NF_SUBSECTOR its a subsector,
  // else it's a node of another subtree.
  unsigned short	children[2];

} mapnode_t;




// Thing definition, position, orientation and type,
// plus skill/visibility flags and attributes.
typedef struct
{
    short		x;
    short		y;
    short		angle;
    short		type;
    short		options;
} mapthing_t;


//
// 3D structure related data
//
//
// The following are the data structures stored in the id Software Doom WAD files
//
typedef struct
   {
    float  v[3];
    float  tu, tv;
   }DW_Vertex3Dv;

typedef struct
   {
    float  x;
    float  y;
    float  z;
   }DW_Vertex3D;

typedef struct DW_Polygon
   {
    DW_Vertex3Dv       Point[4];
    int                VAO, VBO;        // vertex and array buffers
    int*               tex;
    int                Position;
    int                LineDef;
    int                SideDef;
    dboolean           Switch;
    int                Texture[6];
    float              Lighting[4];
    int                Sector;
    int                BackSector;
    int                LSector;
    uint64_t              coloff;
    struct DW_Polygon *Next;
   }DW_Polygon;

typedef struct DW_FloorCeil
   {
    int                  PCount;
    int                  ceilVAO, floorVAO;
    int                  ceilVBO, floorVBO;
    int                 *texCeil, *texFloor;
    DW_Vertex3Dv        *Point;
    //int                  Texture[6];
    //float                Lighting[4];
    int                  Sector;
   }DW_FloorCeil;


typedef struct
{
    DW_FloorCeil**  subsectors;
    int             ss_count;
}sector_plane_t;

typedef struct
   {
    char  szName[10];
    int   Number;
    int   DWide;
    int   DHigh;
    int   GLWide;
    int   GLHigh;
    char  Type;
    dboolean Switch;
    dboolean Transparent;
    int   glTexture;
   }DW_TexList;


typedef enum
   {
    dm_door,
    dm_plat,
    dm_floord,
    dm_flooru,
    dm_ceild,
    dm_ceilu,
    dm_stairs
   }dm_movables;

typedef struct
   {
    int         flattype;
    float       offset;
    int         height;
    int         trigger;
//    DW_Polygon *Sides;
   }Flat3D_t;

typedef enum { ds_unknown, ds_draw, ds_nodraw } drawside_t;

#endif			// __DOOMDATA__
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

