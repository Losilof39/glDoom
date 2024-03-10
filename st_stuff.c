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
//	Status bar code.
//	Does the face/direction indicator animatin.
//	Does palette indicators as well (red pain/berserk, bright pickup)
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: st_stuff.c,v 1.6 1997/02/03 22:45:13 b1 Exp $";

//#include <windows.h>
//#include <gl/gl.h>
#include <glad/glad.h>
#include "renderer2D.h"
#include "v_video.h"

#include <stdio.h>

#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "m_random.h"
#include "w_wad.h"

#include "doomdef.h"

#include "g_game.h"

#include "st_stuff.h"
#include "st_lib.h"
#include "r_local.h"

#include "p_local.h"
#include "p_inter.h"

#include "am_map.h"
#include "m_cheat.h"

#include "s_sound.h"

// Needs access to LFB.
#include "v_video.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"
#include "sounds.h"

#include "gl_texture.h"
#include "glsbar.h"

//
// STATUS BAR DATA
//

// Palette indices.
// For damage/bonus red-/gold-shifts
#define STARTREDPALS		1
#define STARTBONUSPALS		9
#define NUMREDPALS			8
#define NUMBONUSPALS		4
// Radiation suit, green shift.
#define RADIATIONPAL		13

// N/256*100% probability
//  that the normal face state will change
#define ST_FACEPROBABILITY		96

// For Responder
#define ST_TOGGLECHAT		SDL_SCANCODE_RETURN

// Location of status bar
//#define ST_X				0
#define ST_X				((SCREENWIDTH-ST_WIDTH)/2)
//#define ST_X2				104
#define ST_X2				((SCREENWIDTH/2)-56)

//#define ST_FX  			143
#define ST_FX  			((SCREENWIDTH/2)-17)
//#define ST_FY  			169
#define ST_FY  			(SCREENHEIGHT-31)

// Should be set to patch width
//  for tall numbers later on
#define ST_TALLNUMWIDTH		(tallnum[0]->width)

// Number of status faces.
#define ST_NUMPAINFACES		5
#define ST_NUMSTRAIGHTFACES	3
#define ST_NUMTURNFACES		2
#define ST_NUMSPECIALFACES		3

#define ST_FACESTRIDE \
          (ST_NUMSTRAIGHTFACES+ST_NUMTURNFACES+ST_NUMSPECIALFACES)

#define ST_NUMEXTRAFACES		2

#define ST_NUMFACES \
          (ST_FACESTRIDE*ST_NUMPAINFACES+ST_NUMEXTRAFACES)

#define ST_TURNOFFSET		(ST_NUMSTRAIGHTFACES)
#define ST_OUCHOFFSET		(ST_TURNOFFSET + ST_NUMTURNFACES)
#define ST_EVILGRINOFFSET		(ST_OUCHOFFSET + 1)
#define ST_RAMPAGEOFFSET		(ST_EVILGRINOFFSET + 1)
#define ST_GODFACE			(ST_NUMPAINFACES*ST_FACESTRIDE)
#define ST_DEADFACE			(ST_GODFACE+1)

//#define ST_FACESX			143
#define ST_FACESX			((SCREENWIDTH/2)-12)
//#define ST_FACESY			168
#define ST_FACESY			(SCREENHEIGHT-32)

#define ST_EVILGRINCOUNT		(2*TICRATE)
#define ST_STRAIGHTFACECOUNT	(TICRATE/2)
#define ST_TURNCOUNT		(1*TICRATE)
#define ST_OUCHCOUNT		(1*TICRATE)
#define ST_RAMPAGEDELAY		(2*TICRATE)

#define ST_MUCHPAIN			20


// Location and size of statistics,
//  justified according to widget type.
// Problem is, within which space? STbar? Screen?
// Note: this could be read in by a lump.
//       Problem is, is the stuff rendered
//       into a buffer,
//       or into the frame buffer?

// AMMO number pos.
#define ST_AMMOWIDTH		3	
//#define ST_AMMOX			44
#define ST_AMMOX			((SCREENWIDTH/2)-116)
//#define ST_AMMOY			171
#define ST_AMMOY			(SCREENHEIGHT-29)

// HEALTH number pos.
#define ST_HEALTHWIDTH		3	
//#define ST_HEALTHX			90
#define ST_HEALTHX			((SCREENWIDTH/2)-70)
//#define ST_HEALTHY			171
#define ST_HEALTHY			(SCREENHEIGHT-29)

// Weapon pos.
//#define ST_ARMSX			111
#define ST_ARMSX			((SCREENWIDTH/2)-49)
#define ST_ARMSX2			((SCREENWIDTH/2)-57)
//#define ST_ARMSY			172
#define ST_ARMSY			(SCREENHEIGHT-28)
//#define ST_ARMSBGX			104
#define ST_ARMSBGX			((SCREENWIDTH/2)-56)
//#define ST_ARMSBGY			168
#define ST_ARMSBGY			(SCREENHEIGHT-32)
#define ST_ARMSXSPACE		12
#define ST_ARMSYSPACE		10

// Frags pos.
//#define ST_FRAGSX			138
#define ST_FRAGSX			((SCREENWIDTH/2)-22)
//#define ST_FRAGSY			171	
#define ST_FRAGSY			(SCREENHEIGHT-29)	
#define ST_FRAGSWIDTH		2

// ARMOR number pos.
#define ST_ARMORWIDTH		3
//#define ST_ARMORX			221
#define ST_ARMORX			((SCREENWIDTH/2)+61)
//#define ST_ARMORY			171
#define ST_ARMORY			(SCREENHEIGHT-29)

// Key icon positions.
#define ST_KEY0WIDTH		8
#define ST_KEY0HEIGHT		5
//#define ST_KEY0X			239
#define ST_KEY0X			((SCREENWIDTH/2)+79)
//#define ST_KEY0Y			171
#define ST_KEY0Y			(SCREENHEIGHT-29)
#define ST_KEY1WIDTH		ST_KEY0WIDTH
//#define ST_KEY1X			239
#define ST_KEY1X			((SCREENWIDTH/2)+79)
//#define ST_KEY1Y			181
#define ST_KEY1Y			(SCREENHEIGHT-19)
#define ST_KEY2WIDTH		ST_KEY0WIDTH
//#define ST_KEY2X			239
#define ST_KEY2X			((SCREENWIDTH/2)+79)
//#define ST_KEY2Y			191
#define ST_KEY2Y			(SCREENHEIGHT-9)

// Ammunition counter.
#define ST_AMMO0WIDTH		3
#define ST_AMMO0HEIGHT		6
//#define ST_AMMO0X			288
#define ST_AMMO0X			((SCREENWIDTH/2)+128)
//#define ST_AMMO0Y			173
#define ST_AMMO0Y			(SCREENHEIGHT-27)
#define ST_AMMO1WIDTH		ST_AMMO0WIDTH
//#define ST_AMMO1X			288
#define ST_AMMO1X			((SCREENWIDTH/2)+128)
//#define ST_AMMO1Y			179
#define ST_AMMO1Y			(SCREENHEIGHT-21)
#define ST_AMMO2WIDTH		ST_AMMO0WIDTH
//#define ST_AMMO2X			288
#define ST_AMMO2X			((SCREENWIDTH/2)+128)
//#define ST_AMMO2Y			191
#define ST_AMMO2Y			(SCREENHEIGHT-9)
#define ST_AMMO3WIDTH		ST_AMMO0WIDTH
//#define ST_AMMO3X			288
#define ST_AMMO3X			((SCREENWIDTH/2)+128)
//#define ST_AMMO3Y			185
#define ST_AMMO3Y			(SCREENHEIGHT-15)

// Indicate maximum ammunition.
// Only needed because backpack exists.
#define ST_MAXAMMO0WIDTH		3
#define ST_MAXAMMO0HEIGHT		5
//#define ST_MAXAMMO0X		314
#define ST_MAXAMMO0X		((SCREENWIDTH/2)+154)
//#define ST_MAXAMMO0Y		173
#define ST_MAXAMMO0Y		(SCREENHEIGHT-27)
#define ST_MAXAMMO1WIDTH		ST_MAXAMMO0WIDTH
//#define ST_MAXAMMO1X		314
#define ST_MAXAMMO1X		((SCREENWIDTH/2)+154)
//#define ST_MAXAMMO1Y		179
#define ST_MAXAMMO1Y		(SCREENHEIGHT-21)
#define ST_MAXAMMO2WIDTH		ST_MAXAMMO0WIDTH
//#define ST_MAXAMMO2X		314
#define ST_MAXAMMO2X		((SCREENWIDTH/2)+154)
//#define ST_MAXAMMO2Y		191
#define ST_MAXAMMO2Y		(SCREENHEIGHT-9)
#define ST_MAXAMMO3WIDTH		ST_MAXAMMO0WIDTH
//#define ST_MAXAMMO3X		314
#define ST_MAXAMMO3X		((SCREENWIDTH/2)+154)
//#define ST_MAXAMMO3Y		185
#define ST_MAXAMMO3Y		(SCREENHEIGHT-15)

// pistol
//#define ST_WEAPON0X			110 
#define ST_WEAPON0X			((SCREENWIDTH/2)-50)
//#define ST_WEAPON0Y			172
#define ST_WEAPON0Y			(SCREENHEIGHT-28)

// shotgun
//#define ST_WEAPON1X			122 
#define ST_WEAPON1X			((SCREENWIDTH/2)-38)
//#define ST_WEAPON1Y			172
#define ST_WEAPON1Y			(SCREENHEIGHT-28)

// chain gun
//#define ST_WEAPON2X			134 
#define ST_WEAPON2X			((SCREENWIDTH/2)-26)
//#define ST_WEAPON2Y			172
#define ST_WEAPON2Y			(SCREENHEIGHT-28)

// missile launcher
//#define ST_WEAPON3X			110 
#define ST_WEAPON3X			((SCREENWIDTH/2)-50) 
//#define ST_WEAPON3Y			181
#define ST_WEAPON3Y			(SCREENHEIGHT-19)

// plasma gun
//#define ST_WEAPON4X			122 
#define ST_WEAPON4X			((SCREENWIDTH/2)-38) 
//#define ST_WEAPON4Y			181
#define ST_WEAPON4Y			(SCREENHEIGHT-19)

 // bfg
//#define ST_WEAPON5X			134
#define ST_WEAPON5X			((SCREENWIDTH/2)-26)
//#define ST_WEAPON5Y			181
#define ST_WEAPON5Y			(SCREENHEIGHT-19)

// WPNS title
//#define ST_WPNSX			109 
#define ST_WPNSX			((SCREENWIDTH/2)-51)
//#define ST_WPNSY			191
#define ST_WPNSY			(SCREENHEIGHT-9)

 // DETH title
//#define ST_DETHX			109
#define ST_DETHX			((SCREENWIDTH/2)-51)
//#define ST_DETHY			191
#define ST_DETHY			(SCREENHEIGHT-9)

//Incoming messages window location
//UNUSED
// #define ST_MSGTEXTX	   (viewwindowx)
// #define ST_MSGTEXTY	   (viewwindowy+viewheight-18)
#define ST_MSGTEXTX			0
#define ST_MSGTEXTY			0
// Dimensions given in characters.
#define ST_MSGWIDTH			52
// Or shall I say, in lines?
#define ST_MSGHEIGHT		1

#define ST_OUTTEXTX			0
#define ST_OUTTEXTY			6

// Width, in characters again.
#define ST_OUTWIDTH			52 
 // Height, in lines. 
#define ST_OUTHEIGHT		1

#define ST_MAPWIDTH	\
    (strlen(mapnames[(gameepisode-1)*9+(gamemap-1)]))

#define ST_MAPTITLEX \
    (SCREENWIDTH - ST_MAPWIDTH * ST_CHATFONTWIDTH)

#define ST_MAPTITLEY		0
#define ST_MAPHEIGHT		1

	    
// main player in game
static player_t*	plyr; 

// ST_Start() has just been called
static dboolean		st_firsttime;

// used to execute ST_Init() only once
static int		veryfirsttime = 1;

// lump number for PLAYPAL
static int		lu_palette;

// used for timing
static unsigned int	st_clock;

// used for making messages go away
static int		st_msgcounter=0;

// used when in chat 
static st_chatstateenum_t	st_chatstate;

// whether in automap or first-person
static st_stateenum_t	st_gamestate;

// whether left-side main status bar is active
//static dboolean		st_statusbaron;
dboolean		st_statusbaron;

// whether status bar chat is active
static dboolean		st_chat;

// value of st_chat before message popped up
static dboolean		st_oldchat;

// whether chat window has the cursor on
static dboolean		st_cursoron;

// !deathmatch
static dboolean		st_notdeathmatch; 

// !deathmatch && st_statusbaron
static dboolean		st_armson;

// !deathmatch
static dboolean		st_fragson; 

// main bar left
//static patch_t*		sbar;
patch_t                *sbar;

// 0-9, tall numbers
static patch_t*		tallnum[10];

// tall % sign
static patch_t*		tallpercent;

// 0-9, short, yellow (,different!) numbers
static patch_t*		shortnum[10];

// 3 key-cards, 3 skulls
static patch_t*		keys[NUMCARDS]; 

// face status patches
patch_t*		faces[ST_NUMFACES];
//static patch_t*		faces[ST_NUMFACES];

// face background
static patch_t*		faceback;

 // main bar right
//static patch_t*		armsbg;
patch_t*		armsbg;

// weapon ownership patches
static patch_t*		arms[6][2]; 

// ready-weapon widget
//static st_number_t	w_ready;
st_number_t	w_ready;


 // in deathmatch only, summary of frags stats
static st_number_t	w_frags;

// health widget
//static st_percent_t	w_health;
st_percent_t	w_health;

// arms background
static st_binicon_t	w_armsbg; 


// weapon ownership widgets
static st_multicon_t	w_arms[6];

// face status widget
static st_multicon_t	w_faces; 

// keycard widgets
static st_multicon_t	w_keyboxes[3];

// armor widget
//static st_percent_t	w_armor;
st_percent_t	w_armor;

// ammo widgets
//static st_number_t	w_ammo[4];
st_number_t	w_ammo[4];

// max ammo widgets
//static st_number_t	w_maxammo[4]; 
st_number_t	w_maxammo[4]; 



 // number of frags so far in deathmatch
static int	st_fragscount;

// used to use appopriately pained face
static int	st_oldhealth = -1;

// used for evil grin
static dboolean	oldweaponsowned[NUMWEAPONS]; 

 // count until face changes
static int	st_facecount = 0;

// current face index, used by w_faces
//static int	st_faceindex = 0;
int	st_faceindex = 0;

// holds key-type for each key box on bar
static int	keyboxes[3]; 

// a random number per tick
static int	st_randomnumber;  



// Massive bunches of cheat shit
//  to keep it from being easy to figure them out.
// Yeah, right...
unsigned char	cheat_mus_seq[] =
{
    0xb2, 0x26, 0xb6, 0xae, 0xea, 1, 0, 0, 0xff
};

unsigned char	cheat_choppers_seq[] =
{
    0xb2, 0x26, 0xe2, 0x32, 0xf6, 0x2a, 0x2a, 0xa6, 0x6a, 0xea, 0xff // id...
};

unsigned char	cheat_god_seq[] =
{
    0xb2, 0x26, 0x26, 0xaa, 0x26, 0xff  // iddqd
};

unsigned char	cheat_ammo_seq[] =
{
    0xb2, 0x26, 0xf2, 0x66, 0xa2, 0xff	// idkfa
};

unsigned char	cheat_ammonokey_seq[] =
{
    0xb2, 0x26, 0x66, 0xa2, 0xff	// idfa
};


// Smashing Pumpkins Into Samml Piles Of Putried Debris. 
unsigned char	cheat_noclip_seq[] =
{
    0xb2, 0x26, 0xea, 0x2a, 0xb2,	// idspispopd
    0xea, 0x2a, 0xf6, 0x2a, 0x26, 0xff
};

//
unsigned char	cheat_commercial_noclip_seq[] =
{
    0xb2, 0x26, 0xe2, 0x36, 0xb2, 0x2a, 0xff	// idclip
}; 



unsigned char	cheat_powerup_seq[7][10] =
{
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x6e, 0xff }, 	// beholdv
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xea, 0xff }, 	// beholds
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xb2, 0xff }, 	// beholdi
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x6a, 0xff }, 	// beholdr
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xa2, 0xff }, 	// beholda
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0x36, 0xff }, 	// beholdl
    { 0xb2, 0x26, 0x62, 0xa6, 0x32, 0xf6, 0x36, 0x26, 0xff }		// behold
};


unsigned char	cheat_clev_seq[] =
{
    0xb2, 0x26,  0xe2, 0x36, 0xa6, 0x6e, 1, 0, 0, 0xff	// idclev
};


// my position cheat
unsigned char	cheat_mypos_seq[] =
{
    0xb2, 0x26, 0xb6, 0xba, 0x2a, 0xf6, 0xea, 0xff	// idmypos
}; 


// Now what?
cheatseq_t	cheat_mus = { cheat_mus_seq, 0 };
cheatseq_t	cheat_god = { cheat_god_seq, 0 };
cheatseq_t	cheat_ammo = { cheat_ammo_seq, 0 };
cheatseq_t	cheat_ammonokey = { cheat_ammonokey_seq, 0 };
cheatseq_t	cheat_noclip = { cheat_noclip_seq, 0 };
cheatseq_t	cheat_commercial_noclip = { cheat_commercial_noclip_seq, 0 };

cheatseq_t	cheat_powerup[7] =
{
    { cheat_powerup_seq[0], 0 },
    { cheat_powerup_seq[1], 0 },
    { cheat_powerup_seq[2], 0 },
    { cheat_powerup_seq[3], 0 },
    { cheat_powerup_seq[4], 0 },
    { cheat_powerup_seq[5], 0 },
    { cheat_powerup_seq[6], 0 }
};

cheatseq_t	cheat_choppers = { cheat_choppers_seq, 0 };
cheatseq_t	cheat_clev = { cheat_clev_seq, 0 };
cheatseq_t	cheat_mypos = { cheat_mypos_seq, 0 };


extern int gl_premalpha;

// 
extern char*	mapnames[];

extern video_t video;

//extern int face_tex[45], tallnums[10], shortnums[2][10], tallperc;

//
// STATUS BAR CODE
//
void ST_Stop(void);

void ST_refreshBackground(void)
{

    if (st_statusbaron)
    {
	V_DrawPatch(ST_X, 0, BG, sbar);

	if (netgame)
       {
	    V_DrawPatch(ST_FX, 0, BG, faceback);
       }

	V_CopyRect(ST_X, 0, BG, ST_WIDTH, ST_HEIGHT, ST_X, ST_Y, FG);
    }

}


// Respond to keyboard input events,
//  intercept cheats.
dboolean
ST_Responder (event_t* ev)
{
  int		i;
    
  // Filter automap on/off.
  if (ev->type == ev_keyup
      && ((ev->data1 & 0xffff0000) == AM_MSGHEADER))
  {
    switch(ev->data1)
    {
      case AM_MSGENTERED:
	st_gamestate = AutomapState;
	st_firsttime = true;
	break;
	
      case AM_MSGEXITED:
	//	fprintf(stderr, "AM exited\n");
	st_gamestate = FirstPersonState;
	break;
    }
  }

  // if a user keypress...
  else if (ev->type == ev_keydown)
  {
    if (!netgame)
    {
      // b. - enabled for more debug fun.
      // if (gameskill != sk_nightmare) {
      
      // 'dqd' cheat for toggleable god mode
      if (cht_CheckCheat(&cheat_god, ev->data1))
      {
	plyr->cheats ^= CF_GODMODE;
	if (plyr->cheats & CF_GODMODE)
	{
	  if (plyr->mo)
	    plyr->mo->health = 100;
	  
	  plyr->health = 100;
	  plyr->message = STSTR_DQDON;
	}
	else 
	  plyr->message = STSTR_DQDOFF;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'fa' cheat for killer fucking arsenal
      else if (cht_CheckCheat(&cheat_ammonokey, ev->data1))
      {
	plyr->armorpoints = 200;
	plyr->armortype = 2;
	
	for (i=0;i<NUMWEAPONS;i++)
	  plyr->weaponowned[i] = true;
	
	for (i=0;i<NUMAMMO;i++)
	  plyr->ammo[i] = plyr->maxammo[i];
	
	plyr->message = STSTR_FAADDED;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'kfa' cheat for key full ammo
      else if (cht_CheckCheat(&cheat_ammo, ev->data1))
      {
	plyr->armorpoints = 200;
	plyr->armortype = 2;
	
	for (i=0;i<NUMWEAPONS;i++)
	  plyr->weaponowned[i] = true;
	
	for (i=0;i<NUMAMMO;i++)
	  plyr->ammo[i] = plyr->maxammo[i];
	
	for (i=0;i<NUMCARDS;i++)
	  plyr->cards[i] = true;
	
	plyr->message = STSTR_KFAADDED;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'mus' cheat for changing music
      else
      if (cht_CheckCheat(&cheat_mus, ev->data1))
         {
          char   buf[3];
          int    musnum;
          char   maxep;
         
          plyr->message = STSTR_MUS;
          cht_GetParam(&cheat_mus, buf);

          if (gamemode == commercial)
             {
              if ((buf[0] >= '0' && buf[0] <= '3') &&
                  (buf[1] >= '0' && buf[1] <= '9') &&
                  (strncmp(buf, "00", 2) != 0))
                 {
                  musnum = mus_runnin + (buf[0]-'0')*10 + buf[1]-'0' - 1;
            
                  // German version only has 30 levels. NO id wad has more
                  // than 32 need a switch to tell how many are "valid" or
                  // better, a MAP counter...
                  if ((((buf[0]-'0')*10 + buf[1]-'0') - 1) > 31)
                      plyr->message = STSTR_NOMUS;
                  else
                      S_ChangeMusic(musnum, 1);
                 }
              else
                 {
                  plyr->message = STSTR_NOMUS;
                 }
             }
          else
             {
              switch(gamemode)
                 {
                  case shareware:
                       maxep = '1';
                       break;
                  case registered:
                       maxep = '3';
                       break;
                  case retail:
                       maxep = '4';
                       break;
                 }
              if ((buf[0] >= '1' && buf[0] <= maxep) &&
                  (buf[1] >= '1' && buf[1] <= '9'))
                 {
                  musnum = mus_e1m1 + (buf[0]-'1')*9 + (buf[1]-'1');
             
                  if (((buf[0]-'1')*9 + buf[1]-'1') > 35)
                      plyr->message = STSTR_NOMUS;
                  else
                      S_ChangeMusic(musnum, 1);
                 }
              else
                 {
                  plyr->message = STSTR_NOMUS;
                 }
             }
         }
      // Simplified, accepting both "noclip" and "idspispopd".
      // no clipping mode cheat
      else if ( cht_CheckCheat(&cheat_noclip, ev->data1) 
		|| cht_CheckCheat(&cheat_commercial_noclip,ev->data1) )
      {	
	plyr->cheats ^= CF_NOCLIP;
	
	if (plyr->cheats & CF_NOCLIP)
	  plyr->message = STSTR_NCON;
	else
	  plyr->message = STSTR_NCOFF;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'behold?' power-up cheats
      for (i=0;i<6;i++)
      {
	if (cht_CheckCheat(&cheat_powerup[i], ev->data1))
	{
	  if (!plyr->powers[i])
	    P_GivePower( plyr, i);
	  else if (i!=pw_strength)
	    plyr->powers[i] = 1;
	  else
	    plyr->powers[i] = 0;
	  
	  plyr->message = STSTR_BEHOLDX;
      totalscore = 0; keepscore = 0; showscoreHUD = 0;
	}
      }
      
      // 'behold' power-up menu
      if (cht_CheckCheat(&cheat_powerup[6], ev->data1))
      {
	plyr->message = STSTR_BEHOLD;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'choppers' invulnerability & chainsaw
      else if (cht_CheckCheat(&cheat_choppers, ev->data1))
      {
	plyr->weaponowned[wp_chainsaw] = true;
	plyr->powers[pw_invulnerability] = true;
	plyr->message = STSTR_CHOPPERS;
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
      }
      // 'mypos' for player position
      else if (cht_CheckCheat(&cheat_mypos, ev->data1))
      {
	static char	buf[ST_MSGWIDTH];
	sprintf(buf, "ang=0x%x;x,y=(0x%x,0x%x)",
		players[consoleplayer].mo->angle,
		players[consoleplayer].mo->x,
		players[consoleplayer].mo->y);
	plyr->message = buf;
      }
    }
    
    // 'clev' change-level cheat
    if (cht_CheckCheat(&cheat_clev, ev->data1))
    {
      char		buf[3];
      int		epsd;
      int		map;
      
      cht_GetParam(&cheat_clev, buf);
      
      if (gamemode == commercial)
         {
          epsd = 1;
          map = (buf[0] - '0')*10 + buf[1] - '0';
         }
      else
         {
          epsd = buf[0] - '0';
          map = buf[1] - '0';
         }

      // Catch invalid maps.
          // Catch invalid maps.
      if (gamemode != commercial)
      {
          if (epsd < 1)
          {
              return false;
          }
          if (epsd > 4)
          {
              // [crispy] Sigil
              if (!(haved1e5 && epsd == 5))
                  return false;
          }
          if (epsd == 4 && gamemode < retail)
          {
              return false;
          }
          if (map < 1)
          {
              return false;
          }
          if (map > 9)
          {
              return false;
          }
      }
      else
      {
          if (map < 1)
          {
              return false;
          }
          if (map > 40)
          {
              return false;
          }
      }


      // So be it.
      plyr->message = STSTR_CLEV;
      totalscore = 0; keepscore = 0; showscoreHUD = 0;
      G_DeferedInitNew(gameskill, epsd, map);
    }    
  }
  return false;
}



int ST_calcPainOffset(void)
{
    int		health;
    static int	lastcalc;
    static int	oldhealth = -1;
    
    health = plyr->health > 100 ? 100 : plyr->health;

    if (health != oldhealth)
    {
	lastcalc = ST_FACESTRIDE * (((100 - health) * ST_NUMPAINFACES) / 101);
	oldhealth = health;
    }
    return lastcalc;
}


//
// This is a not-very-pretty routine which handles
//  the face states and their timing.
// the precedence of expressions is:
//  dead > evil grin > turned head > straight ahead
//
void ST_updateFaceWidget(void)
{
    int		i;
    angle_t	badguyangle;
    angle_t	diffang;
    static int	lastattackdown = -1;
    static int	priority = 0;
    dboolean	doevilgrin;

    if (priority < 10)
    {
	// dead
	if (!plyr->health)
	{
	    priority = 9;
	    st_faceindex = ST_DEADFACE;
	    st_facecount = 1;
	}
    }

    if (priority < 9)
    {
	if (plyr->bonuscount)
	{
	    // picking up bonus
	    doevilgrin = false;

	    for (i=0;i<NUMWEAPONS;i++)
	    {
		if (oldweaponsowned[i] != plyr->weaponowned[i])
		{
		    doevilgrin = true;
		    oldweaponsowned[i] = plyr->weaponowned[i];
		}
	    }
	    if (doevilgrin) 
	    {
		// evil grin if just picked up weapon
		priority = 8;
		st_facecount = ST_EVILGRINCOUNT;
		st_faceindex = ST_calcPainOffset() + ST_EVILGRINOFFSET;
	    }
	}

    }
  
    if (priority < 8)
    {
	if (plyr->damagecount
	    && plyr->attacker
	    && plyr->attacker != plyr->mo)
	{
	    // being attacked
	    priority = 7;
	    
	    if (plyr->health - st_oldhealth > ST_MUCHPAIN)
	    {
		st_facecount = ST_TURNCOUNT;
		st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
	    }
	    else
	    {
		badguyangle = R_PointToAngle2(plyr->mo->x,
					      plyr->mo->y,
					      plyr->attacker->x,
					      plyr->attacker->y);
		
		if (badguyangle > plyr->mo->angle)
		{
		    // whether right or left
		    diffang = badguyangle - plyr->mo->angle;
		    i = diffang > ANG180; 
		}
		else
		{
		    // whether left or right
		    diffang = plyr->mo->angle - badguyangle;
		    i = diffang <= ANG180; 
		} // confusing, aint it?

		
		st_facecount = ST_TURNCOUNT;
		st_faceindex = ST_calcPainOffset();
		
		if (diffang < ANG45)
		{
		    // head-on    
		    st_faceindex += ST_RAMPAGEOFFSET;
		}
		else if (i)
		{
		    // turn face right
		    st_faceindex += ST_TURNOFFSET;
		}
		else
		{
		    // turn face left
		    st_faceindex += ST_TURNOFFSET+1;
		}
	    }
	}
    }
  
    if (priority < 7)
    {
	// getting hurt because of your own damn stupidity
	if (plyr->damagecount)
	{
	    if (plyr->health - st_oldhealth > ST_MUCHPAIN)
	    {
		priority = 7;
		st_facecount = ST_TURNCOUNT;
		st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
	    }
	    else
	    {
		priority = 6;
		st_facecount = ST_TURNCOUNT;
		st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
	    }

	}

    }
  
    if (priority < 6)
    {
	// rapid firing
	if (plyr->attackdown)
	{
	    if (lastattackdown==-1)
		lastattackdown = ST_RAMPAGEDELAY;
	    else if (!--lastattackdown)
	    {
		priority = 5;
		st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
		st_facecount = 1;
		lastattackdown = 1;
	    }
	}
	else
	    lastattackdown = -1;

    }
  
    if (priority < 5)
    {
	// invulnerability
	if ((plyr->cheats & CF_GODMODE)
	    || plyr->powers[pw_invulnerability])
	{
	    priority = 4;

	    st_faceindex = ST_GODFACE;
	    st_facecount = 1;

	}

    }

    // look left or look right if the facecount has timed out
    if (!st_facecount)
    {
	st_faceindex = ST_calcPainOffset() + (st_randomnumber % 3);
	st_facecount = ST_STRAIGHTFACECOUNT;
	priority = 0;
    }

    st_facecount--;

}

void ST_updateWidgets(void)
{
    static int	largeammo = 1994; // means "n/a"
    int		i;

    // must redirect the pointer if the ready weapon has changed.
    //  if (w_ready.data != plyr->readyweapon)
    //  {
    if (weaponinfo[plyr->readyweapon].ammo == am_noammo)
	w_ready.num = &largeammo;
    else
	w_ready.num = &plyr->ammo[weaponinfo[plyr->readyweapon].ammo];
    //{
    // static int tic=0;
    // static int dir=-1;
    // if (!(tic&15))
    //   plyr->ammo[weaponinfo[plyr->readyweapon].ammo]+=dir;
    // if (plyr->ammo[weaponinfo[plyr->readyweapon].ammo] == -100)
    //   dir = 1;
    // tic++;
    // }
    w_ready.data = plyr->readyweapon;

    // if (*w_ready.on)
    //  STlib_updateNum(&w_ready, true);
    // refresh weapon change
    //  }

    // update keycard multiple widgets
    for (i=0;i<3;i++)
    {
	keyboxes[i] = plyr->cards[i] ? i : -1;

	if (plyr->cards[i+3])
	    keyboxes[i] = i+3;
    }

    // refresh everything if this is him coming back to life
    ST_updateFaceWidget();

    // used by the w_armsbg widget
    st_notdeathmatch = !deathmatch;
    
    // used by w_arms[] widgets
    st_armson = st_statusbaron && !deathmatch; 

    // used by w_frags widget
    st_fragson = deathmatch && st_statusbaron; 
    st_fragscount = 0;

    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (i != consoleplayer)
	    st_fragscount += plyr->frags[i];
	else
	    st_fragscount -= plyr->frags[i];
    }

    // get rid of chat window if up because of message
    if (!--st_msgcounter)
	st_chat = st_oldchat;

}

void ST_Ticker (void)
{

    st_clock++;
    st_randomnumber = M_Random();
    ST_updateWidgets();
    st_oldhealth = plyr->health;

}

static int st_palette = 0;
dboolean       RedBias, GreenBias;

void ST_doPaletteStuff(void)
{

    int		palette;
    dbyte*	pal;
    int		cnt;
    int		bzc;

    cnt = plyr->damagecount;

    if (plyr->powers[pw_strength])
    {
	// slowly fade the berzerk out
  	bzc = 12 - (plyr->powers[pw_strength]>>6);

	if (bzc > cnt)
	    cnt = bzc;
    }
	
    RedBias = false;
    GreenBias = false;
    if (cnt)
    {
	palette = (cnt+7)>>3;
	
	if (palette >= NUMREDPALS)
	    palette = NUMREDPALS-1;

	palette += STARTREDPALS;
     RedBias = true;
    }
    else if (plyr->bonuscount)
    {
	palette = (plyr->bonuscount+7)>>3;

	if (palette >= NUMBONUSPALS)
	    palette = NUMBONUSPALS-1;

	palette += STARTBONUSPALS;
    }

    else if ( plyr->powers[pw_ironfeet] > 4*32
	      || plyr->powers[pw_ironfeet]&8)
       {
	palette = RADIATIONPAL;
        GreenBias = true;
       }
    else
       {
	palette = 0;
       }

    if (palette != st_palette)
    {
	st_palette = palette;
	pal = (dbyte *) W_CacheLumpNum (lu_palette, PU_CACHE)+palette*768;
	I_SetPalette (pal);
    }

}

// single frame sprites
// Boxes of ammo
GLTexData   glBBullet, glBRocket,  glBCells,  glBShot;
//          AMMOA0     BROKA0      CELPA0     SBOXA0
// Ammo packs
GLTexData   glBullet,  glRocket,   glCells,   glShot;
//          CLIPA0     ROCKA0      CELLA0     SHELA0
// Weapons  
GLTexData   glShot1,   glShot2,    glRLaunch, glPlasma,
//          SHOTA0     SGN2A0      LAUNA0     PLASA0
            glBFG,     glChainSaw, glMiniGun, glSTArms;
//          BFUGA0     CSAWA0      MGUNA0     STARMS
// Dying Player
GLTexData   glPlayFrag;
// Medical Kits
GLTexData   glStimPak, glMediKit;
//          STIMA0     MEDIA0
// Powerups
GLTexData   glBerserk,    glRadSuit;
//          PSTRA0        SUITA0

// animated sprites
// Armor Types
GLTexData   glGArmor[2], glBArmor[2];
// Keys
GLTexData   glYCKey[2], glBCKey[2], glRCKey[2];
GLTexData   glYSKey[2], glBSKey[2], glRSKey[2];
// Powerups
GLTexData   glAutoMapI[4], glInvul[4],      glInviso[4];
//          Automap       Invulnerability  Invisibility

GLTexData SBarTex;
GLTexData FaceTex[45], TallNums[10], TallPerc, ShortNums[2][10], KeyTex[6];

GLTexData       SBarTexture;

typedef struct
   {
    int  x, y;
   }Vertex2D;

Vertex2D v_arms[6] = { 110, 172, 122, 172, 134, 172,
                       110, 182, 122, 182, 134, 182 };
Vertex2D v_keys[3] = { 239, 171, 239, 181, 239, 191 };

extern float Scale;
extern float SetBack;

// Numbers are restricted to values under 1000 (three digits at most)
// Max number digit width is 11 (so space them at 11) (all are 12 high)
void GL_DrawLargeNum(int x, int y, int value, dboolean percent)
   {
    char numstr[4];
    int  digit;
    vec3 pos = { 0 };
    vec2 size = { 0 };

    numstr[0] = (value / 100) % 10;
    numstr[1] = (value % 100)/10;
    numstr[2] = value % 10;

    if (numstr[0] == 0)
       numstr[0] = ' ';
    if ((numstr[1] == 0) && (numstr[0] == ' '))
       numstr[1] = ' ';

    pos[0] = x;
    pos[1] = y;

    size[0] = 14;
    size[1] = 16;

    for (digit = 0; digit < 3; digit++)
       {
        if (numstr[digit] != ' ')
           {
            R2D_DrawSprite(pos, size, &TallNums[numstr[digit]]);
           }
        pos[0] += 14.0f;
       }

    if (percent == true)
       {
        R2D_DrawSprite(pos, size, &TallPerc);
       }
   }

// Numbers are restricted to values under 1000 (three digits at most)
// This draws the small yellow numbers...
// Will put a flag on here to draw yellow or grey...
void GL_DrawSmallNum(int x, int y, int value, int digits, dboolean yellow)
   {
    char numstr[4];
    int  digit, c;

    vec3 pos = { 0 };
    vec2 size = { 0 };

    c = 0;

    if (digits > 2)
       {
        numstr[c] = (value / 100) % 10;
        c++;
       }
    if (digits > 1)
       {
        numstr[c] = (value % 100)/10;
        c++;
       }
    numstr[c] = value % 10;
    c++;

    if ((numstr[0] == 0) && (digits > 1))
       numstr[0] = ' ';
    if ((numstr[1] == 0) && (numstr[0] == ' ') && (digits > 2))
       numstr[1] = ' ';

    pos[0] = x;
    pos[1] = y;

    size[0] = 4;
    size[1] = 6;

    for (digit = 0; digit < digits; digit++)
       {
        if (numstr[digit] != ' ')
           {
            /*if (yellow == true)
               {
                glBindTexture(GL_TEXTURE_2D, ShortNums[1][numstr[digit]].TexName);
               }
            else
               {
                glBindTexture(GL_TEXTURE_2D, ShortNums[0][numstr[digit]].TexName);
               }
               */

            R2D_DrawSprite(pos, size, &ShortNums[yellow][numstr[digit]]);
           }
        pos[0] += 4.0f;
       }
   }

void GL_DrawPatch(GLTexData *Tex, float x, float y);

int ammoxlate[] = { 0, 1, 3, 2 };

void GL_DrawStatusBar0()
   {
    int           atype, weapon, card;
    int          *num;

    vec3 pos_face = { ST_FX + 5, 200 - FaceTex[st_faceindex].glHeight, 0.0f };
    vec2 size_face = { FaceTex[st_faceindex].glWidth, FaceTex[st_faceindex].glHeight };

    vec3 pos_st = { 0.0f, 200.0f - SBarTexture.glHeight, 0.0f };
    vec2 size_st = { SBarTexture.glWidth, SBarTexture.glHeight };

    R2D_DrawSprite(pos_st, size_st, &SBarTexture);

    R2D_DrawSprite((vec3) { ST_ARMSX2, 200 - glSTArms.glHeight }, (vec2){ glSTArms.glWidth, glSTArms.glHeight }, &glSTArms);

    // Draw ready weapon ammo count (up to max ammo for weapon)
    if (w_ready.num != NULL)
       {
        if (*w_ready.num != 1994)
           {
            GL_DrawLargeNum( 2, 171, *w_ready.num, false);
           }
       }
    // Draw current health percentage (up to 200)
    if (w_health.n.num != NULL)
       {
        GL_DrawLargeNum( 48, 171, *w_health.n.num, true);
       }
    // Draw current armor percentage (up to 200)
    if (w_armor.n.num != NULL)
       {
        GL_DrawLargeNum( 179, 171, *w_armor.n.num, true);
       }
    for (atype = 0; atype < 4; atype++)
       {
        num = w_ammo[ammoxlate[atype]].num;
        if (num != NULL)
           {
            if (*num != 1994)
               {
                GL_DrawSmallNum( 276, 173+(atype*6), *num, 3, true);
               }
           }
        num = w_maxammo[ammoxlate[atype]].num;
        if (num != NULL)
           {
            if ((*num != 1994) && (*num < 1000))
               {
                GL_DrawSmallNum( 302, 173+(atype*6), *num, 3, true);
               }
           }
       }

    // Draw which keys the player has...
    for (card = 0; card < NUMCARDS; card++)
       {
        if (players[consoleplayer].cards[card] == true)
           {
            GL_DrawPatch(&KeyTex[card],  (float)v_keys[card%3].x, (float)v_keys[card%3].y);
           }
       }

    if (deathmatch == 0)
       {
        // Draw grey numbers for weapons we have not got.
        // Draw yellow numbers the ones we have got.
        for (weapon = 0; weapon < 6; weapon++)
           {
            if (players[consoleplayer].weaponowned[weapon+1] == true)
               {
                GL_DrawSmallNum( v_arms[weapon].x, v_arms[weapon].y, weapon+2, 1, true);
               }
            else
               {
                GL_DrawSmallNum( v_arms[weapon].x, v_arms[weapon].y, weapon, 1, false);
               }
           }
       }
    else
       {
        // Draw current frags
        if (w_frags.num != NULL)
           {
            GL_DrawLargeNum( 96, 171, *w_frags.num, false);
           }
       }

    R2D_DrawSprite(pos_face, size_face, &FaceTex[st_faceindex]);
    
   }

void GL_DrawStatusBar1()
   {
    int card;

    GL_DrawPatch( &glStimPak, 3.0f, ((float)video.height - glStimPak.glHeight)/1.2f - 5);

    // Draw current health percentage (up to 200)
    if (w_health.n.num != NULL)
       {
        GL_DrawLargeNum( 20, ((float)video.height - glStimPak.glHeight) / 1.2f - 3, *w_health.n.num, true);
       }
    
    if ((w_frags.num != NULL) && (deathmatch))
       {
        float scaleh;

        scaleh = Scale;
        Scale = 1.0 / 2.5;
        GL_DrawPatch( &glPlayFrag, 116.0f, 3.0f );
        Scale = scaleh;

        GL_DrawLargeNum(146, 6, *w_frags.num, false);
       }

    switch(players[consoleplayer].readyweapon)
       {
        case wp_pistol:
        case wp_chaingun:
             GL_DrawPatch( &glBBullet, 274.0f-glBBullet.Width, 189.0f-(glBBullet.Height/2));
             break;
        case wp_missile:
             GL_DrawPatch( &glBRocket, 274.0f-glBRocket.Width, 189.0f-(glBRocket.Height/2));
             break;
        case wp_plasma:
        case wp_bfg:
             GL_DrawPatch( &glBCells, 274.0f-glBCells.Width, 189.0f-(glBCells.Height/2));
             break;
        case wp_shotgun:
        case wp_supershotgun:
             GL_DrawPatch( &glBShot, 274.0f-glBShot.Width, 189.0f-(glBShot.Height/2));
             break;
       }

    // Draw ready weapon ammo count (up to max ammo for weapon)
    if (w_ready.num != NULL)
       {
        if (*w_ready.num != 1994)
           {
            GL_DrawLargeNum( 276, 182, *w_ready.num, false);
           }
       }
    
    // Draw current armor percentage (up to 200)
    if (w_armor.n.num != NULL)
       {
        if (*w_armor.n.num > 0)
           {
            if (*w_armor.n.num < 101)
               {
                GL_DrawPatch( &glGArmor[0], 260.0f-glGArmor[0].Width, 11.0f-(glGArmor[0].Height/2.0f));
               }
            else
               {
                GL_DrawPatch( &glBArmor[0], 262.0f-glBArmor[0].Width, 11.0f-(glBArmor[0].Height/2.0f));
               }

            GL_DrawLargeNum( 262, 4, *w_armor.n.num, true);
           }
       }

    // Draw which keys the player has...
    for (card = 0; card < NUMCARDS; card++)
       {
        if (players[consoleplayer].cards[card] == true)
           {
            GL_DrawPatch(&KeyTex[card], 278.0f+((card%3)*14), 172.0f);
           }
       }

/*
    // Draw the "Doom Guy's" face at the bottom
    glBindTexture(GL_TEXTURE_2D, face_tex[st_faceindex]);
    glBegin( GL_QUADS );
      glNormal3f( 0.0f, 0.0f, 1.0f);
      glTexCoord2f( 0.0f, 0.97f );
      glVertex3f( -12.0f, -84.0f, SetBack);
      glTexCoord2f( 0.0f, 0.0f );
      glVertex3f( -12.0f, -119.0f, SetBack);
      glTexCoord2f( 0.8f, 0.0f );
      glVertex3f(  14.0f, -119.0f, SetBack);
      glTexCoord2f( 0.8f, 0.97f );
      glVertex3f(  14.0f, -84.0f, SetBack);
    glEnd();
*/
    /*glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);*/
   }

void GL_DrawStatusBar2()
   {
    int card;

    float height = video.height - glStimPak.glHeight;

    GL_DrawPatch( &glStimPak, 3.0f, height);

    // Draw current health percentage (up to 200)
    if (w_health.n.num != NULL)
       {
        GL_DrawLargeNum( 20, 182, *w_health.n.num, true);
       }
    
    if ((w_frags.num != NULL) && (deathmatch))
       {
        float scaleh;

        scaleh = Scale;
        Scale = 1.0 / 2.5;
        GL_DrawPatch( &glPlayFrag, 116.0f, 3.0f );
        Scale = scaleh;

        GL_DrawLargeNum(146, 6, *w_frags.num, false);
       }

    switch(players[consoleplayer].readyweapon)
       {
        case wp_pistol:
        case wp_chaingun:
             GL_DrawPatch( &glBBullet, 274.0f-glBBullet.Width, 11.0f-(glBBullet.Height/2));
             break;
        case wp_missile:
             GL_DrawPatch( &glBRocket, 274.0f-glBRocket.Width, 11.0f-(glBRocket.Height/2));
             break;
        case wp_plasma:
        case wp_bfg:
             GL_DrawPatch( &glBCells, 274.0f-glBCells.Width, 11.0f-(glBCells.Height/2));
             break;
        case wp_shotgun:
        case wp_supershotgun:
             GL_DrawPatch( &glBShot, 274.0f-glBShot.Width, 11.0f-(glBShot.Height/2));
             break;
       }

    // Draw ready weapon ammo count (up to max ammo for weapon)
    if (w_ready.num != NULL)
       {
        if (*w_ready.num != 1994)
           {
            GL_DrawLargeNum( 276, 4, *w_ready.num, false);
           }
       }
    
    // Draw current armor percentage (up to 200)
    if (w_armor.n.num != NULL)
       {
        if (*w_armor.n.num > 0)
           {
            if (*w_armor.n.num < 101)
               {
                GL_DrawPatch( &glGArmor[0], 260.0f-glGArmor[0].Width, 189.0f-(glGArmor[0].Height/2.0f));
               }
            else
               {
                GL_DrawPatch( &glBArmor[0], 262.0f-glBArmor[0].Width, 189.0f-(glBArmor[0].Height/2.0f));
               }

            GL_DrawLargeNum( 262, 182, *w_armor.n.num, true);
           }
       }

    // Draw which keys the player has...
    for (card = 0; card < NUMCARDS; card++)
       {
        if (players[consoleplayer].cards[card] == true)
           {
            GL_DrawPatch(&KeyTex[card], 278.0f+((card%3)*14), 172.0f);
           }
       }

/*
    // Draw the "Doom Guy's" face at the bottom
    glBindTexture(GL_TEXTURE_2D, face_tex[st_faceindex]);
    glBegin( GL_QUADS );
      glNormal3f( 0.0f, 0.0f, 1.0f);
      glTexCoord2f( 0.0f, 0.97f );
      glVertex3f( -12.0f, -84.0f, SetBack);
      glTexCoord2f( 0.0f, 0.0f );
      glVertex3f( -12.0f, -119.0f, SetBack);
      glTexCoord2f( 0.8f, 0.0f );
      glVertex3f(  14.0f, -119.0f, SetBack);
      glTexCoord2f( 0.8f, 0.97f );
      glVertex3f(  14.0f, -84.0f, SetBack);
    glEnd();
*/
   }

int hudmode;

void GL_DrawStatusBar(int mode)
   {
    switch(mode)
       {
        case 1:
             GL_DrawStatusBar0();
             break;
        case 2:
             GL_DrawStatusBar1();
             break;
        case 3:
             GL_DrawStatusBar2();
             break;
       }
   }

void ST_drawWidgets(dboolean refresh)
{
    int		i;

    // used by w_arms[] widgets
    st_armson = st_statusbaron && !deathmatch;

    // used by w_frags widget
    st_fragson = deathmatch && st_statusbaron; 

    STlib_updateNum(&w_ready, refresh);

    for (i=0;i<4;i++)
    {
	STlib_updateNum(&w_ammo[i], refresh);
	STlib_updateNum(&w_maxammo[i], refresh);
    }

    STlib_updatePercent(&w_health, refresh);
    STlib_updatePercent(&w_armor, refresh);

    STlib_updateBinIcon(&w_armsbg, refresh);

    for (i=0;i<6;i++)
	STlib_updateMultIcon(&w_arms[i], refresh);

    STlib_updateMultIcon(&w_faces, refresh);

    for (i=0;i<3;i++)
	STlib_updateMultIcon(&w_keyboxes[i], refresh);

    STlib_updateNum(&w_frags, refresh);

}

void ST_doRefresh(void)
{

    st_firsttime = false;

    // draw status bar background to off-screen buff
    ST_refreshBackground();

    // and refresh all widgets
    ST_drawWidgets(true);

}

void ST_diffDraw(void)
{
    // update all widgets
    ST_drawWidgets(false);
}

void ST_Drawer (dboolean fullscreen, dboolean refresh)
{
// FIXME...
    fullscreen = false;
    st_statusbaron = (!fullscreen) || automapactive;
    st_firsttime = st_firsttime || refresh;

    // Do red-/gold-shifts from damage/items
    ST_doPaletteStuff();

    // If just after ST_Start(), refresh all
    if (st_firsttime) ST_doRefresh();
    // Otherwise, update as little as possible
    else
       {
    ST_diffDraw();
       }

}

void ST_loadGraphics(void)
   {
    int		i;
    int		j;
    int		facenum;
    
    char	namebuf[9];

    // Load the numbers, tall and short
    for (i = 0; i < 10; i++)
       {
        sprintf(namebuf, "STTNUM%d", i);
        tallnum[i] = (patch_t *) W_CacheLumpName(namebuf, PU_STATIC);
        GL_MakeSpriteTexture(tallnum[i], &TallNums[i], true);

        sprintf(namebuf, "STYSNUM%d", i);
        shortnum[i] = (patch_t *) W_CacheLumpName(namebuf, PU_STATIC);
        GL_MakeSpriteTexture(shortnum[i], &ShortNums[1][i], false);
       }

    // Load percent key.
    //Note: why not load STMINUS here, too?
    tallpercent = (patch_t *) W_CacheLumpName("STTPRCNT", PU_STATIC);
    GL_MakeSpriteTexture(tallpercent, &TallPerc, true);

    // key cards
    for (i = 0; i < NUMCARDS; i++)
       {
        sprintf(namebuf, "STKEYS%d", i);
        keys[i] = (patch_t *) W_CacheLumpName(namebuf, PU_STATIC);
        GL_MakeSpriteTexture(keys[i], &KeyTex[i], true);
       }

    // arms background
    armsbg = (patch_t *) W_CacheLumpName("STARMS", PU_STATIC);
    GL_RGBStatArms(armsbg);

    // arms ownership widgets
    for (i = 0; i < 6; i++)
       {
	    sprintf(namebuf, "STGNUM%d", i+2);
        // gray #
        arms[i][0] = (patch_t *) W_CacheLumpName(namebuf, PU_STATIC);
        GL_MakeSpriteTexture(arms[i][0], &ShortNums[0][i], false);
        // yellow #
        arms[i][1] = shortnum[i+2]; 
       }

    for (i = 0; i < 4; i++)
       {
        sprintf(namebuf, "STFB%d", i);
        faceback = (patch_t *) W_CacheLumpName(namebuf, PU_CACHE);
        GL_RGBStatBack(faceback, i);
       }
    // face backgrounds for different color players
    sprintf(namebuf, "STFB%d", consoleplayer);
    faceback = (patch_t *) W_CacheLumpName(namebuf, PU_STATIC);

    // status bar background bits
    sbar = (patch_t *) W_CacheLumpName("STBAR", PU_STATIC);
    GL_CreateStatusBar(sbar);
    GL_RGBStatFrag(sbar);

    // face states
    facenum = 0;
    for (i = 0; i < ST_NUMPAINFACES; i++)
       {
        for (j = 0; j < ST_NUMSTRAIGHTFACES; j++)
           {
            sprintf(namebuf, "STFST%d%d", i, j);
            faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
           }
        sprintf(namebuf, "STFTR%d0", i);	// turn right
        faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
        sprintf(namebuf, "STFTL%d0", i);	// turn left
        faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
        sprintf(namebuf, "STFOUCH%d", i);	// ouch!
        faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
        sprintf(namebuf, "STFEVL%d", i);	// evil grin ;)
        faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
        sprintf(namebuf, "STFKILL%d", i);	// pissed off
        faces[facenum++] = W_CacheLumpName(namebuf, PU_STATIC);
       }
    faces[facenum++] = W_CacheLumpName("STFGOD0", PU_STATIC);
    faces[facenum++] = W_CacheLumpName("STFDEAD0", PU_STATIC);

    for (i = 0; i < facenum; i++)
       GL_MakeSpriteTexture(faces[i], &FaceTex[i], false);


    // load alternate HUD textures here...
// single frame sprites
// Boxes of ammo
    GL_MakeSpriteTexture(W_CacheLumpName("AMMOA0", PU_CACHE), &glBBullet, true);
    GL_MakeSpriteTexture(W_CacheLumpName("BROKA0", PU_CACHE), &glBRocket, true);
    if (gamemode != shareware) // No BFG or Plasma Rifle in the shareware
       {
        GL_MakeSpriteTexture(W_CacheLumpName("CELPA0", PU_CACHE), &glBCells, true);
       }
    GL_MakeSpriteTexture(W_CacheLumpName("SBOXA0", PU_CACHE), &glBShot, true);
// Ammo packs
    GL_MakeSpriteTexture(W_CacheLumpName("CLIPA0", PU_CACHE), &glBullet, true);
    GL_MakeSpriteTexture(W_CacheLumpName("ROCKA0", PU_CACHE), &glRocket, true);
    if (gamemode != shareware) // No BFG or Plasma Rifle in the shareware
       {
        GL_MakeSpriteTexture(W_CacheLumpName("CELLA0", PU_CACHE), &glCells, true);
       }
    GL_MakeSpriteTexture(W_CacheLumpName("SHELA0", PU_CACHE), &glShot, true);
// Weapons  
    GL_MakeSpriteTexture(W_CacheLumpName("SHOTA0", PU_CACHE), &glShot1, true);
    GL_MakeSpriteTexture(W_CacheLumpName("STARMS", PU_CACHE), &glSTArms, true);

    if (gamemode == commercial)
       {
        GL_MakeSpriteTexture(W_CacheLumpName("SGN2A0", PU_CACHE), &glShot2, true);
       }
    GL_MakeSpriteTexture(W_CacheLumpName("LAUNA0", PU_CACHE), &glRLaunch, true);
    if (gamemode != shareware) // No BFG or Plasma rifle in the shareware
       {
        GL_MakeSpriteTexture(W_CacheLumpName("PLASA0", PU_CACHE), &glPlasma, true);
        GL_MakeSpriteTexture(W_CacheLumpName("BFUGA0", PU_CACHE), &glBFG, true);
       }
    GL_MakeSpriteTexture(W_CacheLumpName("CSAWA0", PU_CACHE), &glChainSaw, true);
    GL_MakeSpriteTexture(W_CacheLumpName("MGUNA0", PU_CACHE), &glMiniGun, true);
// Medical Kits
    GL_MakeSpriteTexture(W_CacheLumpName("STIMA0", PU_CACHE), &glStimPak, true);
    GL_MakeSpriteTexture(W_CacheLumpName("MEDIA0", PU_CACHE), &glMediKit, true);

    GL_MakeSpriteTexture(W_CacheLumpName("PLAYH0", PU_CACHE), &glPlayFrag, true);

// animated sprites
// Armor Types
    GL_MakeSpriteTexture(W_CacheLumpName("ARM1A0", PU_CACHE), &glGArmor[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("ARM1B0", PU_CACHE), &glGArmor[1], true);
    GL_MakeSpriteTexture(W_CacheLumpName("ARM2A0", PU_CACHE), &glBArmor[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("ARM2B0", PU_CACHE), &glBArmor[1], true);
// Key cards
    GL_MakeSpriteTexture(W_CacheLumpName("YKEYA0", PU_CACHE), &glYCKey[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("YKEYB0", PU_CACHE), &glYCKey[1], true);
    GL_MakeSpriteTexture(W_CacheLumpName("BKEYA0", PU_CACHE), &glBCKey[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("BKEYB0", PU_CACHE), &glBCKey[1], true);
    GL_MakeSpriteTexture(W_CacheLumpName("RKEYA0", PU_CACHE), &glRCKey[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("RKEYB0", PU_CACHE), &glRCKey[1], true);
// Skull keys
    if (gamemode != shareware)  // only the later episodes use the skull keys
       {
        GL_MakeSpriteTexture(W_CacheLumpName("YSKUA0", PU_CACHE), &glYSKey[0], true);
        GL_MakeSpriteTexture(W_CacheLumpName("YSKUB0", PU_CACHE), &glYSKey[1], true);
        GL_MakeSpriteTexture(W_CacheLumpName("BSKUA0", PU_CACHE), &glBSKey[0], true);
        GL_MakeSpriteTexture(W_CacheLumpName("BSKUB0", PU_CACHE), &glBSKey[1], true);
        GL_MakeSpriteTexture(W_CacheLumpName("RSKUA0", PU_CACHE), &glRSKey[0], true);
        GL_MakeSpriteTexture(W_CacheLumpName("RSKUB0", PU_CACHE), &glRSKey[1], true);
       }
// Automap
    GL_MakeSpriteTexture(W_CacheLumpName("PMAPA0", PU_CACHE), &glAutoMapI[0], true);
    GL_MakeSpriteTexture(W_CacheLumpName("PMAPB0", PU_CACHE), &glAutoMapI[1], true);
    GL_MakeSpriteTexture(W_CacheLumpName("PMAPC0", PU_CACHE), &glAutoMapI[2], true);
    GL_MakeSpriteTexture(W_CacheLumpName("PMAPD0", PU_CACHE), &glAutoMapI[3], true);
   }

void ST_loadData(void)
{
    lu_palette = W_GetNumForName ("PLAYPAL");
    ST_loadGraphics();
}

void ST_unloadGraphics(void)
{

    int i;

    // unload the numbers, tall and short
    for (i=0;i<10;i++)
    {
	Z_ChangeTag(tallnum[i], PU_CACHE);
	Z_ChangeTag(shortnum[i], PU_CACHE);
    }
    // unload tall percent
    Z_ChangeTag(tallpercent, PU_CACHE); 

    // unload arms background
    Z_ChangeTag(armsbg, PU_CACHE); 

    // unload gray #'s
    for (i=0;i<6;i++)
	Z_ChangeTag(arms[i][0], PU_CACHE);
    
    // unload the key cards
    for (i=0;i<NUMCARDS;i++)
	Z_ChangeTag(keys[i], PU_CACHE);

    Z_ChangeTag(sbar, PU_CACHE);
    Z_ChangeTag(faceback, PU_CACHE);

    for (i=0;i<ST_NUMFACES;i++)
	Z_ChangeTag(faces[i], PU_CACHE);

    // Note: nobody ain't seen no unloading
    //   of stminus yet. Dude.
    

}

void ST_unloadData(void)
{
    ST_unloadGraphics();
}

void ST_initData(void)
{

    int		i;

    st_firsttime = true;
    plyr = &players[consoleplayer];

    st_clock = 0;
    st_chatstate = StartChatState;
    st_gamestate = FirstPersonState;

    st_statusbaron = true;
    st_oldchat = st_chat = false;
    st_cursoron = false;

    st_faceindex = 0;
    st_palette = -1;

    st_oldhealth = -1;

    for (i=0;i<NUMWEAPONS;i++)
	oldweaponsowned[i] = plyr->weaponowned[i];

    for (i=0;i<3;i++)
	keyboxes[i] = -1;

    STlib_init();

}



void ST_createWidgets(void)
{

    int i;

    // ready weapon ammo
    STlib_initNum(&w_ready,
		  ST_AMMOX,
		  ST_AMMOY,
		  tallnum,
		  &plyr->ammo[weaponinfo[plyr->readyweapon].ammo],
		  &st_statusbaron,
		  ST_AMMOWIDTH );

    // the last weapon type
    w_ready.data = plyr->readyweapon; 

    // health percentage
    STlib_initPercent(&w_health,
		      ST_HEALTHX,
		      ST_HEALTHY,
		      tallnum,
		      &plyr->health,
		      &st_statusbaron,
		      tallpercent);

    // arms background
    STlib_initBinIcon(&w_armsbg,
		      ST_ARMSBGX,
		      ST_ARMSBGY,
		      armsbg,
		      &st_notdeathmatch,
		      &st_statusbaron);

    // weapons owned
    for(i=0;i<6;i++)
    {
	STlib_initMultIcon(&w_arms[i],
			   ST_ARMSX+(i%3)*ST_ARMSXSPACE,
			   ST_ARMSY+(i/3)*ST_ARMSYSPACE,
			   arms[i], (int *) &plyr->weaponowned[i+1],
			   &st_armson);
    }

    // frags sum
    STlib_initNum(&w_frags,
		  ST_FRAGSX,
		  ST_FRAGSY,
		  tallnum,
		  &st_fragscount,
		  &st_fragson,
		  ST_FRAGSWIDTH);

    // faces
    STlib_initMultIcon(&w_faces,
		       ST_FACESX,
		       ST_FACESY,
		       faces,
		       &st_faceindex,
		       &st_statusbaron);

    // armor percentage - should be colored later
    STlib_initPercent(&w_armor,
		      ST_ARMORX,
		      ST_ARMORY,
		      tallnum,
		      &plyr->armorpoints,
		      &st_statusbaron, tallpercent);

    // keyboxes 0-2
    STlib_initMultIcon(&w_keyboxes[0],
		       ST_KEY0X,
		       ST_KEY0Y,
		       keys,
		       &keyboxes[0],
		       &st_statusbaron);
    
    STlib_initMultIcon(&w_keyboxes[1],
		       ST_KEY1X,
		       ST_KEY1Y,
		       keys,
		       &keyboxes[1],
		       &st_statusbaron);

    STlib_initMultIcon(&w_keyboxes[2],
		       ST_KEY2X,
		       ST_KEY2Y,
		       keys,
		       &keyboxes[2],
		       &st_statusbaron);

    // ammo count (all four kinds)
    STlib_initNum(&w_ammo[0],
		  ST_AMMO0X,
		  ST_AMMO0Y,
		  shortnum,
		  &plyr->ammo[0],
		  &st_statusbaron,
		  ST_AMMO0WIDTH);

    STlib_initNum(&w_ammo[1],
		  ST_AMMO1X,
		  ST_AMMO1Y,
		  shortnum,
		  &plyr->ammo[1],
		  &st_statusbaron,
		  ST_AMMO1WIDTH);

    STlib_initNum(&w_ammo[2],
		  ST_AMMO2X,
		  ST_AMMO2Y,
		  shortnum,
		  &plyr->ammo[2],
		  &st_statusbaron,
		  ST_AMMO2WIDTH);
    
    STlib_initNum(&w_ammo[3],
		  ST_AMMO3X,
		  ST_AMMO3Y,
		  shortnum,
		  &plyr->ammo[3],
		  &st_statusbaron,
		  ST_AMMO3WIDTH);

    // max ammo count (all four kinds)
    STlib_initNum(&w_maxammo[0],
		  ST_MAXAMMO0X,
		  ST_MAXAMMO0Y,
		  shortnum,
		  &plyr->maxammo[0],
		  &st_statusbaron,
		  ST_MAXAMMO0WIDTH);

    STlib_initNum(&w_maxammo[1],
		  ST_MAXAMMO1X,
		  ST_MAXAMMO1Y,
		  shortnum,
		  &plyr->maxammo[1],
		  &st_statusbaron,
		  ST_MAXAMMO1WIDTH);

    STlib_initNum(&w_maxammo[2],
		  ST_MAXAMMO2X,
		  ST_MAXAMMO2Y,
		  shortnum,
		  &plyr->maxammo[2],
		  &st_statusbaron,
		  ST_MAXAMMO2WIDTH);
    
    STlib_initNum(&w_maxammo[3],
		  ST_MAXAMMO3X,
		  ST_MAXAMMO3Y,
		  shortnum,
		  &plyr->maxammo[3],
		  &st_statusbaron,
		  ST_MAXAMMO3WIDTH);

}

static dboolean	st_stopped = true;


void ST_Start (void)
{

    if (!st_stopped)
	ST_Stop();

    ST_initData();
    ST_createWidgets();
    st_stopped = false;

}

void ST_Stop (void)
{
    if (st_stopped)
	return;

    I_SetPalette (W_CacheLumpNum (lu_palette, PU_CACHE));

    st_stopped = true;
}

void ST_Init (void)
{
    veryfirsttime = 0;
    ST_loadData();
    //screens[4] = (dbyte *) Z_Malloc(ST_WIDTH*ST_HEIGHT, PU_STATIC, 0);
}
