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
//	Game completion, final screen animation.
//
//-----------------------------------------------------------------------------


//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <glad/glad.h>

static const char
rcsid[] = "$Id: f_finale.c,v 1.5 1997/02/03 21:26:34 b1 Exp $";

#include <ctype.h>

// Functions.
#include "i_system.h"
#include "m_swap.h"
#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"
#include "s_sound.h"

// Data.
#include "hu_stuff.h"
#include "dstrings.h"
#include "sounds.h"

#include "doomstat.h"
#include "r_state.h"

#include "gldefs.h"
#include "gl_utils.h"
#include "sys_sdl.h"

void GL_DrawFullScreen(GLTexData *Image);
void GL_DrawPatch(GLTexData *Tex, float x, float y);

// Fullscreen textures - 2 parts
extern GLTexData glHelp2[2], glCredit[2];
GLTexData        glVictory2[2], glEndPic[2], glBossBack[2];
GLTexData        glPFub1[2], glPFub2[2];

// Plates to go on the screen
GLTexData         glEndX[7];
// Flat to be tiled onto the screen
DW_TexList       *glFinaleFlat;


// HUD font information
extern GLTexData  GLHudFont[HU_FONTSIZE];
extern float GLFontHeight;

extern DW_TexList TexList[1024];
extern int        TexCount;
extern int        ftranslate[1024];
extern dboolean       TexTransparent;

// ?
//#include "doomstat.h"
//#include "r_local.h"
//#include "f_finale.h"

// Stage of animation:
//  0 = text, 1 = art screen, 2 = character cast
int		finalestage;

int		finalecount;

#define	TEXTSPEED	3
#define	TEXTWAIT	250

char*	e1text = E1TEXT;
char*	e2text = E2TEXT;
char*	e3text = E3TEXT;
char*	e4text = E4TEXT;

char*	c1text = C1TEXT;
char*	c2text = C2TEXT;
char*	c3text = C3TEXT;
char*	c4text = C4TEXT;
char*	c5text = C5TEXT;
char*	c6text = C6TEXT;

char*	p1text = P1TEXT;
char*	p2text = P2TEXT;
char*	p3text = P3TEXT;
char*	p4text = P4TEXT;
char*	p5text = P5TEXT;
char*	p6text = P6TEXT;

char*	t1text = T1TEXT;
char*	t2text = T2TEXT;
char*	t3text = T3TEXT;
char*	t4text = T4TEXT;
char*	t5text = T5TEXT;
char*	t6text = T6TEXT;

char*	finaletext;
char*	finaleflat;

void	F_StartCast (void);
void	F_CastTicker (void);
dboolean F_CastResponder (event_t *ev);
void	F_CastDrawer (void);

extern dboolean tnt, plutonia;

//
// F_StartFinale
//
void F_StartFinale (void)
   {
    int lump, texnumb;

    gameaction = ga_nothing;
    gamestate = GS_FINALE;
    viewactive = false;
    automapactive = false;

    // Okay - IWAD dependent stuff.
    // This has been changed severly, and
    //  some stuff might have changed in the process.
    switch ( gamemode )
    {

      // DOOM 1 - E1, E3 or E4, but each nine missions
      case shareware:
      case registered:
      case retail:
      {
	S_ChangeMusic(mus_victor, true);
	
	switch (gameepisode)
	{
	  case 1:
	    finaleflat = "FLOOR4_8";
	    finaletext = e1text;
	    break;
	  case 2:
	    finaleflat = "SFLR6_1";
	    finaletext = e2text;
	    break;
	  case 3:
	    finaleflat = "MFLR8_4";
	    finaletext = e3text;
	    break;
	  case 4:
	    finaleflat = "MFLR8_3";
	    finaletext = e4text;
	    break;
	  default:
	    // Ouch.
	    break;
	}
	break;
      }
      
      // DOOM II and missions packs with E1, M34
      case commercial:
      {
	  S_ChangeMusic(mus_read_m, true);

	  switch (gamemap)
	  {
	    case 6:
	      finaleflat = "SLIME16";
          if (tnt == true)
              finaletext = t1text;
          else
          if (plutonia == true)
              finaletext = p1text;
          else
	      finaletext = c1text;
	      break;
	    case 11:
	      finaleflat = "RROCK14";
          if (tnt == true)
              finaletext = t2text;
          else
          if (plutonia == true)
              finaletext = p2text;
          else
	      finaletext = c2text;
	      break;
	    case 20:
	      finaleflat = "RROCK07";
          if (tnt == true)
              finaletext = t3text;
          else
          if (plutonia == true)
              finaletext = p3text;
          else
	      finaletext = c3text;
	      break;
	    case 30:
	      finaleflat = "RROCK17";
          if (tnt == true)
              finaletext = t4text;
          else
          if (plutonia == true)
              finaletext = p4text;
          else
	      finaletext = c4text;
	      break;
	    case 15:
	      finaleflat = "RROCK13";
          if (tnt == true)
              finaletext = t5text;
          else
          if (plutonia == true)
              finaletext = p5text;
          else
	      finaletext = c5text;
	      break;
	    case 31:
	      finaleflat = "RROCK19";
          if (tnt == true)
              finaletext = t6text;
          else
          if (plutonia == true)
              finaletext = p6text;
          else
	      finaletext = c6text;
	      break;
	    default:
	      // Ouch.
	      break;
	  }
	  break;
      }	

   
      // Indeterminate.
      default:
	S_ChangeMusic(mus_read_m, true);
	finaleflat = "F_SKY1"; // Not used anywhere else.
	finaletext = c1text;  // FIXME - other text, music?
	break;
    }
    
    lump = W_CheckNumForName(finaleflat);
    for (texnumb = 0; texnumb < TexCount; texnumb++)
       {
        if (((TexList[texnumb].Number+firstflat) == lump) && (TexList[texnumb].Type == 'F'))
           {
            glFinaleFlat = &TexList[texnumb];
            break;
           }
       }
    if (texnumb == TexCount)
       {
        lump -= firstflat;
        ftranslate[lump] = TexCount;
        TexList[TexCount].Number = lump;
        TexList[TexCount].DWide = 64;
        TexList[TexCount].DHigh = 64;
        TexList[TexCount].Type = 'F';
        TexList[TexCount].glTexture = GL_LoadFlatTexture(TexCount);
        TexList[TexCount].Transparent = TexTransparent;
        TexCount++;
        glFinaleFlat = &TexList[texnumb];
       }

    finalestage = 0;
    finalecount = 0;
	
}



dboolean F_Responder (event_t *event)
{
    if (finalestage == 2)
	return F_CastResponder (event);
	
    return false;
}


//
// F_Ticker
//
void F_Ticker (void)
{
    int		i;
    
    // check for skipping
    if ( (gamemode == commercial)
      && ( finalecount > 50) )
    {
      // go on to the next level
      for (i=0 ; i<MAXPLAYERS ; i++)
	if (players[i].cmd.buttons)
	  break;
				
      if (i < MAXPLAYERS)
      {	
	if (gamemap == 30)
	  F_StartCast ();
	else
	  gameaction = ga_worlddone;
      }
    }
    
    // advance animation
    finalecount++;
	
    if (finalestage == 2)
    {
	F_CastTicker ();
	return;
    }
	
    if ( gamemode == commercial)
	return;
		
    if (!finalestage && (unsigned)finalecount>strlen (finaletext)*TEXTSPEED + TEXTWAIT)
    {
	finalecount = 0;
	finalestage = 1;
	wipegamestate = -1;		// force a wipe
	if (gameepisode == 3)
	    S_StartMusic (mus_bunny);
    }
}



//
// F_TextWrite
//
void GL_WriteText( int x, int y, char *string);
extern float SetBack;

#include "hu_stuff.h"
extern	patch_t *hu_font[HU_FONTSIZE];

// This will take a flat ONLY
void GL_TileBackground(DW_TexList *tex)
   {
    float   rtc, ttc;
    
    rtc = 320.0f/64.0f;
    ttc = 240.0f/64.0f;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex->glTexture);
    glBegin(GL_QUADS);
       glTexCoord2f( 0.0f, ttc);
       glVertex3f( -160.0f, 120.0f, SetBack);
       glTexCoord2f( 0.0f, 0.0f);
       glVertex3f( -160.0f,-120.0f, SetBack);
       glTexCoord2f( rtc, 0.0f);
       glVertex3f(  160.0f,-120.0f, SetBack);
       glTexCoord2f( rtc, ttc);
       glVertex3f(  160.0f, 120.0f, SetBack);
    glEnd();

    glDisable(GL_TEXTURE_2D);
   }

void GL_F_TextWrite (void)
   {
    
    float		x,y;
    int		count;
    char*	ch;
    int		c;
    float		cx, cy;
    float   Left, Bottom, Top, Right;

    GL_TileBackground( glFinaleFlat );

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    x = 10.0f;
    y = 10.0f;

    // draw some of the text onto the screen
    cx = x;
    cy = y;
    ch = finaletext;
	
    count = (finalecount - 10)/TEXTSPEED;
    if (count < 0)
        count = 0;

    for ( ; count ; count--)
       {
        c = *ch++;
        if (!c)
            break;

        if (c == '\n')
           {
            cx = x;
            cy += (GLFontHeight*1.2f);
            continue;
           }
		
        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c >= HU_FONTSIZE)
           {
            cx += 4;
            continue;
           }
		
        if (cx+GLHudFont[c].Width > SCREENWIDTH)
            break;

        Top = (120.0f-(((cy-GLHudFont[c].TopOff)-3)*1.2f));
        Bottom = Top-(GLHudFont[c].Height*1.2f);

        Left = (-160.0f+cx);
        Right = Left + GLHudFont[c].Width;

        glBindTexture(GL_TEXTURE_2D, GLHudFont[c].TexName);
        glBegin(GL_QUADS);
          glTexCoord2f( 0.0f, 1.0f);
          glVertex3f( Left, Top, SetBack);
          glTexCoord2f( 0.0f, GLHudFont[c].YDisp);
          glVertex3f( Left, Bottom, SetBack);
          glTexCoord2f( GLHudFont[c].XDisp, GLHudFont[c].YDisp);
          glVertex3f( Right, Bottom, SetBack);
          glTexCoord2f( GLHudFont[c].XDisp, 1.0f);
          glVertex3f( Right, Top, SetBack);
        glEnd();

        cx += GLHudFont[c].Width;
       }
    glDisable(GL_TEXTURE_2D);
   }

void F_TextWrite (void)
   {
    byte*	src;
    byte*	dest;
    
    int		x,y,w;
    int		count;
    char*	ch;
    int		c;
    int		cx;
    int		cy;
    
    // erase the entire screen to a tiled background
    src = W_CacheLumpName ( finaleflat , PU_CACHE);
    dest = screens[0];
	
    for (y = 0; y < SCREENHEIGHT; y++)
       {
        for (x = 0; x < SCREENWIDTH/64; x++)
           {
            memcpy (dest, src+((y&63)<<6), 64);
            dest += 64;
           }
       if (SCREENWIDTH&63)
          {
           memcpy (dest, src+((y&63)<<6), SCREENWIDTH&63);
           dest += (SCREENWIDTH&63);
          }
      }

    V_MarkRect (0, 0, SCREENWIDTH, SCREENHEIGHT);
    
    // draw some of the text onto the screen
    cx = ((SCREENWIDTH-320)/2)+10;
    cy = ((SCREENHEIGHT-200)/2)+10;
    ch = finaletext;
	
    count = (finalecount - 10)/TEXTSPEED;
    if (count < 0)
        count = 0;
    for ( ; count ; count-- )
       {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
           {
            cx = ((SCREENWIDTH-320)/2)+10;
            cy += 11;
            continue;
           }
	
        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c> HU_FONTSIZE)
           {
            cx += 4;
            continue;
           }
	
        w = DSHORT (hu_font[c]->width);
        if (cx+w > SCREENWIDTH)
            break;
        V_DrawPatch(cx, cy, 0, hu_font[c]);
        cx += w;
       }
   }

//
// Final DOOM 2 animation
// Casting by id Software.
//   in order of appearance
//
typedef struct
{
    char		*name;
    mobjtype_t	type;
} castinfo_t;

castinfo_t	castorder[] = {
    {CC_ZOMBIE, MT_POSSESSED},
    {CC_SHOTGUN, MT_SHOTGUY},
    {CC_HEAVY, MT_CHAINGUY},
    {CC_IMP, MT_TROOP},
    {CC_DEMON, MT_SERGEANT},
    {CC_LOST, MT_SKULL},
    {CC_CACO, MT_HEAD},
    {CC_HELL, MT_KNIGHT},
    {CC_BARON, MT_BRUISER},
    {CC_ARACH, MT_BABY},
    {CC_PAIN, MT_PAIN},
    {CC_REVEN, MT_UNDEAD},
    {CC_MANCU, MT_FATSO},
    {CC_ARCH, MT_VILE},
    {CC_SPIDER, MT_SPIDER},
    {CC_CYBER, MT_CYBORG},
    {CC_HERO, MT_PLAYER},

    {NULL,0}
};

int		castnum;
int		casttics;
state_t*	caststate;
dboolean		castdeath;
int		castframes;
int		castonmelee;
dboolean		castattacking;


//
// F_StartCast
//
extern	gamestate_t     wipegamestate;


void F_StartCast (void)
{
    wipegamestate = -1;		// force a screen wipe
    castnum = 0;
    caststate = &states[mobjinfo[castorder[castnum].type].seestate];
    casttics = caststate->tics;
    castdeath = false;
    finalestage = 2;	
    castframes = 0;
    castonmelee = 0;
    castattacking = false;
    S_ChangeMusic(mus_evil, true);
}


//
// F_CastTicker
//
void F_CastTicker (void)
{
    int		st;
    int		sfx;
	
    if (--casttics > 0)
	return;			// not time to change state yet
		
    if (caststate->tics == -1 || caststate->nextstate == S_NULL)
    {
	// switch from deathstate to next monster
	castnum++;
	castdeath = false;
	if (castorder[castnum].name == NULL)
	    castnum = 0;
	if (mobjinfo[castorder[castnum].type].seesound)
	    S_StartSound (NULL, mobjinfo[castorder[castnum].type].seesound);
	caststate = &states[mobjinfo[castorder[castnum].type].seestate];
	castframes = 0;
    }
    else
    {
	// just advance to next state in animation
	if (caststate == &states[S_PLAY_ATK1])
	    goto stopattack;	// Oh, gross hack!
	st = caststate->nextstate;
	caststate = &states[st];
	castframes++;
	
	// sound hacks....
	switch (st)
	{
	  case S_PLAY_ATK1:	sfx = sfx_dshtgn; break;
	  case S_POSS_ATK2:	sfx = sfx_pistol; break;
	  case S_SPOS_ATK2:	sfx = sfx_shotgn; break;
	  case S_VILE_ATK2:	sfx = sfx_vilatk; break;
	  case S_SKEL_FIST2:	sfx = sfx_skeswg; break;
	  case S_SKEL_FIST4:	sfx = sfx_skepch; break;
	  case S_SKEL_MISS2:	sfx = sfx_skeatk; break;
	  case S_FATT_ATK8:
	  case S_FATT_ATK5:
	  case S_FATT_ATK2:	sfx = sfx_firsht; break;
	  case S_CPOS_ATK2:
	  case S_CPOS_ATK3:
	  case S_CPOS_ATK4:	sfx = sfx_shotgn; break;
	  case S_TROO_ATK3:	sfx = sfx_claw; break;
	  case S_SARG_ATK2:	sfx = sfx_sgtatk; break;
	  case S_BOSS_ATK2:
	  case S_BOS2_ATK2:
	  case S_HEAD_ATK2:	sfx = sfx_firsht; break;
	  case S_SKULL_ATK2:	sfx = sfx_sklatk; break;
	  case S_SPID_ATK2:
	  case S_SPID_ATK3:	sfx = sfx_shotgn; break;
	  case S_BSPI_ATK2:	sfx = sfx_plasma; break;
	  case S_CYBER_ATK2:
	  case S_CYBER_ATK4:
	  case S_CYBER_ATK6:	sfx = sfx_rlaunc; break;
	  case S_PAIN_ATK3:	sfx = sfx_sklatk; break;
	  default: sfx = 0; break;
	}
		
	if (sfx)
	    S_StartSound (NULL, sfx);
    }
	
    if (castframes == 12)
    {
	// go into attack frame
	castattacking = true;
	if (castonmelee)
	    caststate=&states[mobjinfo[castorder[castnum].type].meleestate];
	else
	    caststate=&states[mobjinfo[castorder[castnum].type].missilestate];
	castonmelee ^= 1;
	if (caststate == &states[S_NULL])
	{
	    if (castonmelee)
		caststate=
		    &states[mobjinfo[castorder[castnum].type].meleestate];
	    else
		caststate=
		    &states[mobjinfo[castorder[castnum].type].missilestate];
	}
    }
	
    if (castattacking)
    {
	if (castframes == 24
	    ||	caststate == &states[mobjinfo[castorder[castnum].type].seestate] )
	{
	  stopattack:
	    castattacking = false;
	    castframes = 0;
	    caststate = &states[mobjinfo[castorder[castnum].type].seestate];
	}
    }
	
    casttics = caststate->tics;
    if (casttics == -1)
	casttics = 15;
}


//
// F_CastResponder
//

dboolean F_CastResponder (event_t* ev)
{
    if (ev->type != ev_keydown)
	return false;
		
    if (castdeath)
	return true;			// already in dying frames
		
    // go into death frame
    castdeath = true;
    caststate = &states[mobjinfo[castorder[castnum].type].deathstate];
    casttics = caststate->tics;
    castframes = 0;
    castattacking = false;
    if (mobjinfo[castorder[castnum].type].deathsound)
	S_StartSound (NULL, mobjinfo[castorder[castnum].type].deathsound);
	
    return true;
}


void F_CastPrint (char* text)
{
    char*	ch;
    int		c;
    int		cx;
    int		w;
    int		width;
    
    // find width
    ch = text;
    width = 0;
	
    while (ch)
    {
	c = *ch++;
	if (!c)
	    break;
	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    width += 4;
	    continue;
	}
		
	w = DSHORT (hu_font[c]->width);
	width += w;
    }
    
    // draw it
    cx = (SCREENWIDTH/2)-width/2;
    ch = text;
    while (ch)
    {
	c = *ch++;
	if (!c)
	    break;
	c = toupper(c) - HU_FONTSTART;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    cx += 4;
	    continue;
	}
		
	w = DSHORT (hu_font[c]->width);
	V_DrawPatch(cx, ((SCREENHEIGHT-200)/2)+180, 0, hu_font[c]);
	cx+=w;
    }
	
}

extern GLTexData *SprData;

void GL_F_CastDrawer (void)
{
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump, ch;
    byte 		flip;
    int       TextWidth;
    char        c;
    float       fTop, fBottom, fOffset, x1, x2;
    
    // erase the entire screen to a background
    GL_DrawFullScreen(glBossBack);

    TextWidth = 0;
    ch = 0;
    for (ch = 0; castorder[castnum].name[ch]; ch++)
       {
        c = castorder[castnum].name[ch];
        if (c == '\0')
           break;
        c = toupper(c) - HU_FONTSTART;
        if ((c < 0) || (c > HU_FONTSIZE))
           {
            TextWidth += 4;
            continue;
           }
        TextWidth += (int)GLHudFont[c].Width;
       }
    GL_WriteText(160-(TextWidth/2), 180, castorder[castnum].name);
    //F_CastPrint (castorder[castnum].name);
    
    // draw the current frame in the middle of the screen
    sprdef = &sprites[caststate->sprite];
    sprframe = &sprdef->spriteframes[ caststate->frame & FF_FRAMEMASK];
    lump = sprframe->lump[0];
    flip = sprframe->flip[0];

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, SprData[lump].TexName);

    if (SprData[lump].Height != SprData[lump].TopOff)
       {
        fOffset = ((SprData[lump].Height - (SprData[lump].TopOff)) / 2.0f) - 2.0f;
        if (fOffset < 0.0f)
            fOffset = 0.0f;
       }
    else
       fOffset = 0.0f;
    fTop = (SprData[lump].Height - fOffset)-60;
    fBottom = fTop - SprData[lump].Height;
    if (flip == true)
       {
        x1 = SprData[lump].XDisp;
        x2 = 0.0f;
       }
    else
       {
        x1 = 0.0f;
        x2 = SprData[lump].XDisp;
       }
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    glBegin(GL_QUADS);
       glTexCoord2f( x1,  1.0f );
       glVertex3f( 0.0f-(SprData[lump].Width/2.0f),  fTop, -160.0f );
       glTexCoord2f( x1,  SprData[lump].YDisp );
       glVertex3f( 0.0f-(SprData[lump].Width/2.0f), fBottom, -160.0f );
       glTexCoord2f( x2,  SprData[lump].YDisp );
       glVertex3f(  0.0f+(SprData[lump].Width/2.0f), fBottom, -160.0f );
       glTexCoord2f( x2,  1.0f );
       glVertex3f(  0.0f+(SprData[lump].Width/2.0f),  fTop, -160.0f );
    glEnd();
}

//
// F_CastDrawer
//
void V_DrawPatchFlipped (int x, int y, int scrn, patch_t *patch);

void F_CastDrawer (void)
{
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    byte		flip;
    patch_t*		patch;
    
    // erase the entire screen to a background
    V_DrawPatch (((SCREENWIDTH-320)/2)+0,((SCREENHEIGHT-200)/2)+0,0, W_CacheLumpName ("BOSSBACK", PU_CACHE));

    F_CastPrint (castorder[castnum].name);
    
    // draw the current frame in the middle of the screen
    sprdef = &sprites[caststate->sprite];
    sprframe = &sprdef->spriteframes[ caststate->frame & FF_FRAMEMASK];
    lump = sprframe->lump[0];
    flip = sprframe->flip[0];
			
    patch = W_CacheLumpNum (lump+firstspritelump, PU_CACHE);
    if (flip)
	V_DrawPatchFlipped ((SCREENWIDTH/2),((SCREENHEIGHT-200)/2)+170,0,patch);
    else
	V_DrawPatch ((SCREENWIDTH/2),((SCREENHEIGHT-200)/2)+170,0,patch);
}


//
// F_DrawPatchCol
//
void
F_DrawPatchCol
( int		x,
  patch_t*	patch,
  int		col )
{
    column_t*	column;
    byte*	source;
    byte*	dest;
    byte*	desttop;
    int		count;
	
    column = (column_t *)((byte *)patch + DLONG(patch->columnofs[col]));
    desttop = screens[0]+x+(((SCREENHEIGHT-200)/2)*(SCREENWIDTH));

    // step through the posts in a column
    while (column->topdelta != 0xff )
    {
	source = (byte *)column + 3;
	dest = desttop + column->topdelta*SCREENWIDTH;
	count = column->length;
		
	while (count--)
	{
	    *dest = *source++;
	    dest += SCREENWIDTH;
	}
	column = (column_t *)(  (byte *)column + column->length + 4 );
    }
}


//
// F_BunnyScroll
//
void GL_F_BunnyScroll (void)
{
    int		scrolled;
    int		stage;
    static int	laststage;
    int     lx, rx;
    float   ltc, rtc;
    float      Top, Left, Right, Bottom;

    Top = 120.0f;
    Bottom = -120.0f;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //p1 = W_CacheLumpName ("PFUB2", PU_LEVEL);
    //p2 = W_CacheLumpName ("PFUB1", PU_LEVEL);

    //V_MarkRect (0, 0, SCREENWIDTH, SCREENHEIGHT);
	
    scrolled = 320 - (finalecount-230)/2;
    if (scrolled > 320)
        scrolled = 320;
    if (scrolled < 0)
        scrolled = 0;

    lx = scrolled;
    rx = 320 + scrolled;

    if (scrolled < 256)
       {
        lx = scrolled;
        // draw part 1 of pfub2
        Left = -160.0f;
        Right = -160.0f + (256.0f-scrolled);

        ltc = scrolled / 255.0f;
        rtc = 1.0f;

        glBindTexture(GL_TEXTURE_2D, glPFub2[0].TexName);

        glBegin(GL_QUADS);
           glTexCoord2f(ltc, 1.0f);
           glVertex3f(Left, Top, SetBack);

           glTexCoord2f(ltc, glPFub2[0].YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(rtc, glPFub2[0].YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(rtc, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();
       }
    if (scrolled < 320)
       {
        lx = scrolled;
        // draw part 2 of pfub2
        Left = -160.0f + (256.0f-scrolled);
        Right = -160.0f + (320.0f-scrolled);

        ltc = 1.0f-((Right-Left)/64.0f);
        rtc = 1.0f;

        glBindTexture(GL_TEXTURE_2D, glPFub2[1].TexName);

        glBegin(GL_QUADS);
           glTexCoord2f(ltc, 1.0f);
           glVertex3f(Left, Top, SetBack);

           glTexCoord2f(ltc, glPFub2[1].YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(rtc, glPFub2[1].YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(rtc, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();
       }
    if (scrolled > 1)
       {
        lx = scrolled;
        // draw part 1 of pfub1
        Left = 160.0f-scrolled;
        Right = Left + 256.0f;
        if (Right > 160.0f)
            Right = 160.0f;

        ltc = 0.0f;
        rtc = (Right-Left)/256.0f;

        glBindTexture(GL_TEXTURE_2D, glPFub1[0].TexName);

        glBegin(GL_QUADS);
           glTexCoord2f(ltc, 1.0f);
           glVertex3f(Left, Top, SetBack);

           glTexCoord2f(ltc, glPFub1[0].YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(rtc, glPFub1[0].YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(rtc, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();
       }
    if (scrolled > 256)
       {
        lx = scrolled;
        // draw part 2 of pfub1
        Left = 160.0f-(scrolled-256);
        Right = Left + 64.0f;
        if (Right > 160.0f)
            Right = 160.0f;

        ltc = 0.0f;
        rtc = (Right-Left)/64.0f;
        glBindTexture(GL_TEXTURE_2D, glPFub1[1].TexName);

        glBegin(GL_QUADS);
           glTexCoord2f(ltc, 1.0f);
           glVertex3f(Left, Top, SetBack);
 
           glTexCoord2f(ltc, glPFub1[1].YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(rtc, glPFub1[1].YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(rtc, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();
       }

/*
    for (x = 0, xx = 0; x < 320; x++, xx++)
       {
	    if (xx + scrolled < 320)
	        F_DrawPatchCol (x, p1, xx+scrolled);
	    else
	        F_DrawPatchCol (x, p2, xx+scrolled - 320);		
        }
*/	
    if (finalecount < 1130)
	    return;
    if (finalecount < 1180)
       {
	    GL_DrawPatch(&glEndX[0], (float)(SCREENWIDTH-13*8)/2,(float)(SCREENHEIGHT-8*8)/2);
	    laststage = 0;
	    return;
       }
	
    stage = (finalecount-1180) / 5;
    if (stage > 6)
	stage = 6;
    if (stage > laststage)
    {
	S_StartSound (NULL, sfx_pistol);
	laststage = stage;
    }
	
    GL_DrawPatch(&glEndX[stage], (float)(SCREENWIDTH-13*8)/2,(float)(SCREENHEIGHT-8*8)/2);
    //sprintf (name,"END%i",stage);
    //V_DrawPatch ((SCREENWIDTH-13*8)/2, (SCREENHEIGHT-8*8)/2,0, W_CacheLumpName (name,PU_CACHE));
}

//
// F_BunnyScroll
//
void F_BunnyScroll (void)
{
    int		scrolled;
    int		x, xx;
    patch_t*	p1;
    patch_t*	p2;
    char	name[10];
    int		stage;
    static int	laststage;

    p1 = W_CacheLumpName ("PFUB2", PU_LEVEL);
    p2 = W_CacheLumpName ("PFUB1", PU_LEVEL);

    V_MarkRect (0, 0, SCREENWIDTH, SCREENHEIGHT);
	
    scrolled = 320 - (finalecount-230)/2;
    if (scrolled > 320)
        scrolled = 320;
    if (scrolled < 0)
        scrolled = 0;

    //for (x = 0; x < SCREENWIDTH ; x++)
    for (x = ((SCREENWIDTH-320)/2), xx = 0; x < ((SCREENWIDTH-320)/2)+320; x++, xx++)
    {
	if (xx + scrolled < 320)
	    F_DrawPatchCol (x, p1, xx+scrolled);
	else
	    F_DrawPatchCol (x, p2, xx+scrolled - 320);		
    }
	
    if (finalecount < 1130)
	return;
    if (finalecount < 1180)
    {
	V_DrawPatch ((SCREENWIDTH-13*8)/2,
		     (SCREENHEIGHT-8*8)/2,0, W_CacheLumpName ("END0",PU_CACHE));
	laststage = 0;
	return;
    }
	
    stage = (finalecount-1180) / 5;
    if (stage > 6)
	stage = 6;
    if (stage > laststage)
    {
	S_StartSound (NULL, sfx_pistol);
	laststage = stage;
    }
	
    sprintf (name,"END%i",stage);
    V_DrawPatch ((SCREENWIDTH-13*8)/2, (SCREENHEIGHT-8*8)/2,0, W_CacheLumpName (name,PU_CACHE));
}


//
// F_Drawer
//
void GL_F_Drawer (void)
   {
    switch(finalestage)
       {
        case 2:
             GL_F_CastDrawer();
             break;
        case 0:
             GL_F_TextWrite();
             break;
        default:
             switch (gameepisode)
                {
                 case 1:
                      if ( gamemode == retail )
                         {
                          GL_DrawFullScreen(glCredit);
                         }
                      else
                         {
                          GL_DrawFullScreen(glHelp2);
                         }
	                  break;
	             case 2:
                      GL_DrawFullScreen(glVictory2);
                      break;
                 case 3:
                      GL_F_BunnyScroll();
                      break;
                 case 4:
                      GL_DrawFullScreen(glEndPic);
                      break;
                }
             
       }
   }

//
// F_Drawer
//
void F_Drawer (void)
{
    if (finalestage == 2)
    {
	F_CastDrawer ();
	return;
    }

    if (!finalestage)
	F_TextWrite ();
    else
    {
	switch (gameepisode)
	{
	  case 1:
	    if ( gamemode == retail )
	      V_DrawPatch (0,0,0,
			 W_CacheLumpName("CREDIT",PU_CACHE));
	    else
	      V_DrawPatch (0,0,0,
			 W_CacheLumpName("HELP2",PU_CACHE));
	    break;
	  case 2:
	    V_DrawPatch(0,0,0,
			W_CacheLumpName("VICTORY2",PU_CACHE));
	    break;
	  case 3:
	    F_BunnyScroll ();
	    break;
	  case 4:
	    V_DrawPatch (0,0,0,
			 W_CacheLumpName("ENDPIC",PU_CACHE));
	    break;
	}
    }
			
}

void F_Init()
   {
    int  i;
    char texname[10];

    if (gamemode != commercial)
       {
        if (gamemode != shareware)
           {
            GL_MakeScreenTexture(W_CacheLumpName("VICTORY2", PU_CACHE), glVictory2);
           }
        if (gamemode == retail)
           {
            GL_MakeScreenTexture(W_CacheLumpName("ENDPIC", PU_CACHE), glEndPic);
           }
        if (gamemode != shareware)
           {
            for (i = 0; i < 7; i++)
               {
                sprintf(texname, "END%d", i);
                glEndX[i].TexName = GL_MakeSpriteTexture(W_CacheLumpName(texname, PU_CACHE), &glEndX[i], true);
               }
            GL_MakeScreenTexture(W_CacheLumpName("PFUB2", PU_CACHE), glPFub2);
            GL_MakeScreenTexture(W_CacheLumpName("PFUB1", PU_CACHE), glPFub1);
           }
       }
    else
       {
        GL_MakeScreenTexture(W_CacheLumpName("BOSSBACK", PU_CACHE), glBossBack);
       }
   }

