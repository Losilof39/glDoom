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
//
// $Log:$
//
// DESCRIPTION:
//	Main loop menu stuff.
//	Default Config File.
//	PCX Screenshots.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_misc.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include "thirdparty/SDL2/include/SDL.h"
#include "thirdparty/glad/include/glad/glad.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <io.h>
#else
    #include <inttypes.h>
    #include <unistd.h>
#endif

#include <ctype.h>


#include "doomdef.h"

#include "z_zone.h"

#include "m_swap.h"
#include "m_argv.h"

#include "w_wad.h"

#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

#include "hu_stuff.h"

// State.
#include "doomstat.h"

// Data.
#include "dstrings.h"

#include "m_misc.h"

#include "gldefs.h"
#include "sys_sdl.h"

#include "doomcmd.h"

unsigned short kbd_game_txt[256];
unsigned short kbd_game_cmd[256];
unsigned short jsb_game_cmd[2][32];
unsigned short mb_game_cmd[6];

//
// M_DrawText
// Returns the final X coordinate
// HU_Init must have been called to init the font
//
extern patch_t*		hu_font[HU_FONTSIZE];
extern GLTexData    GLHudFont[HU_FONTSIZE];
extern float        GLFontHeight;

extern video_t      video;

int
M_DrawText
( int		x,
  int		y,
  dboolean	direct,
  char*		string )
{
    int 	c;
    int		w;

    while (*string)
    {
	c = toupper(*string) - HU_FONTSTART;
	string++;
	if (c < 0 || c> HU_FONTSIZE)
	{
	    x += 4;
	    continue;
	}
		
	w = SHORT (hu_font[c]->width);
	if (x+w > SCREENWIDTH)
	    break;
	if (direct)
	    V_DrawPatchDirect(x, y, 0, hu_font[c]);
	else
	    V_DrawPatch(x, y, 0, hu_font[c]);
	x+=w;
    }

    return x;
}

extern float SetBack;

int M_GLDrawText( int x, int y, char *string )
   {
    int 	c;
    float   fx;
    float   Left, Right, Top, Bottom;

    fx = (float)x;
    while (*string)
       {
        c = toupper(*string) - HU_FONTSTART;
        string++;
        if (c < 0 || c > HU_FONTSIZE)
           {
            fx += 4.0f;
            continue;
           }
		
        Left = -160.0f+(fx);
        Right = Left+GLHudFont[c].Width;
        Top = 120.0f-(y-GLHudFont[c].TopOff);
        Bottom = Top - GLHudFont[c].Height;

        glBindTexture(GL_TEXTURE_2D, GLHudFont[c].TexName);
        glBegin( GL_QUADS );
           glNormal3f(  0.0f, 0.0f, 1.0f);
           glTexCoord2f( 0.0f, 1.0f );
           glVertex3f( Left, Top, SetBack);
           glTexCoord2f( 0.0f, GLHudFont[c].YDisp );
           glVertex3f( Left, Bottom, SetBack);
           glTexCoord2f( GLHudFont[c].XDisp, GLHudFont[c].YDisp );
           glVertex3f( Right, Bottom, SetBack);
           glTexCoord2f( GLHudFont[c].XDisp, 1.0f );
           glVertex3f( Right, Top, SetBack);
        glEnd();

        if (fx + GLHudFont[c].Width > 320.0f)
            break;
        fx += GLHudFont[c].Width;
       }

    return x;
   }



//
// M_GetFileSize
//
#ifndef O_BINARY
#define O_BINARY 0
#endif

int M_GetFileSize( char const*	name )
   {
    int		handle;
    int		count;
	
    handle = Open ( name, O_RDWR | O_BINARY);

    if (handle == -1)
        return 0;

    count = LSeek(handle, 0, SEEK_END);
    Close (handle);
	
    return count;
   }

//
// M_WriteFile
//
dboolean
M_WriteFile
( char const*	name,
  void*		source,
  int		length )
{
    int		handle;
    int		count;
	
    handle = Open ( name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);

    if (handle == -1)
	return false;

    count = Write (handle, source, length);
    Close (handle);
	
    if (count < length)
	return false;
		
    return true;
}

//
// M_AppendFile
//
dboolean M_AppendFile(char const *name, void *source, int length )
   {
    int		handle;
    int		count;
	
    handle = Open( name, O_RDWR | O_BINARY);

    if (handle == -1)
        return false;

    LSeek(handle, 0L, SEEK_END);

    count = Write (handle, source, length);
    Close (handle);
	
    if (count < length)
        return false;
		
    return true;
   }


//
// M_ReadFile
//
int
M_ReadFile
( char const*	name,
  byte**	buffer )
{
    int	handle, count, length;
    struct stat	fileinfo;
    byte		*buf;
	
    handle = Open (name, O_RDONLY | O_BINARY, 0666);
    if (handle == -1)
	I_Error ("Couldn't read file %s", name);
    if (fstat (handle,&fileinfo) == -1)
	I_Error ("Couldn't read file %s", name);
    length = fileinfo.st_size;
    buf = Z_Malloc (length, PU_STATIC, NULL);
    count = Read (handle, buf, length);
    Close (handle);
	
    if (count < length)
	I_Error ("Couldn't read file %s", name);
		
    *buffer = buf;
    return length;
}


//
// DEFAULTS
//
int		usemouse;
int		usejoystick;

char  playername[18];
char  playerskin[18];

extern int	key_lookup;
extern int	key_lookdown;
extern int	key_mlook;

extern int	key_right;
extern int	key_left;
extern int	key_up;
extern int	key_down;

extern int	key_strafeleft;
extern int	key_straferight;

extern int	key_fire;
extern int	key_use;
extern int	key_strafe;
extern int	key_speed;

extern int  key_mvert;

extern int  autorun;
extern int  swap_stereo;
extern int  nosound;
extern int  mvert;
extern int  mlook;
extern int  keylink;

extern int	mousebfire;
extern int	mousebstrafe;
extern int	mousebforward;

extern int	mouseb1;
extern int	mouseb2;
extern int	mouseb3;

extern int  mouse_factor;

extern int  gl_fog;
extern int  gl_poffsetf;
extern int  gl_poffsetu;
extern int  gl_modeltest;
extern int  gl_alphatest;
extern int  gl_premalpha;
extern int  gl_widetex;

extern int  vsync;

extern char gamename[128];
extern int  hudmode;

extern int	joybfire;
extern int	joybstrafe;
extern int	joybuse;
extern int	joybspeed;

extern int  joydead;

extern int  joyb[32];

extern int  joyb1;
extern int  joyb2;
extern int  joyb3;
extern int  joyb4;
extern int  joyb5;
extern int  joyb6;
extern int  joyb7;
extern int  joyb8;
extern int  joyb9;
extern int  joyb10;
extern int  joyb11;
extern int  joyb12;
extern int  joyb13;
extern int  joyb14;
extern int  joyb15;
extern int  joyb16;
extern int  joyb17;
extern int  joyb18;
extern int  joyb19;
extern int  joyb20;
extern int  joyb21;
extern int  joyb22;
extern int  joyb23;
extern int  joyb24;
extern int  joyb25;
extern int  joyb26;
extern int  joyb27;
extern int  joyb28;
extern int  joyb29;
extern int  joyb30;
extern int  joyb31;
extern int  joyb32;

extern int	viewwidth;
extern int	viewheight;

extern int  mouseVertical;
extern int  mouseHorizontal;
extern int	mouseSensitivity;
extern int	showMessages;

extern int	detailLevel;

extern int	screenblocks;

extern int	showMessages;

// machine-independent sound params
extern	int	numChannels;


extern int key_hudup;
extern int key_huddn;

extern int key_f1;
extern int key_f2;
extern int key_f3;
extern int key_f4;
extern int key_f5;
extern int key_f6;
extern int key_f7;
extern int key_f8;
extern int key_f9;
extern int key_f10;
extern int key_f11;
extern int key_f12;

extern int key_menu_right;                                   // phares 3/7/98
extern int key_menu_left;                                    //     |
extern int key_menu_up;                                      //     V
extern int key_menu_down;                                     
extern int key_menu_backspace;                               //     ^
extern int key_menu_escape;                                  //     |
extern int key_menu_enter;                                   // phares 3/7/98

extern int key_escape;
extern int key_savegame;
extern int key_loadgame;
extern int key_autorun;
extern int key_reverse;
extern int key_zoomin;
extern int key_zoomout;
extern int key_chat;
extern int key_backspace;
extern int key_enter;
extern int key_help;
extern int key_soundvolume;
extern int key_hud;
extern int key_quicksave;
extern int key_endgame;
extern int key_messages;
extern int key_quickload;
extern int key_quit;
extern int key_gamma;

extern int key_spy;
extern int key_pause;
extern int key_forward;
extern int key_leftturn;
extern int key_rightturn;
extern int key_backward;

extern int key_weapontoggle;
extern int key_weapon1;
extern int key_weapon2;
extern int key_weapon3;
extern int key_weapon4;
extern int key_weapon5;
extern int key_weapon6;
extern int key_weapon7;
extern int key_weapon8;
extern int key_weapon9;                                            
extern int key_map_right;                                           // phares
extern int key_map_left;                                            //    |
extern int key_map_up;                                              //    V
extern int key_map_down;
extern int key_map_zoomin;
extern int key_map_zoomout;
extern int key_map;
extern int key_map_gobig;
extern int key_map_follow;
extern int key_map_mark;                                            //    ^
extern int key_map_clear;                                           //    |
extern int key_map_grid;                                            // phares
extern int key_screenshot;    // killough 2/22/98

// UNIX hack, to be removed.
#ifdef SNDSERV
extern char*	sndserver_filename;
extern int	mb_used;
#endif

#ifdef LINUX
char*		mousetype;
char*		mousedev;
#endif

int          nosound_t;

extern char  chat_macros[10][80];

typedef struct
{
    char*	name;
    int*	location;
    int		defaultvalue;
    int		scantranslate;		// PC scan code hack
    int		untranslated;		// lousy hack
} default_t;

typedef enum { d_string, d_key, d_value, d_finished } deftype_t;

typedef struct
   {
    deftype_t   deftype;
    char*     name;
    char*     contents;
    char*     defvalue;
    int         max;
   }win_defaults_t;

win_defaults_t wdefaults[] = 
   {
    d_string, "playername",      playername,     "PLAYER",         18,        // defaults to PLAYER
    d_string, "playerskin",      playerskin,     "NORMAL",         18,        // defaults to NORMAL
    d_string, "game",            gamename,       "doom2",          _MAX_PATH, // defaults to Doom2

    d_string, "chatmacro0",      &chat_macros[0][0], HUSTR_CHATMACRO0, 80,
    d_string, "chatmacro1",      &chat_macros[1][0], HUSTR_CHATMACRO1, 80,
    d_string, "chatmacro2",      &chat_macros[2][0], HUSTR_CHATMACRO2, 80,
    d_string, "chatmacro3",      &chat_macros[3][0], HUSTR_CHATMACRO3, 80,
    d_string, "chatmacro4",      &chat_macros[4][0], HUSTR_CHATMACRO4, 80,
    d_string, "chatmacro5",      &chat_macros[5][0], HUSTR_CHATMACRO5, 80,
    d_string, "chatmacro6",      &chat_macros[6][0], HUSTR_CHATMACRO6, 80,
    d_string, "chatmacro7",      &chat_macros[7][0], HUSTR_CHATMACRO7, 80,
    d_string, "chatmacro8",      &chat_macros[8][0], HUSTR_CHATMACRO8, 80,
    d_string, "chatmacro9",      &chat_macros[9][0], HUSTR_CHATMACRO9, 80,
    d_finished, 0, 0, 0, 0
   };

typedef struct
   {
    deftype_t   deftype;
    char       *name;
    int        *location;
    int         defvalue;
   }win_defaulti_t;

win_defaulti_t wdefaultv[] = 
   {
    d_value, "mouse_horizontal", &mouseHorizontal,    10,
    d_value, "mouse_vertical",   &mouseVertical,      10,
    d_value, "mouse_sensitivity",&mouseSensitivity,   5,
    d_value, "sfx_volume",       &snd_SfxVolume,      8,
    d_value, "music_volume",     &snd_MusicVolume,    8,
    d_value, "show_messages",    &showMessages,       1,
    
    d_key,   "key_fire",         &key_fire,           SDL_SCANCODE_LCTRL,
    d_key,   "key_use",          &key_use,            SDL_SCANCODE_SPACE,
    d_key,   "key_strafe",       &key_strafe,         SDL_SCANCODE_RALT,
    d_key,   "key_speed",        &key_speed,          SDL_SCANCODE_LSHIFT,

    d_key,   "key_right",        &key_right,          SDL_SCANCODE_RIGHT,
    d_key,   "key_left",         &key_left,           SDL_SCANCODE_LEFT,
    d_key,   "key_up",           &key_up,             SDL_SCANCODE_W,
    d_key,   "key_down",         &key_down,           SDL_SCANCODE_S,

    d_key,   "key_strafeleft",   &key_strafeleft,     SDL_SCANCODE_A,
    d_key,   "key_straferight",  &key_straferight,    SDL_SCANCODE_D,

    d_key,   "key_lookup",       &key_lookup,         SDL_SCANCODE_KP_DECIMAL,
    d_key,   "key_lookdown",     &key_lookdown,       SDL_SCANCODE_KP_3,
    d_key,   "key_mlook",        &key_mlook,          SDL_SCANCODE_M,

    d_key,   "key_mvert",        &key_mvert,          SDL_SCANCODE_SLASH,

    d_value, "mouse_factor",     &mouse_factor,       1,
    d_value, "autorun",          &autorun,            0,
    d_value, "nosound",          &nosound_t,          0,
    d_value, "mvert",            &mvert,              1,
    d_value, "mlook",            &mlook,              1,
    d_value, "keylink",          &keylink,            1,
    d_value, "gl_fog",           &gl_fog,             1,
    d_value, "gl_poffsetf",      &gl_poffsetf,        255,
    d_value, "gl_poffsetu",      &gl_poffsetu,        255,
    d_value, "gl_modeltest",     &gl_modeltest,       0,
    d_value, "gl_alphatest",     &gl_alphatest,       0,
    d_value, "gl_premalpha",     &gl_premalpha,       0,
    d_value, "gl_widetex",       &gl_widetex,         1,

    d_value, "hudmode",          &hudmode,            1,  // defaults to original

    // Mouse button setup
    d_value, "usemouse",         &usemouse,           1,  // defaults to true
    d_value, "mouseb_fire",      &mousebfire,         0,
    d_value, "mouseb_strafe",    &mousebstrafe,       1,
    d_value, "mouseb_forward",   &mousebforward,      2,

    d_key,   "mouseb1",          &mouseb1,            CMD_NULL,
    d_key,   "mouseb2",          &mouseb2,            CMD_NULL,
    d_key,   "mouseb3",          &mouseb3,            CMD_NULL,

    // Joystick control/setup
    d_value, "use_joystick",     &usejoystick,        0,  // defaults to false
    d_value, "joyb_fire",        &joybfire,           0,
    d_value, "joyb_strafe",      &joybstrafe,         1,
    d_value, "joyb_use",         &joybuse,            2,
    d_value, "joyb_speed",       &joybspeed,          3,
    d_value, "joydead",          &joydead,            5000,            

    d_key, "joyb1",              &joyb1,              SDL_SCANCODE_RCTRL,
    d_key, "joyb2",              &joyb2,              SDL_SCANCODE_RALT,
    d_key, "joyb3",              &joyb3,              SDL_SCANCODE_SPACE,
    d_key, "joyb4",              &joyb4,              SDL_SCANCODE_RSHIFT,
    d_key, "joyb5",              &joyb5,              CMD_NULL,
    d_key, "joyb6",              &joyb6,              CMD_NULL,
    d_key, "joyb7",              &joyb7,              CMD_NULL,
    d_key, "joyb8",              &joyb8,              CMD_NULL,
    d_key, "joyb9",              &joyb9,              CMD_NULL,
    d_key, "joyb10",             &joyb10,             CMD_NULL,
    d_key, "joyb11",             &joyb11,             CMD_NULL,
    d_key, "joyb12",             &joyb12,             CMD_NULL,
    d_key, "joyb13",             &joyb13,             CMD_NULL,
    d_key, "joyb14",             &joyb14,             CMD_NULL,
    d_key, "joyb15",             &joyb15,             CMD_NULL,
    d_key, "joyb16",             &joyb16,             CMD_NULL,
    d_key, "joyb17",             &joyb17,             CMD_NULL,
    d_key, "joyb18",             &joyb18,             CMD_NULL,
    d_key, "joyb19",             &joyb19,             CMD_NULL,
    d_key, "joyb20",             &joyb20,             CMD_NULL,
    d_key, "joyb21",             &joyb21,             CMD_NULL,
    d_key, "joyb22",             &joyb22,             CMD_NULL,
    d_key, "joyb23",             &joyb23,             CMD_NULL,
    d_key, "joyb24",             &joyb24,             CMD_NULL,
    d_key, "joyb25",             &joyb25,             CMD_NULL,
    d_key, "joyb26",             &joyb26,             CMD_NULL,
    d_key, "joyb27",             &joyb27,             CMD_NULL,
    d_key, "joyb28",             &joyb28,             CMD_NULL,
    d_key, "joyb29",             &joyb29,             CMD_NULL,
    d_key, "joyb30",             &joyb30,             CMD_NULL,
    d_key, "joyb31",             &joyb31,             CMD_NULL,
    d_key, "joyb32",             &joyb32,             CMD_NULL,

    // These values are not useful for OpenGL rendering
    //d_value, "screenblocks",         &screenblocks,         11,   // 0 - 11 (11 is fullscreen)
    //d_value, "detaillevel",          &detaillevel,          0,    // 0 - 1
    //d_value, "usegamma",             &usegamma,             0,    // 0 - 4

    // These values are not useful for DirectSound
    d_value, "snd_channels",     &numChannels,        16,
    d_value, "swap_stereo",      &swap_stereo,        0,

    // "new" keyboard mappings
    d_key, "key_escape",         &key_escape,        SDL_SCANCODE_ESCAPE,
    //d_key, "key_autorun",        &key_autorun,        SDL_SCANCODE_F2,
    d_key, "key_reverse",        &key_reverse,        SDL_SCANCODE_BACKSPACE,
    d_key, "key_zoomin",         &key_zoomin,         SDL_SCANCODE_EQUALS,
    d_key, "key_zoomout",        &key_zoomout,        SDL_SCANCODE_MINUS,
    d_key, "key_chat",           &key_chat,           SDL_SCANCODE_T,
    d_key, "key_backspace",      &key_backspace,      SDL_SCANCODE_BACKSPACE,
    d_key, "key_enter",          &key_enter,          SDL_SCANCODE_RETURN,
    d_key, "key_help",           &key_help,           SDL_SCANCODE_F1,
    d_key, "key_savegame",       &key_savegame,       SDL_SCANCODE_F2,
    d_key, "key_loadgame",       &key_loadgame,       SDL_SCANCODE_F3,
    d_key, "key_soundvolume",    &key_soundvolume,    SDL_SCANCODE_F4,
//    d_key, "key_hud",            &key_hud,            KEY_F5,
    d_key, "key_quicksave",      &key_quicksave,      SDL_SCANCODE_F6,
    d_key, "key_endgame",        &key_endgame,        SDL_SCANCODE_F7,
    d_key, "key_messages",       &key_messages,       SDL_SCANCODE_F8,
    d_key, "key_quickload",      &key_quickload,      SDL_SCANCODE_F9,
    d_key, "key_quit",           &key_quit,           SDL_SCANCODE_F10,
//    d_key, "key_gamma",          &key_gamma,          KEY_F11,
    d_key, "key_screenshot",     &key_screenshot,     SDL_SCANCODE_F12,

    d_key, "key_pause",          &key_pause,          SDL_SCANCODE_PAUSE,
    d_key, "key_forward",        &key_forward,        SDL_SCANCODE_UP,
    d_key, "key_leftturn",       &key_leftturn,       SDL_SCANCODE_LEFT,
    d_key, "key_rightturn",      &key_rightturn,      SDL_SCANCODE_RIGHT,
    d_key, "key_backward",       &key_backward,       SDL_SCANCODE_DOWN,

    d_key, "key_weapon1",        &key_weapon1,        SDL_SCANCODE_1,
    d_key, "key_weapon2",        &key_weapon2,        SDL_SCANCODE_2,
    d_key, "key_weapon3",        &key_weapon3,        SDL_SCANCODE_3,
    d_key, "key_weapon4",        &key_weapon4,        SDL_SCANCODE_4,
    d_key, "key_weapon5",        &key_weapon5,        SDL_SCANCODE_5,
    d_key, "key_weapon6",        &key_weapon6,        SDL_SCANCODE_6,
    d_key, "key_weapon7",        &key_weapon7,        SDL_SCANCODE_7,
    d_key, "key_weapon8",        &key_weapon8,        SDL_SCANCODE_8,
    d_key, "key_weapon9",        &key_weapon9,        SDL_SCANCODE_9,

    d_key, "key_map_right",      &key_map_right,      SDL_SCANCODE_RIGHT,
    d_key, "key_map_left",       &key_map_left,       SDL_SCANCODE_LEFT,
    d_key, "key_map_up",         &key_map_up,         SDL_SCANCODE_UP,
    d_key, "key_map_down",       &key_map_down,       SDL_SCANCODE_DOWN,
    d_key, "key_map_zoomin",     &key_map_zoomin,     SDL_SCANCODE_EQUALS,
    d_key, "key_map_zoomout",    &key_map_zoomout,    SDL_SCANCODE_MINUS,
    d_key, "key_map",            &key_map,            SDL_SCANCODE_TAB,
    d_key, "key_map_gobig",      &key_map_gobig,      SDL_SCANCODE_0,
    d_key, "key_map_follow",     &key_map_follow,     SDL_SCANCODE_F,
    d_key, "key_map_mark",       &key_map_mark,       SDL_SCANCODE_M,
    d_key, "key_map_clear",      &key_map_clear,      SDL_SCANCODE_C,
    d_key, "key_map_grid",       &key_map_grid,       SDL_SCANCODE_G, 

    d_key, "key_menu_right",     &key_menu_right,     SDL_SCANCODE_RIGHT,
    d_key, "key_menu_left",      &key_menu_left,      SDL_SCANCODE_LEFT,
    d_key, "key_menu_up",        &key_menu_up,        SDL_SCANCODE_UP,
    d_key, "key_menu_down",      &key_menu_down,      SDL_SCANCODE_DOWN,
    d_key, "key_menu_backspace", &key_menu_backspace, SDL_SCANCODE_BACKSPACE,
    d_key, "key_menu_escape",    &key_menu_escape,    SDL_SCANCODE_ESCAPE,
    d_key, "key_menu_enter",     &key_menu_enter,     SDL_SCANCODE_RETURN,

    d_key, "key_hudup",          &key_hudup,          SDL_SCANCODE_EQUALS,
    d_key, "key_huddn",          &key_huddn,          SDL_SCANCODE_MINUS,


    d_finished, 0, 0, 0
   };

/*
default_t	defaults[] =
{
    {"mouse_sensitivity",&mouseSensitivity, 5},
    {"sfx_volume",&snd_SfxVolume, 8},
    {"music_volume",&snd_MusicVolume, 8},
    {"show_messages",&showMessages, 1},
    

//#ifdef NORMALUNIX
    {"key_right",&key_right, KEY_RIGHTARROW},
    {"key_left",&key_left, KEY_LEFTARROW},
    {"key_up",&key_up, KEY_UPARROW},
    {"key_down",&key_down, KEY_DOWNARROW},
    {"key_strafeleft",&key_strafeleft, ','},
    {"key_straferight",&key_straferight, '.'},

    {"key_fire",&key_fire, KEY_RCTRL},
    {"key_use",&key_use, ' '},
    {"key_strafe",&key_strafe, KEY_RALT},
    {"key_speed",&key_speed, KEY_RSHIFT},

#ifndef WIN32
// UNIX hack, to be removed. 
#ifdef SNDSERV
    {"sndserver", (int *) &sndserver_filename, (int) "sndserver"},
    {"mb_used", &mb_used, 2},
#endif
#endif
    
//#endif

#ifdef LINUX
    {"mousedev", (int*)&mousedev, (int)"/dev/ttyS0"},
    {"mousetype", (int*)&mousetype, (int)"microsoft"},
#endif

    {"use_mouse",&usemouse, 1},
    {"mouseb_fire",&mousebfire,0},
    {"mouseb_strafe",&mousebstrafe,1},
    {"mouseb_forward",&mousebforward,2},

    {"gl_fog",&gl_fog,1},
    {"gl_alphatest",&gl_alphatest,0},
    {"gamename", (int *)gamename, (int) ""},
    {"hudmode",&hudmode,1},

    {"use_joystick",&usejoystick, 0},
    {"joyb_fire",&joybfire,0},
    {"joyb_strafe",&joybstrafe,1},
    {"joyb_use",&joybuse,3},
    {"joyb_speed",&joybspeed,2},

    {"screenblocks",&screenblocks, 9},
    {"detaillevel",&detailLevel, 0},

    {"snd_channels",&numChannels, 3},

    {"usegamma",&usegamma, 0},

    {"chatmacro0", (int *) &chat_macros[0], (int) HUSTR_CHATMACRO0 },
    {"chatmacro1", (int *) &chat_macros[1], (int) HUSTR_CHATMACRO1 },
    {"chatmacro2", (int *) &chat_macros[2], (int) HUSTR_CHATMACRO2 },
    {"chatmacro3", (int *) &chat_macros[3], (int) HUSTR_CHATMACRO3 },
    {"chatmacro4", (int *) &chat_macros[4], (int) HUSTR_CHATMACRO4 },
    {"chatmacro5", (int *) &chat_macros[5], (int) HUSTR_CHATMACRO5 },
    {"chatmacro6", (int *) &chat_macros[6], (int) HUSTR_CHATMACRO6 },
    {"chatmacro7", (int *) &chat_macros[7], (int) HUSTR_CHATMACRO7 },
    {"chatmacro8", (int *) &chat_macros[8], (int) HUSTR_CHATMACRO8 },
    {"chatmacro9", (int *) &chat_macros[9], (int) HUSTR_CHATMACRO9 }

};
*/

int	numdefaults = 134;
const char* defaultfile = "default.cfg";

char DoomDir[128], szValue[32];
void GetCfgName(void);

//
// M_SaveDefaults
//
void M_SaveDefaults (void)
   {
    int		i;
    int		v;
    FILE* f;

    f = fopen(defaultfile, "w");
    if (!f)
        return; // can't write the file, but don't complain

    while(1)
    {
        if (wdefaultv[i].deftype == d_finished)
            break;

        if (wdefaultv[i].defvalue > -0xfff
            && wdefaultv[i].defvalue < 0xfff)
        {
            v = *wdefaultv[i].location;
            fprintf(f, "%s\t\t%i\n", wdefaultv[i].name, v);
        }

        i++;
    }

    fclose(f);
   }


//
// M_LoadDefaults
//
extern byte	scantokey[128];

void GetCfgName()
   {
    /*char *pbreak = NULL;
    int   i;

    if (strlen(myargv[0]) != 0)
       {
        pbreak = strrchr(myargv[0], '\\');
        if (pbreak == NULL)
            pbreak = strrchr(myargv[0], '/');
        if (pbreak == NULL)
           {
            _getcwd(DoomDir, 128);
           }
        else
           {
            i = pbreak-myargv[0];
            strncpy(DoomDir, myargv[0], i);
            DoomDir[i] = 0;
           }
       }
    else
       {
        GetProfileString("GLDOOM", "DIRECTORY", "", DoomDir, 128 );
        if (strlen(DoomDir) == 0)
           {
            _getcwd(DoomDir, 128);
           }
       }
    WriteProfileString("GLDOOM", "DIRECTORY", DoomDir );
    strcat(DoomDir, "\\GLDoom.INI");
    i = 0;*/
   }

//extern windata_t  WinData;

void M_LoadDefaults (void)
{
    //int		i;

    ////// i dunno why this works..
    //i = 0;
    //while (wdefaultv[i].deftype != d_finished)
    //   {
    //    *wdefaultv[i].location = wdefaultv[i].defvalue;
    //    i++;
    //   }

    int		i;
    int		len;
    FILE* f;
    char	def[80];
    char	strparm[100];
    char* newstring;
    int		parm;
    dboolean	isstring;

    // set everything to base values
    numdefaults = sizeof(wdefaultv) / sizeof(wdefaultv[0]);
    for (i = 0; i < numdefaults; i++)
    {
        if (wdefaultv[i].deftype == d_finished)
            break;

        *wdefaultv[i].location = wdefaultv[i].defvalue;
    }

    // check for a custom default file
    i = M_CheckParm("-config");
    if (i && i < myargc - 1)
    {
        defaultfile = myargv[i + 1];
        printf("	default file: %s\n", defaultfile);
    }
    else
        defaultfile = basedefault;

    // read the file in, overriding any set defaults
    f = fopen(defaultfile, "r");
    if (f)
    {
        while (!feof(f))
        {
            isstring = false;
            if (fscanf(f, "%79s %[^\n]\n", def, strparm) == 2)
            {
                if (strparm[0] == '"')
                {
                    // get a string default
                    isstring = true;
                    len = strlen(strparm);
                    newstring = (char*)malloc(len);
                    strparm[len - 1] = 0;
                    strcpy(newstring, strparm + 1);
                }
                else if (strparm[0] == '0' && strparm[1] == 'x')
                    sscanf(strparm + 2, "%x", &parm);
                else
                    sscanf(strparm, "%i", &parm);
                for (i = 0; i < numdefaults; i++) {
                    if (wdefaultv[i].deftype == d_finished)
                        break;

                    if (!strcmp(def, wdefaultv[i].name))
                    {
                        if (!isstring)
                            *wdefaultv[i].location = parm;
                        else
                            *wdefaultv[i].location =
                            (int)newstring;
                        break;
                    }
                }
            }
        }

        fclose(f);
    }
 }

/*
snd_musicdevice		3
snd_sfxdevice		3
snd_sbport		544
snd_sbirq		5
snd_sbdma		1
snd_mport		816

chatmacro0		"no macro"
chatmacro1		"no macro"
chatmacro2		"no macro"
chatmacro3		"no macro"
chatmacro4		"no macro"
chatmacro5		"no macro"
chatmacro6		"no macro"
chatmacro7		"no macro"
chatmacro8		"no macro"
chatmacro9		"no macro"
*/

//
// SCREEN SHOTS
//


typedef struct
{
    char		manufacturer;
    char		version;
    char		encoding;
    char		bits_per_pixel;

    unsigned short	xmin;
    unsigned short	ymin;
    unsigned short	xmax;
    unsigned short	ymax;
    
    unsigned short	hres;
    unsigned short	vres;

    unsigned char	palette[48];
    
    char		reserved;
    char		color_planes;
    unsigned short	bytes_per_line;
    unsigned short	palette_type;
    
    char		filler[58];
    unsigned char	data;		// unbounded
} pcx_t;


//
// WritePCXfile
//
void
WritePCXfile
( char*		filename,
  byte*		data,
  int		width,
  int		height,
  byte*		palette )
{
    int		i;
    int		length;
    pcx_t*	pcx;
    byte*	pack;
	
    pcx = Z_Malloc (width*height*2+1000, PU_STATIC, NULL);

    pcx->manufacturer = 0x0a;		// PCX id
    pcx->version = 5;			// 256 color
    pcx->encoding = 1;			// uncompressed
    pcx->bits_per_pixel = 8;		// 256 color
    pcx->xmin = 0;
    pcx->ymin = 0;
    pcx->xmax = SHORT(width-1);
    pcx->ymax = SHORT(height-1);
    pcx->hres = SHORT(width);
    pcx->vres = SHORT(height);
    memset (pcx->palette,0,sizeof(pcx->palette));
    pcx->color_planes = 1;		// chunky image
    pcx->bytes_per_line = SHORT(width);
    pcx->palette_type = SHORT(2);	// not a grey scale
    memset (pcx->filler,0,sizeof(pcx->filler));


    // pack the image
    pack = &pcx->data;
	
    for (i=0 ; i<width*height ; i++)
    {
	if ( (*data & 0xc0) != 0xc0)
	    *pack++ = *data++;
	else
	{
	    *pack++ = 0xc1;
	    *pack++ = *data++;
	}
    }
    
    // write the palette
    *pack++ = 0x0c;	// palette ID byte
    for (i=0 ; i<768 ; i++)
	*pack++ = gammatable[usegamma][*palette++];
    
    // write output file
    length = pack - (byte *)pcx;
    M_WriteFile (filename, pcx, length);

    Z_Free (pcx);
}

typedef enum
   {
    tga_id = 0,
    tga_cmaptype = 1,
    tga_imgtype = 2,
    tga_cmapfirst = 3,
    tga_cmaplen = 5,
    tga_cmapsize = 7,
    tga_imgorgx = 8,
    tga_imgorgy = 10,
    tga_imgwidth = 12,
    tga_imgheight = 14,
    tga_imgbpp = 16,
    tga_imgbits = 17
   }tga_header_t;

void WriteTGAFile(char *filename, int width, int height, char *buffer)
   {
    int            fn, i;
    short         *s;
    unsigned char  tgahead[18], *cr, *cb, c;

    if ((fn = Open(filename, O_RDWR | O_BINARY | O_CREAT | O_TRUNC, 0666)) != -1)
       {
        memset(tgahead, 0, 18);
        tgahead[tga_imgtype] = 2;
        s = (short *)&tgahead[tga_imgwidth];
        *s = width;
        s = (short *)&tgahead[tga_imgheight];
        *s = height;
        tgahead[tga_imgbpp] = 24;
        // flip the blue and red color components
        cr = &buffer[2];
        cb = &buffer[0];
        for (i = 0; i < (width*height); i++)
           {
            c = *cb;
            *cb = *cr;
            *cr = c;
            cr += 3;
            cb += 3;
           }
        Write(fn, tgahead, 18);
        Write(fn, buffer, (width*height*3));
        Close(fn);
       }
   }

void M_ScreenShot(void)
   {
    int    fn, i, x, y;
    char  *buffer, *c;
    char   lbmname[14];
    
    // find a file name to save it to
    strcpy(lbmname,"DOOM0000.TGA");
		
    for (i = 0; i <= 9999; i++)
       {
	    lbmname[4] =  i / 1000 + '0';
	    lbmname[5] = (i % 1000) / 100 + '0';
	    lbmname[6] = (i % 100) / 10 + '0';
	    lbmname[7] = i % 10 + '0';
	    if (Access(lbmname,0) == -1)
            break;	// file doesn't exist
       }

    buffer = (unsigned char *)malloc(video.width*video.height*3);

    //wglMakeCurrent(hGDC, hRC);
    glReadBuffer( GL_FRONT );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels( 0, 0, video.width, video.height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    WriteTGAFile(lbmname, video.width, video.height, buffer);

    free(buffer);

    //wglMakeCurrent(NULL, NULL);
   }

//
// M_ScreenShot
//

/*
void M_ScreenShot (void)
{
    int		i;
    byte*	linear;
    char	lbmname[12];
    
    GL_ScreenShot();

    // munge planar buffer to linear
    linear = screens[2];
    I_ReadScreen (linear);
    
    // find a file name to save it to
    strcpy(lbmname,"DOOM00.pcx");
		
    for (i=0 ; i<=99 ; i++)
    {
	lbmname[4] = i/10 + '0';
	lbmname[5] = i%10 + '0';
	if (access(lbmname,0) == -1)
	    break;	// file doesn't exist
    }
    if (i==100)
	I_Error ("M_ScreenShot: Couldn't create a PCX");
    
    // save the pcx file
    WritePCXfile (lbmname, linear,
		  SCREENWIDTH, SCREENHEIGHT,
		  W_CacheLumpName ("PLAYPAL",PU_CACHE));
	
    players[consoleplayer].message = "screen shot";
}

*/
