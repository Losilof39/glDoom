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
// $Log:$
//
// DESCRIPTION:
//	Rendering main loop and setup functions,
//	 utility functions (BSP, geometry, trigonometry).
//	See tables.c, too.
//
//-----------------------------------------------------------------------------


static const char rcsid[] = "$Id: r_main.c,v 1.5 1997/02/03 22:45:12 b1 Exp $";

#include <glad/glad.h>

#include <stdlib.h>
#include <math.h>
#include "mathlib.h"

#include "doomdef.h"
#include "d_net.h"

#include "m_bbox.h"

#include "r_local.h"
#include "r_sky.h"

#include "sys_sdl.h"
#include "gldefs.h"

#include "v_video.h"
#include "renderer3D.h"

// Fineangles in the SCREENWIDTH wide window.
#define FIELDOFVIEW		2048
#define SBARHEIGHT      32 // also defined in r_draw.c rather than r_draw.h (BAD)


extern float SetBack, glLeft, glTop, glRight, glBottom;

extern int              sorted_flats_count, sorted_walls_count;
extern sector_plane_t** sorted_flats;
extern DW_Polygon**     sorted_walls;

video_t  video;

int			viewangleoffset;

// increment every time a check is made
int			validcount = 1;		


lighttable_t*		fixedcolormap;
extern lighttable_t**	walllights;

int			centerx;
int			centery;

fixed_t			centerxfrac;
fixed_t			centeryfrac;
fixed_t			projection;

// just for profiling purposes
int			framecount;	

int			sscount;
int			linecount;
int			loopcount;

fixed_t			viewx;
fixed_t			viewy;
fixed_t			viewz;

angle_t			viewangle;

camera_t        camera;
float           langle, rangle;

fixed_t			viewcos;
fixed_t			viewsin;

player_t*		viewplayer;

// 0 = high, 1 = low
int			detailshift;	

//
// precalculated math tables
//
angle_t			clipangle;

// The viewangletox[viewangle + FINEANGLES/4] lookup
// maps the visible view angles to screen X coordinates,
// flattening the arc to a flat projection plane.
// There will be many angles mapped to the same X. 
int			viewangletox[FINEANGLES/2];

// The xtoviewangleangle[] table maps a screen pixel
// to the lowest viewangle that maps back to x ranges
// from clipangle to -clipangle.
angle_t          *xtoviewangle;
//angle_t			xtoviewangle[SCREENWIDTH+1];


// UNUSED.
// The finetangentgent[angle+FINEANGLES/4] table
// holds the fixed_t tangent values for view angles,
// ranging from MININT to 0 to MAXINT.
// fixed_t		finetangent[FINEANGLES/2];

// fixed_t		finesine[5*FINEANGLES/4];
fixed_t*		finecosine = &finesine[FINEANGLES/4];


lighttable_t*		scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
lighttable_t*		scalelightfixed[MAXLIGHTSCALE];
lighttable_t*		zlight[LIGHTLEVELS][MAXLIGHTZ];

// bumped light from gun blasts
int			extralight;			

extern int skyflatnum;



void (*colfunc) (void);
void (*basecolfunc) (void);
void (*fuzzcolfunc) (void);
void (*transcolfunc) (void);
void (*spanfunc) (void);


//
// R_AddPointToBox
// Expand a given bbox
// so that it encloses a given point.
//
void
R_AddPointToBox
( int		x,
  int		y,
  fixed_t*	box )
{
    if (x< box[BOXLEFT])
	box[BOXLEFT] = x;
    if (x> box[BOXRIGHT])
	box[BOXRIGHT] = x;
    if (y< box[BOXBOTTOM])
	box[BOXBOTTOM] = y;
    if (y> box[BOXTOP])
	box[BOXTOP] = y;
}


//
// R_PointOnSide
// Traverse BSP (sub) tree,
//  check point against partition plane.
// Returns side 0 (front) or 1 (back).
//
int
R_PointOnSide
( fixed_t	x,
  fixed_t	y,
  node_t*	node )
{
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;
	
    if (!node->dx)
    {
	if (x <= node->x)
	    return node->dy > 0;
	
	return node->dy < 0;
    }
    if (!node->dy)
    {
	if (y <= node->y)
	    return node->dx < 0;
	
	return node->dx > 0;
    }
	
    dx = (x - node->x);
    dy = (y - node->y);
	
    // Try to quickly decide by looking at sign bits.
    if ( (node->dy ^ node->dx ^ dx ^ dy)&0x80000000 )
    {
	if  ( (node->dy ^ dx) & 0x80000000 )
	{
	    // (left is negative)
	    return 1;
	}
	return 0;
    }

    left = FixedMul ( node->dy>>FRACBITS , dx );
    right = FixedMul ( dy , node->dx>>FRACBITS );
	
    if (right < left)
    {
	// front side
	return 0;
    }
    // back side
    return 1;			
}


int
R_PointOnSegSide
( fixed_t	x,
  fixed_t	y,
  seg_t*	line )
{
    fixed_t	lx;
    fixed_t	ly;
    fixed_t	ldx;
    fixed_t	ldy;
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	left;
    fixed_t	right;
	
    lx = line->v1->x;
    ly = line->v1->y;
	
    ldx = line->v2->x - lx;
    ldy = line->v2->y - ly;
	
    if (!ldx)
    {
	if (x <= lx)
	    return ldy > 0;
	
	return ldy < 0;
    }
    if (!ldy)
    {
	if (y <= ly)
	    return ldx < 0;
	
	return ldx > 0;
    }
	
    dx = (x - lx);
    dy = (y - ly);
	
    // Try to quickly decide by looking at sign bits.
    if ( (ldy ^ ldx ^ dx ^ dy)&0x80000000 )
    {
	if  ( (ldy ^ dx) & 0x80000000 )
	{
	    // (left is negative)
	    return 1;
	}
	return 0;
    }

    left = FixedMul ( ldy>>FRACBITS , dx );
    right = FixedMul ( dy , ldx>>FRACBITS );
	
    if (right < left)
    {
	// front side
	return 0;
    }
    // back side
    return 1;			
}


//
// R_PointToAngle
// To get a global angle from cartesian coordinates,
//  the coordinates are flipped until they are in
//  the first octant of the coordinate system, then
//  the y (<=x) is scaled and divided by x to get a
//  tangent (slope) value which is looked up in the
//  tantoangle[] table.

//




angle_t
R_PointToAngle
( fixed_t	x,
  fixed_t	y )
{	
    x -= viewx;
    y -= viewy;
    
    if ( (!x) && (!y) )
	return 0;

    if (x>= 0)
    {
	// x >=0
	if (y>= 0)
	{
	    // y>= 0

	    if (x>y)
	    {
		// octant 0
		return tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 1
		return ANG90-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 8
		return (tantoangle[SlopeDiv(y,x)] * -1);
	    }
	    else
	    {
		// octant 7
		return ANG270+tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    else
    {
	// x<0
	x = -x;

	if (y>= 0)
	{
	    // y>= 0
	    if (x>y)
	    {
		// octant 3
		return ANG180-1-tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 2
		return ANG90+ tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 4
		return ANG180+tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		 // octant 5
		return ANG270-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    return 0;
}


angle_t
R_PointToAngle2
( fixed_t	x1,
  fixed_t	y1,
  fixed_t	x2,
  fixed_t	y2 )
{	
    viewx = x1;
    viewy = y1;
    
    return R_PointToAngle (x2, y2);
}


fixed_t
R_PointToDist
( fixed_t	x,
  fixed_t	y )
{
    int		angle;
    fixed_t	dx;
    fixed_t	dy;
    fixed_t	temp;
    fixed_t	dist;
	
    dx = abs(x - viewx);
    dy = abs(y - viewy);
	
    if (dy>dx)
    {
	temp = dx;
	dx = dy;
	dy = temp;
    }
	
    angle = (tantoangle[ FixedDiv(dy,dx)>>DBITS ]+ANG90) >> ANGLETOFINESHIFT;

    // use as cosine
    dist = FixedDiv (dx, finesine[angle] );	
	
    return dist;
}




//
// R_InitPointToAngle
//
void R_InitPointToAngle (void)
{
    // UNUSED - now getting from tables.c
#if 0
    int	i;
    int	t;
    float	f;
//
// slope (tangent) to angle lookup
//
    for (i=0 ; i<=SLOPERANGE ; i++)
    {
	f = atan( (float)i/SLOPERANGE )/(3.141592657*2);
	t = 0xffffffff*f;
	tantoangle[i] = t;
    }
#endif
}


//
// R_ScaleFromGlobalAngle
// Returns the texture mapping scale
//  for the current line (horizontal span)
//  at the given angle.
// rw_distance must be calculated first.
//
fixed_t R_ScaleFromGlobalAngle (angle_t visangle)
{
    fixed_t		scale;
    int			anglea;
    int			angleb;
    int			sinea;
    int			sineb;
    fixed_t		num;
    int			den;

    // UNUSED
#if 0
{
    fixed_t		dist;
    fixed_t		z;
    fixed_t		sinv;
    fixed_t		cosv;
	
    sinv = finesine[(visangle-rw_normalangle)>>ANGLETOFINESHIFT];	
    dist = FixedDiv (rw_distance, sinv);
    cosv = finecosine[(viewangle-visangle)>>ANGLETOFINESHIFT];
    z = abs(FixedMul (dist, cosv));
    scale = FixedDiv(projection, z);
    return scale;
}
#endif

    anglea = ANG90 + (visangle-viewangle);
    angleb = ANG90 + (visangle-rw_normalangle);

    // both sines are allways positive
    sinea = finesine[anglea>>ANGLETOFINESHIFT];	
    sineb = finesine[angleb>>ANGLETOFINESHIFT];
    num = FixedMul(projection,sineb)<<detailshift;
    den = FixedMul(rw_distance,sinea);

    if (den > num>>16)
    {
	scale = FixedDiv (num, den);

    if (scale > 256 * FRACUNIT)
        scale = 256 * FRACUNIT;
    else if (scale < 1024)
        scale = 1024;
    }
    else
	scale = 256 * FRACUNIT;
	
    return scale;
}



//
// R_InitTables
//
void R_InitTables (void)
{
    // UNUSED: now getting from tables.c
#if 0
    int		i;
    float	a;
    float	fv;
    int		t;
    
    // viewangle tangent table
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	a = (i-FINEANGLES/4+0.5)*PI*2/FINEANGLES;
	fv = FRACUNIT*tan (a);
	t = fv;
	finetangent[i] = t;
    }
    
    // finesine table
    for (i=0 ; i<5*FINEANGLES/4 ; i++)
    {
	// OPTIMIZE: mirror...
	a = (i+0.5)*PI*2/FINEANGLES;
	t = FRACUNIT*sin (a);
	finesine[i] = t;
    }
#endif

}



//
// R_InitTextureMapping
//
void R_InitTextureMapping (void)
{
    int			i;
    int			x;
    int			t;
    fixed_t		focallength;
    
    // Use tangent table to generate viewangletox:
    //  viewangletox will give the next greatest x
    //  after the view angle.
    //
    // Calc focallength
    //  so FIELDOFVIEW angles covers SCREENWIDTH.
    focallength = FixedDiv (centerxfrac,
			    finetangent[FINEANGLES/4+FIELDOFVIEW/2] );
	
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	if (finetangent[i] > FRACUNIT*2)
	    t = -1;
	else if (finetangent[i] < -FRACUNIT*2)
	    t = viewwidth+1;
	else
	{
	    t = FixedMul (finetangent[i], focallength);
	    t = (centerxfrac - t+FRACUNIT-1)>>FRACBITS;

	    if (t < -1)
		t = -1;
	    else if (t>viewwidth+1)
		t = viewwidth+1;
	}
	viewangletox[i] = t;
    }
    
    // Scan viewangletox[] to generate xtoviewangle[]:
    //  xtoviewangle will give the smallest view angle
    //  that maps to x.	
    for (x=0;x<=viewwidth;x++)
    {
	i = 0;
	while (viewangletox[i]>x)
	    i++;
	xtoviewangle[x] = (i<<ANGLETOFINESHIFT)-ANG90;
    }
    
    // Take out the fencepost cases from viewangletox.
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	t = FixedMul (finetangent[i], focallength);
	t = centerx - t;
	
	if (viewangletox[i] == -1)
	    viewangletox[i] = 0;
	else if (viewangletox[i] == viewwidth+1)
	    viewangletox[i]  = viewwidth;
    }
	
    clipangle = xtoviewangle[0];
}



//
// R_InitLightTables
// Only inits the zlight table,
//  because the scalelight table changes with view size.
//
#define DISTMAP		2

void R_InitLightTables (void)
{
    int		i;
    int		j;
    int		level;
    int		startmap; 	
    int		scale;
    
    // Calculate the light levels to use
    //  for each level / distance combination.
    for (i=0 ; i< LIGHTLEVELS ; i++)
    {
	startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
	for (j=0 ; j<MAXLIGHTZ ; j++)
	{
	    scale = FixedDiv ((SCREENWIDTH/2*FRACUNIT), (j+1)<<LIGHTZSHIFT);
	    scale >>= LIGHTSCALESHIFT;
	    level = startmap - scale/DISTMAP;
	    
	    if (level < 0)
		level = 0;

	    if (level >= NUMCOLORMAPS)
		level = NUMCOLORMAPS-1;

	    zlight[i][j] = colormaps + level*256;
	}
    }
}



//
// R_SetViewSize
// Do not really change anything here,
//  because it might be in the middle of a refresh.
// The change will take effect next refresh.
//
dboolean		setsizeneeded;
int		setblocks;
int		setdetail;


void
R_SetViewSize
( int		blocks,
  int		detail )
{
    setsizeneeded = true;
    setblocks = blocks;
    setdetail = detail;
}


//
// R_ExecuteSetViewSize
//
void R_ExecuteSetViewSize (void)
   {
    fixed_t	cosadj;
    fixed_t	dy;
    int		i;
    int		j;
    int		level;
    int		startmap; 	

    setsizeneeded = false;

    if (setblocks == 11)
       {
        scaledviewwidth = SCREENWIDTH;
        viewheight = SCREENHEIGHT;
       }
    else
       {
        scaledviewwidth = (setblocks*SCREENWIDTH)/10;
        viewheight = ((setblocks*(SCREENHEIGHT-SBARHEIGHT))/10)&~7;
       }
    
    detailshift = setdetail;
    viewwidth = scaledviewwidth>>detailshift;
	
    centery = viewheight/2;
    centerx = viewwidth/2;
    centerxfrac = centerx<<FRACBITS;
    centeryfrac = centery<<FRACBITS;
    projection = centerxfrac;

    if (!detailshift)
    {
	colfunc = basecolfunc = R_DrawColumn;
	fuzzcolfunc = R_DrawFuzzColumn;
	transcolfunc = R_DrawTranslatedColumn;
	spanfunc = R_DrawSpan;
    }
    else
    {
	colfunc = basecolfunc = R_DrawColumnLow;
	fuzzcolfunc = R_DrawFuzzColumn;
	transcolfunc = R_DrawTranslatedColumn;
	spanfunc = R_DrawSpanLow;
    }

    R_InitBuffer (scaledviewwidth, viewheight);
	
    R_InitTextureMapping ();
    
    // psprite scales
//    pspritescale = FRACUNIT*viewwidth/SCREENWIDTH;
//    pspriteiscale = FRACUNIT*SCREENWIDTH/viewwidth;

    //pspritescale = FRACUNIT*viewwidth/320;
    //pspriteiscale = FRACUNIT*320/viewheight;
    if (setblocks != 10)
       {
        pspritescale = FRACUNIT*viewheight/200;
        pspriteiscale = FRACUNIT*200/viewheight;
       }
    else
       {
        pspritescale = FRACUNIT*(viewheight+32)/200;
        pspriteiscale = FRACUNIT*200/(viewheight+32);
       }
    
    // thing clipping
    for (i=0 ; i<viewwidth ; i++)
	screenheightarray[i] = viewheight;
    
    // planes
    for (i=0 ; i<viewheight ; i++)
    {
	dy = ((i-viewheight/2)<<FRACBITS)+FRACUNIT/2;
	dy = abs(dy);
	yslope[i] = FixedDiv ( (viewwidth<<detailshift)/2*FRACUNIT, dy);
    }
	
    for (i=0 ; i<viewwidth ; i++)
    {
	cosadj = abs(finecosine[xtoviewangle[i]>>ANGLETOFINESHIFT]);
	distscale[i] = FixedDiv (FRACUNIT,cosadj);
    }
    
    // Calculate the light levels to use
    //  for each level / scale combination.
    for (i=0 ; i< LIGHTLEVELS ; i++)
    {
	startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
	for (j=0 ; j<MAXLIGHTSCALE ; j++)
	{
	    level = startmap - j*SCREENWIDTH/(viewwidth<<detailshift)/DISTMAP;
	    
	    if (level < 0)
		level = 0;

	    if (level >= NUMCOLORMAPS)
		level = NUMCOLORMAPS-1;

	    scalelight[i][j] = colormaps + level*256;
	}
    }
    R_DrawViewBorder();
}



//
// R_Init
//
extern int	detailLevel;
extern int	screenblocks;



void R_Init (void)
   {
    R_InitData();
    R_InitPointToAngle();
    R_InitTables ();
    R_SetViewSize (screenblocks, detailLevel);
    R_InitPlanes ();
    R_InitLightTables ();
    R_InitSkyMap ();
    R_InitTranslationTables ();
    R_InitFuzzTable();
    framecount = 0;
   }


//
// R_PointInSubsector
//
subsector_t*
R_PointInSubsector
( fixed_t	x,
  fixed_t	y )
{
    node_t*	node;
    int		side;
    int		nodenum;

    // single subsector is a special case
    if (!numnodes)				
	return subsectors;
		
    nodenum = numnodes-1;

    while (! (nodenum & NF_SUBSECTOR) )
    {
	node = &nodes[nodenum];
	side = R_PointOnSide (x, y, node);
	nodenum = node->children[side];
    }
	
    return &subsectors[nodenum & ~NF_SUBSECTOR];
}


extern double glFov;

double     pfactor = (1.0/65536.0);
double     nfactor = (-1.0/65536.0);

//
// R_SetupFrame
//
void R_SetupFrame (player_t* player)
   {		
    int     i;
    
    viewplayer = player;
    viewx = player->mo->x;
    viewy = player->mo->y;
    viewangle = player->mo->angle + viewangleoffset;

    camera.x = (float)player->mo->x*(float)pfactor;
    camera.y = (float)player->mo->y*(float)nfactor;
    camera.z = (float)player->mo->z*(float)nfactor;

    camera.oy = (((float)(viewangle/63536.0f)/-16384.0f)*90.0f)+90.0f;
    if (camera.oy >= 360.0f)
        camera.oy -= 360.0f;
    else
    if (camera.oy < 0.0f)
        camera.oy += 360.0f;

    langle = camera.oy + (video.fov / 2.0f);
    if (langle >= 360.0f)
        langle -= 360.0f;
    rangle = camera.oy - (video.fov / 2.0f);
    if (rangle < 0.0f)
        rangle += 360.0f;
    if (langle < rangle)
       {
        langle += 360.0f;
       }

    extralight = player->extralight;

    viewz = player->viewz;
    
    viewsin = finesine[viewangle>>ANGLETOFINESHIFT];
    viewcos = finecosine[viewangle>>ANGLETOFINESHIFT];
	
    sscount = 0;
	
    if (player->fixedcolormap)
       {
        fixedcolormap = colormaps + player->fixedcolormap*256*sizeof(lighttable_t);
        walllights = scalelightfixed;
        for (i = 0; i < MAXLIGHTSCALE; i++)
           scalelightfixed[i] = fixedcolormap;
       }
    else
       fixedcolormap = 0;
	
    framecount++;
    validcount++;
   }

extern float SetBack, glTop, glLeft, glRight, glBottom, glAspect;
extern int  GL_SkyTexture[4], GL_SkyParts, GL_SkyTop;
extern dboolean RedBias, GreenBias, WhiteBias;


void GL_DrawSky(float compass)
{
    int   bquad, equad, tcomp;
    float lcomp, rcomp;
    float tseam, middle;

    float fSkyTop, fSkyBottom, fSkyMiddle, fSkyHalfHeight;
    float fSkyHeightScale, fDefaultAspect;

    fDefaultAspect  = 320.0f / 240.0f;
    fSkyHalfHeight  = (2.0f - 0.5f) * 0.5f;
    fSkyMiddle      = 0.5f + fSkyHalfHeight;
    fSkyHeightScale = fDefaultAspect / glAspect;
    fSkyTop         = fSkyMiddle + fSkyHalfHeight * fSkyHeightScale;
    fSkyBottom      = fSkyMiddle - fSkyHalfHeight * fSkyHeightScale;

    if (fSkyTop > 2.0f)
        fSkyTop = 2.0f;

    if (fSkyBottom < 0.5f)
        fSkyBottom = 0.5f;

    /*glPushMatrix();
    glColor3f( 1.0f, 1.0f, 1.0f );*/

    lcomp = compass - 45.0f;
    if (lcomp < 0.0f)
       lcomp += 360.0f;
    rcomp = compass + 44.0f;
    if (rcomp > 360.0f)
       rcomp -= 360.0f;

    bquad = 3 - ((int)lcomp / 90);
    equad = 3 - ((int)rcomp / 90);

    bquad++;
    bquad %= 4;
    equad++;
    equad %= 4;

    if (bquad == equad)
    {
        /*glBindTexture(GL_TEXTURE_2D, GL_SkyTexture[bquad]);
        glBegin( GL_QUADS );
           glTexCoord2f(1.0f, fSkyTop);
           glVertex3f(glLeft, glTop, SetBack);
           glTexCoord2f(1.0f, fSkyBottom);
           glVertex3f(glLeft, glBottom, SetBack);
           glTexCoord2f(0.0f, fSkyBottom);
           glVertex3f(glRight, glBottom, SetBack);
           glTexCoord2f(0.0f, fSkyTop);
           glVertex3f(glRight, glTop, SetBack);
        glEnd();*/
    }
    else
    {
        tcomp = (((int)lcomp / 90) * 90);
        tseam =  (lcomp - (float)tcomp) / 90.0f;
        middle =  glRight - ((glRight - glLeft) * tseam);
        tseam = 1.0f - tseam;

        /*glBindTexture(GL_TEXTURE_2D, GL_SkyTexture[bquad]);
        glBegin( GL_QUADS );
           glTexCoord2f(tseam, fSkyTop);
           glVertex3f(glLeft, glTop, SetBack);
           glTexCoord2f(tseam, fSkyBottom);
           glVertex3f(glLeft, glBottom, SetBack);
           glTexCoord2f(0.0f, fSkyBottom);
           glVertex3f(middle, glBottom, SetBack);
           glTexCoord2f(0.0f, fSkyTop);
           glVertex3f(middle, glTop, SetBack);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, GL_SkyTexture[equad]);
        glBegin( GL_QUADS );
           glTexCoord2f(1.0f, fSkyTop);
           glVertex3f(middle, glTop, SetBack);
           glTexCoord2f(1.0f, fSkyBottom);
           glVertex3f(middle, glBottom, SetBack);
           glTexCoord2f(tseam, fSkyBottom);
           glVertex3f(glRight, glBottom, SetBack);
           glTexCoord2f( tseam, fSkyTop);
           glVertex3f(glRight, glTop ,SetBack);
        glEnd();*/
    }

    //glPopMatrix();
}


extern DW_TexList      TexList[1024];

extern sector_plane_t *planes;
extern int             translate[1024];
extern int             ftranslate[1024];
extern DW_Polygon     *PolyList;
extern DW_FloorCeil   *FloorList, *CeilList;
extern drawside_t     *DrawSide;
extern dbyte       *DrawFlat;

extern dboolean RedBias, GreenBias, WhiteBias;

void GL_DrawThings(void);

extern int   gl_fog;
extern int   gl_modeltest;

float fYAngle = 0.0f, fXAngle = 0.0f;
float foglight = 0.75f;

void FixWallCoords(DW_Polygon *TempPoly)
   {
    float         fHigh, fVertOff, GLYOff, UnPegOff;
    float         GLHigh, DHigh, YRatio;
    //int           p;

    GLHigh = (float)TexList[TempPoly->Texture[0]].GLHigh;
    DHigh  = (float)TexList[TempPoly->Texture[0]].DHigh;
    YRatio = DHigh / GLHigh;
    // tv = vertical texture coordinate
    fHigh = (TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1]);
    GLYOff = (GLHigh - DHigh) / GLHigh;
    if (((sides[TempPoly->SideDef].rowoffset >> FRACBITS) == 0)||(DHigh == 0.0f))
       {
        fVertOff = 0.0f;
       }
    else
       {
        fVertOff = ((float)(sides[TempPoly->SideDef].rowoffset >> FRACBITS) / DHigh)*-1.0f;
       }
    while (fVertOff < 0.0f)
       {
        fVertOff += 1.0f;
       }
    switch(TempPoly->Position)
       { // line_t
        case DW_UPPER:
             if ((lines[TempPoly->LineDef].flags & DW_UPUNPEG) == DW_UPUNPEG)
                 TempPoly->Point[0].tv = 1.0f + fVertOff;
             else
                 TempPoly->Point[0].tv = (((fHigh / DHigh) + fVertOff) * YRatio) + GLYOff;
             break;
        case DW_MIDDLE:
             if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                 TempPoly->Point[0].tv = (fHigh/DHigh) + fVertOff;
             else
                 TempPoly->Point[0].tv = 1.0f + fVertOff;
             break;
        case DW_LOWER:
             if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                {
                 UnPegOff = (float)((sectors[sides[TempPoly->SideDef].sectornumb].ceilingheight >> FRACBITS)- TempPoly->Point[0].v[1])/GLHigh;
                 while (UnPegOff > 1.0f)
                    {
                     UnPegOff -= 1.0f;
                    }
                 UnPegOff = 1.0f - UnPegOff;
                 TempPoly->Point[0].tv = UnPegOff;
                }
             else
                TempPoly->Point[0].tv = 1.0f + fVertOff;
             break;
       }

    switch(TempPoly->Position)
       {
        case DW_UPPER:
             if ((lines[TempPoly->LineDef].flags & DW_UPUNPEG) == DW_UPUNPEG)
                 TempPoly->Point[1].tv = (1.0f - (fHigh / GLHigh)) + fVertOff;
             else
                 TempPoly->Point[1].tv = 0.0f + fVertOff + GLYOff;
             break;
        case DW_MIDDLE:
             if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                 TempPoly->Point[1].tv = ((GLHigh-DHigh)/GLHigh) + fVertOff;
             else
                 TempPoly->Point[1].tv = (1.0f - (fHigh / GLHigh)) + fVertOff;
             break;
        case DW_LOWER:
             if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) == DW_LWUNPEG)
                {
                 UnPegOff = TempPoly->Point[0].tv - ((TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1])/GLHigh);
                 TempPoly->Point[1].tv = UnPegOff;
                }
             else
                TempPoly->Point[1].tv = (1.0f - (fHigh / GLHigh)) + fVertOff;
       }

    TempPoly->Point[2].tv = TempPoly->Point[1].tv;
    TempPoly->Point[3].tv = TempPoly->Point[0].tv;
   }


void FixWallHorzCoords(DW_Polygon *TempPoly)
   {
    float         fLength, fHorzOff, GLXOff;

    // tu = horizontal texture coordinate
    // tv = vertical texture coordinate
    fLength = (float)sqrt(((TempPoly->Point[0].v[0] - TempPoly->Point[3].v[0])*(TempPoly->Point[0].v[0] - TempPoly->Point[3].v[0]))+
                          ((TempPoly->Point[0].v[2] - TempPoly->Point[3].v[2])*(TempPoly->Point[0].v[2] - TempPoly->Point[3].v[2])));
    GLXOff = (float)(TexList[TempPoly->Texture[0]].GLWide - TexList[TempPoly->Texture[0]].DWide) / fLength;
    if (((sides[TempPoly->SideDef].textureoffset >> FRACBITS) == 0)||(TexList[TempPoly->Texture[0]].DWide == 0))
       fHorzOff = 0.0f;
    else
       {
        fHorzOff = (float)((sides[TempPoly->SideDef].textureoffset >> FRACBITS) * (1.0f/(float)TexList[TempPoly->Texture[0]].DWide));
       }
    while (fHorzOff < 0.0f)
       fHorzOff += 1.0f;
    TempPoly->Point[0].tu = 0.0f + fHorzOff;
    TempPoly->Point[1].tu = TempPoly->Point[0].tu;
    TempPoly->Point[2].tu = (fLength / (float)TexList[TempPoly->Texture[0]].DWide) + fHorzOff;
    TempPoly->Point[3].tu = TempPoly->Point[2].tu;
   }

extern GLTexData AmmoBoxSkin;
double RFactor = (360.0 * .0002);

void DrawAmmoBox(void)
   {
    double       now;
    float        rotation;

    /*glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, AmmoBoxSkin.TexName);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);*/
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    /*glPushMatrix();

    glTranslatef( 1056.0f, -16.0f, 3232.0f );*/
    now = (double)(GetTicks() % 5000)*RFactor;
    rotation = (float)now;
    //glRotatef(rotation, 0.0f, 1.0f, 0.0f );

    // Ammo box front
    /*glBegin(GL_QUADS);
       glTexCoord2f(0.21875f, 0.5625f );
       glVertex3f(  14.0f, 14.0f, -7.0f);
       glTexCoord2f(0.21875f, 0.359375f );
       glVertex3f(  14.0f,  0.0f, -7.0f);
       glTexCoord2f(0.65625f, 0.359375f );
       glVertex3f( -14.0f,  0.0f, -7.0f);
       glTexCoord2f(0.65625f, 0.5625f );
       glVertex3f( -14.0f, 14.0f, -7.0f);
    glEnd();*/

    // Ammo box top
    /*glBegin(GL_QUADS);
       glTexCoord2f(0.22222f, 0.78125f );
       glVertex3f(  14.0f, 14.0f,  7.0f);
       glTexCoord2f(0.22222f, 0.578125f );
       glVertex3f(  14.0f, 14.0f, -7.0f);
       glTexCoord2f(0.65079f, 0.578125f );
       glVertex3f( -14.0f, 14.0f, -7.0f);
       glTexCoord2f(0.65079f, 0.78125f );
       glVertex3f( -14.0f, 14.0f,  7.0f);
    glEnd();*/

    // Ammo box left side
    /*glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 0.5546875f );
       glVertex3f(  14.0f, 14.0f,  7.0f);
       glTexCoord2f(0.0f, 0.359375f );
       glVertex3f(  14.0f,  0.0f,  7.0f);
       glTexCoord2f(0.21875f, 0.359375f );
       glVertex3f(  14.0f,  0.0f, -7.0f);
       glTexCoord2f(0.21875f, 0.5546875f );
       glVertex3f(  14.0f, 14.0f, -7.0f);
    glEnd();*/

    // Ammo box right side
    /*glBegin(GL_QUADS);
       glTexCoord2f(0.65625f, 0.5546875f );
       glVertex3f( -14.0f, 14.0f, -7.0f);
       glTexCoord2f(0.65625f, 0.359375f );
       glVertex3f( -14.0f,  0.0f, -7.0f);
       glTexCoord2f(0.859375f, 0.359375f );
       glVertex3f( -14.0f,  0.0f,  7.0f);
       glTexCoord2f(0.859375f, 0.5546875f );
       glVertex3f( -14.0f, 14.0f,  7.0f);
    glEnd();*/

    // Ammo box back
    /*glBegin(GL_QUADS);
       glTexCoord2f(0.22222f, 0.796875f );
       glVertex3f( -14.0f, 14.0f,  7.0f);
       glTexCoord2f(0.22222f, 1.0f );
       glVertex3f( -14.0f,  0.0f,  7.0f);
       glTexCoord2f(0.65079f, 1.0f );
       glVertex3f(  14.0f,  0.0f,  7.0f);
       glTexCoord2f(0.65079f, 0.796875f );
       glVertex3f(  14.0f, 14.0f,  7.0f);
    glEnd();

    glPopMatrix();*/
   }

extern int       gl_poffsetf, gl_poffsetu;

void             GL_DrawPlayerSprites(void);
extern int       WhiteLightMap, WhiteColorMap;

double           div255 = ( 1.0 / 255.0 );

ml_vec3_t         viewmat[3];

#define NUM_FRUSTUM_PLANES  4
#define CLIP_PLANE_EPSILON  0.0001

typedef struct
   {
    double  v[3];
   }point_t;

typedef struct
   {
    double    distance;
    ml_vec3_t normal;
   }plane_t;

ml_vec3_t vpn, vright, vup;
plane_t   fixviewplanes[NUM_FRUSTUM_PLANES];
plane_t   frustumplanes[NUM_FRUSTUM_PLANES];

// Externals for Level Polygon data
extern ml_vec3_t *Normal;

/////////////////////////////////////////////////////////////////////
// Concatenate two 3x3 matrices.
/////////////////////////////////////////////////////////////////////
void MConcat(double in1[3][3], double in2[3][3], double out[3][3])
   {

    out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
    out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
    out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];

    out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
    out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
    out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];

    out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
    out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
    out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
   }

/////////////////////////////////////////////////////////////////////
// Move the view position and set the world->view transform.
/////////////////////////////////////////////////////////////////////
void UpdateViewAngles(ml_vec3_t angles)
   {
    ml_vec3_t mtemp[3], mroll[3], mpitch[3], myaw[3];
    double    s, c, roll, pitch, yaw;
   
    pitch = angles[0];
    yaw = angles[1];
    roll = angles[2];

    // Set up the world-to-view rotation.
    // Note: much of the work done in concatenating these matrices
    // can be factored out, since it contributes nothing to the
    // final result; multiply the three matrices together on paper
    // to generate a minimum equation for each of the 9 final elements
    s = sin(pitch);
    c = cos(pitch);
    mpitch[1][1] = c;
    mpitch[1][2] = s;
    mpitch[2][1] = -s;
    mpitch[2][2] = c;
    
    s = sin(yaw);
    c = cos(yaw);
    myaw[0][0] = c;
    myaw[0][2] = -s;
    myaw[2][0] = s;
    myaw[2][2] = c;

    s = sin(roll);
    c = cos(roll);
    mroll[0][0] = c;
    mroll[0][1] = s;
    mroll[1][0] = -s;
    mroll[1][1] = c;
    
//  concat two rotations

    mtemp[0][0] = myaw[0][0];
    mtemp[0][2] = myaw[0][2];

    mtemp[1][0] = mpitch[1][2] * myaw[2][0];
    mtemp[1][1] = mpitch[1][1] * myaw[1][1];
    mtemp[1][2] = mpitch[1][2] * myaw[2][2];

    // set the 'Z' axis here - we're done with it
    vpn[0] = mpitch[2][2] * myaw[2][0];
    vpn[1] = mpitch[2][1] * myaw[1][1];
    vpn[2] = mpitch[2][2] * myaw[2][2];

//  concat third rotation - roll doesn't affect the 'Z' axis

    vright[0] = mroll[0][0] * mtemp[0][0] + mroll[0][1] * mtemp[1][0];
    vright[1] = mroll[0][1] * mtemp[1][1];
    vright[2] = mroll[0][0] * mtemp[0][2] + mroll[0][1] * mtemp[1][2];

    vup[0] = mroll[1][0] * mtemp[0][0] + mroll[1][1] * mtemp[1][0];
    vup[1] = mroll[1][1] * mtemp[1][1];
    vup[2] = mroll[1][0] * mtemp[0][2] + mroll[1][1] * mtemp[1][2];
   }

/////////////////////////////////////////////////////////////////////
// Rotate a vector from viewspace to worldspace.
/////////////////////////////////////////////////////////////////////
void BackRotateVector(ml_vec3_t pin, ml_vec3_t pout)
   {
    pout[0] = pin[0] * vright[0] + pin[1] * vup[0] + pin[2] * vpn[0];
    pout[1] = pin[0] * vright[1] + pin[1] * vup[1] + pin[2] * vpn[1];
    pout[2] = pin[0] * vright[2] + pin[1] * vup[2] + pin[2] * vpn[2];
   }

/////////////////////////////////////////////////////////////////////
// Set up a clip plane with the specified normal.
/////////////////////////////////////////////////////////////////////
void SetWorldspaceClipPlane(ml_vec3_t currentpos, ml_vec3_t normal, plane_t *plane)
   {

    // Rotate the plane normal into worldspace
    BackRotateVector(normal, plane->normal);

    plane->distance = ml_DotProduct(currentpos, plane->normal) + CLIP_PLANE_EPSILON;
   }

// 
void R_InitViewData()
   {
    int       i;
    float     halfwidth, halfheight;
    double    angle, s, c;
    ml_vec3_t normal;

    video.nearclip = 1.5f;
    video.farclip = 5000.0f;

    // calculate this only on view size/fov changes
    video.hFov = (float)tan(DEG2RAD(video.fov / 2.0f)) * 2.0f;
    video.vFov = video.hFov * ((float)video.height / (float)video.width);

    halfwidth = video.nearclip * video.hFov;
    halfheight = video.nearclip * video.vFov;

    video.nearplane[0][0] = -halfwidth;
    video.nearplane[0][1] = halfheight;
    video.nearplane[0][2] = video.nearclip;
        
    video.nearplane[1][0] = -halfwidth;
    video.nearplane[1][1] = -halfheight;
    video.nearplane[1][2] = video.nearclip;
        
    video.nearplane[2][0] = halfwidth;
    video.nearplane[2][1] = -halfheight;
    video.nearplane[2][2] = video.nearclip;
        
    video.nearplane[3][0] = halfwidth;
    video.nearplane[3][1] = halfheight;
    video.nearplane[3][2] = video.nearclip;
        
    halfwidth = video.farclip * video.hFov;
    halfheight = video.farclip * video.vFov;

    video.farplane[0][0] = -halfwidth;
    video.farplane[0][1] = halfheight;
    video.farplane[0][2] = video.farclip;
        
    video.farplane[1][0] = -halfwidth;
    video.farplane[1][1] = -halfheight;
    video.farplane[1][2] = video.farclip;
        
    video.farplane[2][0] = halfwidth;
    video.farplane[2][1] = -halfheight;
    video.farplane[2][2] = video.farclip;
        
    video.farplane[3][0] = halfwidth;
    video.farplane[3][1] = halfheight;
    video.farplane[3][2] = video.farclip;
        
    // top side clip
	video.frustum[0][0] = 0;
	video.frustum[0][1] = -1.0 / (0.5*video.vFov);
	video.frustum[0][2] = 1;
	
    // right side clip
	video.frustum[1][0] = -1.0 / (0.5*video.hFov);
	video.frustum[1][1] = 0;
	video.frustum[1][2] = 1;
	
    // bottom side clip
	video.frustum[2][0] = 0;
	video.frustum[2][1] = 1.0 / ((0.5)*video.vFov);
	video.frustum[2][2] = 1;	
	
    // left side clip
	video.frustum[3][0] = 1.0 / ((0.5)*video.hFov);
	video.frustum[3][1] = 0;
	video.frustum[3][2] = 1;
	
    video.frustum[4][0] = 0.0;
    video.frustum[4][1] = 0.0;
    video.frustum[4][2] = 1.0;

	for (i = 0; i < 4; i++)
       {
        // frustum[4] is already a "unit" vector
        ml_VectorNormalise(video.frustum[i]);
       }

    //angle = atan(2.0 / fieldofview * maxscale / xscreenscale);
    angle = DEG2RAD(video.fov / 2.0f);

    s = sin(angle);
    c = cos(angle);

    // Left clip plane
    normal[0] = s;
    normal[1] = 0;
    normal[2] = c;
    // Rotate the plane normal into worldspace
    BackRotateVector(normal, fixviewplanes[LCLIP].normal);

    // Right clip plane
    normal[0] = -s;
    // Rotate the plane normal into worldspace
    BackRotateVector(normal, fixviewplanes[RCLIP].normal);

    //angle = atan(2.0 / fieldofview * maxscale / yscreenscale);
    angle = DEG2RAD(video.fovy / 2.0f);
    s = sin(angle);
    c = cos(angle);

    // Bottom clip plane
    normal[0] = 0;
    normal[1] = s;
    normal[2] = c;
    // Rotate the plane normal into worldspace
    BackRotateVector(normal, fixviewplanes[BCLIP].normal);

    // Top clip plane
    normal[1] = -s;
    // Rotate the plane normal into worldspace
    BackRotateVector(normal, fixviewplanes[TCLIP].normal);
   }

/////////////////////////////////////////////////////////////////////
// Set up the planes of the frustum, in worldspace coordinates.
/////////////////////////////////////////////////////////////////////
void R_AlignFrustum(ml_vert3_t position, ml_vec3_t orient)
   {
    UpdateViewAngles(orient);

    SetWorldspaceClipPlane(position, fixviewplanes[LCLIP].normal, &frustumplanes[LCLIP]);

    SetWorldspaceClipPlane(position, fixviewplanes[RCLIP].normal, &frustumplanes[RCLIP]);

    SetWorldspaceClipPlane(position, fixviewplanes[BCLIP].normal, &frustumplanes[BCLIP]);

    SetWorldspaceClipPlane(position, fixviewplanes[TCLIP].normal, &frustumplanes[TCLIP]);

    video.origin[0] = position[0]*-1.0;
    video.origin[1] = position[1]*-1.0;
    video.origin[2] = position[2]*-1.0;
   }

/*
/////////////////////////////////////////////////////////////////////
// Clip a polygon to a plane.
/////////////////////////////////////////////////////////////////////
int ClipToPlane(polygon_t *pin, plane_t *pplane, polygon_t *pout)
   {
    int     i, j, nextvert, curin, nextin;
    double  curdot, nextdot, scale;
    point_t *pinvert, *poutvert;

    pinvert = pin->verts;
    poutvert = pout->verts;

    curdot = ml_DotProduct(pinvert, &pplane->normal);
    curin = (curdot >= pplane->distance);

    for (i = 0; i < pin->numverts; i++)
       {
        nextvert = (i + 1) % pin->numverts;

        // Keep the current vertex if it's inside the plane
        if (curin)
            *poutvert++ = *pinvert;

        nextdot = ml_DotProduct(&pin->verts[nextvert], &pplane->normal);
        nextin = (nextdot >= pplane->distance);

        // Add a clipped vertex if one end of the current edge is
        // inside the plane and the other is outside
        if (curin != nextin)
           {
            scale = (pplane->distance - curdot) / (nextdot - curdot);
            for (j=0; j<3; j++)
               {
                poutvert->v[j] = pinvert->v[j] +
                    ((pin->verts[nextvert].v[j] - pinvert->v[j]) *
                     scale);
               }
            poutvert++;
           }

        curdot = nextdot;
        curin = nextin;
        pinvert++;
       }

    pout->numverts = poutvert - pout->verts;
    if (pout->numverts < 3)
        return 0;

    pout->color = pin->color;
    return 1;
   }

/////////////////////////////////////////////////////////////////////
// Clip a polygon to the frustum.
/////////////////////////////////////////////////////////////////////
int ClipToFrustum(TempPoly *pin, polygon_t *pout)
   {
    int         i, curpoly;
    polygon_t   tpoly[2], *ppoly;

    curpoly = 0;
    ppoly = pin;

    for (i = 0; i < (NUM_FRUSTUM_PLANES-1); i++)
       {
        if (!ClipToPlane(ppoly, &frustumplanes[i], &tpoly[curpoly]))
           {
            return 0;
           }
        ppoly = &tpoly[curpoly];
        curpoly ^= 1;
       }

    return ClipToPlane(ppoly, &frustumplanes[NUM_FRUSTUM_PLANES-1], pout);
   }
*/
dboolean R_ClipVertsToFrustum(DW_Polygon *TempPoly)
   {

    return true;
   }

// recalculates wall ceiling height, floor height
// and UV coords for moving walls
void R_BuildRenderQueue()
{
    int           i, wall;
    dboolean       inside;
    DW_Polygon   *TempPoly;
    float         wallhigh, newhigh;
    float         fh, ch;

    for (i = 0; i < numsides; i++)
        if (DrawSide[i] == ds_unknown)
            DrawSide[i] = ds_nodraw;

    // a common vertex pool would be quicker
    // for culling

    for (wall = 0; wall < sorted_walls_count; wall++)
    {
        TempPoly = sorted_walls[wall];
        while (TempPoly != NULL)
        {
            // Check left and right sides for inside the frustum
            //inside = (DrawSide[TempPoly->SideDef] == ds_draw);

            // this functions will alaways return true.. 
            //(this could be used to cull all non-visible walls) 
            inside = R_ClipVertsToFrustum(TempPoly);

            if (inside && DrawSide[TempPoly->SideDef] == ds_draw)
            {
                wallhigh = TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1];
                switch (TempPoly->Position)
                {
                case DW_LOWER:
                    ch = (float)sectors[TempPoly->BackSector].floorheight * (float)pfactor;
                    TempPoly->Point[0].v[1] = ch;
                    TempPoly->Point[3].v[1] = ch;
                    fh = (float)sectors[TempPoly->Sector].floorheight * (float)pfactor;
                    TempPoly->Point[1].v[1] = fh;
                    TempPoly->Point[2].v[1] = fh;
                    break;
                case DW_MIDDLE:
                    if (TempPoly->BackSector != -1)
                    {
                        if (sectors[TempPoly->Sector].ceilingheight > sectors[TempPoly->BackSector].ceilingheight)
                            ch = (float)sectors[TempPoly->BackSector].ceilingheight * (float)pfactor;
                        else
                            ch = (float)sectors[TempPoly->Sector].ceilingheight * (float)pfactor;

                        if ((sectors[TempPoly->Sector].floorheight > sectors[TempPoly->BackSector].floorheight) ||
                            (sides[TempPoly->SideDef].bottomtexture == 0))
                            fh = (float)sectors[TempPoly->Sector].floorheight * (float)pfactor;
                        else
                            fh = (float)sectors[TempPoly->BackSector].floorheight * (float)pfactor;
                    }
                    else
                    {
                        ch = (float)sectors[TempPoly->Sector].ceilingheight * (float)pfactor;
                        fh = (float)sectors[TempPoly->Sector].floorheight * (float)pfactor;
                    }
                    if (TexList[TempPoly->Texture[0]].Transparent == true)
                    {
                        if (TexList[TempPoly->Texture[0]].DHigh < (ch - fh))
                        {
                            if ((lines[TempPoly->LineDef].flags & DW_LWUNPEG) != DW_LWUNPEG)
                                fh = ch - TexList[TempPoly->Texture[0]].DHigh;
                            else
                                ch = fh + TexList[TempPoly->Texture[0]].DHigh;

                        }
                    }
                    TempPoly->Point[0].v[1] = ch;
                    TempPoly->Point[3].v[1] = ch;
                    TempPoly->Point[1].v[1] = fh;
                    TempPoly->Point[2].v[1] = fh;
                    break;
                case DW_UPPER:
                    ch = (float)sectors[TempPoly->Sector].ceilingheight * (float)pfactor;
                    TempPoly->Point[0].v[1] = ch;
                    TempPoly->Point[3].v[1] = ch;
                    fh = (float)sectors[TempPoly->BackSector].ceilingheight * (float)pfactor;
                    TempPoly->Point[1].v[1] = fh;
                    TempPoly->Point[2].v[1] = fh;
                    break;
                default:
                    TempPoly->Point[1].v[1] = TempPoly->Point[2].v[1] = (float)sectors[TempPoly->BackSector].ceilingheight * (float)pfactor;
                    break;
                }
                newhigh = TempPoly->Point[0].v[1] - TempPoly->Point[1].v[1];
            }

            //inside = R_IsVisibleVertical(TempPoly);

            if (inside && TempPoly->Position)
            {
                // calculate new vertical texture coordinates for vertical "flats" motion
                if (wallhigh != newhigh)
                {
                    FixWallCoords(TempPoly);
                }
                // handle scrolling horizontal textures
                if (TempPoly->coloff != sides[TempPoly->SideDef].textureoffset)
                {
                    FixWallHorzCoords(TempPoly);
                    TempPoly->coloff = sides[TempPoly->SideDef].textureoffset;
                }
            }

            if (!inside)
                DrawSide[TempPoly->SideDef] = ds_nodraw;

            TempPoly = TempPoly->Next;
        }
    }
 }

void GL_RenderPlayerView(player_t* player)
   {
    int           i, texnumb, sector, subsector, wall, flat;
    float        fview;
    float         yangle, lightv;
    float         flathigh;//, radius = 128;
    float         fogcolor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    double        offsetf, offsetu;
    //DW_Vertex3D   PlayerPosition;
    static ml_vert3_t    ViewPosition;
    static ml_vec3_t     ViewOrient;
    static vec3                 pos;
    static vec3                 dir;
    DW_Polygon   *TempPoly;
    static dboolean   FirstTime = true;

    DW_FloorCeil*   psubsector;
    sector_t*       psector;
    sector_plane_t* pplane;

    //glPushMatrix();

    yangle = (float)(viewangle >> ANGLETOFINESHIFT) * 360.0f / FINEANGLES - 90.0f;
    if (yangle >= 180.0f)
        yangle -=360.0f;
    else
    if (yangle < -180.0f)
        yangle += 360.0f;
    fYAngle = yangle;

    ViewOrient[0] = fXAngle;
    ViewOrient[1] = fYAngle;
    ViewOrient[2] = 0.0f;

    ViewPosition[0] = FIXED_TO_FLOAT(player->mo->x);
    ViewPosition[1] = FIXED_TO_FLOAT(player->viewz);
    ViewPosition[2] = -FIXED_TO_FLOAT(player->mo->y);

    R_AlignFrustum(ViewPosition, ViewOrient);

    pos[0] = FIXED_TO_FLOAT(player->mo->x);
    pos[1] = FIXED_TO_FLOAT(player->viewz);
    pos[2] = -FIXED_TO_FLOAT(player->mo->y);

    dir[0] = ViewOrient[0];
    dir[1] = ViewOrient[1];
    dir[2] = ViewOrient[2];

    R3D_UpdateCamera(pos, dir);

    R_BuildRenderQueue();

    //glEnable(GL_TEXTURE_2D);

    //glRotatef(fXAngle, 1.0f, 0.0f, 0.0f );

    GL_DrawSky(yangle);

    //glRotatef(yangle, 0.0f, 1.0f, 0.0f );

    /*glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);*/

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc( GL_LEQUAL );

    if (gl_fog == 1)
       {
        /*glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 128.0f);
        glFogf(GL_FOG_END, 2048.0f);
        glFogfv(GL_FOG_COLOR, fogcolor);*/
       }

    //glTranslatef((GLfloat)ViewPosition[0], (GLfloat)ViewPosition[1], (GLfloat)ViewPosition[2]);

    for (wall = 0; wall < sorted_walls_count; wall++)
    {
        TempPoly = sorted_walls[wall];
        while (TempPoly != NULL)
        {
            // test if wall is in visibility list
            if (DrawSide[TempPoly->SideDef] == ds_draw)
            {
                if (TempPoly->Position)
                {
                    lightv = sectors[TempPoly->LSector].lightlevel;
                    lightv /= 255.0f;
                    if (WhiteBias == true)
                        lightv = 1.0f;

                    //glColor4f(lightv, lightv, lightv, 1.0f);
                    //if ((lightv >= foglight) && (gl_fog == 1))
                        //glDisable(GL_FOG);

                    switch (TempPoly->Position)
                    {
                    case DW_LOWER:
                        texnumb = sides[TempPoly->SideDef].bottomtexture;
                        break;
                    case DW_MIDDLE:
                        texnumb = sides[TempPoly->SideDef].midtexture;
                        break;
                    case DW_UPPER:
                        texnumb = sides[TempPoly->SideDef].toptexture;
                        break;
                    }


                    //glBindTexture(GL_TEXTURE_2D, TexList[translate[texturetranslation[texnumb]]].glTexture);

                    if (TexList[TempPoly->Texture[0]].Transparent == true)
                    {
                        /*glEnable(GL_ALPHA_TEST);
                        glAlphaFunc(GL_GREATER, 0.0f);*/
                    }

                    //R3D_SetMaterial(&TexList[translate[texturetranslation[texnumb]]].glTexture);
                    
                    R3D_RenderWall(TempPoly, &TexList[translate[texturetranslation[texnumb]]].glTexture, lightv);

                    /*glBegin(GL_QUADS);
                    glTexCoord2f(TempPoly->Point[0].tu, TempPoly->Point[0].tv);
                    glVertex3fv(TempPoly->Point[0].v);
                    glTexCoord2f(TempPoly->Point[1].tu, TempPoly->Point[1].tv);
                    glVertex3fv(TempPoly->Point[1].v);
                    glTexCoord2f(TempPoly->Point[2].tu, TempPoly->Point[2].tv);
                    glVertex3fv(TempPoly->Point[2].v);
                    glTexCoord2f(TempPoly->Point[3].tu, TempPoly->Point[3].tv);
                    glVertex3fv(TempPoly->Point[3].v);
                    glEnd();*/

                    //if (TexList[TempPoly->Texture[0]].Transparent == true)
                        //glDisable(GL_ALPHA_TEST);


                    //if ((lightv >= foglight) && (gl_fog == 1))
                        //glEnable(GL_FOG);

                }
                else
                {
                    //if (gl_fog == 1)
                        //glDisable(GL_FOG);

                    /*glDisable(GL_TEXTURE_2D);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ZERO, GL_ONE);*/

                    /*glColor4f(0.0f, 0.0, 0.0, 0.0f);

                    glBegin(GL_QUADS);
                    glVertex3fv(TempPoly->Point[0].v);
                    glVertex3fv(TempPoly->Point[1].v);
                    glVertex3fv(TempPoly->Point[2].v);
                    glVertex3fv(TempPoly->Point[3].v);
                    glEnd();

                    glEnable(GL_TEXTURE_2D);
                    glDisable(GL_BLEND);*/


                    //if (gl_fog == 1)
                        //glEnable(GL_FOG);
                }

            }
            TempPoly = TempPoly->Next;
        }
    }

    /*glDisable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );*/

    //glDisable(GL_CULL_FACE);

    offsetf = (double)gl_poffsetf * div255;
    offsetu = (double)gl_poffsetu * div255;

    for(flat = 0; flat < sorted_flats_count; flat++)
    {
        pplane  = sorted_flats[flat];
        sector  = pplane - planes;
        psector = sectors + sector;

        if (psector->floorheight != psector->ceilingheight)
        {
            //glPolygonOffset((float)offsetf,(float)offsetu);

            if (player->viewz > psector->floorheight)
            {
                flathigh  = FIXED_TO_FLOAT(psector->floorheight);
                lightv    = psector->lightlevel;
                lightv   /= 255.0f;

                /*if ((lightv >= foglight)  && (gl_fog == 1))
                    glDisable(GL_FOG);*/

                /*if (WhiteBias == true)
                    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
                else
                    glColor4f( lightv, lightv, lightv, 1.0f );*/
       
                //glBindTexture(GL_TEXTURE_2D, TexList[ftranslate[flattranslation[psector->floorpic]]].glTexture);

                for (subsector = 0; subsector < pplane->ss_count; subsector++)
                {
                    psubsector = pplane->subsectors[subsector];

                    if (psector->floorpic == skyflatnum)
                        continue;

                    if ((gl_poffsetf != 0) && (gl_poffsetu != 0))
                        //glEnable(GL_POLYGON_OFFSET_FILL);
        
                    //glBegin(GL_POLYGON);
                    for (i = 0; i < psubsector->PCount; i++)
                    {
                        //glTexCoord2f(psubsector->Point[i].tu, psubsector->Point[i].tv);
                        psubsector->Point[i].v[1] = flathigh;
                        //glVertex3fv(psubsector->Point[i].v);
                    }

                    R3D_RenderFloor(psubsector, &TexList[ftranslate[flattranslation[psector->floorpic]]].glTexture, lightv);

                    //glEnd();

                    //if ((gl_poffsetf != 0) && (gl_poffsetu != 0))
                    //{
                    //    /*glDisable(GL_POLYGON_OFFSET_FILL);
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    //    glBegin(GL_POLYGON);*/

                    //    /*glDisable(GL_POLYGON_OFFSET_FILL);
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/

                    //    for (i = 0; i < psubsector->PCount; i++)
                    //    {
                    //        //glTexCoord2f(psubsector->Point[i].tu, psubsector->Point[i].tv);
                    //        psubsector->Point[i].v[1] = flathigh;
                    //        //glVertex3fv(psubsector->Point[i].v);
                    //    }

                    //    R3D_RenderFlat(psubsector, &TexList[ftranslate[flattranslation[psector->floorpic]]].glTexture);

                    //    /*glEnd();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
                    //    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //}
                }

                /*if ((lightv >= foglight) && (gl_fog == 1))
                    glEnable(GL_FOG);*/
            }

            if (player->viewz < psector->ceilingheight)
            {
                flathigh  = FIXED_TO_FLOAT(psector->ceilingheight);
                lightv    = psector->lightlevel;
                lightv   /= 255.0f;

                /*if ((lightv >= foglight) && (gl_fog == 1))
                    glDisable(GL_FOG);

                if (WhiteBias == true)
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                else
                    glColor4f(lightv, lightv, lightv, 1.0f);
           
                glBindTexture(GL_TEXTURE_2D, TexList[ftranslate[flattranslation[psector->ceilingpic]]].glTexture);*/
                for (subsector = 0; subsector < pplane->ss_count; subsector++)
                {
                    psubsector = pplane->subsectors[subsector];

                    if (psector->ceilingpic == skyflatnum)
                        continue;

                    if ((gl_poffsetf != 0) && (gl_poffsetu != 0))
                        //glEnable(GL_POLYGON_OFFSET_FILL);

                    //glBegin(GL_POLYGON);
                    for (i = 0; i < psubsector->PCount; i++)
                    {
                        //glTexCoord2f(psubsector->Point[i].tu, psubsector->Point[i].tv);
                        psubsector->Point[i].v[1] = flathigh;
                        //glVertex3fv(psubsector->Point[i].v);
                    }

                    R3D_RenderCeil(psubsector, &TexList[ftranslate[flattranslation[psector->ceilingpic]]].glTexture, lightv);

                    //glEnd();

                    //if ((gl_poffsetf != 0) && (gl_poffsetu != 0))
                    //{
                    //    /*glDisable(GL_POLYGON_OFFSET_FILL);
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    //    glBegin(GL_POLYGON);*/
                    //    /*glDisable(GL_POLYGON_OFFSET_FILL);
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
                    //    for (i = 0; i < psubsector->PCount; i++)
                    //    {
                    //        //glTexCoord2f(psubsector->Point[i].tu, psubsector->Point[i].tv);
                    //        psubsector->Point[i].v[1] = flathigh;
                    //        //glVertex3fv(psubsector->Point[i].v);
                    //    }

                    //    R3D_RenderFlat(psubsector, &TexList[ftranslate[flattranslation[psector->ceilingpic]]].glTexture);

                    //    /*glEnd();
                    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
                    //    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    //}
                }

                /*if ((lightv >= foglight) && (gl_fog == 1))
                    glEnable(GL_FOG);*/
 
            }
        }

    }

    if (gl_modeltest)
       {
        DrawAmmoBox();
       }

    GL_DrawThings();

    /*glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_DEPTH_TEST);

    if (gl_fog == 1)
       {
        glDisable(GL_FOG);
       }

    glPopMatrix();

    glPushMatrix();*/

    GL_DrawPlayerSprites();

    /*if (RedBias == true)
       {
        glColor4f( 1.0f, 0.0f, 0.0f, 0.2f );
       }
    if (GreenBias == true)
       {
        glColor4f( 0.0f, 1.0f, 0.0f, 0.2f );
       }

    if ((GreenBias == true) || (RedBias == true))
    {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBegin(GL_QUADS);
           glVertex3f(glLeft,  glTop,    SetBack);
           glVertex3f(glLeft,  glBottom, SetBack);
           glVertex3f(glRight, glBottom, SetBack);
           glVertex3f(glRight, glTop,    SetBack);
        glEnd(); 
        glDisable(GL_BLEND);
    }

    glPopMatrix();*/
}

/*
float AngleTo( float x1, float z1, float x2, float z2 );

void R_CheckSides()
   {
    DW_Polygon  *TempPoly;
    float        fangle1, fangle2;

    TempPoly = PolyList;
    while (TempPoly != 0)
       {
        if (DrawSide[TempPoly->SideDef] == ds_unknown)
           {
            fangle1 = AngleTo( TempPoly->Point[0].v[0], TempPoly->Point[0].v[3], camera.x, camera.z );
            fangle2 = AngleTo( TempPoly->Point[3].v[0], TempPoly->Point[3].v[3], camera.x, camera.z );

            if (fangle1 < fangle2)
               {
                fangle1 += 360.0f;
               }

            DrawSide[TempPoly->SideDef] = ds_nodraw;
            if ((fangle1 >= rangle) || (fangle2 <= langle))
               {
                // the line is in view
                if ((fangle1 - fangle2) < 180.0f)
                   {
                    //it's facing us -- draw it...
                    DrawSide[TempPoly->SideDef] = ds_draw;
                   }
               }
           }
        TempPoly = TempPoly->Next;
       }
   }
*/
//
// R_RenderView
//
void R_RenderPlayerView (player_t* player)
{	
    R_SetupFrame(player);

    // Clear buffers.
    R_ClearClipSegs();
    R_ClearDrawSegs();
    R_ClearPlanes();
    R_ClearSprites();

    //R_CheckSides();

    // check for new console commands.
    NetUpdate ();

    memset(DrawFlat, 0, sizeof(dbyte *) * numsectors);
    memset(DrawSide, 0, sizeof(drawside_t) * numsides);
    sorted_flats_count = 0;
    sorted_walls_count = 0;

    // The head node is the last node output.
    R_RenderBSPNode (numnodes-1);
    
    // Check for new console commands.
    NetUpdate ();
    
    R_DrawPlanes ();
    
    // Check for new console commands.
    NetUpdate ();
    
    R_DrawMasked ();

    // Check for new console commands.
    NetUpdate ();				
}
