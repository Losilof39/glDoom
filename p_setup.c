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
// $Log:$
//
// DESCRIPTION:
//	Do all the WAD I/O, get map description,
//	set up initial state and misc. LUTs.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: p_setup.c,v 1.5 1997/02/03 22:45:12 b1 Exp $";

//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <glad/glad.h>
#include <math.h>
#include <stdlib.h>

#include "z_zone.h"

#include "m_swap.h"
#include "m_bbox.h"

#include "g_game.h"

#include "i_system.h"
#include "w_wad.h"

#include "doomdef.h"
#include "p_local.h"

#include "s_sound.h"

#include "doomstat.h"
#include "sys_sdl.h"
#include "gldefs.h"

#include "info.h"

#include "doomlib.h"
#include "mathlib.h"

#include <malloc.h>

#define FREE(x)  {if((x) != NULL) free(x);}
#define ZFREE(x) {if((x) != NULL){free(x); (x) = NULL;}}

void lfprintf(char *message, ... );
void P_SpawnMapThing(mapthing_t* mthing);

//
// MAP related Lookup tables.
// Store VERTEXES, LINEDEFS, SIDEDEFS, etc.
//
int		        numvertexes;
vertex_t*	    vertexes;

int		        numsegs;
seg_t*		    segs;

int		        numsectors;
sector_t*	    sectors;
int             lastsectors = 0;

int		        numsubsectors;
subsector_t*    subsectors;

int		        numnodes;
node_t*		    nodes;

int		        numlines;
line_t*		    lines;

int		        numsides;
side_t*		    sides;
int             lastsides = 0;

// BLOCKMAP
// Created from axis aligned bounding box
// of the map, a rectangular array of
// blocks of size ...
// Used to speed up collision detection
// by spatial subdivision in 2D.
//
// Blockmap size.
int		    bmapwidth;
int		    bmapheight;	// size in mapblocks
short*		blockmap;	// int for larger maps
// offsets in blockmap are from here
short*		blockmaplump;		
// origin of block map
fixed_t		bmaporgx;
fixed_t		bmaporgy;
// for thing chains
mobj_t**	blocklinks;		

// REJECT
// For fast sight rejection.
// Speeds up enemy AI by skipping detailed
//  LineOf Sight calculation.
// Without special effect, this could be
//  used as a PVS lookup as well.
//
dbyte*		rejectmatrix;

// Maintain single and multi player starting spots.
#define MAX_DEATHMATCH_STARTS 10

mapthing_t	deathmatchstarts[MAX_DEATHMATCH_STARTS];
mapthing_t*	deathmatch_p;
mapthing_t	playerstarts[MAXPLAYERS];

dboolean    SpritePresent[NUMMOBJTYPES];

//
// P_LoadVertexes
//
void P_LoadVertexes(int lump)
{
    dbyte*		    data;
    int			    i;
    mapvertex_t*	ml;
    vertex_t*		li;

    // Determine number of lumps:
    //  total lump length / vertex record length.
    numvertexes = W_LumpLength(lump) / sizeof(mapvertex_t);

    // Allocate zone memory for buffer.
    vertexes = Z_Malloc(numvertexes * sizeof(vertex_t),PU_LEVEL,0);	

    // Load data into cache.
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    ml = (mapvertex_t *)data;
    li = vertexes;

    // Copy and convert vertex coordinates,
    // internal representation as fixed.
    for (i=0 ; i<numvertexes ; i++, li++, ml++)
    {
	    li->x = DSHORT(ml->x)<<FRACBITS;
	    li->y = DSHORT(ml->y)<<FRACBITS;
    }

    // Free buffer memory.
    Z_Free (data);
}

//
// P_LoadSegs
//
void P_LoadSegs(int lump)
{
    dbyte*		data;
    int			i;
    mapseg_t*	ml;
    seg_t*		li;
    line_t*		ldef;
    int			linedef;
    int			side;
	
    numsegs = W_LumpLength(lump) / sizeof(mapseg_t);
    segs = Z_Malloc(numsegs * sizeof(seg_t),PU_LEVEL,0);	
    memset (segs, 0, numsegs * sizeof(seg_t));
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    ml = (mapseg_t *)data;
    li = segs;
    for (i=0 ; i<numsegs ; i++, li++, ml++)
    {
	    li->v1 = &vertexes[DSHORT(ml->v1)];
	    li->v2 = &vertexes[DSHORT(ml->v2)];
					
        li->side = DSHORT(ml->side);
	    li->angle = (DSHORT(ml->angle))<<16;
	    li->offset = (DSHORT(ml->offset))<<16;
	    linedef = DSHORT(ml->linedef);
	    ldef = &lines[linedef];
	    li->linedef = ldef;
	    side = DSHORT(ml->side);
	    li->sidedef = &sides[ldef->sidenum[side]];
	    li->frontsector = sides[ldef->sidenum[side]].sector;
	if (ldef-> flags & ML_TWOSIDED)
	    li->backsector = sides[ldef->sidenum[side^1]].sector;
	else
	    li->backsector = 0;
    }
	
    Z_Free (data);
}

//
// P_LoadSubsectors
//
void P_LoadSubsectors(int lump)
{
    dbyte*		    data;
    int			    i;
    mapsubsector_t*	ms;
    subsector_t*	ss;
	
    numsubsectors = W_LumpLength(lump) / sizeof(mapsubsector_t);
    subsectors = Z_Malloc(numsubsectors * sizeof(subsector_t),PU_LEVEL,0);	
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    ms = (mapsubsector_t *)data;
    memset (subsectors,0, numsubsectors*sizeof(subsector_t));
    ss = subsectors;
    
    for (i=0 ; i<numsubsectors ; i++, ss++, ms++)
    {
	    ss->numlines = DSHORT(ms->numsegs);
	    ss->firstline = DSHORT(ms->firstseg);
    }
	
    Z_Free (data);
}

//
// P_LoadSectors
//
void P_LoadSectors(int lump)
{
    dbyte*		    data;
    int			    i;
    mapsector_t*	ms;
    sector_t*		ss;
	
    numsectors = W_LumpLength(lump) / sizeof(mapsector_t);
    sectors = Z_Malloc(numsectors * sizeof(sector_t),PU_LEVEL,0);	
    memset (sectors, 0, numsectors * sizeof(sector_t));
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    ms = (mapsector_t *)data;
    ss = sectors;
    for (i=0 ; i<numsectors ; i++, ss++, ms++)
    {
	    ss->floorheight = DSHORT(ms->floorheight)<<FRACBITS;
	    ss->ceilingheight = DSHORT(ms->ceilingheight)<<FRACBITS;
	    ss->floorpic = R_FlatNumForName(ms->floorpic);
	    ss->ceilingpic = R_FlatNumForName(ms->ceilingpic);
	    ss->lightlevel = DSHORT(ms->lightlevel);
	    ss->special = DSHORT(ms->special);
	    ss->tag = DSHORT(ms->tag);
	    ss->thinglist = NULL;
    }
	
    Z_Free (data);
}

//
// P_LoadNodes
//
void P_LoadNodes(int lump)
{
    dbyte*	    data;
    int		    i;
    int		    j;
    int		    k;
    mapnode_t*	mn;
    node_t*	    no;
	
    numnodes = W_LumpLength(lump) / sizeof(mapnode_t);
    nodes = Z_Malloc(numnodes * sizeof(node_t),PU_LEVEL,0);	
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    mn = (mapnode_t *)data;
    no = nodes;
    
    for (i=0 ; i<numnodes ; i++, no++, mn++)
    {
	    no->x = DSHORT(mn->x)<<FRACBITS;
	    no->y = DSHORT(mn->y)<<FRACBITS;
	    no->dx = DSHORT(mn->dx)<<FRACBITS;
	    no->dy = DSHORT(mn->dy)<<FRACBITS;
	for (j=0 ; j<2 ; j++)
	{
	    no->children[j] = DSHORT(mn->children[j]);
	    for (k=0 ; k<4 ; k++)
		no->bbox[j][k] = DSHORT(mn->bbox[j][k])<<FRACBITS;
	}
    }
	
    Z_Free (data);
}

//
// P_LoadThings
//
void P_LoadThings(int lump)
   {
    dbyte*		    data;
    int			    i;
    mapthing_t*		mt;
    int			    numthings;
    dboolean		spawn;

    for (i = 0; i < NUMMOBJTYPES; i++)
        SpritePresent[i] = 0;

    data = W_CacheLumpNum(lump,PU_STATIC);
    numthings = W_LumpLength(lump) / sizeof(mapthing_t);
	
    mt = (mapthing_t *) data;
    for (i=0 ; i<numthings ; i++, mt++)
       {
        spawn = true;

        // Do not spawn cool, new monsters if !commercial
        if ( gamemode != commercial)
           {
            switch(mt->type)
               {
                case 68:	// Arachnotron
                case 64:	// Archvile
                case 88:	// Boss Brain
                case 89:	// Boss Shooter
                case 69:	// Hell Knight
                case 67:	// Mancubus
                case 71:	// Pain Elemental
                case 65:	// Former Human Commando
                case 66:	// Revenant
                case 84:	// Wolf SS
                     spawn = false;
                     break;
               }
           }
        if (spawn == false)
            break;

        // Do spawn all other stuff. 
        mt->x = DSHORT(mt->x);
        mt->y = DSHORT(mt->y);
        mt->angle = DSHORT(mt->angle);
        mt->type = DSHORT(mt->type);
        mt->options = DSHORT(mt->options);
	
        P_SpawnMapThing(mt);
       }
	
    Z_Free(data);
   }


//
// P_LoadLineDefs
// Also counts secret lines for intermissions.
//
void P_LoadLineDefs(int lump)
{
    dbyte*		data;
    int			i;
    maplinedef_t*	mld;
    line_t*		ld;
    vertex_t*		v1;
    vertex_t*		v2;
	
    numlines = W_LumpLength (lump) / sizeof(maplinedef_t);
    lines = Z_Malloc (numlines*sizeof(line_t),PU_LEVEL,0);
    memset (lines, 0, numlines*sizeof(line_t));
    data = W_CacheLumpNum (lump,PU_STATIC);
	
    mld = (maplinedef_t *)data;
    ld = lines;
    for (i=0 ; i<numlines ; i++, mld++, ld++)
    {
	ld->flags = DSHORT(mld->flags);
	ld->special = DSHORT(mld->special);
	ld->tag = DSHORT(mld->tag);
	v1 = ld->v1 = &vertexes[DSHORT(mld->v1)];
	v2 = ld->v2 = &vertexes[DSHORT(mld->v2)];
	ld->dx = v2->x - v1->x;
	ld->dy = v2->y - v1->y;
	
	if (!ld->dx)
	    ld->slopetype = ST_VERTICAL;
	else if (!ld->dy)
	    ld->slopetype = ST_HORIZONTAL;
	else
	{
	    if (FixedDiv (ld->dy , ld->dx) > 0)
		ld->slopetype = ST_POSITIVE;
	    else
		ld->slopetype = ST_NEGATIVE;
	}
		
	if (v1->x < v2->x)
	{
	    ld->bbox[BOXLEFT] = v1->x;
	    ld->bbox[BOXRIGHT] = v2->x;
	}
	else
	{
	    ld->bbox[BOXLEFT] = v2->x;
	    ld->bbox[BOXRIGHT] = v1->x;
	}

	if (v1->y < v2->y)
	{
	    ld->bbox[BOXBOTTOM] = v1->y;
	    ld->bbox[BOXTOP] = v2->y;
	}
	else
	{
	    ld->bbox[BOXBOTTOM] = v2->y;
	    ld->bbox[BOXTOP] = v1->y;
	}

	ld->sidenum[0] = DSHORT(mld->sidenum[0]);
	ld->sidenum[1] = DSHORT(mld->sidenum[1]);

	if (ld->sidenum[0] != -1)
	    ld->frontsector = sides[ld->sidenum[0]].sector;
	else
	    ld->frontsector = 0;

	if (ld->sidenum[1] != -1)
	    ld->backsector = sides[ld->sidenum[1]].sector;
	else
	    ld->backsector = 0;
    }
	
    Z_Free (data);
}

//
// P_LoadSideDefs
//
void P_LoadSideDefs(int lump)
{
    dbyte*		data;
    int			i;
    mapsidedef_t*	msd;
    side_t*		sd;
	
    numsides = W_LumpLength(lump) / sizeof(mapsidedef_t);
    sides = Z_Malloc(numsides * sizeof(side_t),PU_LEVEL,0);	
    memset(sides, 0, numsides * sizeof(side_t));
    data = W_CacheLumpNum(lump,PU_STATIC);
	
    msd = (mapsidedef_t *)data;
    sd = sides;
    for (i=0 ; i<numsides ; i++, msd++, sd++)
    {
	    sd->textureoffset = DSHORT(msd->textureoffset)<<FRACBITS;
	    sd->rowoffset = DSHORT(msd->rowoffset)<<FRACBITS;
	    sd->toptexture = R_TextureNumForName(msd->toptexture);
	    sd->bottomtexture = R_TextureNumForName(msd->bottomtexture);
	    sd->midtexture = R_TextureNumForName(msd->midtexture);
	    sd->sector = &sectors[DSHORT(msd->sector)];
        sd->sectornumb = msd->sector;
    }

    Z_Free (data);
}

//
// P_LoadBlockMap
//
void P_LoadBlockMap(int lump)
{
    int		i;
    int		count;
	
    blockmaplump = W_CacheLumpNum(lump,PU_LEVEL);
    blockmap = blockmaplump+4;
    count = W_LumpLength(lump)/2;

    for (i=0 ; i<count ; i++)
       blockmaplump[i] = DSHORT(blockmaplump[i]);

    bmaporgx = blockmaplump[0]<<FRACBITS;
    bmaporgy = blockmaplump[1]<<FRACBITS;
    bmapwidth = blockmaplump[2];
    bmapheight = blockmaplump[3];
	
    // clear out mobj chains
    count = sizeof(*blocklinks)* bmapwidth*bmapheight;
    blocklinks = Z_Malloc(count,PU_LEVEL, 0);
    memset(blocklinks, 0, count);
}

//
// P_GroupLines
// Builds sector line lists and subsector sector numbers.
// Finds block bounding boxes for sectors.
//
void P_GroupLines(void)
{
    line_t**	    linebuffer;
    int			    i;
    int			    j;
    int			    total;
    line_t*		    li;
    sector_t*	    sector;
    subsector_t*    ss;
    seg_t*		    seg;
    fixed_t		    bbox[4];
    int			    block;
	
    // look up sector number for each subsector
    ss = subsectors;
    for (i=0 ; i<numsubsectors ; i++, ss++)
    {
	    seg = &segs[ss->firstline];
	    ss->sector = seg->sidedef->sector;
    }

    // count number of lines in each sector
    li = lines;
    total = 0;
    for (i=0 ; i<numlines ; i++, li++)
    {
	    total++;
	    li->frontsector->linecount++;

	if (li->backsector && li->backsector != li->frontsector)
	{
	    li->backsector->linecount++;
	    total++;
	}
    }
	
    // build line tables for each sector	
    linebuffer = Z_Malloc(total * sizeof(*linebuffer), PU_LEVEL, 0);
    sector = sectors;
    for (i=0 ; i<numsectors ; i++, sector++)
    {
	M_ClearBox(bbox);
	sector->lines = linebuffer;
	li = lines;
	for (j=0 ; j<numlines ; j++, li++)
	{
	    if (li->frontsector == sector || li->backsector == sector)
	    {
		*linebuffer++ = li;
		M_AddToBox(bbox, li->v1->x, li->v1->y);
		M_AddToBox(bbox, li->v2->x, li->v2->y);
	    }
	}
	if (linebuffer - sector->lines != sector->linecount)
	    I_Error("P_GroupLines: miscounted");
			
	// set the degenmobj_t to the middle of the bounding box
	sector->soundorg.x = (bbox[BOXRIGHT]+bbox[BOXLEFT])/2;
	sector->soundorg.y = (bbox[BOXTOP]+bbox[BOXBOTTOM])/2;
		
	// adjust bounding box to map blocks
	block = (bbox[BOXTOP]-bmaporgy+MAXRADIUS)>>MAPBLOCKSHIFT;
	block = block >= bmapheight ? bmapheight-1 : block;
	sector->blockbox[BOXTOP]=block;

	block = (bbox[BOXBOTTOM]-bmaporgy-MAXRADIUS)>>MAPBLOCKSHIFT;
	block = block < 0 ? 0 : block;
	sector->blockbox[BOXBOTTOM]=block;

	block = (bbox[BOXRIGHT]-bmaporgx+MAXRADIUS)>>MAPBLOCKSHIFT;
	block = block >= bmapwidth ? bmapwidth-1 : block;
	sector->blockbox[BOXRIGHT]=block;

	block = (bbox[BOXLEFT]-bmaporgx-MAXRADIUS)>>MAPBLOCKSHIFT;
	block = block < 0 ? 0 : block;
	sector->blockbox[BOXLEFT]=block;
    }
	
}

void  Build3DLevel(void);
void  BuildThingList(void);

//
// P_SetupLevel
//
void
P_SetupLevel
( int		episode,
  int		map,
  int		playermask,
  skill_t	skill)
{
    int		i;
    char	lumpname[9];
    int		lumpnum;
	
    totalkills = totalitems = totalsecret = wminfo.maxfrags = 0;
    totalscore = 0; //showscoreHUD=keepscore=1; // on for testing
    wminfo.partime = 180;
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	    players[i].killcount = players[i].secretcount 
	        = players[i].itemcount = 0;
    }

    // Initial height of PointOfView
    // will be set by player think.
    players[consoleplayer].viewz = 1; 

    // Make sure all sounds are stopped before Z_FreeTags.
    S_Start();			

    
#if 0 // UNUSED
    if (debugfile)
    {
	    Z_FreeTags(PU_LEVEL, PU_PURGELEVEL - 1);
	    Z_FileDumpHeap(debugfile);
    }
    else
#endif
	Z_FreeTags(PU_LEVEL, PU_PURGELEVEL);


    // UNUSED W_Profile ();
    P_InitThinkers();

    // if working with a devlopment map, reload it
    W_Reload();			
	   
    // find map name
    if (gamemode == commercial)
    {
	if (map<10)
	    sprintf(lumpname,"map0%i", map);
	else
	    sprintf(lumpname,"map%i", map);
    }
    else
    {
	    lumpname[0] = 'E';
	    lumpname[1] = '0' + episode;
	    lumpname[2] = 'M';
	    lumpname[3] = '0' + map;
	    lumpname[4] = 0;
    }

    lumpnum = W_GetNumForName (lumpname);
	
    leveltime = 0;
	
    // note: most of this ordering is important
    P_LoadBlockMap(lumpnum+ML_BLOCKMAP);
    P_LoadVertexes(lumpnum+ML_VERTEXES);
    P_LoadSectors(lumpnum+ML_SECTORS);
    P_LoadSideDefs(lumpnum+ML_SIDEDEFS);

    P_LoadLineDefs(lumpnum+ML_LINEDEFS);
    P_LoadSubsectors(lumpnum+ML_SSECTORS);
    P_LoadNodes(lumpnum+ML_NODES);
    P_LoadSegs(lumpnum+ML_SEGS);
	
    rejectmatrix = W_CacheLumpNum(lumpnum+ML_REJECT,PU_LEVEL);
    P_GroupLines();

    bodyqueslot = 0;
    deathmatch_p = deathmatchstarts;
    P_LoadThings(lumpnum+ML_THINGS);
    
    // if deathmatch, randomly spawn the active players
    if (deathmatch)
    {
	for (i=0 ; i<MAXPLAYERS ; i++)
	    if (playeringame[i])
	    {
		    players[i].mo = NULL;
		    G_DeathMatchSpawnPlayer(i);
	    }	
    }

    // clear special respawning que
    iquehead = iquetail = 0;		
	
    // set up world state
    P_SpawnSpecials();
	
    // build subsector connect matrix
    //	UNUSED P_ConnectSubsectors ();

    // preload graphics
    if (precache)
       {
	        R_PrecacheLevel();
       }

    //BuildThingList();

    Build3DLevel();

    //printf ("free memory: 0x%x\n", Z_FreeMemory());

}

//
// P_Init
//
void P_Init(void)
{
    P_InitSwitchList();
    P_InitPicAnims();
    R_InitSprites(sprnames);
}



////////////////////////////////////////////////////////////////////
// Polygon Creation
////////////////////////////////////////////////////////////////////

typedef struct
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} rec_t;

void  NormalVector(float, float, float, float, int);
float InnerProduct(float *f, float *m, float *e);

rec_t* SectorBBox = 0;

// keeps track which walls (side) or flats are visible
drawside_t        *DrawSide           = NULL;
dbyte          *DrawFlat           = NULL;
sector_plane_t   **sorted_flats       = NULL;
int                sorted_flats_count = 0;


DW_Vertex3Dv      *Normal = 0;
extern DW_TexList         TexList[1024];
int                TexCount;

// global list of sides, from which sorted_walls/flats are populated
DW_Polygon        **side_polygons      = NULL;
DW_Polygon        **sorted_walls       = NULL;
int                 sorted_walls_count = 0;

DW_FloorCeil      *subsector_planes = NULL;
sector_plane_t    *planes           = NULL;

float InnerProduct(float *f, float *m, float *e)
   {
    float v1[3], v2[3], v3[3], d, n[3];

    v1[0] = e[0] - f[0];
    v1[1] = e[1] - f[1];
    v1[2] = e[2] - f[2];

    v2[0] = e[0] - e[0];
    v2[1] = e[1] - e[1];
    v2[2] = e[2] - 128.0f;

    n[0] = v1[1]*v2[2] - v2[1]*v1[2];
    n[1] = v1[2]*v2[0] - v2[2]*v1[0];
    n[2] = v1[0]*v2[1] - v2[0]*v1[1];

    d = (n[0]*n[0])+(n[1]*n[1])+(n[2]*n[2]);

    if (d == 0)
       d = 1.0f;
    else
       d = 1.0f / (float)sqrt(d);

    n[0] *= d;
    n[1] *= d;
    n[2] *= d;

    v3[0] = f[0] - m[0];
    v3[1] = f[1] - m[1];
    v3[2] = f[2] - m[2];

    d = (v3[0]*v3[0])+(v3[1]*v3[1])+(v3[2]*v3[2]);

    if (d == 0)
       d = 1.0f;
    else
       d = 1.0f / (float)sqrt(d);

    v3[0] *= d;
    v3[1] *= d;
    v3[2] *= d;

	 return (n[0]*v3[0])+(n[1]*v3[1])+(n[2]*v3[2]);
   }

void NormalVector(float x1, float y1, float x2, float y2, int side)
   {
    float /* v1[3], v2[3], */ d, n[3];

    // horizontal vector
    //v1[0] = x2 - x1;
    //v1[1] = 0.0f;
    //v1[2] = y2 - y1;

    // vertical vector - walls all vertical
    //v2[0] = 0.0f;
    //v2[1] = -1.0f;
    //v2[2] = 0.0f;

    // cross product...
    //n[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    //n[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    //n[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    // or we can use the "short form..." :o)
    n[0] = y2 - y1;
    n[2] = 0.0f - (x2 - x1);

    d = (n[0]*n[0])+(n[2]*n[2]);

    if (d == 0)
       d = 1.0f;
    else
       d = 1.0f / (float)sqrt(d);

    Normal[side].v[0] = n[0] * d;
    Normal[side].v[1] = 0.0f;
    Normal[side].v[2] = n[2] * d;
   }

// Original Doom geometry data...

typedef struct
{
    dboolean	istexture;	// if false, it is a flat
    char	endname[9];
    char	startname[9];
    int		speed;
} animdef_t;


// A single patch from a texture definition,
//  basically a rectangular area within
//  the texture rectangle.
typedef struct
{
    // Block origin (allways UL),
    // which has allready accounted
    // for the internal origin of the patch.
    int		originx;	
    int		originy;
    int		patch;
} texpatch_t;


// A maptexturedef_t describes a rectangular texture,
//  which is composed of one or more mappatch_t structures
//  that arrange graphic patches.
typedef struct
{
    // Keep name for switch changing, etc.
    char	name[8];		
    short	width;
    short	height;
    
    // All the patches[patchcount]
    //  are drawn back to front into the cached texture.
    short	patchcount;
    texpatch_t	patches[1];		
    
} texture_t;

int    translate[1024];
int    ftranslate[1024];
extern animdef_t    animdefs[];
extern texture_t**	textures;
extern switchlist_t alphSwitchList[];
extern int numswitches;
extern int switchlist[];
extern dboolean  TexTransparent;

extern int skytexture;
int        GL_SkyTexture[4], GL_SkyParts = 0, GL_SkyTop;

void CalcTexCoords(void);

void BuildWallTexList()
   {
    int   line, side, texture;
    dboolean  TexDupe;

    for (side = 0; side < 2; side++)
       {
        for (line = 0; line < numlines; line++)
           {
            if (lines[line].sidenum[side] >= 0)
               {
                TexDupe = false;
                if (sides[lines[line].sidenum[side]].toptexture > 0)
                   {
                    for (texture = 0; texture < TexCount; texture++)
                       {
                        if (TexList[texture].Number == sides[lines[line].sidenum[side]].toptexture)
                           {
                            TexDupe = true;
                            break;
                           }
                       }
                    if (TexDupe == false)
                       {
                        translate[sides[lines[line].sidenum[side]].toptexture] = TexCount;
                        TexList[TexCount].Number = sides[lines[line].sidenum[side]].toptexture;
                        TexList[TexCount].Type = 'W';
                        TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                        TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                        strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                        TexList[TexCount].szName[8] = '\0';
                        TexCount++;
                       }
                   }
                TexDupe = false;
                if (sides[lines[line].sidenum[side]].bottomtexture > 0)
                   {
                    for (texture = 0; texture < TexCount; texture++)
                       {
                        if (TexList[texture].Number == sides[lines[line].sidenum[side]].bottomtexture)
                           {
                            TexDupe = true;
                            break;
                           }
                       }
                    if (TexDupe == false)
                       {
                        translate[sides[lines[line].sidenum[side]].bottomtexture] = TexCount;
                        TexList[TexCount].Number = sides[lines[line].sidenum[side]].bottomtexture;
                        TexList[TexCount].Type = 'W';
                        TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                        TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                        strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                        TexList[TexCount].szName[8] = '\0';
                        TexCount++;
                       }
                   }
                TexDupe = false;
                if (sides[lines[line].sidenum[side]].midtexture > 0)
                   {
                    for (texture = 0; texture < TexCount; texture++)
                       {
                        if (TexList[texture].Number == sides[lines[line].sidenum[side]].midtexture)
                           {
                            TexDupe = true;
                            break;
                           }
                       }
                    if (TexDupe == false)
                       {
                        translate[sides[lines[line].sidenum[side]].midtexture] = TexCount;
                        TexList[TexCount].Number = sides[lines[line].sidenum[side]].midtexture;
                        TexList[TexCount].Type = 'W';
                        TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                        TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                        strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                        TexList[TexCount].szName[8] = '\0';
                        TexCount++;
                       }
                   }
               }
           }
       }
/*
   for (texture = 0; texture < TexCount; texture++)
      {
       lfprintf( "Texture %3d -> %5d, %s\n", texture, TexList[texture].Number, TexList[texture].szName);
      }
*/
   }

dboolean CheckTexture(char *name)
   {
    int t;

    for (t = 0; t < TexCount; t++)
       {
        if (strcasecmp(name, TexList[t].szName) == 0)
           {
            return true;
           }
       }

    return false;
   }

void AddSwitchTextures()
   {
    int  a, b, c;
    char tname[10];

    b = TexCount;
    for (a = 0; a < b; a++)
       {
        strncpy(tname, textures[TexList[a].Number]->name, 8);
        tname[8] = '\0';
        for (c = 0; c < numswitches; c++)
           {
            if (strcasecmp(tname, alphSwitchList[c].name1) == 0)
               {
                if (CheckTexture(alphSwitchList[c].name2) == false)
                   {
                    translate[switchlist[(c*2)+1]] = TexCount;
                    strncpy(TexList[TexCount].szName, alphSwitchList[c].name2, 8);
                    TexList[TexCount].szName[8] = '\0';
                    TexList[TexCount].Number = switchlist[(c*2)+1];
                    TexList[TexCount].Type = 'W';
                    TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                    TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                    strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                    TexCount++;
                   }
                break;
               }
            else
            if (strcasecmp(tname, alphSwitchList[c].name2) == 0)
               {
                if (CheckTexture(alphSwitchList[c].name1) == false)
                   {
                    translate[switchlist[c*2]] = TexCount;
                    strncpy(TexList[TexCount].szName, alphSwitchList[c].name1, 8);
                    TexList[TexCount].szName[8] = '\0';
                    TexList[TexCount].Number = switchlist[c*2];
                    TexList[TexCount].Type = 'W';
                    TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                    TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                    strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                    TexCount++;
                   }
                break;
               }
           }
       }
   }

void AddWallAnimations()
   {
    int a, b, c, bpic, epic, anims, anime;

    b = TexCount;
    for (a = 0; a < b; a++)
       {
        if (TexList[a].Type != 'W')
           continue;

        //lfprintf( "Matching wall texture : %s\n", textures[TexList[a].Number]->name);
        for (c = 0; animdefs[c].istexture != -1; c++)
           {
            if (animdefs[c].istexture == true)
               {
                bpic = R_CheckTextureNumForName(animdefs[c].startname);
                epic = R_CheckTextureNumForName(animdefs[c].endname);
                if ((bpic != -1) && (epic != -1))
                   {
                    //lfprintf( "Wall Animation : %s to %s\n", animdefs[c].startname, animdefs[c].endname);
                    anims = R_TextureNumForName (animdefs[c].startname);
                    anime = R_TextureNumForName (animdefs[c].endname);
                    if ((TexList[a].Number >= anims) && (TexList[a].Number <= anime))
                       {
                        //lfprintf( "Wall Animation : %s to %s\n", animdefs[c].startname, animdefs[c].endname);
                        for (; anims <= anime; anims++)
                           {
                            if (TexList[a].Number == anims)
                               continue;
                            translate[anims] = TexCount;
                            TexList[TexCount].Number = anims;
                            TexList[TexCount].Type = 'W';
                            TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
                            TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
                            strncpy(TexList[TexCount].szName, textures[TexList[TexCount].Number]->name, 8);
                            TexCount++;
                           }
                        break;
                       }
                   }
               }
           }
       }
   }

void AddNewFlatTextures()
   {
    int  sector, t, TexFlat;
    dboolean TexDupe;

    TexFlat = TexCount;
    for (sector = 0; sector < numsectors; sector++)
       {
        TexDupe = false;
        for (t = TexFlat; t < TexCount; t++)
           {
            if (TexList[t].Number  == sectors[sector].floorpic)
               {
                TexDupe = true;
                break;
               }
           }
        if (TexDupe == false)
           {
            ftranslate[sectors[sector].floorpic] = TexCount;
            TexList[TexCount].Number = sectors[sector].floorpic;
            TexList[TexCount].DWide = 64;
            TexList[TexCount].DHigh = 64;
            TexList[TexCount].Type = 'F';
            TexCount++;
           }
        TexDupe = false;
        for (t = TexFlat; t < TexCount; t++)
           {
            if (TexList[t].Number == sectors[sector].ceilingpic)
               {
                TexDupe = true;
                break;
               }
           }
        if (TexDupe == false)
           {
            ftranslate[sectors[sector].ceilingpic] = TexCount;
            TexList[TexCount].Number = sectors[sector].ceilingpic;
            TexList[TexCount].DWide = 64;
            TexList[TexCount].DHigh = 64;
            TexList[TexCount].Type = 'F';
            TexCount++;
           }
       }
/*
   for (t = 0; t < TexCount; t++)
      {
       if (TexList[t].Type != 'F')
          {
           continue;
          }
       lfprintf( "Flat %3d -> %5d, %s\n", t, TexList[t].Number, TexList[t].szName);
      }
*/
   }

void AddFlatAnimations(int TexFlat)
   {
    int  a, b, c, bpic, epic, anims, anime;

    b = TexCount;
    for (a = TexFlat; a < b; a++)
       {
        if (TexList[a].Type != 'F')  // "shouldn't" be necessary
           continue;

        //lfprintf( "Checking flat %d\n", TexList[a].Number);
        for (c = 0; animdefs[c].istexture != -1; c++)
           {
            if (animdefs[c].istexture == false)
               {
                bpic = W_CheckNumForName(animdefs[c].startname);
                epic = W_CheckNumForName(animdefs[c].endname);
                if ((bpic != -1) && (epic != -1))
                   {
                    anims = R_FlatNumForName (animdefs[c].startname);
                    anime = R_FlatNumForName (animdefs[c].endname);
                    //if (((firstflat+TexList[a].Number) >= anims) && ((firstflat+TexList[a].Number) <= anime))
                    if ((TexList[a].Number >= anims) && (TexList[a].Number <= anime))
                       {
                        //lfprintf( "Flat Animation : %s to %s\n", animdefs[c].startname, animdefs[c].endname);
                        for (; anims <= anime; anims++)
                           {
                            ftranslate[anims] = TexCount;
                            TexList[TexCount].Number = anims;
                            TexList[TexCount].DWide = 64;
                            TexList[TexCount].DHigh = 64;
                            TexList[TexCount].Type = 'F';
                            TexCount++;
                          }
                        break;
                       }
                   }
               }
           }
       }
   }

void CreateSkyEntry()
   {
    TexList[TexCount].Number = skytexture;
    TexList[TexCount].DWide = textures[TexList[TexCount].Number]->width;
    TexList[TexCount].DHigh = textures[TexList[TexCount].Number]->height;
    TexList[TexCount].Type = 'S';
    TexCount++;
   }

int GL_LoadTexture(int texture);
int GL_LoadSkyTexture(int texture, int *tlist);
int GL_LoadFlatTexture(int texture);
int GL_LoadSkyTop(char *filename);

void CreateNewTextures()
   {
    int texture;

    for (texture = 0; texture < TexCount; texture++)
       {
        if (TexList[texture].Type == 'W')
           {
            TexList[texture].glTexture = GL_LoadTexture(texture);
            TexList[texture].Transparent = TexTransparent;
//            lfprintf( "Texture %-8s : %dx%d -> %dx%d\n", TexList[texture].szName,
//                    TexList[texture].DWide, TexList[texture].DHigh,
//                    TexList[texture].GLWide, TexList[texture].GLHigh);
           }
        else
        if (TexList[texture].Type == 'S')
           {
//            lfprintf("Creating sky texture...\n");
            GL_SkyParts = GL_LoadSkyTexture(texture, GL_SkyTexture);
            TexList[texture].Transparent = false;
//            lfprintf( "Texture %-8s : %dx%d -> %dx%d - parts %d\n", TexList[texture].szName,
//                    TexList[texture].DWide, TexList[texture].DHigh,
//                    TexList[texture].GLWide, TexList[texture].GLHigh, GL_SkyParts);
           }
        else
           {
            TexList[texture].glTexture = GL_LoadFlatTexture(texture);
            TexList[texture].Transparent = TexTransparent;
//            lfprintf( "Flat - %d - %d\n", texture, TexList[texture].Number);
           }
       }

//    GL_SkyTop = GL_LoadSkyTop("sky1top.bmp");
   }

void LoadNewTextures()
   {
    int       texture, TexFlat;
    GLdouble *v[1024];

    // clear texture translation table
    for (texture = 0; texture < 1024; texture++)
       {
        v[texture] = (GLdouble *)malloc(sizeof(GLdouble)*3);
        translate[texture] = texture;
       }

    TexCount = 0;

    for (texture = 0; texture < 1024; texture++)
       {
        TexList[texture].szName[0] = '\0';
        TexList[texture].DWide = 0;
        TexList[texture].DHigh = 0;
        TexList[texture].GLWide = 0;
        TexList[texture].GLHigh = 0;
        TexList[texture].Type = 'W';
        TexList[texture].glTexture = 0;
       }
    BuildWallTexList();
    AddSwitchTextures();
    AddWallAnimations();
    TexFlat = TexCount;
    AddNewFlatTextures();
    AddFlatAnimations(TexFlat);
    CreateSkyEntry();
    CreateNewTextures();
   }

int TextureSearchSector(int sector, int top)
   {
    int side;

    for (side = 0; side < numsides; side++)
       {
        if (top)
           {
            if ((sides[side].sectornumb == sector) && (sides[side].toptexture != 0))
               {
                return sides[side].toptexture;
               }
           }
        else
           {
            if ((sides[side].sectornumb == sector) && (sides[side].bottomtexture != 0))
               {
                return sides[side].bottomtexture;
               }
           }
       }
    return 0;
   }

void CreateNewWalls()
   {
    int         line, PolyCount, i;
    int         Sector1, Sector2, texture;
    float       Ceil1, Ceil2, Floor1, Floor2, MiddleTop, MiddleBottom;
    DW_Polygon *TempPoly;
    char        szCeil[2][10], szFloor[2][10];
    dboolean        SkyCeil, SkyFloor;
    
    float   fHighestCeiling, h;
    line_t* pline;
    side_t* pside;
    int     sky_sector_count;

    if (numsectors > 0)
    {
        fHighestCeiling = FIXED_TO_FLOAT(sectors->ceilingheight);
        for (i = 0; i < numsectors; i++)
        {
            h = FIXED_TO_FLOAT(sectors[i].ceilingheight);
            if (h > fHighestCeiling)
                fHighestCeiling = h;
        }
    }

    //lfprintf( "Number of linedefs : %d\n", numlines);

    if (side_polygons != NULL)
    {
        for (i = 0; i < lastsides; i++)
        {
            TempPoly = side_polygons[i];
            while (TempPoly != 0)
            {
                side_polygons[i] = TempPoly->Next;
                free(TempPoly);
                TempPoly = side_polygons[i];
            }
        }
    }

    ZFREE(side_polygons);
    ZFREE(sorted_walls);
    ZFREE(DrawSide);
    ZFREE(Normal);

    side_polygons      = (DW_Polygon**)malloc(sizeof(DW_Polygon*) * numsides);
    memset(side_polygons, 0, sizeof(DW_Polygon*) * numsides);

    sorted_walls       = (DW_Polygon**)malloc(sizeof(DW_Polygon*) * numsides);
    sorted_walls_count = 0;

    lastsides = numsides;
    PolyCount = 0;


    Normal   = (DW_Vertex3Dv *)malloc(sizeof(DW_Vertex3Dv)*numsides);
    DrawSide = (drawside_t *)malloc(sizeof(drawside_t) * numsides);
    memset(DrawSide, 0, sizeof(drawside_t) * numsides);


    for (line = 0; line < numlines; line++)
    {
        pline = lines + line;

        // Generate the normal for this linedef
        NormalVector((float)(lines[line].v1->x>>FRACBITS), (float)(lines[line].v1->y>>FRACBITS)*-1.0f,
                     (float)(lines[line].v2->x>>FRACBITS), (float)(lines[line].v2->y>>FRACBITS)*-1.0f, lines[line].sidenum[0]);

        Sector1 = sides[lines[line].sidenum[0]].sectornumb;
        Ceil1 =  (float)(sides[lines[line].sidenum[0]].sector->ceilingheight>>FRACBITS);
        Floor1 = (float)(sides[lines[line].sidenum[0]].sector->floorheight>>FRACBITS);

        if (lines[line].sidenum[1] >= 0) // We have a side 2
           {
            // If two sided, then invert the normal for the other side...
            Normal[lines[line].sidenum[1]].v[0] = Normal[lines[line].sidenum[0]].v[0] * -1;
            Normal[lines[line].sidenum[1]].v[1] = Normal[lines[line].sidenum[0]].v[1] * -1;
            Normal[lines[line].sidenum[1]].v[2] = Normal[lines[line].sidenum[0]].v[2] * -1;

            Sector2 = sides[lines[line].sidenum[1]].sectornumb;
            Ceil2   = (float)(sides[lines[line].sidenum[1]].sector->ceilingheight>>FRACBITS);
            Floor2  = (float)(sides[lines[line].sidenum[1]].sector->floorheight>>FRACBITS);

            // Check for "SKY" as the floor or ceiling and don't generate the upper or middle wall if it is "sky" on both sides
            strncpy(szCeil[0], lumpinfo[sides[lines[line].sidenum[0]].sector->ceilingpic+firstflat].name, 8);
            szCeil[0][8] = '\0';
            strncpy(szCeil[1], lumpinfo[sides[lines[line].sidenum[1]].sector->ceilingpic+firstflat].name, 8);
            szCeil[1][8] = '\0';
            if ((strcasecmp(szCeil[0], "F_SKY1") == 0) && (strcasecmp(szCeil[1], "F_SKY1") == 0))
               {
                SkyCeil = true;
               }
            else
               {
                SkyCeil = false;
               }

            strncpy(szFloor[0], lumpinfo[sides[lines[line].sidenum[0]].sector->floorpic+firstflat].name, 8);
            szFloor[0][8] = '\0';
            strncpy(szFloor[1], lumpinfo[sides[lines[line].sidenum[1]].sector->floorpic+firstflat].name, 8);
            szFloor[1][8] = '\0';
            if ((strcasecmp(szFloor[0], "F_SKY1") == 0) && (strcasecmp(szFloor[1], "F_SKY1") == 0))
               {
                SkyFloor = true;
               }
            else
               {
                SkyFloor = false;
               }

            // Check here for upper and lower textures left out...
            // Check uppers first...
            if ((Ceil1 > Ceil2) && (sides[lines[line].sidenum[0]].toptexture == 0) && (SkyCeil == false))
               {
                sides[lines[line].sidenum[0]].toptexture = TextureSearchSector(Sector1, 1);
                if (sides[lines[line].sidenum[0]].toptexture == 0)
                   {
                    if (sides[lines[line].sidenum[1]].toptexture != 0)
                       {
                        sides[lines[line].sidenum[0]].toptexture = sides[lines[line].sidenum[1]].toptexture;
                       }
                    else
                    if (sides[lines[line].sidenum[0]].bottomtexture != 0)
                       {
                        sides[lines[line].sidenum[0]].toptexture = sides[lines[line].sidenum[0]].bottomtexture;
                       }
                    else
                       {
                        sides[lines[line].sidenum[0]].toptexture = 1;
                       }
                   }
               }
            else
            if ((Ceil1 < Ceil2) && (sides[lines[line].sidenum[1]].toptexture == 0) && (SkyCeil == false))
               {
                sides[lines[line].sidenum[1]].toptexture = TextureSearchSector(Sector2, 1);
                if (sides[lines[line].sidenum[1]].toptexture == 0)
                   {
                    if (sides[lines[line].sidenum[0]].toptexture != 0)
                       {
                        sides[lines[line].sidenum[1]].toptexture = sides[lines[line].sidenum[0]].toptexture;
                       }
                    else
                    if (sides[lines[line].sidenum[1]].bottomtexture != 0)
                       {
                        sides[lines[line].sidenum[1]].toptexture = sides[lines[line].sidenum[1]].bottomtexture;
                       }
                    else
                       {
                        sides[lines[line].sidenum[1]].toptexture = 1;
                       }
                   }
               }
            // Then check the lowers...
            if ((Floor1 < Floor2) && (sides[lines[line].sidenum[0]].bottomtexture == 0) && (SkyFloor == false))
               {
                sides[lines[line].sidenum[0]].bottomtexture = TextureSearchSector(Sector1, 0);
                if (sides[lines[line].sidenum[0]].bottomtexture == 0)
                   {
                    if (sides[lines[line].sidenum[1]].bottomtexture != 0)
                       {
                        sides[lines[line].sidenum[0]].bottomtexture = sides[lines[line].sidenum[1]].bottomtexture;
                       }
                    else
                    if (sides[lines[line].sidenum[0]].toptexture != 0)
                       {
                        sides[lines[line].sidenum[0]].bottomtexture = sides[lines[line].sidenum[0]].toptexture;
                       }
                    else
                       {
                        sides[lines[line].sidenum[0]].bottomtexture = 1;
                       }
                   }
               }
            else
            if ((Floor2 < Floor1) && (sides[lines[line].sidenum[1]].bottomtexture == 0) && (SkyFloor == false))
               {
                sides[lines[line].sidenum[1]].bottomtexture = TextureSearchSector(Sector2, 0);
                if (sides[lines[line].sidenum[1]].bottomtexture == 0)
                   {
                    if (sides[lines[line].sidenum[0]].bottomtexture != 0)
                       {
                        sides[lines[line].sidenum[1]].bottomtexture = sides[lines[line].sidenum[0]].bottomtexture;
                       }
                    else
                    if (sides[lines[line].sidenum[1]].toptexture != 0)
                       {
                        sides[lines[line].sidenum[1]].bottomtexture = sides[lines[line].sidenum[1]].toptexture;
                       }
                    else
                       {
                        sides[lines[line].sidenum[1]].bottomtexture = 1;
                       }
                   }
               }
           }
        else
           {
            SkyCeil = false;
            SkyFloor = false;
           }

        if (lines[line].sidenum[1] >= 0) // We have a side 2
        {
            // Generate Polygons for the "sector1" (first) side of the "portal"
            if ((sides[lines[line].sidenum[0]].bottomtexture > 0) && (SkyFloor == false))
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[0];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_LOWER;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector1;
                TempPoly->Lighting[0] = (float)sectors[Sector1].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector1;
                TempPoly->BackSector = Sector2;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[0]].bottomtexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)Floor2;
                TempPoly->Point[0].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)Floor1;
                TempPoly->Point[1].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)Floor1;
                TempPoly->Point[2].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)Floor2;
                TempPoly->Point[3].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            if (sides[lines[line].sidenum[0]].midtexture > 0)
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[0];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_MIDDLE;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector1;
                TempPoly->Lighting[0] = (float)sectors[Sector1].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector1;
                TempPoly->BackSector = Sector2;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[0]].midtexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Figure out the top and bottom of this quad...
                if (Ceil1 < Ceil2)
                   MiddleTop = Ceil1;
                else
                   MiddleTop = Ceil2;
                if (Floor1 > Floor2)
                   MiddleBottom = Floor1;
                else
                   MiddleBottom = Floor2;
                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)MiddleTop;
                TempPoly->Point[0].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)MiddleBottom;
                TempPoly->Point[1].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)MiddleBottom;
                TempPoly->Point[2].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)MiddleTop;
                TempPoly->Point[3].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            if ((sides[lines[line].sidenum[0]].toptexture > 0) && (SkyCeil == false))
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[0];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_UPPER;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector1;
                TempPoly->Lighting[0] = (float)sectors[Sector1].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector1;
                TempPoly->BackSector = Sector2;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[0]].toptexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)Ceil1;
                TempPoly->Point[0].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)Ceil2;
                TempPoly->Point[1].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)Ceil2;
                TempPoly->Point[2].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)Ceil1;
                TempPoly->Point[3].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            // Generate Polygons for the "sector2" (second) side of the "portal"
            if ((sides[lines[line].sidenum[1]].bottomtexture > 0) && (SkyFloor == false))
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[1];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_LOWER;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector2;
                TempPoly->Lighting[0] = (float)sectors[Sector2].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector2;
                TempPoly->BackSector = Sector1;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[1]].bottomtexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)Floor1;
                TempPoly->Point[0].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)Floor2;
                TempPoly->Point[1].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)Floor2;
                TempPoly->Point[2].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)Floor1;
                TempPoly->Point[3].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            if (sides[lines[line].sidenum[1]].midtexture > 0)
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[1];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_MIDDLE;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector2;
                TempPoly->Lighting[0] = (float)sectors[Sector2].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector2;
                TempPoly->BackSector = Sector1;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[1]].midtexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Figure out the top and bottom of this quad...
                if (Ceil1 < Ceil2)
                   MiddleTop = Ceil1;
                else
                   MiddleTop = Ceil2;
                if (Floor1 > Floor2)
                   MiddleBottom = Floor1;
                else
                   MiddleBottom = Floor2;
                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)MiddleTop;
                TempPoly->Point[0].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)MiddleBottom;
                TempPoly->Point[1].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)MiddleBottom;
                TempPoly->Point[2].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)MiddleTop;
                TempPoly->Point[3].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            if ((sides[lines[line].sidenum[1]].toptexture > 0) && (SkyCeil == false))
               {
                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[1];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_UPPER;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector2;
                TempPoly->Lighting[0] = (float)sectors[Sector2].lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                TempPoly->Sector = Sector2;
                TempPoly->BackSector = Sector1;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[1]].toptexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)Ceil2;
                TempPoly->Point[0].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)Ceil1;
                TempPoly->Point[1].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)Ceil1;
                TempPoly->Point[2].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)Ceil2;
                TempPoly->Point[3].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }
        }
        else
        {
            pside = sides + pline->sidenum[0];

            if (sides[lines[line].sidenum[0]].midtexture > 0)
               {
                Sector1 = sides[lines[line].sidenum[0]].sectornumb;

                TempPoly = (DW_Polygon *)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = lines[line].sidenum[0];
                TempPoly->LineDef = line;
                TempPoly->Position = DW_MIDDLE;
                TempPoly->Texture[0] = 0;
                TempPoly->LSector = Sector1;
                TempPoly->Lighting[0] = (float)sides[lines[line].sidenum[0]].sector->lightlevel;
                TempPoly->Lighting[1] = TempPoly->Lighting[0];
                TempPoly->Lighting[2] = TempPoly->Lighting[0];
                TempPoly->Lighting[3] = 1.0f;
                TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;

                for (texture = 0; texture < TexCount; texture++)
                   {
                    if (TexList[texture].Number == sides[lines[line].sidenum[0]].midtexture)
                       {
                        TempPoly->Texture[0] = texture;
                        break;
                       }
                   }

                TempPoly->Sector = Sector1;
                TempPoly->BackSector = -1;

                Floor1 = FIXED_TO_FLOAT(pside->sector->floorheight);
                Ceil1  = FIXED_TO_FLOAT(pside->sector->ceilingheight);

                // Top left first...
                TempPoly->Point[0].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[0].v[1] = (float)Ceil1;
                TempPoly->Point[0].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom left next...
                TempPoly->Point[1].v[0] = (float)(lines[line].v1->x>>FRACBITS);
                TempPoly->Point[1].v[1] = (float)Floor1;
                TempPoly->Point[1].v[2] = (float)(lines[line].v1->y>>FRACBITS)*-1;
                // Bottom right next...
                TempPoly->Point[2].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[2].v[1] = (float)Floor1;
                TempPoly->Point[2].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;
                // Top right next...
                TempPoly->Point[3].v[0] = (float)(lines[line].v2->x>>FRACBITS);
                TempPoly->Point[3].v[1] = (float)Ceil1;
                TempPoly->Point[3].v[2] = (float)(lines[line].v2->y>>FRACBITS)*-1;

                TempPoly->Next                   = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
               }

            
        }

        for (sky_sector_count = 0, i = 0; i < 2; i++)
        {
            if (pline->sidenum[i] != -1
                && sides[pline->sidenum[i]].sector->ceilingpic == skyflatnum)
            {
                pside = sides + pline->sidenum[i];
                sky_sector_count++;
            }
        }


        if (sky_sector_count == 1)
        {
            Floor1 = FIXED_TO_FLOAT(pside->sector->floorheight);
            Ceil1  = FIXED_TO_FLOAT(pside->sector->ceilingheight);

            if (Ceil1 < fHighestCeiling)
            {
                TempPoly = (DW_Polygon*)malloc(sizeof(DW_Polygon));
                TempPoly->SideDef = pline->sidenum[0];
                TempPoly->LineDef = line;
                TempPoly->Position = 0;
                TempPoly->BackSector = pside->sectornumb;

                // Top left first...
                TempPoly->Point[0].v[0] = FIXED_TO_FLOAT(pline->v1->x);
                TempPoly->Point[0].v[1] = fHighestCeiling;
                TempPoly->Point[0].v[2] = -FIXED_TO_FLOAT(pline->v1->y);
                // Bottom left next...
                TempPoly->Point[1].v[0] = FIXED_TO_FLOAT(pline->v1->x);
                TempPoly->Point[1].v[1] = Ceil1;
                TempPoly->Point[1].v[2] = -FIXED_TO_FLOAT(pline->v1->y);
                // Bottom right next...
                TempPoly->Point[2].v[0] = FIXED_TO_FLOAT(pline->v2->x);
                TempPoly->Point[2].v[1] = Ceil1;
                TempPoly->Point[2].v[2] = -FIXED_TO_FLOAT(pline->v2->y);
                // Top right next...
                TempPoly->Point[3].v[0] = FIXED_TO_FLOAT(pline->v2->x);
                TempPoly->Point[3].v[1] = fHighestCeiling;
                TempPoly->Point[3].v[2] = -FIXED_TO_FLOAT(pline->v2->y);

                TempPoly->Next = side_polygons[TempPoly->SideDef];
                side_polygons[TempPoly->SideDef] = TempPoly;
                PolyCount++;
            }
        }
    }

    


    //lfprintf( "Total generated \"wall\" polygon count: %d\n", PolyCount);
}
   
//#define WDEBUG 1



#define  PARTLINE_VERTEX_DIFF   0.45f


typedef enum 
{
    PLC_NONE,
    PLC_V1,
    PLC_MIDDLE,
    PLC_V2,
} partline_cross_e;


typedef struct 
{
    float x, y;
    float dx, dy;
} partline_t;


typedef struct polyvertex_s 
{
    float x, y;
} polyvertex_t;


typedef struct 
{
    int            alloc_count;
    int            vertex_count;
    polyvertex_t** vertexes;
} polygon_t;


typedef struct 
{
    polyvertex_t  cross_point;
    polyvertex_t* pvertex;
    float         frac_along_partline;
    int           v_before, v_after; 
} cross_result_t;


#define POLYVERTEX_STORE_SIZE  256

typedef struct polyvertex_store_s
{
    struct polyvertex_store_s* next;
    int                        vertex_count;
    polyvertex_t               vertexes[POLYVERTEX_STORE_SIZE];
} polyvertex_store_t;


polyvertex_store_t* polyvert_store = NULL;


dboolean AreVertexesSame(polyvertex_t* pv1, polyvertex_t* pv2, float bias)
{
    if (fabsf(pv2->x - pv1->x) > bias)
        return false;

    if (fabsf(pv2->y - pv1->y) > bias)
        return false;

    return true;
}


dboolean IsPointOnRightSide(partline_t* pline, polyvertex_t* pv)
{
    return ((((double)pv->x - (double)pline->x) * (double)pline->dy)
            - (((double)pv->y - (double)pline->y) * (double)pline->dx)
            >= 0);
}


void PVStore_Free(void)
{
    polyvertex_store_t *pcurrent, *pprev;

    pcurrent = polyvert_store;
    while (pcurrent != NULL)
    {
        pprev    = pcurrent;
        pcurrent = pcurrent->next;
        free(pprev);
    }

    polyvert_store = NULL;
}


polyvertex_t* PVStore_NewVertex(void)
{
    polyvertex_store_t* pstore;

    pstore = polyvert_store;
    if (pstore == NULL || pstore->vertex_count == POLYVERTEX_STORE_SIZE)
    {
        polyvert_store               = malloc(sizeof(polyvertex_store_t));
        polyvert_store->next         = pstore;
        polyvert_store->vertex_count = 0;

        pstore = polyvert_store;
    }

    return pstore->vertexes + pstore->vertex_count++;
}


void PVStore_Push(polyvertex_t* pv)
{
    *PVStore_NewVertex() = *pv;
}


void PVStore_Init(void)
{
    int          i;
    polyvertex_t v;

    PVStore_Free();

    for (i = 0; i < numvertexes; i++)
    {
        v.x = FIXED_TO_FLOAT(vertexes[i].x);
        v.y = FIXED_TO_FLOAT(vertexes[i].y);
        PVStore_Push(&v);
    }
}


polyvertex_t* PVStore_FindCloseVertex(polyvertex_t* pv_close_to, float bias)
{
    polyvertex_store_t* pstore;
    polyvertex_t*       pv;
    int                 i;

    pstore = polyvert_store;
    while (pstore != NULL)
    {
        for (i = 0, pv = pstore->vertexes; i < pstore->vertex_count; i++, pv++)
            if (AreVertexesSame(pv, pv_close_to, bias))
                return pv;

        pstore = pstore->next;
    }

    return NULL;
}


polyvertex_t* PVStore_StoreVertex(polyvertex_t* pv, float bias)
{
    polyvertex_t* stored;

    stored = PVStore_FindCloseVertex(pv, bias);
    if (stored == NULL)
    {
        stored  = PVStore_NewVertex();
        *stored = *pv;
    }

    return stored;
}


void Poly_ZeroInit(polygon_t* ppoly)
{
    memset(ppoly, 0, sizeof(polygon_t));
}


void Poly_Init(polygon_t* ppoly, int alloc_count)
{
    ppoly->vertex_count = 0;
    ppoly->alloc_count  = alloc_count;
    ppoly->vertexes     = malloc(sizeof(void*) * alloc_count);
}


void Poly_Free(polygon_t* ppoly)
{
    ZFREE(ppoly->vertexes);
    ppoly->alloc_count = ppoly->vertex_count = 0;
}


void Poly_Move(polygon_t* pfrom, polygon_t* pto)
{
    FREE(pto->vertexes);

    *pto = *pfrom;
    memset(pfrom, 0, sizeof(polygon_t));
}

#define min(a,b) (((a) < (b)) ? (a) : (b))

void Poly_AppendFromPoly(
    polygon_t* pfrom, int range_start, int range_size, polygon_t* pto)
{

    if (range_size > (pto->alloc_count - pto->vertex_count))
        range_size = (pto->alloc_count - pto->vertex_count);

    while (range_size > 0)
    {
        int size = min(pfrom->vertex_count - range_start, range_size);

        memcpy(pto->vertexes + pto->vertex_count,
            pfrom->vertexes + range_start, sizeof(void*) * size);

        pto->vertex_count += size;
        range_size        -= size;
        range_start        = 0;
    }

}


void Poly_SplitCopy(
    polyvertex_t* pv1, polyvertex_t* pv2,
    polygon_t* pfrom, int range_start, int range_size,
    polygon_t* pto)
{
    int initial_vertex_count = 0;

    if (pv1 != NULL)
        initial_vertex_count++;

    if (pv2 != NULL)
        initial_vertex_count++;

    Poly_Free(pto);
    Poly_Init(pto, initial_vertex_count + range_size);

    if (pv1 != NULL)
        pto->vertexes[pto->vertex_count++] = pv1;

    if (pv2 != NULL)
        pto->vertexes[pto->vertex_count++] = pv2;

    Poly_AppendFromPoly(pfrom, range_start, range_size, pto);
}


void Poly_InsertCut(
    polyvertex_t* pv1, polyvertex_t* pv2,
    int save_range_start, int save_range_size,
    polygon_t* ppoly)
{
    polygon_t temp_poly;

    temp_poly = *ppoly;
    memset(ppoly, 0, sizeof(polygon_t));

    Poly_SplitCopy(pv1, pv2, &temp_poly, save_range_start, save_range_size, ppoly);
    Poly_Free(&temp_poly);
}


partline_cross_e GLBSP_TestPartitionLineIntersection(
    partline_t* pline, polyvertex_t* pv1, polyvertex_t* pv2,
    cross_result_t* presult)
{
    typedef struct vector_s
    {
        double x, y;
    }vector_t;

    double   frac;
    double   num, den; 
    vector_t edge, edge_right;
    vector_t line_right, origins_vector;

    edge.x = pv2->x - pv1->x;
    edge.y = pv2->y - pv1->y;

    edge_right.x = edge.y;
    edge_right.y = -edge.x;

    line_right.x = pline->dy;
    line_right.y = -pline->dx;

    den = line_right.x * edge.x + line_right.y * edge.y;
    if (fabs(den) < 1.0E-36f)
        return PLC_NONE;

    origins_vector.x = pline->x - pv1->x;
    origins_vector.y = pline->y - pv1->y;

    num  = origins_vector.x * line_right.x + origins_vector.y * line_right.y;
    frac = num / den;
    if (frac < 0.0 || frac > 1.0)
        return PLC_NONE;

    num = origins_vector.x * edge_right.x + origins_vector.y * edge_right.y;

    presult->frac_along_partline = (float)(num / den);

    presult->cross_point.x = (float)((double)pv1->x + edge.x * frac);
    presult->cross_point.y = (float)((double)pv1->y + edge.y * frac);

    if (frac < 0.05 
        && AreVertexesSame(&presult->cross_point, pv1, PARTLINE_VERTEX_DIFF))
    {
        presult->pvertex  = pv1;
        presult->v_before = -1;
        presult->v_after  = 1;

        return PLC_V1;
    }
    if (frac > 0.95 
        && AreVertexesSame(&presult->cross_point, pv2, PARTLINE_VERTEX_DIFF))
    {
        presult->pvertex  = pv2;
        presult->v_before = 0; 
        presult->v_after  = 2; 

        return PLC_V2;
    }


    presult->pvertex  = NULL;
    presult->v_before = 0; 
    presult->v_after  = 1;

    return PLC_MIDDLE;
}


void GLBSP_CutOutPolygonByLine(partline_t* pline, polygon_t* ppoly)
{
    int i, j;
    int vertex_save_count, first_saving_vertex;

    polyvertex_t *pv1, *pv2;

    cross_result_t    result_A, result_B, *presult;
    dboolean          A_found, B_found;
    dboolean          cut_at_vertexes;

    A_found = B_found = false;
    presult = &result_A;

    for (i = 0; i < ppoly->vertex_count; i++)
    {
        j = (i + 1) % ppoly->vertex_count;

        if (GLBSP_TestPartitionLineIntersection(pline,
            ppoly->vertexes[i], ppoly->vertexes[j], presult) == PLC_NONE)
            continue;

        if (!A_found)
        {
            result_A.v_after  += i;
            result_A.v_before += i;

            presult = &result_B;
            A_found = true;
        }
        else
        {
            if (result_B.pvertex != NULL &&
                result_B.pvertex == result_A.pvertex)
                continue;

            result_B.v_after  += i;
            result_B.v_before += i;

            B_found = true;
            break;
        }
    }

    if (!B_found)
        return;

    if (result_A.frac_along_partline < 0.0f && result_B.frac_along_partline < 0.0f)
        return;

    if (result_A.frac_along_partline > 1.0f && result_B.frac_along_partline > 1.0f)
        return;

    if (cut_at_vertexes = (result_A.pvertex != NULL && result_B.pvertex != NULL))
    {
        if (result_A.v_after + 1 == result_B.v_after)
            return;

        if (result_A.v_after + ppoly->vertex_count == result_B.v_after + 1)
            return;
    }

    if (result_A.pvertex == NULL)
        result_A.pvertex = PVStore_StoreVertex(&result_A.cross_point, 0.25f);

    if (result_B.pvertex == NULL)
        result_B.pvertex = PVStore_StoreVertex(&result_B.cross_point, 0.25f);

    if (result_A.frac_along_partline > result_B.frac_along_partline)
    {
        vertex_save_count   = result_B.v_before - result_A.v_after + 1;
        first_saving_vertex = result_A.v_after;
        
        pv1 = result_B.pvertex;
        pv2 = result_A.pvertex;
    }
    else
    {
        vertex_save_count   = result_A.v_before + ppoly->vertex_count - result_B.v_after + 1;
        first_saving_vertex = result_B.v_after;

        pv1 = result_A.pvertex;
        pv2 = result_B.pvertex;
    }

    if (cut_at_vertexes && ((vertex_save_count + 2) == ppoly->vertex_count))
        return;

    Poly_InsertCut(pv1, pv2, 
        first_saving_vertex % ppoly->vertex_count, vertex_save_count, ppoly);
}


void GLBSP_CutOutSubsectorPoly(int subsector, polygon_t* ppoly)
{
    subsector_t*      psubsector;
    seg_t*            pseg;
    line_t*           pline;
    partline_t        line;

    int i;

    psubsector = subsectors + subsector;

    for (pseg = segs + psubsector->firstline, i = 0;
        i < psubsector->numlines;
        pseg++, i++)
    {
        pline = pseg->linedef;

        if (pline->sidenum[1] != -1)
            continue;
       
        line.x  = FIXED_TO_FLOAT(pline->v1->x);
        line.y  = FIXED_TO_FLOAT(pline->v1->y);
        line.dx = FIXED_TO_FLOAT(pline->dx);
        line.dy = FIXED_TO_FLOAT(pline->dy);

        GLBSP_CutOutPolygonByLine(&line, ppoly);
    }
}


void GLBSP_SplitPoly(
    partline_t* pline, polygon_t* ppoly,
    polygon_t* ppoly_left, polygon_t* ppoly_right)
{
    int i, j;
    int saved_vertex_count;

    polygon_t* ptemppoly;

    cross_result_t  result_A, result_B;
    cross_result_t* presult;
    dboolean        A_found, B_found;

    A_found = B_found = false;
    presult = &result_A;

    for (i = 0; i < ppoly->vertex_count; i++)
    {
        j = (i + 1) % ppoly->vertex_count;

        if (GLBSP_TestPartitionLineIntersection(
            pline, ppoly->vertexes[i], ppoly->vertexes[j], 
            presult) == PLC_NONE)
            continue;

        if (!A_found)
        {
            result_A.v_before += i;
            result_A.v_after  += i;

            presult = &result_B;
            A_found = true;
        }
        else 
        {
            if (result_B.pvertex != NULL &&
                result_B.pvertex == result_A.pvertex)
                continue;

            result_B.v_before += i;
            result_B.v_after  += i;
            B_found            = true;
            break;
        }
    }

    if (B_found)
    {
        if (result_A.pvertex == NULL)
            result_A.pvertex = PVStore_StoreVertex(&result_A.cross_point, 0.01f);

        if (result_B.pvertex == NULL)
            result_B.pvertex = PVStore_StoreVertex(&result_B.cross_point, 0.01f);

        if (result_A.frac_along_partline <= result_B.frac_along_partline)
        {
            ptemppoly   = ppoly_left;
            ppoly_left  = ppoly_right;
            ppoly_right = ptemppoly;
        }
    
        saved_vertex_count = result_B.v_before - result_A.v_after + 1;
        if (saved_vertex_count > 0)
        {
            Poly_SplitCopy(result_B.pvertex, result_A.pvertex,
                ppoly, result_A.v_after, saved_vertex_count, ppoly_right);
        }
        else
            ppoly_right->vertex_count = 0;

        saved_vertex_count = result_A.v_before + ppoly->vertex_count - result_B.v_after + 1;
        if (saved_vertex_count > 0)
        {
            Poly_SplitCopy(result_A.pvertex, result_B.pvertex,
                ppoly, result_B.v_after % ppoly->vertex_count, saved_vertex_count, ppoly_left);
        }
        else
            ppoly_left->vertex_count = 0;
    }
    else
    {
        if (IsPointOnRightSide(pline, ppoly->vertexes[0]))
        {
            Poly_Move(ppoly, ppoly_right);
            ppoly_left->vertex_count = 0;
        }
        else
        {
            Poly_Move(ppoly, ppoly_left);
            ppoly_right->vertex_count = 0;
        }
    }
}


void GLBSP_BuildSubsectorPolygons_Recursive(int bspnum, polygon_t* ppoly)
{
    polygon_t      left_poly, right_poly;
    node_t        *pnode;
    partline_t     line;


    if (bspnum & NF_SUBSECTOR)
    {
        int           subsector = bspnum & ~(NF_SUBSECTOR);
        DW_FloorCeil* pplane    = subsector_planes + subsector;
        int           i;

        GLBSP_CutOutSubsectorPoly(subsector, ppoly);
        
        pplane->Sector = subsectors[subsector].sector - sectors;

        pplane->PCount = ppoly->vertex_count;
        pplane->Point = (DW_Vertex3Dv*)malloc(sizeof(DW_Vertex3Dv) * ppoly->vertex_count);
        for (i = 0; i < ppoly->vertex_count; i++)
        {
            pplane->Point[i].v[0] = ppoly->vertexes[i]->x;
            pplane->Point[i].v[2] = -ppoly->vertexes[i]->y;
        }

        return;
    }

    pnode   = nodes + bspnum;
    line.x  = FIXED_TO_FLOAT(pnode->x);
    line.y  = FIXED_TO_FLOAT(pnode->y);
    line.dx = FIXED_TO_FLOAT(pnode->dx);
    line.dy = FIXED_TO_FLOAT(pnode->dy);

    Poly_ZeroInit(&left_poly);
    Poly_ZeroInit(&right_poly);
    GLBSP_SplitPoly(&line, ppoly, &left_poly, &right_poly);

    if(right_poly.vertex_count > 0)
        GLBSP_BuildSubsectorPolygons_Recursive(pnode->children[0], &right_poly);
    if (left_poly.vertex_count > 0)
        GLBSP_BuildSubsectorPolygons_Recursive(pnode->children[1], &left_poly);

    Poly_Free(&left_poly);
    Poly_Free(&right_poly);
}


void CreateNewFlats()
{
    static int             sector, line, side1, side2, section, side, l1, l2, lc, i, j;
    static int             seg;
    static sector_plane_t *pplane;
    static sector_t       *psector;
    static line_t         *pline;
    static side_t         *pside1, *pside2;
    static rec_t          *pbbox;
    static int             ss_count, ss, ssid, lineid;
    int                    v1x, v2x, v1y, v2y;

    polygon_t  poly;
    fixed_t    rootbbox[4];

    // Free up what we allocated for the previous level...
    if (planes != NULL)
    {
        for (sector = 0, pplane = planes; sector < lastsectors; sector++, pplane++)
            ZFREE(pplane->subsectors);

        ZFREE(planes);
    }

    ZFREE(subsector_planes);

    planes = (sector_plane_t*)malloc(sizeof(sector_plane_t) * numsectors);
    memset(planes, 0, sizeof(sector_plane_t) * numsectors);

    subsector_planes = (DW_FloorCeil*)malloc(sizeof(DW_FloorCeil) * numsubsectors);
    memset(subsector_planes, 0, sizeof(DW_FloorCeil) * numsubsectors);


#ifdef WDEBUG
    lfprintf( "Number of sectors = %d\n", numsectors);
#endif


    for (sector = 0, psector = sectors; sector < numsectors; sector++, psector++) // For each sector...
    {

        if ((psector->floorpic == skyflatnum) && (psector->ceilingpic == skyflatnum))
           continue;

#ifdef WDEBUG
        lfprintf( "Processing sector %d flats\n", sector);
#endif

        pbbox = SectorBBox + sector;
        pbbox->left = pbbox->bottom =  32767;
        pbbox->top  = pbbox->right  = -32767;

        // Build a line list for this sector...
        for (line = 0, pline = lines; line < numlines; line++, pline++)
        {
            pside1 = sides + pline->sidenum[0];
            pside2 = (pline->sidenum[1] != -1) ? (sides + pline->sidenum[1]) : NULL;

            // ignore trip lines...
            if (pside2 != NULL && pside1->sectornumb == pside2->sectornumb)
                continue;

            // If the sector of sidedef 1 or  sidedef 2 of this line equals this sector, we'll use it...
            if ((pside1->sectornumb == sector) || (pside2 != NULL && pside2->sectornumb == sector))
            {
                v1x = (pline->v1->x >> FRACBITS);
                v1y = (pline->v1->y >> FRACBITS)*-1;
                v2x = (pline->v2->x >> FRACBITS);
                v2y = (pline->v2->y >> FRACBITS)*-1;

                if (pbbox->left > v1x)
                    pbbox->left = v1x;
                if (pbbox->bottom > v1y)
                    pbbox->bottom = v1y;
                if (pbbox->left > v2x)
                    pbbox->left = v2x;
                if (pbbox->bottom > v2y)
                    pbbox->bottom = v2y;
                if (pbbox->right < v1x)
                    pbbox->right = v1x;
                if (pbbox->top < v1y)
                    pbbox->top = v1y;
                if (pbbox->right < v2x)
                    pbbox->right = v2x;
                if (pbbox->top < v2y)
                    pbbox->top = v2y;

            }
        }
    }


    PVStore_Init();

    M_ClearBox(rootbbox);
    for (i = 0; i < numvertexes; i++)
        M_AddToBox(rootbbox, vertexes[i].x, vertexes[i].y);

    Poly_Init(&poly, 4);
    for (i = 0; i < 4; i++)
        poly.vertexes[i] = PVStore_NewVertex();
    poly.vertex_count = 4;

    poly.vertexes[0]->x = poly.vertexes[1]->x = FIXED_TO_FLOAT(rootbbox[BOXLEFT]);
    poly.vertexes[2]->x = poly.vertexes[3]->x = FIXED_TO_FLOAT(rootbbox[BOXRIGHT]);
    poly.vertexes[0]->y = poly.vertexes[3]->y = FIXED_TO_FLOAT(rootbbox[BOXBOTTOM]);
    poly.vertexes[1]->y = poly.vertexes[2]->y = FIXED_TO_FLOAT(rootbbox[BOXTOP]);

    GLBSP_BuildSubsectorPolygons_Recursive(numnodes - 1, &poly);

    Poly_Free(&poly);
    PVStore_Free();

    for (i = 0; i < numsubsectors; i++)
        planes[subsector_planes[i].Sector].ss_count++;

    for (i = 0; i < numsectors; i++)
    {
        pplane = planes + i;
        pplane->subsectors = (DW_FloorCeil**)malloc(sizeof(DW_FloorCeil*) * pplane->ss_count);
        memset(pplane->subsectors, 0, sizeof(DW_FloorCeil*) * pplane->ss_count);
        pplane->ss_count = 0;
    }

    for (i = 0; i < numsubsectors; i++)
    {
        pplane = planes + subsector_planes[i].Sector;
        pplane->subsectors[pplane->ss_count++] = subsector_planes + i;
    }


    ZFREE(DrawFlat);

    DrawFlat = (dbyte *)malloc(sizeof(dbyte *) * numsectors);
    for (sector = 0; sector < numsectors; sector++)
        DrawFlat[sector] = false;

    ZFREE(sorted_flats);
    sorted_flats       = (sector_plane_t**)malloc(sizeof(sector_plane_t*) * numsectors);
    sorted_flats_count = 0;

    lastsectors = numsectors;
}


void Build3DLevel()
   {
    LoadNewTextures();

    CreateNewWalls();
   
    SectorBBox = (rec_t *)alloca(sizeof(rec_t)*numsectors);

    CreateNewFlats();

    CalcTexCoords();

    SectorBBox = 0;

    // find place to drop these:
    //
    // flats
    // floorlist
    // ceillist
   }



void CalcTexCoords()
   {
    DW_Polygon   *TempPoly;
    float         fLength, fHigh, fVertOff, fHorzOff, GLXOff, GLYOff, UnPegOff, xGrid, yGrid;
    float         GLHigh, DHigh, YRatio, YPos, YOffset;
    int           p, sector, subsector, side;
    float         SectorX, SectorY;
    DW_FloorCeil *psubsector;

    for (side = 0; side < numsides; side++)
    {
        TempPoly = side_polygons[side];
        while (TempPoly != NULL)
        {
            if (TempPoly->Position)
            {
                GLHigh = (float)TexList[TempPoly->Texture[0]].GLHigh;
                DHigh = (float)TexList[TempPoly->Texture[0]].DHigh;
                YRatio = DHigh / GLHigh;

                // tu = horizontal texture coordinate
                // tv = vertical texture coordinate
                fLength = (float)sqrt(((TempPoly->Point[0].v[0] - TempPoly->Point[3].v[0]) * (TempPoly->Point[0].v[0] - TempPoly->Point[3].v[0])) +
                    ((TempPoly->Point[0].v[2] - TempPoly->Point[3].v[2]) * (TempPoly->Point[0].v[2] - TempPoly->Point[3].v[2])));
                fHigh = (TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1]);
                GLXOff = (float)(TexList[TempPoly->Texture[0]].GLWide - TexList[TempPoly->Texture[0]].DWide) / fLength;
                GLYOff = (float)(GLHigh - DHigh) / GLHigh;
                YOffset = (float)(sides[TempPoly->SideDef].rowoffset >> FRACBITS);
                if (((sides[TempPoly->SideDef].rowoffset >> FRACBITS) == 0) || (DHigh == 0))
                    fVertOff = 0.0f;
                else
                {
                    fVertOff = ((float)(sides[TempPoly->SideDef].rowoffset >> FRACBITS) / DHigh) * -1.0f;
                }
                while (fVertOff < 0.0f)
                    fVertOff += 1.0f;
                if (((sides[TempPoly->SideDef].textureoffset >> FRACBITS) == 0) || (TexList[TempPoly->Texture[0]].DWide == 0))
                    fHorzOff = 0.0f;
                else
                {
                    fHorzOff = ((float)(sides[TempPoly->SideDef].textureoffset >> FRACBITS) / (float)TexList[TempPoly->Texture[0]].DWide);
                }
                while (fHorzOff < 0.0f)
                    fHorzOff += 1.0f;

                TempPoly->Point[0].tu = 0.0f + fHorzOff;
                switch (TempPoly->Position)
                { // line_t
                case DW_UPPER:
                    if ((lines[TempPoly->LineDef].flags & DW_UPUNPEG) == DW_UPUNPEG)
                        TempPoly->Point[0].tv = 1.0f + fVertOff;
                    else
                        TempPoly->Point[0].tv = (((fHigh / DHigh) + fVertOff) * YRatio) + GLYOff;
                    break;

                case DW_MIDDLE:
                    if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                        TempPoly->Point[0].tv = (fHigh / DHigh) + fVertOff;
                    else
                    {
                        YPos = YOffset;
                        TempPoly->Point[0].tv = 1.0f - (YPos / GLHigh);
                    }
                    break;

                case DW_LOWER:
                    if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                    {
                        UnPegOff = (float)((sectors[sides[TempPoly->SideDef].sectornumb].ceilingheight >> FRACBITS) - TempPoly->Point[0].v[1]) / GLHigh;
                        while (UnPegOff > 1.0f)
                            UnPegOff -= 1.0f;
                        UnPegOff = 1.0f - UnPegOff;
                        TempPoly->Point[0].tv = UnPegOff + fVertOff;
                    }
                    else
                        TempPoly->Point[0].tv = 1.0f + fVertOff;
                    break;
                }

                TempPoly->Point[1].tu = TempPoly->Point[0].tu;
                switch (TempPoly->Position)
                {
                case DW_UPPER:
                    if ((lines[TempPoly->LineDef].flags & DW_UPUNPEG) == DW_UPUNPEG)
                        TempPoly->Point[1].tv = (1.0f - (fHigh / GLHigh)) + fVertOff;
                    else
                        TempPoly->Point[1].tv = 0.0f + fVertOff + GLYOff;
                    break;

                case DW_MIDDLE:
                    if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                        TempPoly->Point[1].tv = ((GLHigh - DHigh) / GLHigh) + fVertOff;
                    else
                    {
                        YPos = fHigh + YOffset;
                        TempPoly->Point[1].tv = (1.0f - (YPos / GLHigh));
                    }
                    break;

                case DW_LOWER:
                    if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                    {
                        UnPegOff = TempPoly->Point[0].tv - ((TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1]) / GLHigh);
                        TempPoly->Point[1].tv = UnPegOff;
                    }
                    else
                        TempPoly->Point[1].tv = (1.0f - (fHigh / GLHigh)) + fVertOff;
                }

                TempPoly->Point[2].tu = (fLength / (float)TexList[TempPoly->Texture[0]].DWide) + fHorzOff;
                TempPoly->Point[2].tv = TempPoly->Point[1].tv;

                TempPoly->Point[3].tu = TempPoly->Point[2].tu;
                TempPoly->Point[3].tv = TempPoly->Point[0].tv;
            }
            TempPoly = TempPoly->Next;
        }
    }

    for (sector = 0; sector < numsectors; sector++)
       {
        xGrid = 64.0f;
        yGrid = 64.0f;
        SectorX = ((float)(SectorBBox[sector].left % 64) / 64.0f);
        if (SectorX < 0.0f)
           SectorX += 1.0f;
        SectorY = ((float)(SectorBBox[sector].bottom % 64) / 64.0f);
        if (SectorY < 0.0f)
           SectorY += 1.0f;

        for (subsector = 0; subsector < planes[sector].ss_count; subsector++)
        {
            psubsector = planes[sector].subsectors[subsector];

            for (p = 0; p < psubsector->PCount; p++)
            {
                psubsector->Point[p].tu = ((float)(psubsector->Point[p].v[0] - SectorBBox[sector].left)/xGrid)+SectorX;
                psubsector->Point[p].tv = ((float)(psubsector->Point[p].v[2] - SectorBBox[sector].bottom)/yGrid)+SectorY;
            }
        }

       }
   }


/*
   Figuring out the sprites:

   Trying to figure out which sprites to load for each level is rather slow and it's also quite easy to
   miss sprites if certain states have not been accounted for.  It might just be better to load every one
   of the sprites by it's sprite number and be done with it.  This will result in upwards of three megabytes
   of memory used for the sprites at load time and will cause the initial program load to be quite long.
   It will have the benefit, though of making sure that ALL the sprites get loaded and are available.

   Another possibility is to load the sprites on an as-needed basis.  Which might make for some slow spots
   when monsters are first encountered or a new weapon is picked up and fired. Also, explosions might make
   for stutters as well.

   Try loading all the sprites as textures first then just using them.  For the alpha, treat memory as if
   it is unlimited.

   Optimization can be done once the brute force methods work.
*/


extern int        NumSpriteLumps;
extern int       *SpriteLumps;
extern GLTexData *SprData;

extern spritedef_t*	sprites;
void GL_MakeSpriteTexture(patch_t *Sprite, GLTexData *Tex, dboolean smooth);

void LoadAllSprites()
   {
    int    i, lump, dotchk, dotmod;
    char   tstr[64];

    dotmod = numspritelumps/32;
    dotchk = numspritelumps/dotmod;
    
    printf("[%*s]\b", dotchk, " ");
    for (i = 0; i < dotchk; i++)
       {
        tstr[i] = '\b';
       }
    tstr[i] = '\0';
    printf(tstr);
    for (lump = 0, dotchk = 0; lump < numspritelumps; lump++)
       {
        dotchk++;
        if ((dotchk % dotmod) == 0)
           {
            printf(".");
           }
        GL_MakeSpriteTexture(W_CacheLumpNum(firstspritelump+lump,PU_CACHE), &SprData[lump], true);
       }
    printf("\n");
   }

void BuildThingList()
   {
    int i, frame, angle, lump;
    int bstate, state, pstate;

    // additional player sprites (weapons etc.)
    SpritePresent[MT_PUFF] = 1;
    SpritePresent[MT_ROCKET] = 1;
    if (gamemode != shareware)
       {
        SpritePresent[MT_PLASMA] = 1;
        SpritePresent[MT_BFG] = 1;
        SpritePresent[MT_TFOG] = 1;
       }

    // additional "monster" sprites (weapons and attacks)
    if (gamemode == commercial)
       {
        SpritePresent[MT_FIRE] = 1;
        SpritePresent[MT_BRUISERSHOT] = 1;
        SpritePresent[MT_SPAWNSHOT] = 1;
        SpritePresent[MT_SPAWNFIRE] = 1;
       }
    SpritePresent[MT_IFOG] = 1;
    SpritePresent[MT_TROOPSHOT] = 1;
    SpritePresent[MT_HEADSHOT] = 1;
    SpritePresent[MT_TELEPORTMAN] = 1;

    for (i = 0; i < NUMMOBJTYPES; i++)
       {
        if (SpritePresent[i] > 0)
           {
            state = bstate = mobjinfo[i].spawnstate;
            do
               {
                pstate = state;
                frame = states[state].frame & 0x7FFF;
                for (angle = 0; angle < 8; angle++)
                   {
//                    lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                       frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                    SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                    if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                        break;
                   }
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state > pstate));
            if (mobjinfo[i].seestate != S_NULL)
               {
                state = bstate = mobjinfo[i].seestate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
            if (mobjinfo[i].painstate != S_NULL)
               {
                state = bstate = mobjinfo[i].painstate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
            if (mobjinfo[i].meleestate != S_NULL)
               {
                state = bstate = mobjinfo[i].meleestate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
            if (mobjinfo[i].missilestate != S_NULL)
               {
                state = bstate = mobjinfo[i].missilestate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
            if (mobjinfo[i].deathstate != S_NULL)
               {
                state = bstate = mobjinfo[i].deathstate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
            if (mobjinfo[i].xdeathstate != S_NULL)
               {
                state = bstate = mobjinfo[i].xdeathstate;
                do
                   {
                    pstate = state;
                    frame = states[state].frame & 0x7FFF;
                    for (angle = 0; angle < 8; angle++)
                       {
//                        lfprintf( "Object type %d uses sprite %s frame %d lump %d - %d present.\n", i, sprnames[states[state].sprite],
//                          frame, sprites[states[state].sprite].spriteframes[frame].lump[angle], SpritePresent[i]);
                        SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[angle]]++;
                        if (sprites[states[state].sprite].spriteframes[frame].rotate == false)
                            break;
                       }
                    state = states[state].nextstate;
                   }
                while ((state != S_NULL) && (state != bstate) && (state > pstate));
               }
           }
       }
    for (lump = 0; lump < NumSpriteLumps; lump++)
       {
        if ((SpriteLumps[lump] > 0) && (SprData[lump].Permanent == false))
           {
//            if (modifiedgame)
//                patched = W_GetNumForName(lumpinfo[lump].name);
//            else
//                patched = l;
            //lfprintf( "GL Sprite Lump %d\n", firstspritelump+lump);
            GL_MakeSpriteTexture(W_CacheLumpNum(firstspritelump+lump,PU_CACHE), &SprData[lump], true);
//            lfprintf( "World Sprite lump %d used - texture %d - height %f, top %f\n", lump, SprData[lump].TexName, SprData[lump].Height, SprData[lump].TopOff);
           }
       }
    //wglMakeCurrent(NULL, NULL);
   }

void WS_Init(void) // Setup Weapon Sprites...
   {
    int i, frame, lump;
    int bstate, state, wmax;

    if (gamemode != shareware)
       {
        if (gamemode == commercial)
            wmax = NUMWEAPONS;
        else
            wmax = NUMWEAPONS - 1;
       }
    else
       {
        wmax = 5;
       }


    for (i = 0; i < wmax; i++)
       {
//        lfprintf( "Weapon type %d\n", i);
//        lfprintf("Up state\n");
        if (weaponinfo[i].upstate != S_NULL)
           {
            state = bstate = weaponinfo[i].upstate;
            do
               {
                frame = states[state].frame&0x7FFF;
//                lfprintf( "Weapon type %d upstate sprite %s frame %d lump %d.\n", i, sprnames[states[state].sprite],
//                   frame, sprites[states[state].sprite].spriteframes[frame].lump[0]);
                SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[0]]++;
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state != S_LIGHTDONE) && (state != weaponinfo[i].readystate));
           }
        
//        lfprintf("Down state\n");
        if (weaponinfo[i].downstate != S_NULL)
           {
            state = bstate = weaponinfo[i].downstate;
            do
               {
                frame = states[state].frame&0x7FFF;
//                lfprintf( "Weapon type %d downstate sprite %s frame %d lump %d.\n", i, sprnames[states[state].sprite],
//                   frame, sprites[states[state].sprite].spriteframes[frame].lump[0]);
                SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[0]]++;
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state != S_LIGHTDONE) && (state != weaponinfo[i].readystate));
           }
        
//        lfprintf("Ready state\n");
        if (weaponinfo[i].readystate != S_NULL)
           {
            state = bstate = weaponinfo[i].readystate;
            do
               {
                frame = states[state].frame&0x7FFF;
//                lfprintf( "Weapon type %d readystate sprite %s frame %d lump %d.\n", i, sprnames[states[state].sprite],
//                   frame, sprites[states[state].sprite].spriteframes[frame].lump[0]);
                SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[0]]++;
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state != S_LIGHTDONE));
           }
        
//        lfprintf("Attack state\n");
        if (weaponinfo[i].atkstate != S_NULL)
           {
            state = bstate = weaponinfo[i].atkstate;
            do
               {
                frame = states[state].frame&0x7FFF;
//                lfprintf( "Weapon type %d atkstate sprite %s frame %d lump %d.\n", i, sprnames[states[state].sprite],
//                   frame, sprites[states[state].sprite].spriteframes[frame].lump[0]);
                SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[0]]++;
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state != S_LIGHTDONE) && (state != weaponinfo[i].readystate));
           }
        
//        lfprintf("Flash state\n");
        if (weaponinfo[i].flashstate != S_NULL)
           {
            state = bstate = weaponinfo[i].flashstate;
            do
               {
                frame = states[state].frame&0x7FFF;
//                lfprintf( "Weapon type %d flashstate sprite %s frame %d lump %d.\n", i, sprnames[states[state].sprite],
//                   frame, sprites[states[state].sprite].spriteframes[frame].lump[0]);
                SpriteLumps[sprites[states[state].sprite].spriteframes[frame].lump[0]]++;
                SprData[sprites[states[state].sprite].spriteframes[frame].lump[0]].Translucent = 153;
                state = states[state].nextstate;
               }
            while ((state != S_NULL) && (state != bstate) && (state != S_LIGHTDONE) && (state != weaponinfo[i].readystate));
           }
       }
    for (lump = 0; lump < NumSpriteLumps; lump++)
       {
        SprData[lump].TexName = 0;
        if (SpriteLumps[lump] > 0)
           {
            if (SprData[lump].Translucent != 153)
                SprData[lump].Translucent = 64;
            GL_MakeSpriteTexture(W_CacheLumpNum(firstspritelump+lump,PU_CACHE), &SprData[lump], true);
            //lfprintf( "Weapon Sprite lump %d used - texture %d\n", lump, SprData[lump].TexName);
            SprData[lump].Permanent = true;
           }
       }
   }

