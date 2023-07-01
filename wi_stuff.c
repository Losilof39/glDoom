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
//	Intermission screens.
//
//-----------------------------------------------------------------------------


//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <glad/glad.h>

static const char
rcsid[] = "$Id: wi_stuff.c,v 1.7 1997/02/03 22:45:13 b1 Exp $";

#include <stdio.h>

#include "z_zone.h"

#include "m_random.h"
#include "m_swap.h"

#include "i_system.h"

#include "w_wad.h"

#include "g_game.h"

#include "r_local.h"
#include "s_sound.h"

#include "doomstat.h"

// Data.
#include "sounds.h"

// Needs access to LFB.
#include "v_video.h"

#include "wi_stuff.h"

#include "gldefs.h"
#include "gl_utils.h"

//
// Data needed to add patches to full screen intermission pics.
// Patches are statistics messages, and animations.
// Loads of by-pixel layout and placement, offsets etc.
//


//
// Different vetween registered DOOM (1994) and
//  Ultimate DOOM - Final edition (retail, 1995?).
// This is supposedly ignored for commercial
//  release (aka DOOM II), which had 34 maps
//  in one episode. So there.
#define NUMEPISODES	4
#define NUMMAPS		9


// in tics
//U #define PAUSELEN		(TICRATE*2) 
//U #define SCORESTEP		100
//U #define ANIMPERIOD		32
// pixel distance from "(YOU)" to "PLAYER N"
//U #define STARDIST		10 
//U #define WK 1


// GLOBAL LOCATIONS
#define WI_TITLEY		2
#define WI_SPACINGY    		33

// SINGPLE-PLAYER STUFF
#define SP_STATSX		50
#define SP_STATSY		50

#define SP_TIMEX		16
#define SP_TIMEY		168


// NET GAME STUFF
#define NG_STATSY		50
#define NG_STATSX		(32 + DSHORT(star->width)/2 + 32*!dofrags)

#define NG_SPACINGX    		64


// DEATHMATCH STUFF
#define DM_MATRIXX		42
#define DM_MATRIXY		68

#define DM_SPACINGX		40

#define DM_TOTALSX		269

#define DM_KILLERSX		10
#define DM_KILLERSY		100
#define DM_VICTIMSX    		5
#define DM_VICTIMSY		50




typedef enum
{
    ANIM_ALWAYS,
    ANIM_RANDOM,
    ANIM_LEVEL

} animenum_t;

typedef struct
{
    int		x;
    int		y;
    
} point_t;


//
// Animation.
// There is another anim_t used in p_spec.
//
typedef struct
{
    animenum_t	type;

    // period in tics between animations
    int		period;

    // number of animation frames
    int		nanims;

    // location of animation
    point_t	loc;

    // ALWAYS: n/a,
    // RANDOM: period deviation (<256),
    // LEVEL: level
    int		data1;

    // ALWAYS: n/a,
    // RANDOM: random base period,
    // LEVEL: n/a
    int		data2; 

    // actual graphics for frames of animations
    patch_t*	p[3]; 

    // following must be initialized to zero before use!

    // next value of bcnt (used in conjunction with period)
    int		nexttic;

    // last drawn animation frame
    int		lastdrawn;

    // next frame number to animate
    int		ctr;
    
    // used by RANDOM and LEVEL when animating
    int		state;  

} anim_t;


static point_t lnodes[NUMEPISODES][NUMMAPS] =
{
    // Episode 0 World Map
    {
	{ 185, 164 },	// location of level 0 (CJ)
	{ 148, 143 },	// location of level 1 (CJ)
	{ 69, 122 },	// location of level 2 (CJ)
	{ 209, 102 },	// location of level 3 (CJ)
	{ 116, 89 },	// location of level 4 (CJ)
	{ 166, 55 },	// location of level 5 (CJ)
	{ 71, 56 },	// location of level 6 (CJ)
	{ 135, 29 },	// location of level 7 (CJ)
	{ 71, 24 }	// location of level 8 (CJ)
    },

    // Episode 1 World Map should go here
    {
	{ 254, 25 },	// location of level 0 (CJ)
	{ 97, 50 },	// location of level 1 (CJ)
	{ 188, 64 },	// location of level 2 (CJ)
	{ 128, 78 },	// location of level 3 (CJ)
	{ 214, 92 },	// location of level 4 (CJ)
	{ 133, 130 },	// location of level 5 (CJ)
	{ 208, 136 },	// location of level 6 (CJ)
	{ 148, 140 },	// location of level 7 (CJ)
	{ 235, 158 }	// location of level 8 (CJ)
    },

    // Episode 2 World Map should go here
    {
	{ 156, 168 },	// location of level 0 (CJ)
	{ 48, 154 },	// location of level 1 (CJ)
	{ 174, 95 },	// location of level 2 (CJ)
	{ 265, 75 },	// location of level 3 (CJ)
	{ 130, 48 },	// location of level 4 (CJ)
	{ 279, 23 },	// location of level 5 (CJ)
	{ 198, 48 },	// location of level 6 (CJ)
	{ 140, 25 },	// location of level 7 (CJ)
	{ 281, 136 }	// location of level 8 (CJ)
    }

};


//
// Animation locations for episode 0 (1).
// Using patches saves a lot of space,
//  as they replace 320x200 full screen frames.
//
static anim_t epsd0animinfo[] =
{
    { ANIM_ALWAYS, TICRATE/3, 3, { 224, 104 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 184, 160 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 112, 136 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 72, 112 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 88, 96 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 64, 48 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 192, 40 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 136, 16 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 80, 16 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 64, 24 } }
};

static anim_t epsd1animinfo[] =
{
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 1 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 2 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 3 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 4 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 5 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 6 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 7 },
    { ANIM_LEVEL, TICRATE/3, 3, { 192, 144 }, 8 },
    { ANIM_LEVEL, TICRATE/3, 1, { 128, 136 }, 8 }
};

static anim_t epsd2animinfo[] =
{
    { ANIM_ALWAYS, TICRATE/3, 3, { 104, 168 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 40, 136 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 160, 96 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 104, 80 } },
    { ANIM_ALWAYS, TICRATE/3, 3, { 120, 32 } },
    { ANIM_ALWAYS, TICRATE/4, 3, { 40, 0 } }
};

static int NUMANIMS[NUMEPISODES] =
{
    sizeof(epsd0animinfo)/sizeof(anim_t),
    sizeof(epsd1animinfo)/sizeof(anim_t),
    sizeof(epsd2animinfo)/sizeof(anim_t)
};

static anim_t *anims[NUMEPISODES] =
{
    epsd0animinfo,
    epsd1animinfo,
    epsd2animinfo
};


//
// GENERAL DATA
//

//
// Locally used stuff.
//
#define FB 0


// States for single-player
#define SP_KILLS		0
#define SP_ITEMS		2
#define SP_SECRET		4
#define SP_FRAGS		6 
#define SP_TIME			8 
#define SP_PAR			ST_TIME

#define SP_PAUSE		1

// in seconds
#define SHOWNEXTLOCDELAY	4
//#define SHOWLASTLOCDELAY	SHOWNEXTLOCDELAY


// used to accelerate or skip a stage
static int		acceleratestage;

// wbs->pnum
static int		me;

 // specifies current state
static stateenum_t	state;

// contains information passed into intermission
static wbstartstruct_t*	wbs;

static wbplayerstruct_t* plrs;  // wbs->plyr[]

// used for general timing
static int 		cnt;  

// used for timing of background animation
static int 		bcnt;

// signals to refresh everything for one frame
static int 		firstrefresh; 

static int		cnt_kills[MAXPLAYERS];
static int		cnt_items[MAXPLAYERS];
static int		cnt_secret[MAXPLAYERS];
static int		cnt_time;
static int		cnt_par;
static int		cnt_pause;

// # of commercial levels
//static int		NUMCMAPS;
// Don't know why this was a variable
// turned it into a define...
// Of course, once the WAD is loaded you could count the maps...
// Look into that later...  BAL
#define NUMCMAPS 32

//
//	GRAPHICS
//

// background (map of levels).
static patch_t*		bg;

// You Are Here graphic
static patch_t*		yah[2]; 

// splat
static patch_t*		splat;

// %, : graphics
static patch_t*		percent;
static patch_t*		colon;

// 0-9 graphic
static patch_t*		num[10];

// minus sign
static patch_t*		wiminus;

// "Finished!" graphics
static patch_t*		finished;

// "Entering" graphic
static patch_t*		entering; 

// "secret"
static patch_t*		sp_secret;

 // "Kills", "Scrt", "Items", "Frags"
static patch_t*		kills;
static patch_t*		secret;
static patch_t*		items;
static patch_t*		frags;

// Time sucks.
static patch_t*		time;
static patch_t*		par;
static patch_t*		sucks;

// "killers", "victims"
static patch_t*		killers;
static patch_t*		victims; 

// "Total", your face, your dead face
static patch_t*		total;
static patch_t*		star;
static patch_t*		bstar;

// "red P[1..MAXPLAYERS]"
static patch_t*		p[MAXPLAYERS];

// "gray P[1..MAXPLAYERS]"
static patch_t*		bp[MAXPLAYERS];

 // Name graphics of each level (centered)
static patch_t**	lnames;

// This is a pointer to be set by WI_LoadData for "slamming" the background...
GLTexData *glBackGround;

// Character maps
GLTexData *LevelNames[4];
GLTexData  WiMinus, WiNum[10], Percent, Finished, Entering, Kills, Secret, SP_Secret;
GLTexData  Time, Items, Frags, Colon, Time, Sucks, Par, Killers, Victims, Total, Star, BStar;
GLTexData  YouAreHere[2], Splat, **AnimTex[3];
GLTexData  gl_p[MAXPLAYERS], gl_bp[MAXPLAYERS];

// These are full screen images : treat differently (two part textures [256 & 64])
GLTexData  Interpic[2], GameMap[3][2];

int GL_MakeScreenTexture(patch_t *, GLTexData *);

extern int gl_premalpha;

//
// CODE
//

// slam background
// UNUSED static unsigned char *background=0;


void WI_slamBackground(void)
{
    memcpy(screens[0], screens[1], SCREENWIDTH * SCREENHEIGHT);
    V_MarkRect (0, 0, SCREENWIDTH, SCREENHEIGHT);
}

// The ticker is used to detect keys
//  because of timing issues in netgames.
dboolean WI_Responder(event_t* ev)
{
    return false;
}

float  Scale = 1.0f;

extern float SetBack;

void GL_DrawPatch(GLTexData *Tex, float x, float y)
   {
    float       Left, Bottom, Right, Top;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    Left = -160.0f+x;
    Right = Left+(Tex->Width*Scale);
    Top = 120.0f-(y*1.2f);
    Bottom = Top-((Tex->Height*Scale)*1.2f);

    // draw animation frame
    glBindTexture(GL_TEXTURE_2D, Tex->TexName);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, 1.0f);
       glVertex3f( Left, Top, SetBack);
       glTexCoord2f( 0.0f, Tex->YDisp);
       glVertex3f( Left, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, Tex->YDisp);
       glVertex3f( Right, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, 1.0f);
       glVertex3f( Right, Top, SetBack);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

// Draws "<Levelname> Finished!"
void GL_WI_drawLF(void)
   {
    float   Left, Bottom, Right, Top;
    GLTexData *Tex;

    Tex = &LevelNames[wbs->epsd][wbs->last];
    
    Left = 0.0f-(Tex->Width/2.0f);
    Right = Left+Tex->Width;
    Top = 120.0f-(2.0f*1.2f);
    Bottom = Top-(Tex->Height*1.2f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    // draw <LevelName> 
    glBindTexture(GL_TEXTURE_2D, Tex->TexName);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, 1.0f);
       glVertex3f( Left, Top, SetBack);
       glTexCoord2f( 0.0f, Tex->YDisp);
       glVertex3f( Left, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, Tex->YDisp);
       glVertex3f( Right, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, 1.0f);
       glVertex3f( Right, Top, SetBack);
    glEnd();

    Top -= (5*(Tex->Height*1.2f))/4;

    Tex = &Finished;
    
    Left = 0.0f-(Tex->Width/2.0f);
    Right = Left+Tex->Width;
    Bottom = Top-(Tex->Height*1.2f);

    // draw "Finished!"
    glBindTexture(GL_TEXTURE_2D, Tex->TexName);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, 1.0f);
       glVertex3f( Left, Top, SetBack);
       glTexCoord2f( 0.0f, Tex->YDisp);
       glVertex3f( Left, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, Tex->YDisp);
       glVertex3f( Right, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, 1.0f);
       glVertex3f( Right, Top, SetBack);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

// Draws "<Levelname> Finished!"
void WI_drawLF(void)
{
    int y = WI_TITLEY+((SCREENHEIGHT-200)/2);

    // draw <LevelName> 
    V_DrawPatch((SCREENWIDTH - DSHORT(lnames[wbs->last]->width))/2,
		y, FB, lnames[wbs->last]);

    // draw "Finished!"
    y += (5*DSHORT(lnames[wbs->last]->height))/4;
    
    V_DrawPatch((SCREENWIDTH - DSHORT(finished->width))/2,
		y, FB, finished);
}



// Draws "Entering <LevelName>"
void GL_WI_drawEL(void)
   {
    float   Left, Bottom, Right, Top;
    GLTexData *Tex;

    Tex = &Entering;

    Left = 0.0f-(Tex->Width/2.0f);
    Right = Left+Tex->Width;
    Top = 120.0f-(2.0f*1.2f);
    Bottom = Top-(Tex->Height*1.2f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    // draw "Entering"
    glBindTexture(GL_TEXTURE_2D, Tex->TexName);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, 1.0f);
       glVertex3f( Left, Top, SetBack);
       glTexCoord2f( 0.0f, Tex->YDisp);
       glVertex3f( Left, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, Tex->YDisp);
       glVertex3f( Right, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, 1.0f);
       glVertex3f( Right, Top, SetBack);
    glEnd();

    Top -= (5*(Tex->Height*1.2f))/4;

    Tex = &LevelNames[wbs->epsd][wbs->next];
    
    Left = 0.0f-(Tex->Width/2.0f);
    Right = Left+Tex->Width;
    Bottom = Top-(Tex->Height*1.2f);

    // draw <LevelName>
    glBindTexture(GL_TEXTURE_2D, Tex->TexName);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, 1.0f);
       glVertex3f( Left, Top, SetBack);
       glTexCoord2f( 0.0f, Tex->YDisp);
       glVertex3f( Left, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, Tex->YDisp);
       glVertex3f( Right, Bottom, SetBack);
       glTexCoord2f( Tex->XDisp, 1.0f);
       glVertex3f( Right, Top, SetBack);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

// Draws "Entering <LevelName>"
void WI_drawEL(void)
{
    int y = WI_TITLEY+((SCREENHEIGHT-200)/2);

    // draw "Entering"
    V_DrawPatch((SCREENWIDTH - DSHORT(entering->width))/2,y, FB, entering);

    // draw level
    y += (5*DSHORT(lnames[wbs->next]->height))/4;

    V_DrawPatch((SCREENWIDTH - DSHORT(lnames[wbs->next]->width))/2,y, FB, lnames[wbs->next]);

}


void GL_WI_drawOnLnode( int n, GLTexData *Tex)
   {
    int		i;
    float		left;
    float		top;
    float		right;
    float		bottom;
    dboolean	fits = false;
    float   Left, Bottom, Right, Top;

    i = 0;
    do
      {
       left = lnodes[wbs->epsd][n].x - Tex[i].LeftOff;
       top = lnodes[wbs->epsd][n].y - Tex[i].TopOff;
       right = left + Tex[i].Width;
       bottom = top + Tex[i].Height;

       if (left >= 0 && right < SCREENWIDTH && top >= 0 && bottom < SCREENHEIGHT)
	      {
	       fits = true;
	      }
	   else
	      {
           i++;
          }
      } while (!fits && i != 2);

    if (fits && (i < 2))
       {
        Left = -160.0f+(lnodes[wbs->epsd][n].x-Tex[i].LeftOff);
        Right = Left+Tex[i].Width;
        Top = 120.0f-((lnodes[wbs->epsd][n].y-Tex[i].TopOff)*1.2f);
        Bottom = Top-(Tex[i].Height*1.2f);

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // draw "You Are Here"
        glBindTexture(GL_TEXTURE_2D, Tex[i].TexName);
        glBegin(GL_QUADS);
           glTexCoord2f( 0.0f, 1.0f);
           glVertex3f( Left, Top, SetBack);
           glTexCoord2f( 0.0f, Tex[i].YDisp);
           glVertex3f( Left, Bottom, SetBack);
           glTexCoord2f( Tex[i].XDisp, Tex[i].YDisp);
           glVertex3f( Right, Bottom, SetBack);
           glTexCoord2f( Tex[i].XDisp, 1.0f);
           glVertex3f( Right, Top, SetBack);
        glEnd();
       }
    else
       {
	    // DEBUG
	    printf("Could not place patch on level %d", n+1); 
       }
   }


void
WI_drawOnLnode
( int		n,
  patch_t*	c[] )
{

    int		i;
    int		left;
    int		top;
    int		right;
    int		bottom;
    dboolean	fits = false;

    i = 0;
    do
    {
	left = ((SCREENWIDTH-320)/2)+lnodes[wbs->epsd][n].x - DSHORT(c[i]->leftoffset);
	top = ((SCREENHEIGHT-200)/2)+lnodes[wbs->epsd][n].y - DSHORT(c[i]->topoffset);
	right = left + DSHORT(c[i]->width);
	bottom = top + DSHORT(c[i]->height);

	if (left >= 0
	    && right < SCREENWIDTH
	    && top >= 0
	    && bottom < SCREENHEIGHT)
	{
	    fits = true;
	}
	else
	{
	    i++;
	}
    } while (!fits && i!=2);

    if (fits && i<2)
    {
	V_DrawPatch(((SCREENWIDTH-320)/2)+lnodes[wbs->epsd][n].x, ((SCREENHEIGHT-200)/2)+lnodes[wbs->epsd][n].y,
		    FB, c[i]);
    }
    else
    {
	// DEBUG
	printf("Could not place patch on level %d", n+1); 
    }
}



void WI_initAnimatedBack(void)
{
    int		i;
    anim_t*	a;

    if (gamemode == commercial)
	return;

    if (wbs->epsd > 2)
	return;

    for (i=0;i<NUMANIMS[wbs->epsd];i++)
    {
	a = &anims[wbs->epsd][i];

	// init variables
	a->ctr = -1;

	// specify the next time to draw it
	if (a->type == ANIM_ALWAYS)
	    a->nexttic = bcnt + 1 + (M_Random()%a->period);
	else if (a->type == ANIM_RANDOM)
	    a->nexttic = bcnt + 1 + a->data2+(M_Random()%a->data1);
	else if (a->type == ANIM_LEVEL)
	    a->nexttic = bcnt + 1;
    }

}

void WI_updateAnimatedBack(void)
{
    int		i;
    anim_t*	a;

    if (gamemode == commercial)
	return;

    if (wbs->epsd > 2)
	return;

    for (i=0;i<NUMANIMS[wbs->epsd];i++)
    {
	a = &anims[wbs->epsd][i];

	if (bcnt == a->nexttic)
	{
	    switch (a->type)
	    {
	      case ANIM_ALWAYS:
		if (++a->ctr >= a->nanims) a->ctr = 0;
		a->nexttic = bcnt + a->period;
		break;

	      case ANIM_RANDOM:
		a->ctr++;
		if (a->ctr == a->nanims)
		{
		    a->ctr = -1;
		    a->nexttic = bcnt+a->data2+(M_Random()%a->data1);
		}
		else a->nexttic = bcnt + a->period;
		break;
		
	      case ANIM_LEVEL:
		// gawd-awful hack for level anims
		if (!(state == StatCount && i == 7)
		    && wbs->next == a->data1)
		{
		    a->ctr++;
		    if (a->ctr == a->nanims) a->ctr--;
		    a->nexttic = bcnt + a->period;
		}
		break;
	    }
	}

    }

}

void GL_WI_drawAnimatedBack(void)
   {
    int			i;
    anim_t*		a;
    float       Left, Bottom, Right, Top;
    GLTexData  *Tex;

    if (gamemode == commercial)
        return;

    if (wbs->epsd > 2)
        return;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    for (i = 0; i < NUMANIMS[wbs->epsd]; i++)
       {
        a = &anims[wbs->epsd][i];

        if (a->ctr >= 0)
           {
            //V_DrawPatch(((SCREENWIDTH-320)/2)+a->loc.x, ((SCREENHEIGHT-200)/2)+a->loc.y, FB, a->p[a->ctr]);

            Tex = &AnimTex[wbs->epsd][i][a->ctr];

            Left = -160.0f+a->loc.x;
            Right = Left+Tex->Width;
            Top = 120.0f-(a->loc.y*1.2f);
            Bottom = Top-(Tex->Height*1.2f);

            // draw animation frame
            glBindTexture(GL_TEXTURE_2D, Tex->TexName);
            glBegin(GL_QUADS);
               glTexCoord2f( 0.0f, 1.0f);
               glVertex3f( Left, Top, SetBack);
               glTexCoord2f( 0.0f, Tex->YDisp);
               glVertex3f( Left, Bottom, SetBack);
               glTexCoord2f( Tex->XDisp, Tex->YDisp);
               glVertex3f( Right, Bottom, SetBack);
               glTexCoord2f( Tex->XDisp, 1.0f);
               glVertex3f( Right, Top, SetBack);
            glEnd();

           }
       }
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

void WI_drawAnimatedBack(void)
   {
    int			i;
    anim_t*		a;

    if (gamemode == commercial)
        return;

    if (wbs->epsd > 2)
        return;

    for (i = 0; i < NUMANIMS[wbs->epsd]; i++)
       {
        a = &anims[wbs->epsd][i];

        if (a->ctr >= 0)
            V_DrawPatch(((SCREENWIDTH-320)/2)+a->loc.x, ((SCREENHEIGHT-200)/2)+a->loc.y, FB, a->p[a->ctr]);
       }
   }

//
// Draws a number.
// If digits > 0, then use that many digits minimum,
//  otherwise only use as many as necessary.
// Returns new x position.
//

int GL_WI_drawNum( int x, int y, int n, int digits )
   {
    int		fontwidth = DSHORT(num[0]->width);
    int		neg;
    int		temp;

    if (digits < 0)
       {
        if (!n)
           {
            // make variable-length zeros 1 digit long
            digits = 1;
           }
        else
           {
            // figure out # of digits in #
            digits = 0;
            temp = n;

            while (temp)
               {
                temp /= 10;
                digits++;
               }
           }
       }

    neg = n < 0;
    if (neg)
        n = -n;

    // if non-number, do not draw it
    if (n == 1994)
        return 0;

    // draw the new number
    while (digits--)
       {
        x -= fontwidth;
        GL_DrawPatch(&WiNum[n % 10], (float)x, (float)y);
        n /= 10;
       }

    // draw a minus sign if necessary
    if (neg)
       {
        GL_DrawPatch(&WiMinus, (float)x-=8, (float)y);
       }

    return x;
   }

int WI_drawNum( int x, int y, int n, int digits )
   {
    int		fontwidth = DSHORT(num[0]->width);
    int		neg;
    int		temp;

    if (digits < 0)
       {
        if (!n)
           {
            // make variable-length zeros 1 digit long
            digits = 1;
           }
        else
           {
            // figure out # of digits in #
            digits = 0;
            temp = n;

            while (temp)
               {
                temp /= 10;
                digits++;
               }
           }
       }

    neg = n < 0;
    if (neg)
        n = -n;

    // if non-number, do not draw it
    if (n == 1994)
        return 0;

    // draw the new number
    while (digits--)
       {
        x -= fontwidth;
        V_DrawPatch(x, y, FB, num[ n % 10 ]);
        n /= 10;
       }

    // draw a minus sign if necessary
    if (neg)
        V_DrawPatch(x-=8, y, FB, wiminus);

    return x;
   }

void GL_WI_drawPercent( int x, int y, int p )
   {
    if (p < 0)
        return;

    GL_DrawPatch(&Percent, (float)x, (float)y);
    GL_WI_drawNum(x, y, p, -1);
   }

void WI_drawPercent( int x, int y, int p )
   {
    if (p < 0)
        return;

    V_DrawPatch(x, y, FB, percent);
    WI_drawNum(x, y, p, -1);
   }



//
// Display level completion time and par,
//  or "sucks" message if overflow.
//
void GL_WI_drawTime( int x, int y, int t )
   {
    int		div;
    int		n;

    if (t < 0)
        return;

    if (t <= 61*59)
       {
        div = 1;
        do
           {
            n = (t / div) % 60;
            x = GL_WI_drawNum(x, y, n, 2) - DSHORT(colon->width);
            div *= 60;

            // draw
            if (div==60 || t / div)
               {
                GL_DrawPatch(&Colon, (float)x, (float)y);
               }
	    
           } while (t / div);
       }
    else
       {
        // "sucks"
        GL_DrawPatch(&Sucks, (float)x - DSHORT(sucks->width), (float)y);
       }
   }

//
// Display level completion time and par,
//  or "sucks" message if overflow.
//
void WI_drawTime( int x, int y, int t )
   {
    int		div;
    int		n;

    if (t < 0)
        return;

    if (t <= 61*59)
       {
        div = 1;
        do
           {
            n = (t / div) % 60;
            x = WI_drawNum(x, y, n, 2) - DSHORT(colon->width);
            div *= 60;

            // draw
            if (div==60 || t / div)
                V_DrawPatch(x, y, FB, colon);
	    
           } while (t / div);
       }
    else
       {
        // "sucks"
        V_DrawPatch(x - DSHORT(sucks->width), y, FB, sucks);
       }
   }

extern void WI_unloadData(void);

void WI_End(void)
{
    //--> ? void WI_unloadData(void);
    WI_unloadData();
}

void WI_initNoState(void)
{
    state = NoState;
    acceleratestage = 0;
    cnt = 10;
}

void WI_updateNoState(void) {

    WI_updateAnimatedBack();

    if (!--cnt)
    {
	WI_End();
	G_WorldDone();
    }

}

static dboolean		snl_pointeron = false;


void WI_initShowNextLoc(void)
{
    state = ShowNextLoc;
    acceleratestage = 0;
    cnt = SHOWNEXTLOCDELAY * TICRATE;

    WI_initAnimatedBack();
}

void WI_updateShowNextLoc(void)
{
    WI_updateAnimatedBack();

    if (!--cnt || acceleratestage)
	WI_initNoState();
    else
	snl_pointeron = (cnt & 31) < 20;
}

void GL_WI_drawShowNextLoc(void)
   {
    int i, last;

    // draw animated background
    GL_WI_drawAnimatedBack();
    
    if ( gamemode != commercial)
       {
        if (wbs->epsd > 2)
           {
            GL_WI_drawEL();
            return;
           }
	
        last = (wbs->last == 8) ? wbs->next - 1 : wbs->last;

        // draw a splat on taken cities.
        for (i = 0; i <= last; i++)
            GL_WI_drawOnLnode(i, &Splat);

        // splat the secret level?
        if (wbs->didsecret)
            GL_WI_drawOnLnode(8, &Splat);

        // draw flashing ptr
        if (snl_pointeron)
            GL_WI_drawOnLnode(wbs->next, YouAreHere); 
       }

    // draws which level you are entering..
    if ( (gamemode != commercial) || wbs->next != 30)
        GL_WI_drawEL();  
   }

void WI_drawShowNextLoc(void)
{

    int		i;
    int		last;

    WI_slamBackground();

    // draw animated background
    WI_drawAnimatedBack(); 

    if ( gamemode != commercial)
    {
  	if (wbs->epsd > 2)
	{
	    WI_drawEL();
	    return;
	}
	
	last = (wbs->last == 8) ? wbs->next - 1 : wbs->last;

	// draw a splat on taken cities.
	for (i=0 ; i<=last ; i++)
	    WI_drawOnLnode(i, &splat);

	// splat the secret level?
	if (wbs->didsecret)
	    WI_drawOnLnode(8, &splat);

	// draw flashing ptr
	if (snl_pointeron)
	    WI_drawOnLnode(wbs->next, yah); 
    }

    // draws which level you are entering..
    if ( (gamemode != commercial)
	 || wbs->next != 30)
	WI_drawEL();  

}

void WI_drawNoState(void)
{
    snl_pointeron = true;
    WI_drawShowNextLoc();
}

int WI_fragSum(int playernum)
{
    int		i;
    int		frags = 0;
    
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (playeringame[i]
	    && i!=playernum)
	{
	    frags += plrs[playernum].frags[i];
	}
    }

	
    // JDC hack - negative frags.
    frags -= plrs[playernum].frags[playernum];
    // UNUSED if (frags < 0)
    // 	frags = 0;

    return frags;
}



static int		dm_state;
static int		dm_frags[MAXPLAYERS][MAXPLAYERS];
static int		dm_totals[MAXPLAYERS];



void WI_initDeathmatchStats(void)
{

    int		i;
    int		j;

    state = StatCount;
    acceleratestage = 0;
    dm_state = 1;

    cnt_pause = TICRATE;

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (playeringame[i])
	{
	    for (j=0 ; j<MAXPLAYERS ; j++)
		if (playeringame[j])
		    dm_frags[i][j] = 0;

	    dm_totals[i] = 0;
	}
    }
    
    WI_initAnimatedBack();
}



void WI_updateDeathmatchStats(void)
{

    int		i;
    int		j;
    
    dboolean	stillticking;

    WI_updateAnimatedBack();

    if (acceleratestage && dm_state != 4)
    {
	acceleratestage = 0;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (playeringame[i])
	    {
		for (j=0 ; j<MAXPLAYERS ; j++)
		    if (playeringame[j])
			dm_frags[i][j] = plrs[i].frags[j];

		dm_totals[i] = WI_fragSum(i);
	    }
	}
	

	S_StartSound(0, sfx_barexp);
	dm_state = 4;
    }

    
    if (dm_state == 2)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);
	
	stillticking = false;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (playeringame[i])
	    {
		for (j=0 ; j<MAXPLAYERS ; j++)
		{
		    if (playeringame[j]
			&& dm_frags[i][j] != plrs[i].frags[j])
		    {
			if (plrs[i].frags[j] < 0)
			    dm_frags[i][j]--;
			else
			    dm_frags[i][j]++;

			if (dm_frags[i][j] > 99)
			    dm_frags[i][j] = 99;

			if (dm_frags[i][j] < -99)
			    dm_frags[i][j] = -99;
			
			stillticking = true;
		    }
		}
		dm_totals[i] = WI_fragSum(i);

		if (dm_totals[i] > 99)
		    dm_totals[i] = 99;
		
		if (dm_totals[i] < -99)
		    dm_totals[i] = -99;
	    }
	    
	}
	if (!stillticking)
	{
	    S_StartSound(0, sfx_barexp);
	    dm_state++;
	}

    }
    else if (dm_state == 4)
    {
	if (acceleratestage)
	{
	    S_StartSound(0, sfx_slop);

	    if ( gamemode == commercial)
		WI_initNoState();
	    else
		WI_initShowNextLoc();
	}
    }
    else if (dm_state & 1)
    {
	if (!--cnt_pause)
	{
	    dm_state++;
	    cnt_pause = TICRATE;
	}
    }
}



void GL_WI_drawDeathmatchStats(void)
   {
    int    i;
    int	   j;
    int	   x;
    int	   y;
    int	   w;

    int    lh; // line height

    lh = (3*DSHORT(num[0]->height))/2;

    // draw animated background
    GL_WI_drawAnimatedBack();
    
    GL_WI_drawLF();

    GL_DrawPatch(&Total, (float)DM_TOTALSX-DSHORT(total->width)/2, (float)DM_MATRIXY-WI_SPACINGY+10);

    GL_DrawPatch(&Killers, (float)DM_KILLERSX, (float)DM_KILLERSY);
    GL_DrawPatch(&Victims, (float)DM_VICTIMSX, (float)DM_VICTIMSY);

    // draw P?
    x = DM_MATRIXX + DM_SPACINGX;
    y = DM_MATRIXY;

    for (i = 0; i < MAXPLAYERS; i++)
       {
        if (playeringame[i])
           {
            GL_DrawPatch(&gl_p[i], (float)(x-DSHORT(p[i]->width)/2), (float)(DM_MATRIXY - WI_SPACINGY));
            GL_DrawPatch(&gl_p[i], (float)(DM_MATRIXX-DSHORT(p[i]->width)/2), (float)y);
            if (i == me)
               {
                GL_DrawPatch(&BStar, (float)(x-DSHORT(bstar->width)/2), (float)(DM_MATRIXY - WI_SPACINGY));
                GL_DrawPatch(&Star, (float)(DM_MATRIXX-DSHORT(star->width)/2), (float)y );
               }
           }
/*
        else
           {
            GL_DrawPatch(&gl_bp[i], (float)(x-SHORT(bp[i]->width)/2), (float)(DM_MATRIXY - WI_SPACINGY));
            GL_DrawPatch(&gl_bp[i], (float)(DM_MATRIXX-SHORT(bp[i]->width)/2), (float)y);
           }
*/
        x += DM_SPACINGX;
        y += WI_SPACINGY;
       }

    // draw stats
    y = DM_MATRIXY+10;
    w = DSHORT(num[0]->width);

    for (i = 0; i < MAXPLAYERS; i++)
       {
        x = DM_MATRIXX + DM_SPACINGX;

        if (playeringame[i])
           {
            for (j = 0; j < MAXPLAYERS; j++)
               {
                if (playeringame[j])
                    GL_WI_drawNum(x+w, y, dm_frags[i][j], 2);
                x += DM_SPACINGX;
               }
            GL_WI_drawNum(DM_TOTALSX+w, y, dm_totals[i], 2);
           }
        y += WI_SPACINGY;
       }
   }

void WI_drawDeathmatchStats(void)
{

    int		i;
    int		j;
    int		x;
    int		y;
    int		w;
    
    int		lh;	// line height

    lh = WI_SPACINGY;

    WI_slamBackground();
    
    // draw animated background
    WI_drawAnimatedBack(); 
    WI_drawLF();

    // draw stat titles (top line)
    V_DrawPatch(((SCREENWIDTH-320)/2)+DM_TOTALSX-DSHORT(total->width)/2,
		((SCREENHEIGHT-200)/2)+DM_MATRIXY-WI_SPACINGY+10,
		FB,
		total);
    
    V_DrawPatch(((SCREENWIDTH-320)/2)+DM_KILLERSX, ((SCREENHEIGHT-200)/2)+DM_KILLERSY, FB, killers);
    V_DrawPatch(((SCREENWIDTH-320)/2)+DM_VICTIMSX, ((SCREENHEIGHT-200)/2)+DM_VICTIMSY, FB, victims);

    // draw P?
    x = DM_MATRIXX + DM_SPACINGX;
    y = DM_MATRIXY;

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (playeringame[i])
	{
	    V_DrawPatch(((SCREENWIDTH-320)/2)+x-DSHORT(p[i]->width)/2,
			((SCREENHEIGHT-200)/2)+DM_MATRIXY - WI_SPACINGY,
			FB,
			p[i]);
	    
	    V_DrawPatch(((SCREENWIDTH-320)/2)+DM_MATRIXX-DSHORT(p[i]->width)/2,
			((SCREENHEIGHT-200)/2)+y,
			FB,
			p[i]);

	    if (i == me)
	    {
		V_DrawPatch(((SCREENWIDTH-320)/2)+x-DSHORT(p[i]->width)/2,
			    ((SCREENHEIGHT-200)/2)+DM_MATRIXY - WI_SPACINGY,
			    FB,
			    bstar);

		V_DrawPatch(((SCREENWIDTH-320)/2)+DM_MATRIXX-DSHORT(p[i]->width)/2,
			    ((SCREENHEIGHT-200)/2)+y,
			    FB,
			    star);
	    }
	}
	else
	{
	    // V_DrawPatch(x-SHORT(bp[i]->width)/2,
	    //   DM_MATRIXY - WI_SPACINGY, FB, bp[i]);
	    // V_DrawPatch(DM_MATRIXX-SHORT(bp[i]->width)/2,
	    //   y, FB, bp[i]);
	}
	x += DM_SPACINGX;
	y += WI_SPACINGY;
    }

    // draw stats
    y = DM_MATRIXY+10;
    w = DSHORT(num[0]->width);

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	x = DM_MATRIXX + DM_SPACINGX;

	if (playeringame[i])
	{
	    for (j=0 ; j<MAXPLAYERS ; j++)
	    {
		if (playeringame[j])
		    WI_drawNum(((SCREENWIDTH-320)/2)+x+w, ((SCREENHEIGHT-200)/2)+y, dm_frags[i][j], 2);

		x += DM_SPACINGX;
	    }
	    WI_drawNum(((SCREENWIDTH-320)/2)+DM_TOTALSX+w, ((SCREENHEIGHT-200)/2)+y, dm_totals[i], 2);
	}
	y += WI_SPACINGY;
    }
}

static int	cnt_frags[MAXPLAYERS];
static int	dofrags;
static int	ng_state;

void WI_initNetgameStats(void)
{

    int i;

    state = StatCount;
    acceleratestage = 0;
    ng_state = 1;

    cnt_pause = TICRATE;

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (!playeringame[i])
	    continue;

	cnt_kills[i] = cnt_items[i] = cnt_secret[i] = cnt_frags[i] = 0;

	dofrags += WI_fragSum(i);
    }

    dofrags = !!dofrags;

    WI_initAnimatedBack();
}



void WI_updateNetgameStats(void)
{

    int		i;
    int		fsum;
    
    dboolean	stillticking;

    WI_updateAnimatedBack();

    if (acceleratestage && ng_state != 10)
    {
	acceleratestage = 0;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (!playeringame[i])
		continue;

	    cnt_kills[i] = (plrs[i].skills * 100) / wbs->maxkills;
	    cnt_items[i] = (plrs[i].sitems * 100) / wbs->maxitems;
	    cnt_secret[i] = (plrs[i].ssecret * 100) / wbs->maxsecret;

	    if (dofrags)
		cnt_frags[i] = WI_fragSum(i);
	}
	S_StartSound(0, sfx_barexp);
	ng_state = 10;
    }

    if (ng_state == 2)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	stillticking = false;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (!playeringame[i])
		continue;

	    cnt_kills[i] += 2;

	    if (cnt_kills[i] >= (plrs[i].skills * 100) / wbs->maxkills)
		cnt_kills[i] = (plrs[i].skills * 100) / wbs->maxkills;
	    else
		stillticking = true;
	}
	
	if (!stillticking)
	{
	    S_StartSound(0, sfx_barexp);
	    ng_state++;
	}
    }
    else if (ng_state == 4)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	stillticking = false;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (!playeringame[i])
		continue;

	    cnt_items[i] += 2;
	    if (cnt_items[i] >= (plrs[i].sitems * 100) / wbs->maxitems)
		cnt_items[i] = (plrs[i].sitems * 100) / wbs->maxitems;
	    else
		stillticking = true;
	}
	if (!stillticking)
	{
	    S_StartSound(0, sfx_barexp);
	    ng_state++;
	}
    }
    else if (ng_state == 6)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	stillticking = false;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (!playeringame[i])
		continue;

	    cnt_secret[i] += 2;

	    if (cnt_secret[i] >= (plrs[i].ssecret * 100) / wbs->maxsecret)
		cnt_secret[i] = (plrs[i].ssecret * 100) / wbs->maxsecret;
	    else
		stillticking = true;
	}
	
	if (!stillticking)
	{
	    S_StartSound(0, sfx_barexp);
	    ng_state += 1 + 2*!dofrags;
	}
    }
    else if (ng_state == 8)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	stillticking = false;

	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (!playeringame[i])
		continue;

	    cnt_frags[i] += 1;

	    if (cnt_frags[i] >= (fsum = WI_fragSum(i)))
		cnt_frags[i] = fsum;
	    else
		stillticking = true;
	}
	
	if (!stillticking)
	{
	    S_StartSound(0, sfx_pldeth);
	    ng_state++;
	}
    }
    else if (ng_state == 10)
    {
	if (acceleratestage)
	{
	    S_StartSound(0, sfx_sgcock);
	    if ( gamemode == commercial )
		WI_initNoState();
	    else
		WI_initShowNextLoc();
	}
    }
    else if (ng_state & 1)
    {
	if (!--cnt_pause)
	{
	    ng_state++;
	    cnt_pause = TICRATE;
	}
    }
}


void GL_WI_drawNetgameStats(void)
   {
    int i;
    int x;
    int y;
    int pwidth = DSHORT(percent->width);

    // draw animated background
    GL_WI_drawAnimatedBack(); 

    GL_WI_drawLF();

    // draw stat titles (top line)
    GL_DrawPatch(&Kills, (float)(NG_STATSX+NG_SPACINGX-DSHORT(kills->width)), (float)NG_STATSY);

    GL_DrawPatch(&Items, (float)(NG_STATSX+2*NG_SPACINGX-DSHORT(items->width)), (float)NG_STATSY);

    GL_DrawPatch(&Secret, (float)(NG_STATSX+3*NG_SPACINGX-DSHORT(secret->width)), (float)NG_STATSY);
    
    if (dofrags)
       {
        GL_DrawPatch(&Frags, (float)(NG_STATSX+4*NG_SPACINGX-DSHORT(frags->width)), (float)NG_STATSY);
       }

    // draw stats
    y = NG_STATSY + DSHORT(kills->height);

    for (i = 0; i < MAXPLAYERS; i++)
       {
        if (!playeringame[i])
            continue;

        x = NG_STATSX;
        GL_DrawPatch(&gl_p[i], (float)(x-DSHORT(p[i]->width)), (float)y);

        if (i == me)
           {
            GL_DrawPatch(&Star, (float)(x-((DSHORT(p[i]->width)+DSHORT(star->width))/2)), (float)y);
           }

        x += NG_SPACINGX;
        GL_WI_drawPercent(x-pwidth, y+10, cnt_kills[i]);
        x += NG_SPACINGX;
        GL_WI_drawPercent(x-pwidth, y+10, cnt_items[i]);
        x += NG_SPACINGX;
        GL_WI_drawPercent(x-pwidth, y+10, cnt_secret[i]);
        x += NG_SPACINGX;

        if (dofrags)
           {
            GL_WI_drawNum(x, y+10, cnt_frags[i], -1);
           }

        y += WI_SPACINGY;
       }
   }


void WI_drawNetgameStats(void)
{
    int		i;
    int		x;
    int		y;
    int		pwidth = DSHORT(percent->width);

    WI_slamBackground();
    
    // draw animated background
    WI_drawAnimatedBack(); 

    WI_drawLF();

    // draw stat titles (top line)
    V_DrawPatch(((SCREENWIDTH-320)/2)+NG_STATSX+NG_SPACINGX-DSHORT(kills->width),
		((SCREENHEIGHT-200)/2)+NG_STATSY, FB, kills);

    V_DrawPatch(((SCREENWIDTH-320)/2)+NG_STATSX+2*NG_SPACINGX-DSHORT(items->width),
		((SCREENHEIGHT-200)/2)+NG_STATSY, FB, items);

    V_DrawPatch(((SCREENWIDTH-320)/2)+NG_STATSX+3*NG_SPACINGX-DSHORT(secret->width),
		((SCREENHEIGHT-200)/2)+NG_STATSY, FB, secret);
    
    if (dofrags)
	V_DrawPatch(((SCREENWIDTH-320)/2)+NG_STATSX+4*NG_SPACINGX-DSHORT(frags->width),
		    ((SCREENHEIGHT-200)/2)+NG_STATSY, FB, frags);

    // draw stats
    y = ((SCREENHEIGHT-200)/2)+NG_STATSY + DSHORT(kills->height);

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (!playeringame[i])
	    continue;

	x = ((SCREENWIDTH-320)/2)+NG_STATSX;
	V_DrawPatch(x-DSHORT(p[i]->width), y, FB, p[i]);

	if (i == me)
	    V_DrawPatch(x-DSHORT(p[i]->width), y, FB, star);

	x += NG_SPACINGX;
	WI_drawPercent(x-pwidth, y+10, cnt_kills[i]);	x += NG_SPACINGX;
	WI_drawPercent(x-pwidth, y+10, cnt_items[i]);	x += NG_SPACINGX;
	WI_drawPercent(x-pwidth, y+10, cnt_secret[i]);	x += NG_SPACINGX;

	if (dofrags)
	    WI_drawNum(x, y+10, cnt_frags[i], -1);

	y += WI_SPACINGY;
    }

}

static void WriteDebug(char* Message)
{
    FILE* fn;

    fn = fopen("glDoomScores.txt", "a+");
    fprintf(fn, "%s", Message);
    fclose(fn);
}

static int	sp_state;

void WI_initStats(void)
{
    state = StatCount;
    acceleratestage = 0;
    sp_state = 1;
    cnt_kills[0] = cnt_items[0] = cnt_secret[0] = -1;
    cnt_time = cnt_par = -1;
    cnt_pause = TICRATE;

    WI_initAnimatedBack();
}

void WI_updateStats(void)
{
    char scoretxt[256];

    WI_updateAnimatedBack();

    if (acceleratestage && sp_state != 10)
    {
	acceleratestage = 0;
	cnt_kills[0] = (plrs[me].skills * 100) / wbs->maxkills;
	cnt_items[0] = (plrs[me].sitems * 100) / wbs->maxitems;
	cnt_secret[0] = (plrs[me].ssecret * 100) / wbs->maxsecret;
	cnt_time = plrs[me].stime / TICRATE;
	cnt_par = wbs->partime / TICRATE;
	S_StartSound(0, sfx_barexp);
	sp_state = 10;
    if (keepscore)
    {
        sprintf(scoretxt, "%s Score: %d (Playing ", totalscoretextline, totalscore);
        switch (gameskill)
        {
        case 0: strcat(scoretxt, "BABY Skill Level)\n"); break;
        case 1: strcat(scoretxt, "EASY Skill Level)\n"); break;
        case 2: strcat(scoretxt, "MEDIUM Skill Level)\n"); break;
        case 3: strcat(scoretxt, "HARD Skill Level)\n"); break;
        case 4: strcat(scoretxt, "NIGHTMARE Skill Level)\n"); break;
        }
        WriteDebug(scoretxt);
        sprintf(scoretxt, "%s Kills: %d%%\n", totalscoretextline, cnt_kills[0]);
        WriteDebug(scoretxt);
        sprintf(scoretxt, "%s Items: %d%%\n", totalscoretextline, cnt_items[0]);
        WriteDebug(scoretxt);
        sprintf(scoretxt, "%s Secrets: %d%%\n", totalscoretextline, cnt_secret[0]);
        WriteDebug(scoretxt);
        totalscore += (cnt_kills[0] + cnt_items[0] + cnt_secret[0]);
        sprintf(scoretxt, "%s Final Score (score+percents): %d\n", totalscoretextline, totalscore);
        WriteDebug(scoretxt);
        }
    }

    if (sp_state == 2)
    {
	cnt_kills[0] += 2;

	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	if (cnt_kills[0] >= (plrs[me].skills * 100) / wbs->maxkills)
	{
	    cnt_kills[0] = (plrs[me].skills * 100) / wbs->maxkills;
	    S_StartSound(0, sfx_barexp);
	    sp_state++;
	}
    }
    else if (sp_state == 4)
    {
	cnt_items[0] += 2;

	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	if (cnt_items[0] >= (plrs[me].sitems * 100) / wbs->maxitems)
	{
	    cnt_items[0] = (plrs[me].sitems * 100) / wbs->maxitems;
	    S_StartSound(0, sfx_barexp);
	    sp_state++;
	}
    }
    else if (sp_state == 6)
    {
	cnt_secret[0] += 2;

	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	if (cnt_secret[0] >= (plrs[me].ssecret * 100) / wbs->maxsecret)
	{
	    cnt_secret[0] = (plrs[me].ssecret * 100) / wbs->maxsecret;
	    S_StartSound(0, sfx_barexp);
	    sp_state++;
	}
    }

    else if (sp_state == 8)
    {
	if (!(bcnt&3))
	    S_StartSound(0, sfx_pistol);

	cnt_time += 3;

	if (cnt_time >= plrs[me].stime / TICRATE)
	    cnt_time = plrs[me].stime / TICRATE;

	cnt_par += 3;

	if (cnt_par >= wbs->partime / TICRATE)
	{
	    cnt_par = wbs->partime / TICRATE;

	    if (cnt_time >= plrs[me].stime / TICRATE)
	    {
		S_StartSound(0, sfx_barexp);
		sp_state++;
	    }
	}
    }
    else if (sp_state == 10)
    {
	if (acceleratestage)
	{
	    S_StartSound(0, sfx_sgcock);

	    if (gamemode == commercial)
		WI_initNoState();
	    else
		WI_initShowNextLoc();
	}
    }
    else if (sp_state & 1)
    {
	if (!--cnt_pause)
	{
	    sp_state++;
	    cnt_pause = TICRATE;
	}
    }

}

void GL_WI_drawStats(void)
   {
    // line height
    int lh;	

    lh = (3*DSHORT(num[0]->height))/2;

    // draw animated background
    GL_WI_drawAnimatedBack();
    
    GL_WI_drawLF();

    GL_DrawPatch(&Kills, (float)SP_STATSX, (float)SP_STATSY);
    GL_WI_drawPercent(320-SP_STATSX, SP_STATSY, cnt_kills[0]);

    GL_DrawPatch(&Items, (float)SP_STATSX, (float)SP_STATSY+lh);
    GL_WI_drawPercent(320-SP_STATSX, SP_STATSY+lh, cnt_items[0]);

    GL_DrawPatch(&SP_Secret, (float)SP_STATSX, (float)SP_STATSY+(2*lh));
    GL_WI_drawPercent(320-SP_STATSX, SP_STATSY+2*lh, cnt_secret[0]);

    GL_DrawPatch(&Time, (float)SP_TIMEX, (float)SP_TIMEY);
    GL_WI_drawTime(160-SP_TIMEX, SP_TIMEY, cnt_time);

    if (wbs->epsd < 3)
       {
        GL_DrawPatch(&Par, (float)SP_TIMEX+160, (float)SP_TIMEY);
        GL_WI_drawTime(320-SP_TIMEX, SP_TIMEY, cnt_par);
       }
   }

void WI_drawStats(void)
{
    // line height
    int lh;	

    lh = (3*DSHORT(num[0]->height))/2;

    WI_slamBackground();

    // draw animated background
    WI_drawAnimatedBack();
    
    WI_drawLF();

    V_DrawPatch(((SCREENWIDTH-320)/2)+SP_STATSX, SP_STATSY, FB, kills);
    WI_drawPercent((SCREENWIDTH-((SCREENWIDTH-320)/2))-SP_STATSX, SP_STATSY, cnt_kills[0]);

    V_DrawPatch(((SCREENWIDTH-320)/2)+SP_STATSX, SP_STATSY+lh, FB, items);
    WI_drawPercent((SCREENWIDTH-((SCREENWIDTH-320)/2))-SP_STATSX, SP_STATSY+lh, cnt_items[0]);

    V_DrawPatch(((SCREENWIDTH-320)/2)+SP_STATSX, SP_STATSY+2*lh, FB, sp_secret);
    WI_drawPercent((SCREENWIDTH-((SCREENWIDTH-320)/2))-SP_STATSX, SP_STATSY+2*lh, cnt_secret[0]);

    V_DrawPatch(((SCREENWIDTH-320)/2)+SP_TIMEX, SP_TIMEY, FB, time);
    WI_drawTime(((SCREENWIDTH-320)/2)+160-SP_TIMEX, SP_TIMEY, cnt_time);

    if (wbs->epsd < 3)
    {
	V_DrawPatch(((SCREENWIDTH-320)/2)+160+SP_TIMEX, SP_TIMEY, FB, par);
	WI_drawTime((SCREENWIDTH-((SCREENWIDTH-320)/2))-SP_TIMEX, SP_TIMEY, cnt_par);
    }

}

void WI_checkForAccelerate(void)
{
    int   i;
    player_t  *player;

    // check for button presses to skip delays
    for (i=0, player = players ; i<MAXPLAYERS ; i++, player++)
    {
	if (playeringame[i])
	{
	    if (player->cmd.buttons & BT_ATTACK)
	    {
		if (!player->attackdown)
		    acceleratestage = 1;
		player->attackdown = true;
	    }
	    else
		player->attackdown = false;
	    if (player->cmd.buttons & BT_USE)
	    {
		if (!player->usedown)
		    acceleratestage = 1;
		player->usedown = true;
	    }
	    else
		player->usedown = false;
	}
    }
}



// Updates stuff each tick
void WI_Ticker(void)
{
    // counter for general background animation
    bcnt++;  

    if (bcnt == 1)
    {
	// intermission music
  	if ( gamemode == commercial )
	  S_ChangeMusic(mus_dm2int, true);
	else
	  S_ChangeMusic(mus_inter, true); 
    }

    WI_checkForAccelerate();

    switch (state)
    {
      case StatCount:
	if (deathmatch) WI_updateDeathmatchStats();
	else if (netgame) WI_updateNetgameStats();
	else WI_updateStats();
	break;
	
      case ShowNextLoc:
	WI_updateShowNextLoc();
	break;
	
      case NoState:
	WI_updateNoState();
	break;
    }

}

void GeneratePointPattern(void);
int  CreatePointLightMap(int red, int green, int blue, dboolean alpha);
int  CreateColorMap(int red, int green, int blue);
extern int WhiteLightMap, WhiteColorMap;


// This function is added because we need to load and create the game
// widget data only ONCE for OpenGL.  The textures stay in main memory
// until needed by OpenGL and are downloaded to the card as needed...
void WI_Init(void)
   {
    int     episode, episodes, i, j, frame;
    char	name[10];
    anim_t*	a;

    GeneratePointPattern();

    WhiteLightMap = CreatePointLightMap(255, 255, 255, false);
    WhiteColorMap = CreateColorMap(255, 255, 255);
    
    if ((gamemode == commercial) || (gamemode == retail))
       {
        // Both Doom2 and Ultimate Doom use the "INTERPIC" image.
	    strcpy(name, "INTERPIC");
        GL_MakeScreenTexture(W_CacheLumpName(name, PU_CACHE), Interpic);
       }

    if (gamemode != commercial)
       {
        if (gamemode == shareware)
           {
            episodes = 1;
           }
        else
        if (gamemode == registered)
           {
            episodes = 3;
           }
        else
           {
            episodes = 4;
           }
        for (episode = 0; episode < episodes; episode++)
           {
            if (episode != 3)
               {
                sprintf(name, "WIMAP%d", episode);
               }
            else
               {
                strcpy(name, "INTERPIC");
               }
            GL_MakeScreenTexture(W_CacheLumpName(name, PU_CACHE), GameMap[episode]);
            LevelNames[episode] = (GLTexData *)malloc(sizeof(GLTexData)*NUMMAPS);
            memset(LevelNames[episode], 0, sizeof(GLTexData)*NUMMAPS);
            for (i = 0; i < NUMMAPS; i++)
               {
                sprintf(name, "WILV%d%d", episode, i);
                LevelNames[episode][i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &LevelNames[episode][i], true);
               }
           }
       }
    else
       {
        LevelNames[0] = (GLTexData *)malloc(sizeof(GLTexData)*NUMCMAPS);
        memset(LevelNames[0], 0, sizeof(GLTexData)*NUMCMAPS);
        for (i = 0; i < NUMCMAPS; i++)
	       {
            sprintf(name, "CWILV%2.2d", i);
            LevelNames[0][i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &LevelNames[0][i], true);
	       }
       }

    if (gamemode != commercial)
       {
        // you are here
        YouAreHere[0].TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIURH0", PU_CACHE), &YouAreHere[0], true);

        // you are here (alt.)
        YouAreHere[1].TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIURH1", PU_CACHE), &YouAreHere[1], true);

        // splat
        Splat.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WISPLAT", PU_CACHE), &Splat, true);

        for (episode = 0; episode < episodes; episode++)
           {
            AnimTex[episode] = (GLTexData **)malloc(sizeof(GLTexData *)*NUMANIMS[episode]);

            for (j = 0; j < NUMANIMS[episode]; j++)
               {
                a = &anims[episode][j];
                AnimTex[episode][j] = (GLTexData *)malloc(sizeof(GLTexData)*a->nanims);
                for (frame = 0; frame < a->nanims; frame++)
                   {
                    // MONDO HACK!
                    if (episode != 1 || j != 8) 
                       {
	                    // animations
	                    sprintf(name, "WIA%d%.2d%.2d", episode, j, frame);
                        AnimTex[episode][j][frame].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &AnimTex[episode][j][frame], true);
                       }
                    else
                       {
	                    // HACK ALERT!
                        // Copy the animations from episode 1 level 4 to episode 1 level 8 - BAL
                        memcpy(&AnimTex[episode][j][frame], &AnimTex[1][4][frame], sizeof(GLTexData));
                       }
                   }
               }
           }
       }
    // More hacks on minus sign.
    WiMinus.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIMINUS", PU_CACHE), &WiMinus, true);

    for (i = 0; i < 10;i++)
        {
         // numbers 0-9
         sprintf(name, "WINUM%d", i);     
         WiNum[i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &WiNum[i], true);
        }

    // percent sign
    Percent.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIPCNT", PU_CACHE), &Percent, true);

    // "finished"
    Finished.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIF", PU_CACHE), &Finished, true);

    // "entering"
    Entering.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIENTER", PU_CACHE), &Entering, true);

    // "kills"
    Kills.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIOSTK", PU_CACHE), &Kills, true);

    // "scrt"
    Secret.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIOSTS", PU_CACHE), &Secret, true);

     // "secret"
    SP_Secret.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WISCRT2", PU_CACHE), &SP_Secret, true);

    // Yuck. 
    if (language == french)
       {
        // "items"
        if (netgame && !deathmatch)
            Items.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIOBJ", PU_CACHE), &Items, true);
        else
            Items.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIOSTI", PU_CACHE), &Items, true);
       }
    else
        Items.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIOSTI", PU_CACHE), &Items, true);

    // "frgs"
    Frags.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIFRGS", PU_CACHE), &Frags, true);

    // ":"
    Colon.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WICOLON", PU_CACHE), &Colon, true);

    // "time"
    Time.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WITIME", PU_CACHE), &Time, true);

    // "sucks"
    Sucks.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WISUCKS", PU_CACHE), &Sucks, true);

    // "par"
    Par.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIPAR", PU_CACHE), &Par, true);

    // "killers" (vertical)
    Killers.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIKILRS", PU_CACHE), &Killers, true);

    // "victims" (horiz)
    Victims.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIVCTMS", PU_CACHE), &Victims, true);

    // "total"
    Total.TexName = GL_MakeSpriteTexture(W_CacheLumpName("WIMSTT", PU_CACHE), &Total, true);

    // your face
    Star.TexName = GL_MakeSpriteTexture(W_CacheLumpName("STFST01", PU_CACHE), &Star, true);

    // dead face
    BStar.TexName = GL_MakeSpriteTexture(W_CacheLumpName("STFDEAD0", PU_CACHE), &BStar, true);

    for (i = 0; i < MAXPLAYERS; i++)
       {
        // "1,2,3,4"
        sprintf(name, "STPB%d", i);      
        gl_p[i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &gl_p[i], true);

        // "1,2,3,4"
        sprintf(name, "WIBP%d", i+1);     
        gl_bp[i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &gl_bp[i], true);
       }
   }

void WI_loadData(void)
{
    int		i;
    int		j;
    char	name[9];
    anim_t*	a;

    if (gamemode == commercial)
       {
	strcpy(name, "INTERPIC");
        glBackGround = Interpic;
       }
    else 
       {
	    sprintf(name, "WIMAP%d", wbs->epsd);
        glBackGround = GameMap[wbs->epsd];
       }
    
    if ( gamemode == retail )
       {
        if (wbs->epsd == 3)
           {
            strcpy(name,"INTERPIC");
            glBackGround = Interpic;
           }
       }

    // background
    bg = W_CacheLumpName(name, PU_CACHE);
    V_DrawPatch(((SCREENWIDTH-320)/2)+0, ((SCREENHEIGHT-200)/2)+0, 1, bg);


    // UNUSED unsigned char *pic = screens[1];
    // if (gamemode == commercial)
    // {
    // darken the background image
    // while (pic != screens[1] + SCREENHEIGHT*SCREENWIDTH)
    // {
    //   *pic = colormaps[256*25 + *pic];
    //   pic++;
    // }
    //}

    if (gamemode == commercial)
    {
	//NUMCMAPS = 32;								
	lnames = (patch_t **) Z_Malloc(sizeof(patch_t*) * NUMCMAPS,
				       PU_STATIC, 0);
	for (i=0 ; i<NUMCMAPS ; i++)
	{								
	    sprintf(name, "CWILV%2.2d", i);
	    lnames[i] = W_CacheLumpName(name, PU_STATIC);
	}					
    }
    else
    {
	lnames = (patch_t **) Z_Malloc(sizeof(patch_t*) * NUMMAPS,
				       PU_STATIC, 0);
	for (i=0 ; i<NUMMAPS ; i++)
	{
	    sprintf(name, "WILV%d%d", wbs->epsd, i);
	    lnames[i] = W_CacheLumpName(name, PU_STATIC);
	}

	// you are here
	yah[0] = W_CacheLumpName("WIURH0", PU_STATIC);

	// you are here (alt.)
	yah[1] = W_CacheLumpName("WIURH1", PU_STATIC);

	// splat
	splat = W_CacheLumpName("WISPLAT", PU_STATIC); 
	
	if (wbs->epsd < 3)
	{
	    for (j=0;j<NUMANIMS[wbs->epsd];j++)
	    {
		a = &anims[wbs->epsd][j];
		for (i=0;i<a->nanims;i++)
		{
		    // MONDO HACK!
		    if (wbs->epsd != 1 || j != 8) 
		    {
			// animations
			sprintf(name, "WIA%d%.2d%.2d", wbs->epsd, j, i);  
			a->p[i] = W_CacheLumpName(name, PU_STATIC);
		    }
		    else
		    {
			// HACK ALERT!
			a->p[i] = anims[1][4].p[i]; 
		    }
		}
	    }
	}
    }

    // More hacks on minus sign.
    wiminus = W_CacheLumpName("WIMINUS", PU_STATIC); 

    for (i=0;i<10;i++)
    {
	 // numbers 0-9
	sprintf(name, "WINUM%d", i);     
	num[i] = W_CacheLumpName(name, PU_STATIC);
    }

    // percent sign
    percent = W_CacheLumpName("WIPCNT", PU_STATIC);

    // "finished"
    finished = W_CacheLumpName("WIF", PU_STATIC);

    // "entering"
    entering = W_CacheLumpName("WIENTER", PU_STATIC);

    // "kills"
    kills = W_CacheLumpName("WIOSTK", PU_STATIC);   

    // "scrt"
    secret = W_CacheLumpName("WIOSTS", PU_STATIC);

     // "secret"
    sp_secret = W_CacheLumpName("WISCRT2", PU_STATIC);

    // Yuck. 
    if (language == french)
    {
	// "items"
	if (netgame && !deathmatch)
	    items = W_CacheLumpName("WIOBJ", PU_STATIC);    
  	else
	    items = W_CacheLumpName("WIOSTI", PU_STATIC);
    } else
	items = W_CacheLumpName("WIOSTI", PU_STATIC);

    // "frgs"
    frags = W_CacheLumpName("WIFRGS", PU_STATIC);    

    // ":"
    colon = W_CacheLumpName("WICOLON", PU_STATIC); 

    // "time"
    time = W_CacheLumpName("WITIME", PU_STATIC);   

    // "sucks"
    sucks = W_CacheLumpName("WISUCKS", PU_STATIC);  

    // "par"
    par = W_CacheLumpName("WIPAR", PU_STATIC);   

    // "killers" (vertical)
    killers = W_CacheLumpName("WIKILRS", PU_STATIC);

    // "victims" (horiz)
    victims = W_CacheLumpName("WIVCTMS", PU_STATIC);

    // "total"
    total = W_CacheLumpName("WIMSTT", PU_STATIC);   

    // your face
    star = W_CacheLumpName("STFST01", PU_STATIC);

    // dead face
    bstar = W_CacheLumpName("STFDEAD0", PU_STATIC);    

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	// "1,2,3,4"
	sprintf(name, "STPB%d", i);      
	p[i] = W_CacheLumpName(name, PU_STATIC);

	// "1,2,3,4"
	sprintf(name, "WIBP%d", i+1);     
	bp[i] = W_CacheLumpName(name, PU_STATIC);
    }

}

void WI_unloadData(void)
{
    int		i;
    int		j;

    Z_ChangeTag(wiminus, PU_CACHE);

    for (i=0 ; i<10 ; i++)
	Z_ChangeTag(num[i], PU_CACHE);
    
    if (gamemode == commercial)
    {
  	for (i=0 ; i<NUMCMAPS ; i++)
	    Z_ChangeTag(lnames[i], PU_CACHE);
    }
    else
    {
	Z_ChangeTag(yah[0], PU_CACHE);
	Z_ChangeTag(yah[1], PU_CACHE);

	Z_ChangeTag(splat, PU_CACHE);

	for (i=0 ; i<NUMMAPS ; i++)
	    Z_ChangeTag(lnames[i], PU_CACHE);
	
	if (wbs->epsd < 3)
	{
	    for (j=0;j<NUMANIMS[wbs->epsd];j++)
	    {
		if (wbs->epsd != 1 || j != 8)
		    for (i=0;i<anims[wbs->epsd][j].nanims;i++)
			Z_ChangeTag(anims[wbs->epsd][j].p[i], PU_CACHE);
	    }
	}
    }
    
    Z_Free(lnames);

    Z_ChangeTag(percent, PU_CACHE);
    Z_ChangeTag(colon, PU_CACHE);
    Z_ChangeTag(finished, PU_CACHE);
    Z_ChangeTag(entering, PU_CACHE);
    Z_ChangeTag(kills, PU_CACHE);
    Z_ChangeTag(secret, PU_CACHE);
    Z_ChangeTag(sp_secret, PU_CACHE);
    Z_ChangeTag(items, PU_CACHE);
    Z_ChangeTag(frags, PU_CACHE);
    Z_ChangeTag(time, PU_CACHE);
    Z_ChangeTag(sucks, PU_CACHE);
    Z_ChangeTag(par, PU_CACHE);

    Z_ChangeTag(victims, PU_CACHE);
    Z_ChangeTag(killers, PU_CACHE);
    Z_ChangeTag(total, PU_CACHE);
    //  Z_ChangeTag(star, PU_CACHE);
    //  Z_ChangeTag(bstar, PU_CACHE);
    
    for (i=0 ; i<MAXPLAYERS ; i++)
	Z_ChangeTag(p[i], PU_CACHE);

    for (i=0 ; i<MAXPLAYERS ; i++)
	Z_ChangeTag(bp[i], PU_CACHE);
}

void GL_DrawFullScreen(GLTexData *Image);

void GL_WI_Drawer (void)
   {
    GL_DrawFullScreen(glBackGround);
    switch (state)
       {
        case StatCount:
             if (deathmatch)
                 GL_WI_drawDeathmatchStats();
             else if (netgame)
                 GL_WI_drawNetgameStats();
             else
                 GL_WI_drawStats();
             break;
	
        case ShowNextLoc:
             GL_WI_drawShowNextLoc();
             break;
	
//        case NoState:
//             WI_drawNoState();
//             break;
       }
   }


void WI_Drawer (void)
{
    switch (state)
    {
      case StatCount:
	if (deathmatch)
	    WI_drawDeathmatchStats();
	else if (netgame)
	    WI_drawNetgameStats();
	else
	    WI_drawStats();
	break;
	
      case ShowNextLoc:
	WI_drawShowNextLoc();
	break;
	
      case NoState:
	WI_drawNoState();
	break;
    }
}


void WI_initVariables(wbstartstruct_t* wbstartstruct)
{

    wbs = wbstartstruct;

#ifdef RANGECHECKING
    if (gamemode != commercial)
    {
      if ( gamemode == retail )
	RNGCHECK(wbs->epsd, 0, 3);
      else
	RNGCHECK(wbs->epsd, 0, 2);
    }
    else
    {
	RNGCHECK(wbs->last, 0, 8);
	RNGCHECK(wbs->next, 0, 8);
    }
    RNGCHECK(wbs->pnum, 0, MAXPLAYERS);
    RNGCHECK(wbs->pnum, 0, MAXPLAYERS);
#endif

    acceleratestage = 0;
    cnt = bcnt = 0;
    firstrefresh = 1;
    me = wbs->pnum;
    plrs = wbs->plyr;

    if (!wbs->maxkills)
	wbs->maxkills = 1;

    if (!wbs->maxitems)
	wbs->maxitems = 1;

    if (!wbs->maxsecret)
	wbs->maxsecret = 1;

    if ( gamemode != retail )
      if (wbs->epsd > 2)
	wbs->epsd -= 3;
}

void WI_Start(wbstartstruct_t* wbstartstruct)
{

    WI_initVariables(wbstartstruct);
    WI_loadData();

    if (deathmatch)
	WI_initDeathmatchStats();
    else if (netgame)
	WI_initNetgameStats();
    else
	WI_initStats();
}
