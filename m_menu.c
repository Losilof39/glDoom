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
//	DOOM selection menu, options, episode etc.
//	Sliders and icons. Kinda widget stuff.
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_menu.c,v 1.7 1997/02/03 22:45:10 b1 Exp $";

#include <glad/glad.h>
#include <cglm.h>
#include "renderer.h"

extern sRenderer renderer;

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <inttypes.h>

#include "doomdef.h"
#include "dstrings.h"

#include "d_main.h"

#include "i_system.h"
#include "i_video.h"
#include "z_zone.h"
#include "v_video.h"
#include "w_wad.h"

#include "r_local.h"


#include "hu_stuff.h"

#include "g_game.h"

#include "m_argv.h"
#include "m_swap.h"

#include "s_sound.h"

#include "doomstat.h"

// Data.
#include "sounds.h"

#include "m_menu.h"

#include "d_console.h"
#include "sys_sdl.h"
#include "gl_utils.h"
#include "gldefs.h"

#include "doomcmd.h"

void lfprintf(char *message, ... );

extern patch_t*		hu_font[HU_FONTSIZE];
extern dboolean		message_dontfuckwithme;

extern dboolean		chat_on;		// in heads-up code

extern int          autorun;

//
// defaulted values
//
int         mouseHorizontal;
int         mouseVertical;
int			mouseSensitivity;       // has default

// Show messages has default, 0 = off, 1 = on
int			showMessages;
	

// Blocky mode, has default, 0 = high, 1 = normal
int			detailLevel = 0;		
int			screenblocks = 11;		// has default

// temp for screenblocks (0-9)
int			screenSize;		

// -1 = no quicksave slot picked!
int			quickSaveSlot;          

 // 1 = message to be printed
int			messageToPrint;
// ...and here is the message string!
char*			messageString;		

// message x & y
int			messx;			
int			messy;
int			messageLastMenuActive;

// timed message = no input from user
dboolean			messageNeedsInput;     

void    (*messageRoutine)(int response);

#define SAVESTRINGSIZE 	24

char gammamsg[5][26] =
{
    GAMMALVL0,
    GAMMALVL1,
    GAMMALVL2,
    GAMMALVL3,
    GAMMALVL4
};

// we are going to be entering a savegame string
int			saveStringEnter;              
int             	saveSlot;	// which slot to save in
size_t			saveCharIndex;	// which char we're editing
// old save description before edit
char			saveOldString[SAVESTRINGSIZE];  

dboolean			inhelpscreens;
dboolean			menuactive;

#define SKULLXOFF		-32
#define LINEHEIGHT		16

// jff 3/24/98
extern int defaultskill; // config file specified skill

extern dboolean		sendpause;
char			savegamestrings[10][SAVESTRINGSIZE];

char	endstring[160];

extern int gl_alphatest;
extern int gl_premalpha;

extern int     levelTimeCount;
extern int     levelFragCount;

extern const char playername[18];
extern const char playerskin[18];

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

extern int key_mvert;
extern int key_mlook;
extern int autorun;      // always running?                         // phares
                                                                    //    |
extern int key_right;                                               //    V
extern int key_left;
extern int key_up;
extern int key_down;
extern int key_strafeleft;                                   
extern int key_straferight;
extern int key_fire;
extern int key_use;
extern int key_strafe;
extern int key_speed;
extern int key_escape;
extern int key_savegame;
extern int key_loadgame;
extern int key_autorun;
extern int key_menu_right;                                   // phares 3/7/98
extern int key_menu_left;                                    //     |
extern int key_menu_up;                                      //     V
extern int key_menu_down;                                     
extern int key_menu_backspace;                               //     ^
extern int key_menu_escape;                                  //     |
extern int key_menu_enter;                                   // phares 3/7/98
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

// phares 3/30/98
// externs added for setup menus

extern int destination_keys[MAXPLAYERS];
extern int mousebfire;                                   
extern int mousebstrafe;                               
extern int mousebforward;
extern int mousebbackward;              
                     
extern int default_weapon_recoil;   // weapon recoil        
extern int weapon_recoil;           // weapon recoil           
extern int default_player_bobbing;  // whether player bobs or not         
extern int player_bobbing;          // whether player bobs or not       
extern int weapon_preferences[2][NUMWEAPONS+1];                   
extern int health_red;    // health amount less than which status is red
extern int health_yellow; // health amount less than which status is yellow
extern int health_green;  // health amount above is blue, below is green
extern int armor_red;     // armor amount less than which status is red
extern int armor_yellow;  // armor amount less than which status is yellow
extern int armor_green;   // armor amount above is blue, below is green
extern int ammo_red;      // ammo percent less than which status is red
extern int ammo_yellow;   // ammo percent less is yellow more green
extern int sts_always_red;// status numbers do not change colors
extern int sts_pct_always_gray;// status percents do not change colors
extern int hud_nosecrets; // status does not list secrets/items/kills
extern int sts_traditional_keys;  // display keys the traditional way
extern int hud_list_bgon; // solid window background for list of messages
extern int hud_msg_lines; // number of message lines in window up to 16

extern int mapcolor_back; // map background
extern int mapcolor_grid; // grid lines color
extern int mapcolor_wall; // normal 1s wall color
extern int mapcolor_fchg; // line at floor height change color
extern int mapcolor_cchg; // line at ceiling height change color
extern int mapcolor_clsd; // line at sector with floor=ceiling color
extern int mapcolor_rkey; // red key color
extern int mapcolor_bkey; // blue key color
extern int mapcolor_ykey; // yellow key color
extern int mapcolor_rdor; // red door color  (diff from keys to allow option)
extern int mapcolor_bdor; // blue door color (of enabling one but not other )
extern int mapcolor_ydor; // yellow door color
extern int mapcolor_tele; // teleporter line color
extern int mapcolor_secr; // secret sector boundary color
extern int mapcolor_exit; // jff 4/23/98 exit line
extern int mapcolor_unsn; // computer map unseen line color
extern int mapcolor_flat; // line with no floor/ceiling changes
extern int mapcolor_sprt; // general sprite color
extern int mapcolor_hair; // crosshair color
extern int mapcolor_sngl; // single player arrow color
extern int mapcolor_plyr[4];// colors for player arrows in multiplayer
extern int default_monsters_remember;                     
extern int monsters_remember;                            
extern int hudcolor_titl; // color range of automap level title
extern int hudcolor_xyco; // color range of new coords on automap
extern int hudcolor_mesg; // color range of scrolling messages
extern int hudcolor_chat; // color range of chat lines
extern int hudcolor_list; // color of list of past messages

extern char  chat_macros[10][80];  // chat macros
extern const char* shiftxform;
extern int map_secret_after; //secrets do not appear til after bagged
//extern default_t defaults[];
extern int numdefaults;

extern video_t    video;

//extern int ConFont[2][96];

extern float SetBack;

int    gl_fwidth = 6;
int    gl_fheight = 12;

typedef struct
   {
    float red;
    float green;
    float blue;
   }text_color_t;

typedef enum { tc_white, tc_red, tc_yellow, tc_green, tc_blue, tc_purple, tc_magenta, tc_grey } text_colors_t;

text_color_t  tc[] = { 1.0f, 1.0f, 1.0f,
                       1.0f, 0.0f, 0.0f,
                       1.0f, 1.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       0.0f, 0.0f, 1.0f,
                       1.0f, 0.0f, 1.0f,
                       0.0f, 1.0f, 1.0f,
                       0.75f, 0.75f, 0.75f };

void GL_DrawSetupText(int x, int y, char *text, int color)
   {
    /*int c, ch;
    float left, right, top, bottom;

    left = x - 160.0f;
    top = 120.0f - y;
    bottom = top - gl_fheight;

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

    glColor3f( tc[color].red, tc[color].green, tc[color].blue );

    for (c = 0; text[c] != '\0'; c++)
       {
        right = left + gl_fwidth;
        if (text[c] != ' ')
           {
            ch = text[c] - 33;
            glBindTexture(GL_TEXTURE_2D, ConFont[0][ch]);
            glBegin(GL_QUADS);
               glTexCoord2f(0.0f, 1.0f);
               glVertex3f(left, top, SetBack);
               glTexCoord2f(0.0f, 0.0f);
               glVertex3f(left, bottom, SetBack);
               glTexCoord2f(1.0f, 0.0f);
               glVertex3f(right, bottom, SetBack);
               glTexCoord2f(1.0f, 1.0f);
               glVertex3f(right, top, SetBack);
            glEnd();
           }
        left += gl_fwidth;
       }
    glColor3f( 1.0f, 1.0f, 1.0f );*/
   }

//
// MENU TYPEDEFS
//
typedef struct
{
    // 0 = no cursor here, 1 = ok, 2 = arrows ok
    short	status;
    
    char	name[10];
    
    // choice = menu item #.
    // if status = 2,
    //   choice=0:leftarrow,1:rightarrow
    void	(*routine)(int choice);
    
    // hotkey in menu
    char	alphaKey;			
} menuitem_t;



typedef struct menu_s
{
    short		numitems;	// # of menu items
    struct menu_s*	prevMenu;	// previous menu
    menuitem_t*		menuitems;	// menu items
    void		(*routine)();	// draw routine
    short		x;
    short		y;		// x,y of menu
    short		lastOn;		// last item user was on in menu
} menu_t;

short		itemOn;			// menu item skull is on
short		skullAnimCounter;	// skull animation counter
short		whichSkull;		// which skull to draw

// graphic name of skulls
// warning: initializer-string for array of chars is too long
char    skullName[2][/*8*/9] = {"M_SKULL1","M_SKULL2"};

extern float GLFontHeight;

void CreateGLPalette(void);
void GL_DrawMainMenu(void);
void GL_DrawSaveLoadBorder(int x,int y);
void GL_DrawThermo(int x, int y, int thermWidth, int thermDot );

GLTexData  AmmoBoxSkin;

GLTexData  glMainMenu[7], glEpisode[4], glNewGame[5], glOptions[9], glSound[4], glSetupMenu[7], glMultiPlayer[3];
GLTexData  GameLogo, glOptionTitle, glEpisodeTitle, glNewGameTitle, glSkillTitle, glMPlayerTitle, glDisplayTitle;
GLTexData  glSoundVolTitle, glLoadGameTitle, glSaveGameTitle, glMouseTitle, glGame[3], glMouse[5], glDisplay[6];
GLTexData  glPlayerSetupTitle;

GLTexData  glSetupTitle, glVerSen, glHorSen, glResetBtn[2];
GLTexData  glKeyBind, glAutoMap, glChatStr, glEnemies, glStatusBar, glWeapons, glMessage;

GLTexData  MenuSkull[2], glDtlName[2], glMsgName[2];
GLTexData  GLHudFont[HU_FONTSIZE];
GLTexData  GLGreyFont[HU_FONTSIZE];
GLTexData  glThermL, glThermM, glThermR, glThermO;
GLTexData  LSLeft, LSCenter, LSRight;

GLTexData  glTitlePic[2], glHelp[2], glHelp1[2], glCredit[2], glHelp2[2];

extern int TexWide, TexHigh;

// current menudef
menu_t*	currentMenu;                          

void GL_DrawPatch(GLTexData *Tex, float x, float y);

//
// PROTOTYPES
//
int M_GetPixelWidth(char* ch);

void GL_TileBackground(DW_TexList *tex);
void M_DoNothing(int choice);

void M_Music(int choice);
void M_GameMenu(int choice);
void M_NewGame(int choice);
void M_JoinGame(int choice);
void M_StartGame(int choice);
void M_MultiPlayer(int choice);
void M_Episode(int choice);
void M_ChooseSkill(int choice);
void M_LoadGame(int choice);
void M_SaveGame(int choice);
void M_Options(int choice);
void M_EndGame(int choice);
void M_ReadThis(int choice);
void M_ReadThis2(int choice);
void M_QuitDOOM(int choice);

void M_VertSensitivity(int choice);
void M_HorzSensitivity(int choice);

void M_PlayerSetup(int choice);
void M_Display(int choice);
void M_Mouse(int choice);
void M_Setup(int choice);
void M_ChangeMessages(int choice);
void M_ChangeSensitivity(int choice);
void M_SfxVol(int choice);
void M_MusicVol(int choice);
void M_ChangeDetail(int choice);
void M_SizeDisplay(int choice);
void M_StartGame(int choice);
void M_Sound(int choice);

void M_FinishReadThis(int choice);
void M_LoadSelect(int choice);
void M_SaveSelect(int choice);
void M_ReadSaveStrings(void);
void M_QuickSave(void);
void M_QuickLoad(void);

void M_DrawMusic(void);
void M_DrawJoinGame(void);
void M_DrawStartGame(void);
void M_DrawPlayerSetup(void);
void M_DrawMainMenu(void);
void M_DrawGame(void);
void M_DrawMPlayer(void);
void M_DrawDispOptions(void);
void GL_DrawReadThis1(void);
void GL_DrawReadThis2(void);
void GL_DrawDisplay(void);
void M_DrawReadThis1(void);
void M_DrawReadThis2(void);
void M_DrawNewGame(void);
void M_DrawEpisode(void);
void M_DrawOptions(void);
void M_DrawSetup(void);
void M_DrawDisplay(void);
void M_DrawSound(void);
void M_DrawLoad(void);
void M_DrawSave(void);
void M_DrawMouse(void);
void M_DrawKeybnd(void);
void M_DrawDefVerify(void);

void M_DrawSaveLoadBorder(int x,int y);
void M_SetupNextMenu(menu_t *menudef);
void M_DrawThermo(int x,int y,int thermWidth,int thermDot);
void M_DrawEmptyCell(menu_t *menu,int item);
void M_DrawSelCell(menu_t *menu,int item);
void M_WriteText(int x, int y, char *string);
void GL_WriteText(int x, int y, char *string);
void GL_WriteTextN(int x, int y, char *string, int color);
int  M_StringWidth(char *string);
int  M_StringHeight(char *string);
void M_StartControlPanel(void);
void M_StartMessage(char *string,void *routine,dboolean input);
void M_StopMessage(void);
void M_ClearMenus (void);
void M_KeyBindings(int choice);


extern unsigned char scan2char[256];

//
// DOOM MENU
//
enum
   {
    maintitle,
    newgametitle,
    optiontitle,
    loadgametitle,
    savegametitle,
    skilltitle,
    soundvoltitle,
    episodetitle
   }titles_e;

typedef struct
   {
    char      *lumpname;
    int        y;
    GLTexData *glData;
   }titledata_t;

titledata_t glTitles[] = { "M_DOOM",   2,  &GameLogo,
                           "M_NEWG",   14, &glNewGameTitle,
                           "M_OPTION", 15, &glOptionTitle,
                           "M_LOADG",  28, &glLoadGameTitle,
                           "M_SAVEG",  28, &glSaveGameTitle,
                           "M_SKILL",  38, &glSkillTitle,
                           "M_SVOL",   38, &glSoundVolTitle,
                           "M_EPISOD", 38, &glEpisodeTitle };

enum
{
    //newgame = 0,
    gamemenu = 0,
    mplayer,
    options,
    mvideo,
    //loadgamez,
    //savegamez,
    readthis,
    quitdoom,
    main_end
} main_e;

menuitem_t MainMenu[]=
{
    //{1,"M_NGAME",M_NewGame,'n'},
    {1,"M_GAME",M_GameMenu,'g'},
    {1,"M_MPLAYR",M_MultiPlayer,'m'},
    {1,"M_OPTION",M_Options,'o'},
    {1,"M_DISP",M_Display,'d'},
    //{1,"M_LOADG",M_LoadGame,'l'},
    //{1,"M_SAVEG",M_SaveGame,'s'},
    // Another hickup with Special edition.
    {1,"M_RDTHIS",M_ReadThis,'r'},
    {1,"M_QUITG",M_QuitDOOM,'q'}
};

menu_t  MainDef =
{
    main_end,
    NULL,
    MainMenu,
    M_DrawMainMenu,
    97,64,
    0
};


//
// Game Menu
//

enum
{
    newgame = 0,
    loadgame,
    savegame,
    game_end
} game_e;

menuitem_t GameMenu[]=
{
    //{1,"M_NGAME",M_NewGame,'n'},
    {1,"M_NGAME",M_NewGame,'n'},
    {1,"M_LOADG",M_LoadGame,'l'},
    {1,"M_SAVEG",M_SaveGame,'s'}
};

menu_t  GameDef =
   {
    game_end,		// # of menu items
    &MainDef,		// previous menu
    GameMenu,	    // menuitem_t ->
    M_DrawGame,  	// drawing routine ->
    98,63,          // x,y
    0               // lastOn
   };

//
// EPISODE SELECT
//
enum
{
    ep1,
    ep2,
    ep3,
    ep4,
    ep_end
} episodes_e;

menuitem_t EpisodeMenu[]=
{
    {1,"M_EPI1", M_Episode,'k'},
    {1,"M_EPI2", M_Episode,'t'},
    {1,"M_EPI3", M_Episode,'i'},
    {1,"M_EPI4", M_Episode,'t'}
};

menu_t  EpiDef =
{
    ep_end,		// # of menu items
    &MainDef,		// previous menu
    EpisodeMenu,	// menuitem_t ->
    M_DrawEpisode,	// drawing routine ->
    48,63,              // x,y
    ep1			// lastOn
};

//
// NEW GAME
//
enum
{
    killthings,
    toorough,
    hurtme,
    violence,
    nightmare,
    newg_end
} newgame_e;

menuitem_t NewGameMenu[]=
{
    {1,"M_JKILL",	M_ChooseSkill, 'i'},
    {1,"M_ROUGH",	M_ChooseSkill, 'h'},
    {1,"M_HURT",	M_ChooseSkill, 'h'},
    {1,"M_ULTRA",	M_ChooseSkill, 'u'},
    {1,"M_NMARE",	M_ChooseSkill, 'n'}
};

menu_t  NewDef =
{
    newg_end,		// # of menu items
    &EpiDef,		// previous menu
    NewGameMenu,	// menuitem_t ->
    M_DrawNewGame,	// drawing routine ->
    48,63,              // x,y
    hurtme		// lastOn
};


//
// Multiplayer MENU
//
enum
{
    joingame,
    startgame,
    playersetup,
    mplay_end
} mplayer_e;

menuitem_t MPlayerMenu[]=
{
    {1,"M_JOINGM", M_JoinGame,'j'},
    {1,"M_STRTGM", M_StartGame,'s'},
    {1,"M_PSETUP", M_PlayerSetup,'p'}
};

menu_t  MPlayerDef =
{
    mplay_end,
    &MainDef,
    MPlayerMenu,
    M_DrawMPlayer,
    90,54,
    0
};


//
// OPTIONS MENU
//
enum
{
    msetup,
    endgame,
    messages,
/*
    scrnsize,
    option_empty1,
*/
    musicmenu,
    mousesens,
//    option_empty2,
    soundvol,
    opt_end,
//    detail
} options_e;

menuitem_t OptionsMenu[]=
{
    {1,"M_SETUP",	M_Setup,'s'},
    {1,"M_ENDGAM",	M_EndGame,'e'},
    {1,"M_MESSG",	M_ChangeMessages,'m'},
    {1,"M_MUSIC",	M_Music,'u'},
    //{1,"M_DETAIL",	M_ChangeDetail,'g'},
/*
    {2,"M_SCRNSZ",	M_SizeDisplay,'s'},
    {-1,"",0},
*/
    //{1,"M_DISP",	M_DoNothing,'s'},
//    {2,"M_MSENS",	M_ChangeSensitivity,'m'},
    {1,"M_MSENS",	M_Mouse,'o'},
//    {-1,"",0},
    {1,"M_SVOL",	M_Sound,'v'}
};

menu_t  OptionsDef =
{
    opt_end,
    &MainDef,
    OptionsMenu,
    M_DrawOptions,
    60,37,
    0
};

//
// Display MENU
//
enum
{
    display1,
    display2,
    display_end
} display_e;

menuitem_t DisplayMenu[]=
{
    {1,"M_DISRES", M_DoNothing,'r'},
    {1,"M_FULSCR", M_DoNothing,'f'}
};

menu_t  DisplayDef =
{
    display_end,
    &MainDef,
    DisplayMenu,
    M_DrawDisplay,
    45,60,
    0
};

//
// Join Game MENU
//
enum
{
    findgame,
    joingame_end
} joingame_e;

menuitem_t JoinGameMenu[]=
{
    {1,"", M_DoNothing,'1'}
};

menu_t  JoinGameDef =
{
    joingame_end,
    &MPlayerDef,
    JoinGameMenu,
    M_DrawJoinGame,
    5,5,
    0
};

//
// Join Game MENU
//
enum
{
    localip,
    startgame_end
} startgame_e;

menuitem_t StartGameMenu[]=
{
    {1,"", M_DoNothing,'1'}
};

menu_t  StartGameDef =
{
    startgame_end,
    &MPlayerDef,
    StartGameMenu,
    M_DrawStartGame,
    45,60,
    0
};

//
// Player Setup MENU
//
enum
{
    playname,
    playskin,
    playset3,
    playset4,
    playset5,
    playset6,
    player_end
} player_e;

menuitem_t PlayerSetupMenu[]=
{
    {1,"", M_DoNothing,'1'},
    {1,"", M_DoNothing,'2'},
    {1,"", M_DoNothing,'3'},
    {1,"", M_DoNothing,'4'},
    {1,"", M_DoNothing,'5'},
    {1,"", M_DoNothing,'6'}
};

menu_t  PlayerSetupDef =
{
    player_end,
    &MPlayerDef,
    PlayerSetupMenu,
    M_DrawPlayerSetup,
    45,60,
    0
};

//
// MUSIC MENU
//
enum
   {
    music_type,
    music_end
   }music_e;

menuitem_t MusicMenu[]=
   {
    {1,"", M_DoNothing,'1'}
   };

menu_t  MusicDef =
   {
    music_end,
    &OptionsDef,
    MusicMenu,
    M_DrawMusic,
    45,60,
    0
   };

//
// Mouse MENU
//
enum
{
    vertsens,
    vertentry,
    horzsens,
    horzentry,
    mouse_end
} mouse_e;

menuitem_t MouseMenu[]=
{
    {2,"M_VERSEN",	M_VertSensitivity,'v'},
    {-1,"",0},
    {2,"M_HORSEN",	M_HorzSensitivity,'v'},
    {-1,"",0}
};

menu_t  MouseDef =
{
    mouse_end,
    &OptionsDef,
    MouseMenu,
    M_DrawMouse,
    80,65,
    0
};

/////////////////////////////
//
// The setup_e enum is used to provide a unique number for each group of Setup
// Screens.

enum
  {
  set_key_bindings,                                     
  set_weapons,                                           
  set_statbar,                                           
  set_automap,
  set_enemy,
  set_messages,
  set_chatstrings,
  set_setup_end
  } setup_e;

int setup_screen; // the current setup screen. takes values from setup_e 

/////////////////////////////
//
// SetupMenu is the definition of what the main Setup Screen should look
// like. Each entry shows that the cursor can land on each item (1), the
// built-in graphic lump (i.e. "M_KEYBND") that should be displayed,
// the program which takes over when an item is selected, and the hotkey
// associated with the item.

menuitem_t SetupMenu[]=
  {
  {1,"M_KEYBND",M_KeyBindings,'k'},
  {1,"M_WEAP"  ,M_DoNothing,'w'},
  {1,"M_STAT"  ,M_DoNothing,'s'},               
  {1,"M_AUTO"  ,M_DoNothing,'a'},                    
  {1,"M_ENEM"  ,M_DoNothing,'e'},                     
  {1,"M_MESS"  ,M_DoNothing,'m'},                     
  {1,"M_CHAT"  ,M_DoNothing,'c'},                     
  };

/////////////////////////////
//
// M_DoNothing does just that: nothing. Just a placeholder.

void M_DoNothing(int choice)
  {
  }

/////////////////////////////
//
// Items needed to satisfy the 'Big Font' menu structures:
//
// the generic_setup_e enum mimics the 'Big Font' menu structures, but
// means nothing to the Setup Menus.

enum
  {
  generic_setupempty1,
  generic_setup_end
  } generic_setup_e;

// Generic_Setup is a do-nothing definition that the mainstream Menu code
// can understand, while the Setup Menu code is working. Another placeholder.

menuitem_t Generic_Setup[] =
  {
  {1,"",M_DoNothing,0}
  };

/////////////////////////////
//
// SetupDef is the menu definition that the mainstream Menu code understands.
// This is used by M_Setup (below) to define what is drawn and what is done
// with the main Setup screen.

menu_t  SetupDef =
  {
  set_setup_end, // number of Setup Menu items (Key Bindings, etc.)
  &MainDef,      // menu to return to when BACKSPACE is hit on this menu
  SetupMenu,     // definition of items to show on the Setup Screen
  M_DrawSetup,   // program that draws the Setup Screen
  59,37,         // x,y position of the skull (modified when the skull is
                 // drawn). The skull is parked on the upper-left corner
                 // of the Setup screens, since it isn't needed as a cursor
  0              // last item the user was on for this menu
  };

//
// Read This! MENU 1 & 2
//
enum
{
    rdthsempty1,
    read1_end
} read_e;

menuitem_t ReadMenu1[] =
{
    {1,"",M_ReadThis2,0}
};

menu_t  ReadDef1 =
{
    read1_end,
    &MainDef,
    ReadMenu1,
    GL_DrawReadThis1,
    //M_DrawReadThis1,
    280,175,
    0
};

enum
{
    rdthsempty2,
    read2_end
} read_e2;

menuitem_t ReadMenu2[]=
{
    {1,"",M_FinishReadThis,0}
};

menu_t  ReadDef2 =
{
    read2_end,
    &ReadDef1,
    ReadMenu2,
    GL_DrawReadThis2,
    //M_DrawReadThis2,
    330,175,
    0
};

/*
//
// Start Game Menu
//
enum
{
    gamelevel,
    ip_players,
    playmode,
    timelimit,
    fraglimit,
    startgame_end
} startgame_e;

menuitem_t StartGameMenu[]=
{
//    {2,"M_SFXVOL",M_DoNothing,'s'},
    {-1,"",0},
    {-1,"",0}
};

menu_t  SoundDef =
{
    startgame_end,
    &MplayerDef,
    StartGameMenu,
    M_DrawStartGame,
    80,64,
    0
};

*/
//
// SOUND VOLUME MENU
//
enum
{
    sfx_vol,
    sfx_empty1,
    music_vol,
    sfx_empty2,
    sound_end
} sound_e;

menuitem_t SoundMenu[]=
{
    {2,"M_SFXVOL",M_SfxVol,'s'},
    {-1,"",0},
    {2,"M_MUSVOL",M_MusicVol,'m'},
    {-1,"",0}
};

menu_t  SoundDef =
{
    sound_end,
    &OptionsDef,
    SoundMenu,
    M_DrawSound,
    80,64,
    0
};

//
// LOAD GAME MENU
//
enum
{
    load1,
    load2,
    load3,
    load4,
    load5,
    load6,
    load_end
} load_e;

menuitem_t LoadGameMenu[]=
{
    {1,"", M_LoadSelect,'1'},
    {1,"", M_LoadSelect,'2'},
    {1,"", M_LoadSelect,'3'},
    {1,"", M_LoadSelect,'4'},
    {1,"", M_LoadSelect,'5'},
    {1,"", M_LoadSelect,'6'}
};

menu_t  LoadDef =
{
    load_end,
    &MainDef,
    LoadGameMenu,
    M_DrawLoad,
    80,54,
    0
};

//
// SAVE GAME MENU
//
menuitem_t SaveMenu[]=
{
    {1,"", M_SaveSelect,'1'},
    {1,"", M_SaveSelect,'2'},
    {1,"", M_SaveSelect,'3'},
    {1,"", M_SaveSelect,'4'},
    {1,"", M_SaveSelect,'5'},
    {1,"", M_SaveSelect,'6'}
};

menu_t  SaveDef =
{
    load_end,
    &MainDef,
    SaveMenu,
    M_DrawSave,
    80,54,
    0
};

//
// M_ReadSaveStrings
//  read the strings from the savegame files
//
void M_ReadSaveStrings(void)
{
    int             handle;
    int             count;
    int             i;
    char    name[256];
	
    for (i = 0;i < load_end;i++)
    {
	if (M_CheckParm("-cdrom"))
	    sprintf(name,"c:\\doomdata\\"SAVEGAMENAME"%d.dsg",i);
	else
	    sprintf(name,SAVEGAMENAME"%d.dsg",i);

	handle = Open (name, O_RDONLY | 0, 0666);
	if (handle == -1)
	{
	    strcpy(&savegamestrings[i][0],EMPTYSTRING);
	    LoadGameMenu[i].status = 0;
	    continue;
	}
	count = Read (handle, &savegamestrings[i], SAVESTRINGSIZE);
	Close (handle);
	LoadGameMenu[i].status = 1;
    }
}


//
// M_LoadGame & Cie.
//
void M_DrawLoad(void)
{
/*
    int             i;
	
    V_DrawPatchDirect(((SCREENWIDTH-320)/2)+72,((SCREENHEIGHT-200)/2)+28,0,W_CacheLumpName("M_LOADG",PU_CACHE));
    for (i = 0;i < load_end; i++)
    {
	M_DrawSaveLoadBorder(((SCREENWIDTH-320)/2)+LoadDef.x,((SCREENHEIGHT-200)/2)+LoadDef.y+LINEHEIGHT*i);
	M_WriteText(((SCREENWIDTH-320)/2)+LoadDef.x,((SCREENHEIGHT-200)/2)+LoadDef.y+LINEHEIGHT*i,savegamestrings[i]);
    }
*/
}


void GL_DrawTitle(int y, GLTexData *tex)
   {
    float   Left, Bottom, Right, Top;

    Left = (0.0f - (tex->Width / 2.0f));
    Right = Left + tex->Width;
    Top = 120.0f - (y * 1.2f);
    Bottom = Top - (tex->Height * 1.2f);

    float vertices[] = { Left, Right, Top, Bottom };

    renderer.SetTexture(tex->TexName);
    renderer.RenderSprite(vertices, tex);
    renderer.SetTexture(0);

   }

//
// M_LoadGame & Cie.
//
void GL_DrawLoad(void)
   {
    int     i;


    GL_DrawTitle(28, &glLoadGameTitle);

    for (i = 0;i < load_end; i++)
       {
        GL_DrawSaveLoadBorder(LoadDef.x,LoadDef.y+(LINEHEIGHT*i));
        GL_WriteText(LoadDef.x,LoadDef.y+(LINEHEIGHT*i),savegamestrings[i]);
       }

    glDisable(GL_BLEND);
   }


//
// Draw border for the savegame description
//
void M_DrawSaveLoadBorder(int x,int y)
{
/*
    int             i;
	
    V_DrawPatchDirect (x-8,y+7,0,W_CacheLumpName("M_LSLEFT",PU_CACHE));
	
    for (i = 0;i < 24;i++)
    {
	V_DrawPatchDirect (x,y+7,0,W_CacheLumpName("M_LSCNTR",PU_CACHE));
	x += 8;
    }

    V_DrawPatchDirect (x,y+7,0,W_CacheLumpName("M_LSRGHT",PU_CACHE));
*/
}

//
// Draw border for the savegame description
//
void GL_DrawSaveLoadBorder(int x,int y)
   {
    float    Left, Bottom, Top, Right;
    int      xo, i;
	
    glEnable(GL_TEXTURE_2D);

    xo = x;

    Left = (-160.0f+(float)(xo-LSLeft.Width));
    Right = (-160.0f+(float)xo);
    Top = 120.0f-((y+8)*1.2f);
    Bottom = Top+(LSLeft.Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, LSLeft.TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, LSLeft.YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(LSLeft.XDisp, LSLeft.YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(LSLeft.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, LSCenter.TexName);

    for (i = 0;i < 24;i++)
       {
        Left = (-160.0f+(float)xo);
        Right = (-160.0f+(float)xo+LSCenter.Width);

        glBegin(GL_QUADS);
           glTexCoord2f(0.0f, 1.0f);
           glVertex3f(Left, Top, SetBack);

           glTexCoord2f(0.0f, LSLeft.YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(LSLeft.XDisp, LSLeft.YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(LSLeft.XDisp, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();

        x += 8;
        xo += (int)LSCenter.Width;
       }

    glBindTexture(GL_TEXTURE_2D, LSRight.TexName);
    Left = (-160.0f+(float)xo);
    Right = (-160.0f+(float)xo+LSRight.Width);

    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, LSRight.YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(LSRight.XDisp, LSRight.YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(LSRight.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

    glDisable(GL_TEXTURE_2D);
   }

//
// User wants to load this game
//
void M_LoadSelect(int choice)
{
    char    name[256];
	
    if (M_CheckParm("-cdrom"))
	sprintf(name,"c:\\doomdata\\"SAVEGAMENAME"%d.dsg",choice);
    else
	sprintf(name,SAVEGAMENAME"%d.dsg",choice);
    G_LoadGame (name);
    M_ClearMenus ();
}

//
// Selected from DOOM menu
//
void M_LoadGame (int choice)
{
    if (netgame)
    {
	M_StartMessage(LOADNET,NULL,false);
	return;
    }
	
    M_SetupNextMenu(&LoadDef);
    M_ReadSaveStrings();
}


void M_DrawDispOpt(void)
   {
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+49,((SCREENHEIGHT-200)/2)+28,0,W_CacheLumpName("M_DISOPT",PU_CACHE));
   }

//
//  M_SaveGame & Cie.
//
void M_DrawSave(void)
{
/*
    int             i;
	
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+72,((SCREENHEIGHT-200)/2)+28,0,W_CacheLumpName("M_SAVEG",PU_CACHE));
    for (i = 0;i < load_end; i++)
    {
	M_DrawSaveLoadBorder(((SCREENWIDTH-320)/2)+LoadDef.x,((SCREENHEIGHT-200)/2)+LoadDef.y+LINEHEIGHT*i);
	M_WriteText(((SCREENWIDTH-320)/2)+LoadDef.x,((SCREENHEIGHT-200)/2)+LoadDef.y+LINEHEIGHT*i,savegamestrings[i]);
    }
	
    if (saveStringEnter)
    {
	i = M_StringWidth(savegamestrings[saveSlot]);
	M_WriteText(((SCREENWIDTH-320)/2)+LoadDef.x + i,((SCREENHEIGHT-200)/2)+LoadDef.y+LINEHEIGHT*saveSlot,"_");
    }
*/
}

//
// M_SaveGame & Cie.
//
void GL_DrawSave(void)
   {
    int     i;

    GL_DrawTitle(28, &glSaveGameTitle);

    for (i = 0;i < load_end; i++)
    {
     GL_DrawSaveLoadBorder(LoadDef.x,LoadDef.y+LINEHEIGHT*i);
     GL_WriteText(LoadDef.x,LoadDef.y+LINEHEIGHT*i,savegamestrings[i]);
    }
	
    if (saveStringEnter)
       {
        i = M_StringWidth(savegamestrings[saveSlot]);
        GL_WriteText(LoadDef.x+i,LoadDef.y+LINEHEIGHT*saveSlot,"_");
       }

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

//
// M_Responder calls this when user is finished
//
void M_DoSave(int slot)
{
    G_SaveGame (slot,savegamestrings[slot]);
    M_ClearMenus ();

    // PICK QUICKSAVE SLOT YET?
    if (quickSaveSlot == -2)
	quickSaveSlot = slot;
}

//
// User wants to save. Start string input for M_Responder
//
void M_SaveSelect(int choice)
{
    // we are going to be intercepting all chars
    saveStringEnter = 1;
    
    saveSlot = choice;
    strcpy(saveOldString,savegamestrings[choice]);
    if (!strcmp(savegamestrings[choice],EMPTYSTRING))
	savegamestrings[choice][0] = 0;
    saveCharIndex = strlen(savegamestrings[choice]);
}

//
// Selected from DOOM menu
//
void M_SaveGame (int choice)
{
    if (!usergame)
    {
	M_StartMessage(SAVEDEAD,NULL,false);
	return;
    }
	
    if (gamestate != GS_LEVEL)
	return;
	
    M_SetupNextMenu(&SaveDef);
    M_ReadSaveStrings();
}



//
//      M_QuickSave
//
char    tempstring[80];

void M_QuickSaveResponse(int ch)
{
    if (ch == SDL_SCANCODE_Y)
    {
	M_DoSave(quickSaveSlot);
	S_StartSound(NULL,sfx_swtchx);
    }
}

void M_QuickSave(void)
{
    if (!usergame)
    {
	S_StartSound(NULL,sfx_oof);
	return;
    }

    if (gamestate != GS_LEVEL)
	return;
	
    if (quickSaveSlot < 0)
    {
	M_StartControlPanel();
	M_ReadSaveStrings();
	M_SetupNextMenu(&SaveDef);
	quickSaveSlot = -2;	// means to pick a slot now
	return;
    }
    sprintf(tempstring,QSPROMPT,savegamestrings[quickSaveSlot]);
    M_StartMessage(tempstring,M_QuickSaveResponse,true);
}



//
// M_QuickLoad
//
void M_QuickLoadResponse(int ch)
{
    if (ch == SDL_SCANCODE_Y)
    {
	M_LoadSelect(quickSaveSlot);
	S_StartSound(NULL,sfx_swtchx);
    }
}


void M_QuickLoad(void)
{
    if (netgame)
    {
	M_StartMessage(QLOADNET,NULL,false);
	return;
    }
	
    if (quickSaveSlot < 0)
    {
	M_StartMessage(QSAVESPOT,NULL,false);
	return;
    }
    sprintf(tempstring,QLPROMPT,savegamestrings[quickSaveSlot]);
    M_StartMessage(tempstring,M_QuickLoadResponse,true);
}




//
// Read This Menus
// Had a "quick hack to fix romero bug"
//
void M_DrawReadThis1(void)
{
    inhelpscreens = true;
    switch ( gamemode )
    {
      case commercial:
	V_DrawPatchDirect (((SCREENWIDTH-320)/2)+0,((SCREENHEIGHT-200)/2)+0,0,W_CacheLumpName("HELP",PU_CACHE));
	break;
      case shareware:
      case registered:
      case retail:
	V_DrawPatchDirect (((SCREENWIDTH-320)/2)+0,((SCREENHEIGHT-200)/2)+0,0,W_CacheLumpName("HELP1",PU_CACHE));
	break;
      default:
	break;
    }
    return;
}

void GL_DrawFullScreen(GLTexData *Image)
   {
    float      Top, Left, Right, Bottom;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    Top = 120.0f;
    Bottom = -120.0f;

    Left = -160.0f;
    Right = 96.0f;

    glBindTexture(GL_TEXTURE_2D, Image[0].TexName);

    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, Image[0].YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(Image[0].XDisp, Image[0].YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(Image[0].XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

    Left = 96.0f;
    Right = 160.0f;

    glBindTexture(GL_TEXTURE_2D, Image[1].TexName);

    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, Image[1].YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(Image[1].XDisp, Image[1].YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(Image[1].XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();
   }

void GL_DrawReadThis1(void)
   {
    inhelpscreens = true;
    switch ( gamemode )
       {
        case commercial:
             GL_DrawFullScreen(glHelp);
	         break;
        case shareware:
        case registered:
        case retail:
             GL_DrawFullScreen(glHelp1);
	         break;
        default:
	         break;
       }

    return;
   }



//
// Read This Menus - optional second page.
//
void M_DrawReadThis2(void)
{
    inhelpscreens = true;
    switch ( gamemode )
    {
      case retail:
      case commercial:
  	// This hack keeps us from having to change menus.
	V_DrawPatchDirect (((SCREENWIDTH-320)/2)+0,((SCREENHEIGHT-200)/2)+0,0,W_CacheLumpName("CREDIT",PU_CACHE));
	break;
      case shareware:
      case registered:
	V_DrawPatchDirect (((SCREENWIDTH-320)/2)+0,((SCREENHEIGHT-200)/2)+0,0,W_CacheLumpName("HELP2",PU_CACHE));
	break;
      default:
	break;
    }
    return;
}

void GL_DrawReadThis2(void)
   {
    inhelpscreens = true;
    switch ( gamemode )
       {
        case retail:
        case commercial:
	         // This hack keeps us from having to change menus.
             GL_DrawFullScreen(glCredit);
	         break;
        case shareware:
        case registered:
             GL_DrawFullScreen(glHelp2);
	         break;
        default:
	         break;
       }
    return;
   }


//
// Change Sfx & Music volumes
//
void M_DrawSound(void)
{
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+60,((SCREENHEIGHT-200)/2)+38,0,W_CacheLumpName("M_SVOL",PU_CACHE));

    M_DrawThermo(SoundDef.x,SoundDef.y+LINEHEIGHT*(sfx_vol+1),
		 16,snd_SfxVolume);

    M_DrawThermo(SoundDef.x,SoundDef.y+LINEHEIGHT*(music_vol+1),
		 16,snd_MusicVolume);
*/
}

//
// New Game/Load Game/Save Game
//
void M_DrawGame(void)
   {
   }

void GL_DrawGame(void)
   {
    GL_DrawTitle(28, &glMainMenu[0]);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

//
// Join a multiplayer game
//

void M_JoinGame(int choice)
  {
      M_SetupNextMenu(&JoinGameDef);
  }

void M_DrawJoinGame(void)
   {
   }

void GL_DrawJoinGame(void)
   {
    GL_DrawTitle(28, &glMultiPlayer[0]);

    GL_WriteTextN(  3, 55, "Available",       tc_red);
    GL_WriteTextN(100, 55, "Mode",            tc_red);
    GL_WriteTextN(175, 55, "Mons",            tc_red);
    GL_WriteTextN(210, 55, "Resp",            tc_red);
    GL_WriteTextN(245, 55, "Frags",           tc_red);
    GL_WriteTextN(285, 55, "Time",            tc_red);

    GL_WriteTextN(  3, 70, "192.168.28.55",   tc_white);
    GL_WriteTextN(100, 70, "CO-OP",           tc_white);
    GL_WriteTextN(175, 70, "YES",             tc_white);
    GL_WriteTextN(210, 70, "NO",              tc_white);
    GL_WriteTextN(245, 70, "NONE",            tc_white);
    GL_WriteTextN(285, 70, "NONE",            tc_white);

    GL_WriteTextN(  3, 80, "192.168.106.122", tc_grey);
    GL_WriteTextN(100, 80, "ALTDEATH",        tc_grey);
    GL_WriteTextN(175, 80, "NO",              tc_grey);
    GL_WriteTextN(210, 80, "N/A",             tc_grey);
    GL_WriteTextN(245, 80, "50",              tc_grey);
    GL_WriteTextN(285, 80, "20",              tc_grey);

    GL_WriteTextN(  3, 90, "202.219.200.122", tc_grey);
    GL_WriteTextN(100, 90, "DEATHMATCH",      tc_grey);
    GL_WriteTextN(175, 90, "NO",              tc_grey);
    GL_WriteTextN(210, 90, "N/A",             tc_grey);
    GL_WriteTextN(245, 90, "NONE",            tc_grey);
    GL_WriteTextN(285, 90, "10",              tc_grey);

    GL_WriteTextN(  3,100, "202.219.200.128", tc_grey);
    GL_WriteTextN(100,100, "TEAMDEATH",       tc_grey);
    GL_WriteTextN(175,100, "NO",              tc_grey);
    GL_WriteTextN(210,100, "N/A",             tc_grey);
    GL_WriteTextN(245,100, "30",              tc_grey);
    GL_WriteTextN(285,100, "NONE",            tc_grey);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

//
// Start a multiplayer game
//

void M_StartGame(int choice)
  {
      M_SetupNextMenu(&StartGameDef);
  }

char *sg_captions[] = { "START MAP",
                        "GAME MODE",
                        "MONSTERS",
                        "RESPAWN",
                        "FRAG LIMIT",
                        "TIME LIMIT",
                        "ACCEPT",
                        "" };

char *dm_modes[] = { "CO-OP", "DEATHMATCH", "ALTDEATH", "TEAMDEATH", "CTF" };

int mstartgame_option = 0;

void M_DrawStartGame(void)
   {
   }

void GL_DrawStartGame(void)
   {
    char msgstr[32];
    int i, minutes;

    GL_DrawTitle(28, &glMultiPlayer[1]);

    i = 0;
    while (sg_captions[i][0])
       {
        GL_WriteTextN(156-M_GetPixelWidth(sg_captions[i]), StartGameDef.y+(10*i)+8, sg_captions[i], (i == mstartgame_option) ? tc_yellow : tc_red);
        i++;
       }

    if (gamemode == commercial)
       {
        sprintf(msgstr,"MAP%02d", startmap);
       }
    else
       {
        sprintf(msgstr,"E%dM%d", startepisode, startmap);
       }
    GL_WriteTextN(164, StartGameDef.y+8, msgstr, tc_white);
    sprintf(msgstr, "%s", dm_modes[deathmatch] );
    GL_WriteTextN(164, StartGameDef.y+(10*1)+8, msgstr, tc_grey);
    sprintf(msgstr, "%s", (nomonsters == 0) ? "YES" : "NO" );
    GL_WriteTextN(164, StartGameDef.y+(10*2)+8, msgstr, tc_grey);
    sprintf(msgstr, "%s", respawnmonsters ? "YES" : "NO" );
    GL_WriteTextN(164, StartGameDef.y+(10*3)+8, msgstr, tc_grey);
    if (levelFragCount > 0)
       {
        sprintf(msgstr, "%d", levelFragCount );
       }
    else
       {
        sprintf(msgstr, "NONE" );
       }
    GL_WriteTextN(164, StartGameDef.y+(10*4)+8, msgstr, tc_grey);
    if (levelTimeCount > 0)
       {
        minutes = (levelTimeCount/35)/60;
        sprintf(msgstr, "%d MINUTE%s", minutes, (minutes > 1) ? "S" : "" );
       }
    else
       {
        sprintf(msgstr, "NONE" );
       }
    GL_WriteTextN(164, StartGameDef.y+(10*5)+8, msgstr, tc_grey);
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

//
// M_PlayerSetup
//

int mpsetup_option = 0;

void M_PlayerSetup(int choice)
   {
    M_SetupNextMenu(&PlayerSetupDef);
   }

void M_DrawPlayerSetup(void)
  {
   V_DrawPatchDirect(120,15,0,W_CacheLumpName("M_PSETUP",PU_CACHE));
  }

char *ps_captions[] = { "PLAYER NAME",
                        "PLAYER SKIN",
                        "MALE/FEMALE",
                        "COLOR PREF",
                        "TEAM",
                        "" };

void GL_DrawPlayerSetup(void)
   {
    char    msgstr[32];
    int     i;

    GL_DrawTitle(25, &glPlayerSetupTitle);
    i = 0;
    while (ps_captions[i][0])
       {
        GL_WriteTextN(156-M_GetPixelWidth(ps_captions[i]), PlayerSetupDef.y+(10*i)+8, ps_captions[i], (i == mpsetup_option) ? tc_yellow : tc_red);
        i++;
       }

    sprintf(msgstr, "%s", playername);
    GL_WriteTextN(164, PlayerSetupDef.y+8, msgstr, tc_white);
    sprintf(msgstr, "%s", playerskin);
    GL_WriteTextN(164, PlayerSetupDef.y+(10)+8, msgstr, tc_grey);
    sprintf(msgstr, "%s", "UNKNOWN");
    GL_WriteTextN(164, PlayerSetupDef.y+(10*2)+8, msgstr, tc_grey);
    sprintf(msgstr, "%s", "BLUE");
    GL_WriteTextN(164, PlayerSetupDef.y+(10*3)+8, msgstr, tc_grey);
    sprintf(msgstr, "%s", "RED");
    GL_WriteTextN(164, PlayerSetupDef.y+(10*4)+8, msgstr, tc_grey);
   }

//
// Join/Start/Setup a multiplayer game
//

void M_MultiPlayer(int choice)
{
    M_SetupNextMenu(&MPlayerDef);
}

void M_DrawMPlayer(void)
   {
   }

void GL_DrawMPlayer(void)
   {
    GL_DrawTitle(28, &glMPlayerTitle);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

//
// Music options menu option
//

void M_Music(int choice)
   {
    M_SetupNextMenu(&MusicDef);
   }

char *mm_captions[] = { "Music Source",
                        "Selection Control",
                        "Song Mode",
                        "" };

char inactive[] = "This is menu is currently inactive.";

int mmusic_option = 0;

void M_DrawMusic(void)
   {
   }

void GL_DrawMusic(void)
   {
    int i;

    GL_DrawTitle(28, &glOptions[musicmenu]);

    GL_WriteTextN(160-(M_GetPixelWidth(inactive)/2), 54, inactive, tc_white);

    i = 0;
    while (mm_captions[i][0])
       {
        GL_WriteTextN(156-M_GetPixelWidth(mm_captions[i]), MusicDef.y+(10*i)+8, mm_captions[i], (i == mmusic_option) ? tc_yellow : tc_red);
        i++;
       }

    GL_WriteTextN(164, MusicDef.y+(10*0)+8, "INTERNAL MUS DATA", tc_white);
    GL_WriteTextN(164, MusicDef.y+(10*1)+8, "ORIGINAL CODING", tc_grey);
    GL_WriteTextN(164, MusicDef.y+(10*2)+8, "LOOP SONG", tc_grey);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

//
// Change Sfx & Music volumes
//
void GL_DrawSound(void)
   {
    GL_DrawTitle(38, &glSoundVolTitle);

    GL_DrawThermo(SoundDef.x,SoundDef.y+LINEHEIGHT*(sfx_vol+1),16,snd_SfxVolume);

    GL_DrawThermo(SoundDef.x,SoundDef.y+LINEHEIGHT*(music_vol+1),16,snd_MusicVolume);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

void M_Setup(int choice)
  {
      M_SetupNextMenu(&SetupDef);
  }

void M_Sound(int choice)
{
    M_SetupNextMenu(&SoundDef);
}

void M_SfxVol(int choice)
{
    switch(choice)
    {
      case 0:
	if (snd_SfxVolume)
	    snd_SfxVolume--;
	break;
      case 1:
	if (snd_SfxVolume < 15)
	    snd_SfxVolume++;
	break;
    }
	
    S_SetSfxVolume(snd_SfxVolume /* *8 */);
}

void M_MusicVol(int choice)
{
    switch(choice)
    {
      case 0:
	if (snd_MusicVolume)
	    snd_MusicVolume--;
	break;
      case 1:
	if (snd_MusicVolume < 15)
	    snd_MusicVolume++;
	break;
    }
	
    S_SetMusicVolume(snd_MusicVolume /* *8 */);
}




//
// M_DrawMainMenu
//
void M_DrawMainMenu(void)
{
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+94,((SCREENHEIGHT-200)/2)+2,0,W_CacheLumpName("M_DOOM",PU_CACHE));
*/
}

void GL_DrawMainMenu()
   {
    GL_DrawTitle(2, &GameLogo);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }




//
// M_NewGame
//
void M_DrawNewGame(void)
{
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+96,((SCREENHEIGHT-200)/2)+14,0,W_CacheLumpName("M_NEWG",PU_CACHE));
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+54,((SCREENHEIGHT-200)/2)+38,0,W_CacheLumpName("M_SKILL",PU_CACHE));
*/
}

//
// M_NewGame
//
void GL_DrawNewGame(void)
   {
    float   Left, Right, Top, Bottom;

    GL_DrawTitle(14, &glNewGameTitle);

    Left = (0.0f-(glSkillTitle.Width/2.0f));
    Right = Left+glSkillTitle.Width;
    Top = 120.0f-(38*1.2f);
    Bottom = Top-(glSkillTitle.Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, glSkillTitle.TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, glSkillTitle.YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(glSkillTitle.XDisp, glSkillTitle.YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(glSkillTitle.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

void M_NewGame(int choice)
{
    if (netgame && !demoplayback)
    {
	M_StartMessage(NEWGAME,NULL,false);
	return;
    }
	
    if ( gamemode == commercial )
	M_SetupNextMenu(&NewDef);
    else
	M_SetupNextMenu(&EpiDef);
}


//
//      M_Episode
//
int     epi;

void M_DrawEpisode(void)
{
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+54,((SCREENHEIGHT-200)/2)+38,0,W_CacheLumpName("M_EPISOD",PU_CACHE));
*/
}

void GL_DrawEpisode(void)
   {
    GL_DrawTitle(38, &glEpisodeTitle);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

void M_VerifyNightmare(int ch)
{
    if (ch != SDL_SCANCODE_Y)
	return;
		
    G_DeferedInitNew(nightmare,epi+1,1);
    M_ClearMenus ();
}

extern dboolean bShowTitle;

void M_ChooseSkill(int choice)
{
    if (choice == nightmare)
    {
	M_StartMessage(NIGHTMARE,M_VerifyNightmare,true);
	return;
    }
	
    bShowTitle = false;
    G_DeferedInitNew(choice,epi+1,1);
    M_ClearMenus ();
}

void M_Episode(int choice)
{
    if ( (gamemode == shareware)
	 && choice)
    {
	M_StartMessage(SWSTRING,NULL,false);
	M_SetupNextMenu(&ReadDef1);
	return;
    }

    // Yet another hack...
    if ( (gamemode == registered)
	 && (choice > 2))
    {
      fprintf( stderr,
	       "M_Episode: 4th episode requires UltimateDOOM\n");
      choice = 0;
    }
	 
    epi = choice;
    M_SetupNextMenu(&NewDef);
}


/////////////////////////////
//
// M_DrawBackground tiles a 64x64 patch over the entire screen, providing the
// background for the Help and Setup screens.

void M_DrawBackground(char* patchname)

  {
  int     x,y;
  byte*   back_src;
  byte*   back_dest;

  // killough 4/17/98: 
  back_src = W_CacheLumpNum(firstflat+R_FlatNumForName(patchname),PU_CACHE);

  back_dest = screens[0]; // screen buffer
  for (y = 0 ; y < SCREENHEIGHT ; y++)
    {
    for (x = 0 ; x < SCREENWIDTH/64 ; x++)
      {
      memcpy (back_dest,back_src+((y & 63)<<6),64);
      back_dest += 64;
      }
    if (SCREENWIDTH & 63)
      {
      memcpy (back_dest,back_src+((y & 63)<<6),SCREENWIDTH & 63);
      back_dest += (SCREENWIDTH & 63);
      }
    }
  }

extern DW_TexList TexList[1024];
extern int        TexCount;
extern int        ftranslate[1024];
extern dboolean TexTransparent;

/////////////////////////////
//
// M_DrawBackground tiles a 64x64 patch over the entire screen, providing the
// background for the Help and Setup screens.

void GL_DrawBackground(char* patchname)
   {
    int  texnumb, lump;

    lump = W_CheckNumForName(patchname);
    for (texnumb = 0; texnumb < TexCount; texnumb++)
       {
        if (((TexList[texnumb].Number+firstflat) == lump) && (TexList[texnumb].Type == 'F'))
           {
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
       }
    GL_TileBackground(&TexList[texnumb]);
   }

void M_DrawSetup(void)
  {
  V_DrawPatchDirect(124,15,0,W_CacheLumpName("M_SETUP",PU_CACHE));
  }

void GL_DrawSetup(void)
  {
    GL_DrawTitle(15, &glSetupTitle);
  }


//
// M_Display
//

void M_Display(int choice)
   {
    M_SetupNextMenu(&DisplayDef);
   }

char *ds_captions[] = { "Resolution",
                        "Full Screen",
                        "" };

int mdisplay_option = 0;

void M_DrawDisplay(void)
  {
   V_DrawPatchDirect(120,15,0,W_CacheLumpName("M_DISOPT",PU_CACHE));

   GL_DrawTitle(25, &glPlayerSetupTitle);
   }

void GL_DrawDisplay(void)
   {
    char    msgstr[32];
    int     i;

    GL_DrawTitle(25, &glDisplayTitle);

    i = 0;
    while (ds_captions[i][0])
       {
        GL_WriteTextN(156-M_GetPixelWidth(ds_captions[i]), DisplayDef.y+(10*i)+8, ds_captions[i], (i == mdisplay_option) ? tc_yellow : tc_red);
        i++;
       }

    sprintf(msgstr,"%d X %d X %d", video.width, video.height, video.bpp);
    GL_WriteTextN(164, DisplayDef.y+8, msgstr, tc_white);
    sprintf(msgstr, "%s", video.fullscreen ? "YES" : "NO" );
    GL_WriteTextN(164, DisplayDef.y+(10)+8, msgstr, tc_grey);
   }

//
// M_Mouse
//

void M_Mouse(int choice)
  {
      M_SetupNextMenu(&MouseDef);
  }

void M_DrawMouse(void)
   {
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+108,((SCREENHEIGHT-200)/2)+15,0,W_CacheLumpName("M_OPTTTL",PU_CACHE));
	
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+OptionsDef.x + 175,((SCREENHEIGHT-200)/2)+OptionsDef.y+LINEHEIGHT*detail,0,
		       W_CacheLumpName(detailNames[detailLevel],PU_CACHE));

    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+OptionsDef.x + 120,((SCREENHEIGHT-200)/2)+OptionsDef.y+LINEHEIGHT*messages,0,
		       W_CacheLumpName(msgNames[showMessages],PU_CACHE));

    M_DrawThermo(OptionsDef.x,OptionsDef.y+LINEHEIGHT*(mousesens+1),
		 10,mouseSensitivity);
	
    M_DrawThermo(OptionsDef.x,OptionsDef.y+LINEHEIGHT*(scrnsize+1),9,screenSize);
*/
   }

void GL_DrawMouse(void)
   {

    GL_DrawTitle(35, &glMouseTitle);

    GL_DrawThermo(MouseDef.x, MouseDef.y+(LINEHEIGHT*(vertsens+1))+1,20,mouseVertical);

    GL_DrawThermo(MouseDef.x, MouseDef.y+(LINEHEIGHT*(horzsens+1))+1,20,mouseHorizontal);
   }

//
// M_Options
//
char    detailNames[2][9]	= {"M_GDHIGH","M_GDLOW"};
char	msgNames[2][9]		= {"M_MSGOFF","M_MSGON"};


void M_DrawOptions(void)
   {
/*
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+108,((SCREENHEIGHT-200)/2)+15,0,W_CacheLumpName("M_OPTTTL",PU_CACHE));
	
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+OptionsDef.x + 175,((SCREENHEIGHT-200)/2)+OptionsDef.y+LINEHEIGHT*detail,0,
		       W_CacheLumpName(detailNames[detailLevel],PU_CACHE));

    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+OptionsDef.x + 120,((SCREENHEIGHT-200)/2)+OptionsDef.y+LINEHEIGHT*messages,0,
		       W_CacheLumpName(msgNames[showMessages],PU_CACHE));

    M_DrawThermo(OptionsDef.x,OptionsDef.y+LINEHEIGHT*(mousesens+1),
		 10,mouseSensitivity);
	
    M_DrawThermo(OptionsDef.x,OptionsDef.y+LINEHEIGHT*(scrnsize+1),9,screenSize);
*/
   }

void GL_DrawOptions(void)
   {
    float   Left, Right, Top, Bottom;

    GL_DrawTitle(15, &glOptionTitle);

/*  Detail level is antiquated

    Left = (-160.0f+OptionsDef.x+175);
    Right = Left+glDtlName[detailLevel].Width;
    Top = 120.0f-((OptionsDef.y+(LINEHEIGHT*detail)+3)*1.2f);
    Bottom = Top-(glDtlName[detailLevel].Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, glDtlName[detailLevel].TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, glDtlName[detailLevel].YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(glDtlName[detailLevel].XDisp, glDtlName[detailLevel].YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(glDtlName[detailLevel].XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();
*/
    Left = (-160.0f+OptionsDef.x+120);
    Right = Left+glMsgName[showMessages].Width;
    Top = 120.0f-((OptionsDef.y+(LINEHEIGHT*messages)+3)*1.2f);
    Bottom = Top-(glMsgName[showMessages].Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, glMsgName[showMessages].TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, glMsgName[showMessages].YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(glMsgName[showMessages].XDisp, glMsgName[showMessages].YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(glMsgName[showMessages].XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

//    GL_DrawThermo(OptionsDef.x,OptionsDef.y+(LINEHEIGHT*(scrnsize+1))+1,9,screenSize);

//    GL_DrawThermo(OptionsDef.x,OptionsDef.y+(LINEHEIGHT*(mousesens+1))+1,10,mouseSensitivity);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

void M_Options(int choice)
{
    M_SetupNextMenu(&OptionsDef);
}

void M_GameMenu(int choice)
{
    M_SetupNextMenu(&GameDef);
}





//
//      Toggle messages on/off
//
void M_ChangeMessages(int choice)
{
    // warning: unused parameter `int choice'
    choice = 0;
    showMessages = 1 - showMessages;
	
    if (!showMessages)
	players[consoleplayer].message = MSGOFF;
    else
	players[consoleplayer].message = MSGON ;

    message_dontfuckwithme = true;
}


//
// M_EndGame
//
void M_EndGameResponse(int ch)
{
    if (ch != SDL_SCANCODE_Y)
	return;
		
    currentMenu->lastOn = itemOn;
    M_ClearMenus ();
    D_StartTitle ();
}

void M_EndGame(int choice)
{
    choice = 0;
    if (!usergame)
    {
	S_StartSound(NULL,sfx_oof);
	return;
    }
	
    if (netgame)
    {
	M_StartMessage(NETEND,NULL,false);
	return;
    }
	
    M_StartMessage(ENDGAME,M_EndGameResponse,true);
}




//
// M_ReadThis
//
void M_ReadThis(int choice)
{
    choice = 0;
    M_SetupNextMenu(&ReadDef1);
}

void M_ReadThis2(int choice)
{
    choice = 0;
    M_SetupNextMenu(&ReadDef2);
}

void M_FinishReadThis(int choice)
{
    choice = 0;
    M_SetupNextMenu(&MainDef);
}




//
// M_QuitDOOM
//
int     quitsounds[8] =
{
    sfx_pldeth,
    sfx_dmpain,
    sfx_popain,
    sfx_slop,
    sfx_telept,
    sfx_posit1,
    sfx_posit3,
    sfx_sgtatk
};

int     quitsounds2[8] =
{
    sfx_vilact,
    sfx_getpow,
    sfx_boscub,
    sfx_slop,
    sfx_skeswg,
    sfx_kntdth,
    sfx_bspact,
    sfx_sgtatk
};


static char MsgText[2048];

void M_QuitResponse(int ch)
{
    if (ch != SDL_SCANCODE_Y)
       {
        return;
       }
    if (!netgame)
    {
	if (gamemode == commercial)
	    S_StartSound(NULL,quitsounds2[(gametic>>2)&7]);
	else
	    S_StartSound(NULL,quitsounds[(gametic>>2)&7]);
	I_WaitVBL(105);
    }
    I_Quit ();
}




void M_QuitDOOM(int choice)
{
  // We pick index 0 which is language sensitive,
  //  or one at random, between 1 and maximum number.
  if (language != english )
    sprintf(endstring,"%s\n\n"DOSY, endmsg[0] );
  else
    sprintf(endstring,"%s\n\n"DOSY, endmsg[ (gametic%(NUM_QUITMESSAGES-10))+1 ]);
  
  M_StartMessage(endstring,M_QuitResponse,true);
}



void M_VertSensitivity(int choice)
{
    switch(choice)
    {
      case 0:
	if (mouseVertical)
	    mouseVertical--;
	break;
      case 1:
	if (mouseVertical < 19)
	    mouseVertical++;
	break;
    }
}

void M_HorzSensitivity(int choice)
{
    switch(choice)
    {
      case 0:
	if (mouseHorizontal)
	    mouseHorizontal--;
	break;
      case 1:
	if (mouseHorizontal < 19)
	    mouseHorizontal++;
	break;
    }
}


void M_ChangeSensitivity(int choice)
{
    switch(choice)
    {
      case 0:
	if (mouseSensitivity)
	    mouseSensitivity--;
	break;
      case 1:
	if (mouseSensitivity < 9)
	    mouseSensitivity++;
	break;
    }
}




void M_ChangeDetail(int choice)
{
    choice = 0;
    //detailLevel = 1 - detailLevel;

    // FIXME - does not work. Remove anyway?
/*
    R_SetViewSize (screenblocks, detailLevel);

    if (!detailLevel)
	players[consoleplayer].message = DETAILHI;
    else
	players[consoleplayer].message = DETAILLO;
*/
}




void M_SizeDisplay(int choice)
{

// This will be converted to scaling the status bar
    switch(choice)
    {
      case 0:
	if (screenSize > 0)
	{
	    screenblocks--;
	    screenSize--;
	}
	break;
      case 1:
	if (screenSize < 8)
	{
	    screenblocks++;
	    screenSize++;
	}
	break;
    }
	

    R_SetViewSize (screenblocks, detailLevel);
}




//
//      Menu Functions
//
void
M_DrawThermo
( int	x,
  int	y,
  int	thermWidth,
  int	thermDot )
{
/*
    int		xx;
    int		i;

    xx = x;
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+xx,((SCREENHEIGHT-200)/2)+y,0,W_CacheLumpName("M_THERML",PU_CACHE));
    xx += 8;
    for (i=0;i<thermWidth;i++)
    {
	V_DrawPatchDirect (((SCREENWIDTH-320)/2)+xx,((SCREENHEIGHT-200)/2)+y,0,W_CacheLumpName("M_THERMM",PU_CACHE));
	xx += 8;
    }
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+xx,((SCREENHEIGHT-200)/2)+y,0,W_CacheLumpName("M_THERMR",PU_CACHE));

    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+(x+8) + thermDot*8,((SCREENHEIGHT-200)/2)+y,0,W_CacheLumpName("M_THERMO",PU_CACHE));
*/
}

//
//      Menu Functions
//
void GL_DrawThermo(int x, int y, int thermWidth, int thermDot )
   {
    float   Left, Right, Top, Bottom;
    float   xx;
    int		i;

    xx = (float)x;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    Left = (-160.0f+xx);
    Right = Left+glThermL.Width;
    Top = 120.0f-(y*1.2f);
    Bottom = Top-(glThermL.Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, glThermL.TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, glThermL.YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(glThermL.XDisp, glThermL.YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(glThermL.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, glThermM.TexName);
    Left += glThermL.Width;
    Right = Left+9.0f;

    for (i = 0;i < thermWidth; i++)
       {
        glBegin(GL_QUADS);
           glTexCoord2f(0.0f, 1.0f);
           glVertex3f(Left, Top, SetBack);

           glTexCoord2f(0.0f, glThermM.YDisp);
           glVertex3f(Left, Bottom, SetBack);

           glTexCoord2f(glThermM.XDisp, glThermM.YDisp);
           glVertex3f(Right, Bottom, SetBack);

           glTexCoord2f(glThermM.XDisp, 1.0f);
           glVertex3f(Right, Top, SetBack);
        glEnd();

        Left += 8.0f;
        Right += 8.0f;
       }

    Right = Left+glThermR.Width;
    glBindTexture(GL_TEXTURE_2D, glThermR.TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);
       glTexCoord2f(0.0f, glThermR.YDisp);
       glVertex3f(Left, Bottom, SetBack);
       glTexCoord2f(glThermR.XDisp, glThermR.YDisp);
       glVertex3f(Right, Bottom, SetBack);
       glTexCoord2f(glThermR.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();

    Left = (-160.0f+(x+glThermL.Width)+(thermDot*8));
    Right = Left+glThermO.Width;
    Bottom = Top-(glThermO.Height*1.2f);

    glBindTexture(GL_TEXTURE_2D, glThermO.TexName);
    glBegin(GL_QUADS);
       glTexCoord2f(0.0f, 1.0f);
       glVertex3f(Left, Top, SetBack);

       glTexCoord2f(0.0f, glThermO.YDisp);
       glVertex3f(Left, Bottom, SetBack);

       glTexCoord2f(glThermO.XDisp, glThermO.YDisp);
       glVertex3f(Right, Bottom, SetBack);

       glTexCoord2f(glThermO.XDisp, 1.0f);
       glVertex3f(Right, Top, SetBack);
    glEnd();
   }



void
M_DrawEmptyCell
( menu_t*	menu,
  int		item )
{
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+menu->x - 10,((SCREENHEIGHT-200)/2)+menu->y+item*LINEHEIGHT - 1, 0,
		       W_CacheLumpName("M_CELL1",PU_CACHE));
}

void
M_DrawSelCell
( menu_t*	menu,
  int		item )
{
    V_DrawPatchDirect (((SCREENWIDTH-320)/2)+menu->x - 10,((SCREENHEIGHT-200)/2)+menu->y+item*LINEHEIGHT - 1, 0,
		       W_CacheLumpName("M_CELL2",PU_CACHE));
}


void
M_StartMessage
( char*		string,
  void*		routine,
  dboolean	input )
{
    messageLastMenuActive = menuactive;
    messageToPrint = 1;
    messageString = string;
    messageRoutine = routine;
    messageNeedsInput = input;
    menuactive = true;
    return;
}



void M_StopMessage(void)
{
    menuactive = messageLastMenuActive;
    messageToPrint = 0;
}



//
// Find string width from hu_font chars
//
int M_StringWidth(char* string)
{
    int             i;
    int             w = 0;
    int             c;
	
    for (i = 0;(unsigned)i < strlen(string);i++)
    {
	c = toupper(string[i]) - HU_FONTSTART;
	if (c < 0 || c >= HU_FONTSIZE)
	    w += 4;
	else
	    w += DSHORT (hu_font[c]->width);
    }
		
    return w;
}



//
//      Find string height from hu_font chars
//
int M_StringHeight(char* string)
{
    int             i;
    int             h;
    int             height = DSHORT(hu_font[0]->height);
	
    h = height;
    for (i = 0;(unsigned)i < strlen(string);i++)
	if (string[i] == '\n')
	    h += height;
		
    return h;
}


//
//      Write a string using the hu_font
//
void M_WriteText( int x, int y, char *string)
   {
    int		w;
    char*	ch;
    int		c;
    int		cx;
    int		cy;
		

    ch = string;
    cx = x;
    cy = y;
	
    while(1)
       {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
           {
            cx = x;
            cy += 12;
            continue;
           }
		
        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c>= HU_FONTSIZE)
           {
            cx += 4;
            continue;
           }
		
        w = DSHORT (hu_font[c]->width);
        if (cx+w > SCREENWIDTH)
            break;
        V_DrawPatchDirect(cx, cy, 0, hu_font[c]);
        cx+=w;
       }
   }


//
//      Write a string using the hu_font
//

void GL_WriteTextN( int x, int y, char *string, int color)
   {
    float   Left, Bottom, Top, Right;
    char*	ch;
    int		c;
    int		cx;
    int		cy;
		
    ch = string;
    cx = x;
    cy = y;
	
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    glColor3f( tc[color].red, tc[color].green, tc[color].blue );
    while(1)
       {
        c = *ch++;
        if (!c)
            break;

        if (c == '\n')
           {
            cx = x;
            cy += (int)(GLFontHeight*1.2f);
            continue;
           }
		
        c = toupper(c) - HU_FONTSTART;
        if (c < 0 || c >= HU_FONTSIZE)
           {
            cx += 4;
            continue;
           }
		
        if (cx+GLGreyFont[c].Width > SCREENWIDTH)
            break;

        Top = (120.0f-(((cy-GLGreyFont[c].TopOff)-3)*1.2f));
        Bottom = Top-(GLGreyFont[c].Height*1.2f);

        Left = (-160.0f+cx);
        Right = Left + GLGreyFont[c].Width;

        glBindTexture(GL_TEXTURE_2D, GLGreyFont[c].TexName);
        glBegin(GL_QUADS);
          glTexCoord2f( 0.0f, 1.0f);
          glVertex3f( Left, Top, SetBack);
          glTexCoord2f( 0.0f, GLGreyFont[c].YDisp);
          glVertex3f( Left, Bottom, SetBack);
          glTexCoord2f( GLGreyFont[c].XDisp, GLGreyFont[c].YDisp);
          glVertex3f( Right, Bottom, SetBack);
          glTexCoord2f( GLGreyFont[c].XDisp, 1.0f);
          glVertex3f( Right, Top, SetBack);
        glEnd();

        cx += (int)(GLGreyFont[c].Width-1);
       }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glColor3f( 1.0f, 1.0f, 1.0f );
   }

//
//      Write a string using the hu_font
//

void GL_WriteText( int x, int y, char *string)
   {
    float   Left, Bottom, Top, Right;
    char*	ch;
    int		c;
    int		cx;
    int		cy;
		
    ch = string;
    cx = x;
    cy = y;
	
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    while(1)
       {
        c = *ch++;
        if (!c)
            break;

        if (c == '\n')
           {
            cx = x;
            cy += (int)(GLFontHeight*1.2f);
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

        cx += (int)GLHudFont[c].Width;
       }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }


extern int hudmode;

/////////////////////////////////////////////////////////////////////////////
//
// SETUP MENU (phares)
//
// We've added a set of Setup Screens from which you can configure a number
// of variables w/o having to restart the game. There are 7 screens:
//
//    Key Bindings
//    Weapons
//    Status Bar / HUD
//    Automap
//    Enemies
//    Messages
//    Chat Strings
//

/////////////////////////////
//
// booleans for setup screens
// these tell you what state the setup screens are in, and whether any of
// the overlay screens (automap colors, reset button message) should be
// displayed

dboolean setup_active      = false; // in one of the setup screens
dboolean set_keybnd_active = false; // in key binding setup screens
dboolean set_weapon_active = false; // in weapons setup screen
dboolean set_status_active = false; // in status bar/hud setup screen
dboolean set_auto_active   = false; // in automap setup screen
dboolean set_enemy_active  = false; // in enemies setup screen
dboolean set_mess_active   = false; // in messages setup screen
dboolean set_chat_active   = false; // in chat string setup screen
dboolean setup_select      = false; // changing an item
dboolean setup_gather      = false; // gathering keys for value
dboolean colorbox_active   = false; // color palette being shown
dboolean default_verify    = false; // verify reset defaults decision

/////////////////////////////
//
// The following #defines are for the m_flags field of each item on every
// Setup Screen. They can be OR'ed together where appropriate

#define S_HILITE     0x1 // Cursor is sitting on this item
#define S_SELECT     0x2 // We're changing this item
#define S_TITLE      0x4 // Title item
#define S_YESNO      0x8 // Yes or No item
#define S_CRITEM    0x10 // Message color
#define S_COLOR     0x20 // Automap color
#define S_CHAT      0x40 // Chat String
#define S_RESET     0x80 // Reset to Defaults Button
#define S_PREV     0x100 // Previous menu exists
#define S_NEXT     0x200 // Next menu exists
#define S_KEY      0x400 // Key Binding
#define S_WEAP     0x800 // Weapon #
#define S_NUM     0x1000 // Numerical item
#define S_SKIP    0x2000 // Cursor can't land here
#define S_KEEP    0x4000 // Don't swap key out
#define S_END     0x8000 // Last item in list (dummy)

// S_SHOWDESC = the set of items whose description should be displayed
// S_SHOWSET  = the set of items whose setting should be displayed

#define S_SHOWDESC (S_TITLE|S_YESNO|S_CRITEM|S_COLOR|S_CHAT|S_RESET|S_PREV|S_NEXT|S_KEY|S_WEAP|S_NUM)
#define S_SHOWSET  (S_YESNO|S_CRITEM|S_COLOR|S_CHAT|S_KEY|S_WEAP|S_NUM)

/////////////////////////////
//
// The setup_group enum is used to show which 'groups' keys fall into so
// that you can bind a key differently in each 'group'.

typedef enum {
  m_null,       // Has no meaning; not applicable
  m_scrn,       // A key can not be assigned to more than one action
  m_map,        // in the same group. A key can be assigned to one
  m_menu,       // action in one group, and another action in another.
  } setup_group;

/////////////////////////////
//
// phares 4/17/98:
// State definition for each item.
// This is the definition of the structure for each setup item. Not all
// fields are used by all items.
//
// A setup screen is defined by an array of these items specific to
// that screen.
//
// Future: don't include the m_low and m_high fields here. Go back to
// the defaults config table in m_misc.c to get those values.

typedef struct
  {
  char*       m_text;   // text to display
  int         m_flags;  // phares 4/17/98: flag bits S_* (defined above)
  setup_group m_group;  // Group
  short       m_x;      // screen x position (left is 0)
  short       m_y;      // screen y position (top is 0)
  int*        m_key;    // key value, or 0 if not shown
  int*        m_mouse;  // mouse button value, or 0 if not shown
  int*        m_joy;    // joystick button value, or 0 if not shown
  int*        m_var1;   // value or default setting
  int*        m_var2;   // possible non-default setting
  int         m_low;    // lowest value for numerical settings
  int         m_high;   // highest value for numerical settings
  } setup_menu_t;

/////////////////////////////
//
// set_menu_itemon is an index that starts at zero, and tells you which
// item on the current screen the cursor is sitting on.
//
// current_setup_menu is a pointer to the current setup menu table.

static int set_menu_itemon; // which setup item is selected?
setup_menu_t* current_setup_menu; // points to current setup menu table

/////////////////////////////
//
// The menu_buffer is used to construct strings for display on the screen.

static char menu_buffer[64];

char *sckeyname[]={ "NULL", // no key
                     "ESC", // <ESC>
                       "1", // 1
                       "2",
                       "3",
                       "4", // reserved for weapons...
                       "5",
                       "6",
                       "7",  // 7
                       "8",  // open
                       "9",  // open
                       "0",  // open
                       "-",  // minus key
                       "+",  // equals key
                    "BACK",  // backspace key
                     "TAB",  // tab key
                       "Q",
                       "W",
                       "E",
                       "R",
                       "T",
                       "Y",
                       "U",
                       "I",
                       "O",
                       "P",
                       "[",
                       "]",
                    "ENTR", // enter key
                    "LCTL", // left control key
                       "A",
                       "S",
                       "D",
                       "F",
                       "G",
                       "H",
                       "J",
                       "K",
                       "L",
                       ";",
                       "'",
                       "`", // tilde - console key
                    "LSHF",
                      "\\",
                       "Z",
                       "X",
                       "C",
                       "V",
                       "B",
                       "N",
                       "M",
                       "<",
                       ">",
                       "/",
                    "RSHF",
                    "PRSC",  // print screen - sysrq
                    "LALT",  
                    "SPAC",
                    "CAPS",
                      "F1",  //  F1
                      "F2",  //  F2
                      "F3",  //  F3
                      "F4",  //  F4
                      "F5",  //  F5
                      "F6",  //  F6
                      "F7",  //  F7
                      "F8",  //  F8
                      "F9",  //  F9
                     "F10",  // F10
                    "NMLK",
                    "SCRL",
                    "PAD7",
                    "PAD8",
                    "PAD9",
                    "PAD-",
                    "PAD4",
                    "PAD5",
                    "PAD6",
                    "PAD+",
                    "PAD1",
                    "PAD2",
                    "PAD3",
                    "PAD0",
                    "PAD.",
                        "",  // no key
                        "",  // no key
                        "",
                     "F11",  // F11
                     "F12",  // F12
                        "",  // no key 0x59 89
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key
                        "",  // no key 0x9B 155
                    "PNTR", // 0x9C
                    "RCTL", // 0x9D
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                    "PAD/",
                        "", 
                    "SYSR", // SYS RQ
                    "RALT",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                        "",
                    "HOME",
                    "UARR",
                    "PGUP",
                        "",
                    "LARR",
                        "",
                    "RARR",
                        "",
                     "END",
                    "DARR",
                    "PGDN",
                    "INST",
                    "DELT",
                       "-" };

////////////////////////////////////////////////////////////////////////////
//
// Dynamic HELP screen                     // phares 3/2/98
//
// Rather than providing the static HELP screens from DOOM and its versions,
// BOOM provides the player with a dynamic HELP screen that displays the
// current settings of major key bindings.
//
// The Dynamic HELP screen is defined in a manner similar to that used for
// the Setup Screens above.
//
// M_GetKeyString finds the correct string to represent the key binding
// for the current item being drawn.

int M_GetKeyString(int c,size_t offset)
  {
  const char* s;
  SDL_Keycode keycode;

  if (c == SDL_SCANCODE_PAUSE)
  {
      strcpy(&menu_buffer[offset], "PAUS"); // string to display
      offset += 4;
  }
  else
    {

    keycode = SDL_GetKeyFromScancode(c);
    s = SDL_GetKeyName(keycode);

    if (s == "")
        s = "JUNK";

    strcpy(&menu_buffer[offset],s); // string to display
    offset += strlen(s);
    }
  return (int)offset;
  }

/////////////////////////////
//
// Data that's used by the Setup screen code
//
// Establish the message colors to be used

#define CR_TITLE  CR_GOLD
#define CR_SET    CR_GREEN
#define CR_ITEM   CR_RED
#define CR_HILITE CR_ORANGE
#define CR_SELECT CR_GRAY

// Data used by the Automap color selection code

#define CHIP_SIZE 7 // size of color block for colored items

#define COLORPALXORIG ((320 - 16*(CHIP_SIZE+1))/2)
#define COLORPALYORIG ((200 - 16*(CHIP_SIZE+1))/2)

#define SPACEWIDTH 4

// M_DrawMenuString() draws the string in menu_buffer[]

void M_DrawMenuString(int cx, int cy, int color)
  {
  int   w;
  int   c;
  char* ch = menu_buffer;

  while (*ch)
    {
    c = *ch++;         // get next char
    c = toupper(c) - HU_FONTSTART;
    if (c < 0 || c> HU_FONTSIZE)
      {
      cx += SPACEWIDTH;    // space
      continue;
      }
    w = DSHORT (hu_font[c]->width);
    if (cx + w > SCREENWIDTH)
      break;
    
    // V_DrawpatchTranslated() will draw the string in the
    // desired color, colrngs[color]

// FIXME    
    //V_DrawPatchTranslated(cx,cy,0,hu_font[c],colrngs[color],0);
    cx += w - 1; // The screen is cramped, so trim one unit from each
                 // character so they butt up against each other.
    }
  }

// M_GetPixelWidth() returns the number of pixels in the width of
// the string, NOT the number of chars in the string.

int M_GetPixelWidth(char* ch)
  {
  int len = 0;
  int c;

  while (*ch)
    {
    c = *ch++;    // pick up next char
    c = toupper(c) - HU_FONTSTART;
    if (c < 0 || c > HU_FONTSIZE)
      {
      len += SPACEWIDTH;   // space
      continue;
      }
    len += DSHORT (hu_font[c]->width);
    len--; // adjust so everything fits
    }
  len++; // replace what you took away on the last char only
  return len;
  }

/*
    setup_active = true;
    setup_screen = ss_keys;
    set_keybnd_active = true;
    setup_select = false;
    default_verify = false;
    mult_screens_index = 0;
    current_setup_menu = keys_settings[0];
    set_menu_itemon = 0;
    while ((current_setup_menu + set_menu_itemon)->m_flags & S_SKIP)
       {
        set_menu_itemon++;
       }
    (current_setup_menu + set_menu_itemon)->m_flags |= S_HILITE;
*/
/////////////////////////////
// bal    10/29/98
// phares  4/18/98:
// GL_DrawInstructions writes the instruction text just below the screen title

void GL_DrawInstructions()
  {
  int flags = (current_setup_menu + set_menu_itemon)->m_flags;
  int x = 0;
  int color;

  // There are different instruction messages depending on whether you
  // are changing an item or just sitting on it.

  if (setup_select)
     {
      color = CR_SELECT; 
      if (flags & S_KEY)
         {
          // See if a joystick or mouse button setting is allowed for
          // this item.
          if ((current_setup_menu + set_menu_itemon)->m_mouse ||
              (current_setup_menu + set_menu_itemon)->m_joy)
             {
              strcpy(menu_buffer,"Press key or button for this action");
              x = 49;
             }
          else
             {
              strcpy(menu_buffer,"Press key for this action");
              x = 84;
             }
         }
      else
      if (flags & S_YESNO)
         {
          strcpy(menu_buffer,"Press ENTER key to toggle");
          x = 78;
         }
      else
      if (flags & S_WEAP)
         {
          strcpy(menu_buffer,"Enter weapon number");
          x = 97;
         }
      else
      if (flags & S_NUM)
         {
          strcpy(menu_buffer,"Enter value. Press ENTER when finished.");
          x = 37;
         }
      else
      if (flags & S_COLOR)
         {
          strcpy(menu_buffer,"Select color and press enter");
          x = 70;
         }
      else
      if (flags & S_CRITEM)
         {
          strcpy(menu_buffer,"Enter value");
          x = 125;
         }
      else
      if (flags & S_CHAT)
         {
          strcpy(menu_buffer,"Type/edit chat string and Press ENTER");
          x = 43;
         }
      else
      if (flags & S_RESET)
         {
          strcpy(menu_buffer,"");
          x = 43;
         }
       x = 160-(int)((strlen(menu_buffer)*gl_fwidth)/2);
       GL_WriteTextN(x, 20, menu_buffer, tc_grey);
      }
   else // when you're changing something
      {
       color = CR_HILITE;
       if (flags & S_RESET)
          {
           strcpy(menu_buffer,"Press ENTER key to reset to defaults");
           x = 43;
          }
       else
          {
           strcpy(menu_buffer,"Press Enter to Change");
           x = 91;
          }
       //x = 160-((strlen(menu_buffer)*gl_fwidth)/2);
       GL_WriteTextN(x,20,menu_buffer,tc_green);
      }
    //M_DrawMenuString(x,20,color);
   }

/////////////////////////////
//
// phares 4/18/98:
// M_DrawInstructions writes the instruction text just below the screen title

void M_DrawInstructions()
  {
  int flags = (current_setup_menu + set_menu_itemon)->m_flags;
  int x = 0;
  int color;

  // There are different instruction messages depending on whether you
  // are changing an item or just sitting on it.

  if (setup_select)
    {
    color = CR_SELECT; 
    if (flags & S_KEY)

      // See if a joystick or mouse button setting is allowed for
      // this item.

      if ((current_setup_menu + set_menu_itemon)->m_mouse ||
        (current_setup_menu + set_menu_itemon)->m_joy)
        {
        strcpy(menu_buffer,"Press key or button for this action");
        x = 49;
        }
      else
        {
        strcpy(menu_buffer,"Press key for this action");
        x = 84;
        }
    else if (flags & S_YESNO)
      {
      strcpy(menu_buffer,"Press ENTER key to toggle");
      x = 78;
      }
    else if (flags & S_WEAP)
      {
      strcpy(menu_buffer,"Enter weapon number");
      x = 97;
      }
    else if (flags & S_NUM)
      {
      strcpy(menu_buffer,"Enter value. Press ENTER when finished.");
      x = 37;
      }
    else if (flags & S_COLOR)
      {
      strcpy(menu_buffer,"Select color and press enter");
      x = 70;
      }
    else if (flags & S_CRITEM)
      {
      strcpy(menu_buffer,"Enter value");
      x = 125;
      }
    else if (flags & S_CHAT)
      {
      strcpy(menu_buffer,"Type/edit chat string and Press ENTER");
      x = 43;
      }
    else if (flags & S_RESET)
      {
      strcpy(menu_buffer,"");
      x = 43;
      }
    }
  else // when you're changing something
    {
    color = CR_HILITE;
    if (flags & S_RESET)
      {
      strcpy(menu_buffer,"Press ENTER key to reset to defaults");
      x = 43;
      }
    else
      {
      strcpy(menu_buffer,"Press Enter to Change");
      x = 91;
      }
    }
  M_DrawMenuString(x,20,color);
  }


/////////////////////////////
//
// The Key Binding Screen tables.

#define KB_X  160
#define KB_PREV  57
#define KB_NEXT 310
#define KB_Y   31

// phares 4/16/98:
// X,Y position of reset button. This is the same for every screen, and is
// only defined once here.

#define X_BUTTON 301
#define Y_BUTTON   3

#define PAL_BLACK   0
#define PAL_WHITE   4

static byte colorblock[(CHIP_SIZE+4)*(CHIP_SIZE+4)];

// Data used by the Chat String editing code

#define CHAT_STRING_BFR_SIZE 128
#define MAXCHATWIDTH         287 // chat strings must fit in this screen space

int   chat_index;
char* chat_string_buffer; // points to new chat strings while editing

/////////////////////////////
//
// Data used to draw the "are you sure?" dialogue box when resetting
// to defaults.

#define VERIFYBOXXORG 66
#define VERIFYBOXYORG 88
#define PAL_GRAY1  91
#define PAL_GRAY2  98
#define PAL_GRAY3 105

// And the routine to draw it.

void M_DrawDefVerify()
  {
  byte block[188];
  int i;

  for (i = 0 ; i < 181 ; i++)
    block[i] = PAL_BLACK;
  for (i = 0 ; i < 17 ; i++)
    V_DrawBlock(VERIFYBOXXORG+3,VERIFYBOXYORG+3+i,0,181,1,block);
  for (i = 0 ; i < 187 ; i++)
    block[i] = PAL_GRAY1;
  V_DrawBlock(VERIFYBOXXORG,VERIFYBOXYORG,0,187,1,block);
  V_DrawBlock(VERIFYBOXXORG,VERIFYBOXYORG,0,1,23,block);
  V_DrawBlock(VERIFYBOXXORG+184,VERIFYBOXYORG+2,0,1,19,block);
  V_DrawBlock(VERIFYBOXXORG+2,VERIFYBOXYORG+20,0,183,1,block);
  for (i = 0 ; i < 185 ; i++)
    block[i] = PAL_GRAY2;
  V_DrawBlock(VERIFYBOXXORG+1,VERIFYBOXYORG+1,0,185,1,block);
  V_DrawBlock(VERIFYBOXXORG+185,VERIFYBOXYORG+1,0,1,21,block);
  V_DrawBlock(VERIFYBOXXORG+1,VERIFYBOXYORG+21,0,185,1,block);
  V_DrawBlock(VERIFYBOXXORG+1,VERIFYBOXYORG+1,0,1,21,block);
  for (i = 0 ; i < 187 ; i++)
    block[i] = PAL_GRAY3;
  V_DrawBlock(VERIFYBOXXORG+2,VERIFYBOXYORG+2,0,183,1,block);
  V_DrawBlock(VERIFYBOXXORG+2,VERIFYBOXYORG+2,0,1,19,block);
  V_DrawBlock(VERIFYBOXXORG+186,VERIFYBOXYORG,0,1,23,block);
  V_DrawBlock(VERIFYBOXXORG,VERIFYBOXYORG+22,0,187,1,block);

  // The blinking messages is keyed off of the blinking of the
  // cursor skull.

  if (whichSkull) // blink the text
    {
    strcpy(menu_buffer,"Reset to defaults? (Y or N)");
    M_DrawMenuString(VERIFYBOXXORG+8,VERIFYBOXYORG+8,CR_RED);
    }
  }

// Definitions of the (in this case) four key binding screens.

setup_menu_t keys_settings1[];       
setup_menu_t keys_settings2[];       
setup_menu_t keys_settings3[];       
setup_menu_t keys_settings4[];       

// The table which gets you from one screen table to the next.

setup_menu_t* keys_settings[] =
  {
  keys_settings1,
  keys_settings2,
  keys_settings3,
  keys_settings4,
  NULL
  };

int mult_screens_index; // the index of the current screen in a set

// Here's an example from this first screen, with explanations.
//
//  {
//  "STRAFE",      // The description of the item ('strafe' key)
//  S_KEY,         // This is a key binding item
//  m_scrn,        // It belongs to the m_scrn group. Its key cannot be
//                 // bound to two items in this group.
//  KB_X,          // The X offset of the start of the right-hand side
//  KB_Y+ 8*8,     // The Y offset of the start of the right-hand side.
//                 // Always given in multiples off a baseline.
//  &key_strafe,   // The variable that holds the key value bound to this
//  &mousebstrafe, // The variable that holds the mouse button bound to
                   // this. If zero, no mouse button can be bound here.
//  &joybstrafe,   // The variable that holds the joystick button bound to
                   // this. If zero, no mouse button can be bound here.
//  0,             // Where a pointer to a numerical setting would be.
//  0,             // where a pointer to another numerical setting would be
//  0,             // upper limit if this was a number that had one
//  0              // lower limit if this was a number that had one
//  }

// The first Key Binding screen table.
// Note that the Y values are ascending. If you need to add something to
// this table, (well, this one's not a good example, because it's full)
// you need to make sure the Y values still make sense so everything gets
// displayed.
//
// Note also that the first screen of each set has a line for the reset
// button. If there is more than one screen in a set, the others don't get
// the reset button.
//
// Note also that this screen has a "NEXT ->" line. This acts like an
// item, in that 'activating' it moves you along to the next screen. If
// there's a "<- PREV" item on a screen, it behaves similarly, moving you
// to the previous screen. If you leave these off, you can't move from
// screen to screen.

// FIXME
//
// This needs to be addressed before anything else is done with this.
//
// Mouse buttons - what to do?
// Define new command input values for mouse and joystick - done.
// Handle these commands as inputs that are bound to system commands.
// When we get the input MOUSE_B1 (mouse button 1) we then send the
// CMD value stored in the mouseb1 variable?
// Bindings?  bind mouseb1 fire
//            bind mouseb2 strafe
//            bind mouseb3 forward
// If text commands are stored for key bindings, then we have to
// have keys as letters or symbolic values so that they can have
// commands or aliases bound to them.
// Commands can be tokenized into the internal commands from the
// ini or cfg file. But keyboard input is currently by scan code.
// How to figure out what the translation table is?
// Can we generate a keyboard equivalency table at runtime?

setup_menu_t keys_settings1[] =  // Key Binding screen strings       

{
  {"MOVEMENT"    ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y ,0                  ,0,0,0,0,0,0},
  {"FORWARD"     ,S_KEY       ,m_scrn,KB_X,KB_Y+ 1*8,&key_up            ,&mousebforward,0,0,0,0,0},
  {"BACKWARD"    ,S_KEY       ,m_scrn,KB_X,KB_Y+ 2*8,&key_down          ,0,0,0,0,0,0},
  {"TURN LEFT"   ,S_KEY       ,m_scrn,KB_X,KB_Y+ 3*8,&key_left          ,0,0,0,0,0,0},
  {"TURN RIGHT"  ,S_KEY       ,m_scrn,KB_X,KB_Y+ 4*8,&key_right         ,0,0,0,0,0,0},
  {"RUN"         ,S_KEY       ,m_scrn,KB_X,KB_Y+ 5*8,&key_speed         ,0,0,0,0,0,0},
  {"STRAFE LEFT" ,S_KEY       ,m_scrn,KB_X,KB_Y+ 6*8,&key_strafeleft    ,0,0,0,0,0,0},
  {"STRAFE RIGHT",S_KEY       ,m_scrn,KB_X,KB_Y+ 7*8,&key_straferight   ,0,0,0,0,0,0},
  {"STRAFE"      ,S_KEY       ,m_scrn,KB_X,KB_Y+ 8*8,&key_strafe        ,&mousebstrafe,0,0,0,0,0},
  {"AUTORUN"     ,S_KEY       ,m_scrn,KB_X,KB_Y+ 9*8,&key_autorun       ,0,0,0,0,0,0},
  {"180 TURN"    ,S_KEY       ,m_scrn,KB_X,KB_Y+10*8,&key_reverse       ,0,0,0,0,0,0},
  {"USE"         ,S_KEY       ,m_scrn,KB_X,KB_Y+11*8,&key_use           ,&mousebforward,0,0,0,0,0},

  {"MENUS"       ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y+12*8,0              ,0,0,0,0,0,0},
  {"NEXT ITEM"   ,S_KEY       ,m_menu,KB_X,KB_Y+13*8,&key_menu_down     ,0,0,0,0,0,0},
  {"PREV ITEM"   ,S_KEY       ,m_menu,KB_X,KB_Y+14*8,&key_menu_up       ,0,0,0,0,0,0},
  {"LEFT"        ,S_KEY       ,m_menu,KB_X,KB_Y+15*8,&key_menu_left     ,0,0,0,0,0,0},
  {"RIGHT"       ,S_KEY       ,m_menu,KB_X,KB_Y+16*8,&key_menu_right    ,0,0,0,0,0,0},
  {"BACKSPACE"   ,S_KEY       ,m_menu,KB_X,KB_Y+17*8,&key_menu_backspace,0,0,0,0,0,0},
  {"SELECT ITEM" ,S_KEY       ,m_menu,KB_X,KB_Y+18*8,&key_menu_enter    ,0,0,0,0,0,0},
  {"EXIT"        ,S_KEY       ,m_menu,KB_X,KB_Y+19*8,&key_menu_escape   ,0,0,0,0,0,0},

  // Button for resetting to defaults

  {0,S_RESET,m_null,X_BUTTON,Y_BUTTON,0,0,0,0,0,0,0},

  {"NEXT ->",(S_SKIP|S_NEXT),m_null,KB_NEXT,KB_Y+20*8,0,0,0,(int *)keys_settings2,0,0,0},

  // Final entry

  {0,(S_SKIP|S_END),m_null,0,0,0,0,0,0,0,0,0}

};

setup_menu_t keys_settings2[] =  // Key Binding screen strings       
{
  {"SCREEN"      ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y   ,0         ,0,0,0,0,0,0},

// phares 4/13/98:
// key_help and key_escape can no longer be rebound. This keeps the
// player from getting themselves in a bind where they can't remember how
// to get to the menus, and can't remember how to get to the help screen
// to give them a clue as to how to get to the menus. :)

// Also, the keys assigned to these functions cannot be bound to other
// functions. Introduce an S_KEEP flag to show that you cannot swap this
// key with other keys in the same 'group'. (m_scrn, etc.)

  {"HELP"        ,(S_SKIP|S_KEEP) ,m_scrn,0   ,0    ,&key_help         ,0,0,0,0,0,0},
  {"MENU"        ,(S_SKIP|S_KEEP) ,m_scrn,0   ,0    ,&key_escape       ,0,0,0,0,0,0},

  {"PAUSE"       ,S_KEY       ,m_scrn,KB_X,KB_Y+ 1*8,&key_pause        ,0,0,0,0,0,0},
  {"AUTOMAP"     ,S_KEY       ,m_scrn,KB_X,KB_Y+ 2*8,&key_map          ,0,0,0,0,0,0},
  {"VOLUME"      ,S_KEY       ,m_scrn,KB_X,KB_Y+ 3*8,&key_soundvolume  ,0,0,0,0,0,0},
  {"MVERT"       ,S_KEY       ,m_scrn,KB_X,KB_Y+ 4*8,&key_mvert        ,0,0,0,0,0,0},
  {"MESSAGES"    ,S_KEY       ,m_scrn,KB_X,KB_Y+ 5*8,&key_messages     ,0,0,0,0,0,0},
  {"GAMMA FIX"   ,S_KEY       ,m_scrn,KB_X,KB_Y+ 6*8,&key_gamma        ,0,0,0,0,0,0},
//  {"SPY"         ,S_KEY       ,m_scrn,KB_X,KB_Y+ 7*8,&key_spy          ,0,0,0,0,0,0},
//  {"LARGER VIEW" ,S_KEY       ,m_scrn,KB_X,KB_Y+ 8*8,&key_zoomin       ,0,0,0,0,0,0}, 
//  {"SMALLER VIEW",S_KEY       ,m_scrn,KB_X,KB_Y+ 9*8,&key_zoomout      ,0,0,0,0,0,0},
  {"MLOOK"       ,S_KEY       ,m_scrn,KB_X,KB_Y+ 7*8,&key_mlook        ,0,0,0,0,0,0},
  {"HUD NEXT"    ,S_KEY       ,m_scrn,KB_X,KB_Y+ 8*8,&key_hudup        ,0,0,0,0,0,0}, 
  {"HUD PREV"    ,S_KEY       ,m_scrn,KB_X,KB_Y+ 9*8,&key_huddn        ,0,0,0,0,0,0},
  {"SCREENSHOT"  ,S_KEY       ,m_scrn,KB_X,KB_Y+10*8,&key_screenshot   ,0,0,0,0,0,0},

  {"GAME"        ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y+12*8,0             ,0,0,0,0,0,0},
  {"SAVE"        ,S_KEY       ,m_scrn,KB_X,KB_Y+13*8,&key_savegame     ,0,0,0,0,0,0},
  {"LOAD"        ,S_KEY       ,m_scrn,KB_X,KB_Y+14*8,&key_loadgame     ,0,0,0,0,0,0},
  {"QUICKSAVE"   ,S_KEY       ,m_scrn,KB_X,KB_Y+15*8,&key_quicksave    ,0,0,0,0,0,0},
  {"QUICKLOAD"   ,S_KEY       ,m_scrn,KB_X,KB_Y+16*8,&key_quickload    ,0,0,0,0,0,0},
  {"END GAME"    ,S_KEY       ,m_scrn,KB_X,KB_Y+17*8,&key_endgame      ,0,0,0,0,0,0},
  {"QUIT"        ,S_KEY       ,m_scrn,KB_X,KB_Y+18*8,&key_quit         ,0,0,0,0,0,0},

  {"<- PREV"     ,(S_SKIP|S_PREV),m_null,KB_PREV,KB_Y+20*8,0,0,0,(int *)keys_settings1,0,0,0},
  {"NEXT ->"     ,(S_SKIP|S_NEXT),m_null,KB_NEXT,KB_Y+20*8,0,0,0,(int *)keys_settings3,0,0,0},

  // Final entry

  {0,(S_SKIP|S_END),m_null,0,0,0,0,0,0,0,0,0}

};

setup_menu_t keys_settings3[] =  // Key Binding screen strings       
{
  {"WEAPONS" ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y   ,0              ,0,0,0,0,0,0},
  {"FIST"    ,S_KEY       ,m_scrn,KB_X,KB_Y+ 1*8,&key_weapon1     ,0,0,0,0,0,0},
  {"PISTOL"  ,S_KEY       ,m_scrn,KB_X,KB_Y+ 2*8,&key_weapon2     ,0,0,0,0,0,0},
  {"SHOTGUN" ,S_KEY       ,m_scrn,KB_X,KB_Y+ 3*8,&key_weapon3     ,0,0,0,0,0,0},
  {"CHAINGUN",S_KEY       ,m_scrn,KB_X,KB_Y+ 4*8,&key_weapon4     ,0,0,0,0,0,0},
  {"ROCKET"  ,S_KEY       ,m_scrn,KB_X,KB_Y+ 5*8,&key_weapon5     ,0,0,0,0,0,0},
  {"PLASMA"  ,S_KEY       ,m_scrn,KB_X,KB_Y+ 6*8,&key_weapon6     ,0,0,0,0,0,0},
  {"BFG 9000",S_KEY       ,m_scrn,KB_X,KB_Y+ 7*8,&key_weapon7     ,0,0,0,0,0,0},
  {"CHAINSAW",S_KEY       ,m_scrn,KB_X,KB_Y+ 8*8,&key_weapon8     ,0,0,0,0,0,0},
  {"SSG"     ,S_KEY       ,m_scrn,KB_X,KB_Y+ 9*8,&key_weapon9     ,0,0,0,0,0,0},
  {"BEST"    ,S_KEY       ,m_scrn,KB_X,KB_Y+10*8,&key_weapontoggle,0,0,0,0,0,0},
  {"FIRE"    ,S_KEY       ,m_scrn,KB_X,KB_Y+11*8,&key_fire        ,&mousebfire,0,0,0,0,0},

  {"<- PREV",(S_SKIP|S_PREV),m_null,KB_PREV,KB_Y+20*8,0,0,0,(int *)keys_settings2,0,0,0},
  {"NEXT ->",(S_SKIP|S_NEXT),m_null,KB_NEXT,KB_Y+20*8,0,0,0,(int *)keys_settings4,0,0,0},

  // Final entry

  {0,(S_SKIP|S_END),m_null,0,0,0,0,0,0,0,0,0}

};

setup_menu_t keys_settings4[] =  // Key Binding screen strings       
{
  {"AUTOMAP"    ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y   ,0                 ,0,0,0,0,0,0},
  {"FOLLOW"     ,S_KEY       ,m_map ,KB_X,KB_Y+ 1*8,&key_map_follow     ,0,0,0,0,0,0},
  {"ZOOM IN"    ,S_KEY       ,m_map ,KB_X,KB_Y+ 2*8,&key_map_zoomin     ,0,0,0,0,0,0},
  {"ZOOM OUT"   ,S_KEY       ,m_map ,KB_X,KB_Y+ 3*8,&key_map_zoomout    ,0,0,0,0,0,0},
  {"SHIFT UP"   ,S_KEY       ,m_map ,KB_X,KB_Y+ 4*8,&key_map_up         ,0,0,0,0,0,0},
  {"SHIFT DOWN" ,S_KEY       ,m_map ,KB_X,KB_Y+ 5*8,&key_map_down       ,0,0,0,0,0,0},
  {"SHIFT LEFT" ,S_KEY       ,m_map ,KB_X,KB_Y+ 6*8,&key_map_left       ,0,0,0,0,0,0},
  {"SHIFT RIGHT",S_KEY       ,m_map ,KB_X,KB_Y+ 7*8,&key_map_right      ,0,0,0,0,0,0},
  {"MARK PLACE" ,S_KEY       ,m_map ,KB_X,KB_Y+ 8*8,&key_map_mark       ,0,0,0,0,0,0},
  {"CLEAR MARKS",S_KEY       ,m_map ,KB_X,KB_Y+ 9*8,&key_map_clear      ,0,0,0,0,0,0},
  {"FULL/ZOOM"  ,S_KEY       ,m_map ,KB_X,KB_Y+10*8,&key_map_gobig      ,0,0,0,0,0,0},
  {"GRID"       ,S_KEY       ,m_map ,KB_X,KB_Y+11*8,&key_map_grid       ,0,0,0,0,0,0},

  {"CHATTING"   ,(S_SKIP|S_TITLE),m_null,KB_X,KB_Y+12*8,0               ,0,0,0,0,0,0},
  {"BEGIN CHAT" ,S_KEY       ,m_scrn,KB_X,KB_Y+13*8,&key_chat           ,0,0,0,0,0,0},
  {"PLAYER 1"   ,S_KEY       ,m_scrn,KB_X,KB_Y+14*8,&destination_keys[0],0,0,0,0,0,0},
  {"PLAYER 2"   ,S_KEY       ,m_scrn,KB_X,KB_Y+15*8,&destination_keys[1],0,0,0,0,0,0},
  {"PLAYER 3"   ,S_KEY       ,m_scrn,KB_X,KB_Y+16*8,&destination_keys[2],0,0,0,0,0,0},
  {"PLAYER 4"   ,S_KEY       ,m_scrn,KB_X,KB_Y+17*8,&destination_keys[3],0,0,0,0,0,0},
  {"BACKSPACE"  ,S_KEY       ,m_scrn,KB_X,KB_Y+18*8,&key_backspace      ,0,0,0,0,0,0},
  {"ENTER"      ,S_KEY       ,m_scrn,KB_X,KB_Y+19*8,&key_enter          ,0,0,0,0,0,0},

  {"<- PREV" ,(S_SKIP|S_PREV),m_null,KB_PREV,KB_Y+20*8,0,0,0,(int *)keys_settings3,0,0,0},

  // Final entry

  {0,(S_SKIP|S_END),m_null,0,0,0,0,0,0,0,0,0}

};

/////////////////////////////
//
// Here are the definitions of the individual Setup Menu screens. They
// follow the format of the 'Big Font' menu structures. See the comments
// for SetupDef (above) to help understand what each of these says.

menu_t KeybndDef =
  {
  generic_setup_end,
  &SetupDef,
  Generic_Setup,
  M_DrawKeybnd,
  34,5,      // skull drawn here
  0
  };

/////////////////////////////
//
// phares 4/17/98:
// Added 'Reset to Defaults' Button to Setup Menu screens
// This is a small button that sits in the upper-right-hand corner of
// the first screen for each group. It blinks when selected, thus the
// two patches, which it toggles back and forth.

char ResetButtonName[2][8] = {"M_BUTT1","M_BUTT2"};

/////////////////////////////
//
// phares 4/18/98:
// Consolidate Item drawing code
//
// M_DrawItem draws the description of the provided item (the left-hand
// part). A different color is used for the text depending on whether the
// item is selected or not, or whether it's about to change.

void GL_DrawItem(setup_menu_t* s)
   {
    int color,flags;
    int x,y;
    //BOOL bright = false;

    x = s->m_x;
    y = s->m_y;
    flags = s->m_flags;
    if (flags & S_RESET)
       {
        // This item is the reset button
        // Draw the 'off' version if this isn't the current menu item
        // Draw the blinking version in tune with the blinking skull otherwise
        if (flags & (S_HILITE|S_SELECT))
           {
            GL_DrawPatch(&glResetBtn[whichSkull], (float)x, (float)y);
            //V_DrawPatchDirect(x,y,0,W_CacheLumpName(ResetButtonName[whichSkull],PU_CACHE));
           }
        else
           {
            GL_DrawPatch(&glResetBtn[0], (float)x, (float)y);
            //V_DrawPatchDirect(x,y,0,W_CacheLumpName(ResetButtonName[0],PU_CACHE));
           }
       }
    else // Draw the item string
       {
        if (flags & S_SELECT)
           {
            color = tc_yellow;
            //color = CR_SELECT;
           }
        else
        if (flags & S_HILITE)
           {
            color = tc_white;
            //color = CR_HILITE;
            //bright = true;
           }
        else
        if (flags & (S_TITLE|S_NEXT|S_PREV))
           {
            color = tc_green;
            //color = CR_TITLE;
           }
        else
           {
            color = CR_ITEM;
            color = tc_red;
           }
        GL_WriteTextN(x-M_GetPixelWidth(s->m_text)-4, y, s->m_text, color);
        //strcpy(menu_buffer,s->m_text);
        //M_DrawMenuString(x-M_GetPixelWidth(menu_buffer)-4,y,color);
       }
   }

/////////////////////////////
//
// phares 4/18/98:
// Consolidate Item drawing code
//
// M_DrawItem draws the description of the provided item (the left-hand
// part). A different color is used for the text depending on whether the
// item is selected or not, or whether it's about to change.

void M_DrawItem(setup_menu_t* s)
  {
  int color,flags;
  int x,y;

  x = s->m_x;
  y = s->m_y;
  flags = s->m_flags;
  if (flags & S_RESET)

    // This item is the reset button
    // Draw the 'off' version if this isn't the current menu item
    // Draw the blinking version in tune with the blinking skull otherwise

    if (flags & (S_HILITE|S_SELECT))
      V_DrawPatchDirect(x,y,0,W_CacheLumpName(ResetButtonName[whichSkull],PU_CACHE));
    else
      V_DrawPatchDirect(x,y,0,W_CacheLumpName(ResetButtonName[0],PU_CACHE));

  // Draw the item string

  else
    {
    if (flags & S_SELECT)
      color = CR_SELECT;
    else if (flags & S_HILITE)
      color = CR_HILITE;
    else if (flags & (S_TITLE|S_NEXT|S_PREV))
      color = CR_TITLE;
    else
      color = CR_ITEM;
    strcpy(menu_buffer,s->m_text);
    M_DrawMenuString(x-M_GetPixelWidth(menu_buffer)-4,y,color);
    }
  }

/////////////////////////////
//
// phares 4/18/98:
// Consolidate Item Setting drawing code
//
// M_DrawSetting draws the setting of the provided item (the right-hand
// part. It determines the text color based on whether the item is
// selected or being changed. Then, depending on the type of item, it
// displays the appropriate setting value: yes/no, a key binding, a number,
// a paint chip, etc.

void GL_DrawSetting(setup_menu_t* s)
   {
    int*  key;
    int   flags,x,y,color;
    //dboolean  bright = false;
   
    x = s->m_x;
    y = s->m_y;
    flags = s->m_flags;
   
    // Determine color of the text. This may or may not be used later,
    // depending on whether the item is a text string or not.
   
    if (flags & S_SELECT)
       {
        color = tc_white;
        //color = CR_SELECT;
       }
    else
    if (flags & S_HILITE)
       {
        color = tc_white;
        //color = CR_HILITE;
        //bright = true;
       }
    else
       {
        color = tc_grey;
        //color = CR_SET;
       }
    // Is the item a YES/NO item?
    if (flags & S_YESNO)
       {
        sprintf(menu_buffer,"%s",*(s->m_var1) ? "YES" : "NO");
        GL_WriteTextN(x, y, menu_buffer, color);
        //M_DrawMenuString(x,y,color);
       }
    // Is the item a simple number?
    else
    if (flags & S_NUM)
       {
        sprintf(menu_buffer,"%d",*(s->m_var1));
        GL_WriteTextN(x, y, menu_buffer, color);
        //M_DrawMenuString(x,y,color);
       }
    // Is the item a key binding?
    else
    if (flags & S_KEY) // Key Binding
       {
        // Draw the key bound to the action
        key = s->m_key;
        if (key)
           {
            M_GetKeyString(*key,0); // string to display
            if (key == &key_use)
               {
                // For the 'use' key, you have to build the string
                if (s->m_mouse)
                   {
                    if (*s->m_mouse != -1)
                       {
                        sprintf(&menu_buffer[strlen(menu_buffer)],"/DBL-CLK MB%d",*(s->m_mouse)+1);
                       }
                   }
                if (s->m_joy)
                    sprintf(&menu_buffer[strlen(menu_buffer)],"/JSB%d",*(s->m_joy)+1);
               }
            else
/*
            if ((key == &key_up)   || (key == &key_speed) || (key == &key_quicksave) ||
                (key == &key_fire) || (key == &key_strafe) || (key == &key_map) || (key == &key_pause)) ||
                (key == &key_endgame) || (key == &key_quickload) || (key == &key_quit))
*/
               {
                if (s->m_mouse)
                   {
                    if (*s->m_mouse != -1)
                       {
                        sprintf(&menu_buffer[strlen(menu_buffer)],"/MB%d",*(s->m_mouse)+1);
                       }
                   }
                if (s->m_joy)
                    sprintf(&menu_buffer[strlen(menu_buffer)],"/JSB%d",*(s->m_joy)+1);
               }
            GL_WriteTextN(x, y, menu_buffer, color);
            //M_DrawMenuString(x,y,color);
           }
       }
   }

/////////////////////////////
//
// phares 4/18/98:
// Consolidate Item Setting drawing code
//
// M_DrawSetting draws the setting of the provided item (the right-hand
// part. It determines the text color based on whether the item is
// selected or being changed. Then, depending on the type of item, it
// displays the appropriate setting value: yes/no, a key binding, a number,
// a paint chip, etc.

void M_DrawSetting(setup_menu_t* s)
   {
   int*  key;
   int   weapon,flags,i,cursor_start,char_width,x,y,color;
   size_t len;
   byte  ch,*ptr;
   char* text;
   char  c[2];
   
   x = s->m_x;
   y = s->m_y;
   flags = s->m_flags;
   
   // Determine color of the text. This may or may not be used later,
   // depending on whether the item is a text string or not.
   
   if (flags & S_SELECT)
      color = CR_SELECT;
   else if (flags & S_HILITE)
      color = CR_HILITE;
   else
      color = CR_SET;
   
   // Is the item a YES/NO item?
   
   if (flags & S_YESNO)
      {
      sprintf(menu_buffer,"%s",*(s->m_var1) ? "YES" : "NO");
      M_DrawMenuString(x,y,color);
      }
   
   // Is the item a simple number?
   
   else if (flags & S_NUM)
      {
      sprintf(menu_buffer,"%d",*(s->m_var1));
      M_DrawMenuString(x,y,color);
      }
   
   // Is the item a key binding?
   
   else if (flags & S_KEY) // Key Binding
      {
      
      // Draw the key bound to the action
      
      key = s->m_key;
      if (key)
         {
         M_GetKeyString(*key,0); // string to display
         if (key == &key_use)
            {
            // For the 'use' key, you have to build the string
            if (s->m_mouse)
               {
               if (*s->m_mouse != -1)
                  {
                  sprintf(&menu_buffer[strlen(menu_buffer)],"/DBL-CLK MB%d",*(s->m_mouse)+1);
                  }
               }
            if (s->m_joy)
               sprintf(&menu_buffer[strlen(menu_buffer)],"/JSB%d",*(s->m_joy)+1);
            }
         else if ((key == &key_up)   || (key == &key_speed) ||
            (key == &key_fire) || (key == &key_strafe))
            {
            if (s->m_mouse)
               {
               if (*s->m_mouse != -1)
                  {
                  sprintf(&menu_buffer[strlen(menu_buffer)],"/MB%d",*(s->m_mouse)+1);
                  }
               }
            if (s->m_joy)
               sprintf(&menu_buffer[strlen(menu_buffer)],"/JSB%d",*(s->m_joy)+1);
            }
         M_DrawMenuString(x,y,color);
         }
      }
   
   // Is the item a weapon number?
   // If so, special processing is needed so as not to display weapons that
   // didn't exist in earlier versions of the game.
   
   else if (flags & S_WEAP) // weapon number
      {
      weapon = *(s->m_var1) - 1;
      if (((weapon == wp_plasma) || (weapon == wp_bfg)) &&
         (gamemode == shareware))
         return;
      if ((weapon == wp_supershotgun) && (gamemode != commercial))
         return;
      sprintf(menu_buffer,"%d",weapon + 1);
      M_DrawMenuString(x,y,color);
      }
   
   // Is the item a paint chip?
   
   else if (flags & S_COLOR) // Automap paint chip
      {
      
      // draw the border of the paint chip
      
      ptr = colorblock;
      for (i = 0 ; i < (CHIP_SIZE+2)*(CHIP_SIZE+2) ; i++)
         *ptr++ = PAL_BLACK;
      V_DrawBlock(x,y-1,0,CHIP_SIZE+2,CHIP_SIZE+2,colorblock);
      
      // draw the paint chip
      
      ch = (byte) *(s->m_var1);
      if (ch == 0) // don't show this item in automap mode
         V_DrawPatchDirect (x+1,y,0,W_CacheLumpName("M_PALNO",PU_CACHE));
      else
         {
         ptr = colorblock;
         for (i = 0 ; i < CHIP_SIZE*CHIP_SIZE ; i++)
            *ptr++ = ch;
         V_DrawBlock(x+1,y,0,CHIP_SIZE,CHIP_SIZE,colorblock);
         }
      }
   
   // Is the item a colored text string from the Automap?
   
   else if (flags & S_CRITEM)
      {
      sprintf(menu_buffer,"%d",*(s->m_var1));
      M_DrawMenuString(x,y,*(s->m_var1));
      }
   
   // Is the item a chat string?
   
   else if (flags & S_CHAT)
      {
      text = (char *)(*(s->m_var1));
      
      // Are we editing this string? If so, display a cursor under
      // the correct character.
      
      if (setup_select && (s->m_flags & (S_HILITE|S_SELECT)))
         {
         
         // If the string is too wide for the screen, trim it back,
         // one char at a time until it fits. This should only occur
         // while you're editing the string.
         
         while (M_GetPixelWidth(text) >= MAXCHATWIDTH)
            {
            len = strlen(text); 
            *(text+len-1) = 0;
            len--;
            if (chat_index > (int)len)
               chat_index--;
            }
         
         // Find the distance from the beginning of the string to
         // where the cursor should be drawn, plus the width of
         // the char the cursor is under..
         
         c[0] = *(text + chat_index); // hold temporarily
         c[1] = 0;
         char_width = M_GetPixelWidth(c);
         if (char_width == 1)
            char_width = 7; // default for end of line
         *(text + chat_index) = 0; // NULL to get cursor position
         cursor_start = M_GetPixelWidth(text);
         *(text + chat_index) = c[0]; // replace stored char
         
         // Now draw the cursor
         
         for (i = 0 ; i < char_width ; i++)
            colorblock[i] = PAL_BLACK;
         V_DrawBlock(x+cursor_start-1,y+7,0,char_width,1,colorblock);
         }
      
      // Draw the setting for the item
      
      strcpy(menu_buffer,text);
      M_DrawMenuString(x,y,color);
      }
  }
  
/////////////////////////////
//
// M_DrawScreenItems takes the data for each menu item and gives it to
// the drawing routines above.

void GL_DrawScreenItems(setup_menu_t* src)
   {
    while (!(src->m_flags & S_END))
       {
        // See if we're to draw the item description (left-hand part)
        if (src->m_flags & S_SHOWDESC)
           {
            GL_DrawItem(src);
           }
        // See if we're to draw the setting (right-hand part)
        if (src->m_flags & S_SHOWSET)
           {
            GL_DrawSetting(src);
           }
        src++;
       }
   }

/////////////////////////////
//
// M_DrawScreenItems takes the data for each menu item and gives it to
// the drawing routines above.

void M_DrawScreenItems(setup_menu_t* src)
  {
  while (!(src->m_flags & S_END))
    {

    // See if we're to draw the item description (left-hand part)

    if (src->m_flags & S_SHOWDESC)
      M_DrawItem(src);

    // See if we're to draw the setting (right-hand part)

    if (src->m_flags & S_SHOWSET)
      M_DrawSetting(src);
    src++;
    }
   }

// Setting up for the Key Binding screen. Turn on flags, set pointers,
// locate the first item on the screen where the cursor is allowed to
// land.

void M_KeyBindings(int choice)
   {
    M_SetupNextMenu(&KeybndDef);

    setup_active = true;
    setup_screen = ss_keys;
    set_keybnd_active = true;
    setup_select = false;
    default_verify = false;
    mult_screens_index = 0;
    current_setup_menu = keys_settings[0];
    set_menu_itemon = 0;
    while ((current_setup_menu + set_menu_itemon)->m_flags & S_SKIP)
       {
        set_menu_itemon++;
       }
    (current_setup_menu + set_menu_itemon)->m_flags |= S_HILITE;
   }

// The drawing part of the Key Bindings Setup initialization. Draw the
// background, title, instruction line, and items.

void GL_DrawKeybnd(void)
  {
  float   Left, Right, Top, Bottom;

  inhelpscreens = true;    // killough 4/6/98: Force status bar redraw

  // Set up the Key Binding screen 

  GL_DrawBackground("FLOOR4_6"); // Draw background

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.0f);
  glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

  Left = (0.0f-(glSetupMenu[0].Width/2.0f));
  Right = Left+glSetupMenu[0].Width;
  Top = 120.0f-(1*1.2f);
  Bottom = Top-(glSetupMenu[0].Height*1.2f);

  glBindTexture(GL_TEXTURE_2D, glSetupMenu[0].TexName);
  glBegin(GL_QUADS);
     glTexCoord2f(0.0f, 1.0f);
     glVertex3f(Left, Top, SetBack);

     glTexCoord2f(0.0f, glSetupMenu[0].YDisp);
     glVertex3f(Left, Bottom, SetBack);

     glTexCoord2f(glSetupMenu[0].XDisp, glSetupMenu[0].YDisp);
     glVertex3f(Right, Bottom, SetBack);

     glTexCoord2f(glSetupMenu[0].XDisp, 1.0f);
     glVertex3f(Right, Top, SetBack);
  glEnd();

  GL_DrawInstructions();
  GL_DrawScreenItems(current_setup_menu);

  // If the Reset Button has been selected, an "Are you sure?" message
  // is overlayed across everything else.

/*  FIXME
  if (default_verify)
    M_DrawDefVerify();
*/
  }

// The drawing part of the Key Bindings Setup initialization. Draw the
// background, title, instruction line, and items.

void M_DrawKeybnd(void)
  {
  inhelpscreens = true;    // killough 4/6/98: Force status bar redraw

  // Set up the Key Binding screen 

  M_DrawBackground("FLOOR4_6"); // Draw background
  V_DrawPatchDirect (84,2,0,W_CacheLumpName("M_KEYBND",PU_CACHE));
  M_DrawInstructions();
  M_DrawScreenItems(current_setup_menu);

  // If the Reset Button has been selected, an "Are you sure?" message
  // is overlayed across everything else.

  if (default_verify)
    M_DrawDefVerify();
  }

/////////////////////////////
//
// The Weapon Screen tables.

#define WP_X 203
#define WP_Y  31

// There's only one weapon settings screen (for now). But since we're
// trying to fit a common description for screens, it gets a setup_menu_t,
// which only has one screen definition in it.
//
// Note that this screen has no PREV or NEXT items, since there are no
// neighboring screens.

setup_menu_t weap_settings1[];

setup_menu_t* weap_settings[] =
  {
  weap_settings1,
  NULL
  };

setup_menu_t weap_settings1[] =  // Weapons Settings screen       
{
  {"ENABLE RECOIL"    ,S_YESNO,m_null,WP_X,WP_Y+ 1*8,0,0,0,&default_weapon_recoil ,&weapon_recoil,0,0},
  {"ENABLE BOBBING"   ,S_YESNO,m_null,WP_X,WP_Y+ 2*8,0,0,0,&default_player_bobbing,&player_bobbing,0,0},

  {"1ST CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 4*8,0,0,0,&weapon_preferences[0][0],0,0,0},
  {"2nd CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 5*8,0,0,0,&weapon_preferences[0][1],0,0,0},
  {"3rd CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 6*8,0,0,0,&weapon_preferences[0][2],0,0,0},
  {"4th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 7*8,0,0,0,&weapon_preferences[0][3],0,0,0},
  {"5th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 8*8,0,0,0,&weapon_preferences[0][4],0,0,0},
  {"6th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+ 9*8,0,0,0,&weapon_preferences[0][5],0,0,0},
  {"7th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+10*8,0,0,0,&weapon_preferences[0][6],0,0,0},
  {"8th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+11*8,0,0,0,&weapon_preferences[0][7],0,0,0},
  {"9th CHOICE WEAPON",S_WEAP ,m_null,WP_X,WP_Y+12*8,0,0,0,&weapon_preferences[0][8],0,0,0},

  // Button for resetting to defaults

  {0,S_RESET,m_null,X_BUTTON,Y_BUTTON,0,0,0,0,0,0,0},

  // Final entry

  {0,(S_SKIP|S_END),m_null,0,0,0,0,0,0,0,0,0}

};

// Data used by the color palette that is displayed for the player to
// select colors.

int color_palette_x; // X position of the cursor on the color palette
int color_palette_y; // Y position of the cursor on the color palette
byte palette_background[16*(CHIP_SIZE+1)+8];

static dboolean shiftdown = false; // phares 4/10/98: SHIFT key down or not

// phares 4/17/98:
// M_SelectDone() gets called when you have finished entering your
// Setup Menu item change.

void M_SelectDone(setup_menu_t* ptr)
  {
  ptr->m_flags &= ~S_SELECT;
  ptr->m_flags |= S_HILITE;
  S_StartSound(NULL,sfx_itemup);
  setup_select = false;
  colorbox_active = false;
  }

/*
// phares 4/21/98:
// Array of setup screens used by M_ResetDefaults()

setup_menu_t** setup_screens[] =
  {
  0,
  keys_settings,
  weap_settings,
  stat_settings,
  auto_settings,
  enem_settings,
  mess_settings,
  chat_settings
  };

// phares 4/19/98:
// M_ResetDefaults() resets all values for a setup screen to default values

void M_ResetDefaults()
  {
  int i;
  default_t* d;
  setup_menu_t** ptr1;
  setup_menu_t*  ptr2;

  // Look through the defaults table and reset every variable that
  // belongs to the group we're interested in.

  d = defaults;
  for (i = 0 ; i < numdefaults ; i++, d++)
    if (d->setupscreen == setup_screen)
      if (d->isstr)
        {
        free((char *)(*(d->location)));
        *(d->location) = (int)strdup((char *)(d->defaultvalue));
        }
      else
        *(d->location) = d->defaultvalue;

  // Look through the set of screens for any items that have default
  // fields.

  if (!demoplayback && !demorecording && !netgame)
    {
    ptr1 = setup_screens[setup_screen];
    for (i = 0 ; ptr1[i] ; i++)
      for (ptr2 = ptr1[i] ; !(ptr2->m_flags & S_END) ; ptr2++)
        if (ptr2->m_var2)
          *(ptr2->m_var2) = *(ptr2->m_var1);
    }
  }
*/

//
// CONTROL PANEL
//

#define MAXGATHER 3
int  gather_count;

//
// M_Responder
//
dboolean M_Responder (event_t* ev)
   {
    int             ch;
    int             i, ishift;
    static  int     joywait = 0;
    static  int     mousewait = 0;
    static  int     mousey = 0;
    static  int     lasty = 0;
    static  int     mousex = 0;
    static  int     lastx = 0;
	
    ch = -1;
	
    if (ev->type == ev_joystick && joywait < I_GetTime())
       {
        if (ev->data3 == -1)
           {
            ch = key_menu_up;
            joywait = I_GetTime() + 5;
           }
        else
        if (ev->data3 == 1)
           {
            ch = key_menu_down;
            joywait = I_GetTime() + 5;
           }
        if (ev->data2 == -1)
           {
            ch = key_menu_left;
            joywait = I_GetTime() + 2;
           }
        else
        if (ev->data2 == 1)
	       {
            ch = key_menu_right;
            joywait = I_GetTime() + 2;
           }
		
        if (ev->data1&1)
           {
            ch = key_menu_enter;
            joywait = I_GetTime() + 5;
           }
        if (ev->data1&2)
           {
            ch = key_backspace;
            joywait = I_GetTime() + 5;
           }
       }
    else
       {
        if (ev->type == ev_mouse && mousewait < I_GetTime())
           {
            mousey += ev->data3;
            if (mousey < lasty-30)
               {
                ch = key_menu_down;
                mousewait = I_GetTime() + 5;
                mousey = lasty -= 30;
               }
            else
            if (mousey > lasty+30)
               {
                ch = key_menu_up;
                mousewait = I_GetTime() + 5;
                mousey = lasty += 30;
               }
		
            mousex += ev->data2;
            if (mousex < lastx-30)
               {
                ch = key_menu_left;
                mousewait = I_GetTime() + 5;
                mousex = lastx -= 30;
               }
            else
            if (mousex > lastx+30)
               {
                ch = key_menu_right;
                mousewait = I_GetTime() + 5;
                mousex = lastx += 30;
               }
		
            if (ev->data1&1)
               {
                ch = key_menu_enter;
                mousewait = I_GetTime() + 15;
               }
			
            if (ev->data1&2)
               {
                ch = key_backspace;
                mousewait = I_GetTime() + 15;
               }

            if (ev->data1&4)
               {
                ch = key_menu_up;
                mousewait = I_GetTime() + 15;
               }
           }
        else
        if (ev->type == ev_keydown)
           {
	        ch = ev->data1;
           }
       }
    
    if (ch == -1)
        return false;

    
    // Save Game string input
    if (saveStringEnter)
       {
        if (ch == key_backspace)
           {
            if (saveCharIndex > 0)
               {
                saveCharIndex--;
                savegamestrings[saveSlot][saveCharIndex] = 0;
               }
           }
        else
        if (ch == key_menu_escape)
           {
            saveStringEnter = 0;
            strcpy(&savegamestrings[saveSlot][0],saveOldString);
           }
        else
        if (ch == key_menu_enter)
           {
            saveStringEnter = 0;
            if (savegamestrings[saveSlot][0])
               {
                M_DoSave(saveSlot);
               }
           }
        else
           {
            ch = toupper(scan2char[ch]);
            if ((ch == 32) || (ch-HU_FONTSTART >= 0 && ch-HU_FONTSTART < HU_FONTSIZE))
               {
                if (ch >= 32 && ch <= 127 && saveCharIndex < SAVESTRINGSIZE-1 &&
                    M_StringWidth(savegamestrings[saveSlot]) < (SAVESTRINGSIZE-2)*8)
                   {
                    savegamestrings[saveSlot][saveCharIndex++] = ch;
                    savegamestrings[saveSlot][saveCharIndex] = 0;
                   }
               }
           }
	    return true;
       }
    
    // Take care of any messages that need input
    if (messageToPrint)
       {
        if (messageNeedsInput == true &&
	        !(ch == SDL_SCANCODE_SPACE || ch == SDL_SCANCODE_N || ch == SDL_SCANCODE_Y || ch == SDL_SCANCODE_ESCAPE))
           {
            return false;
           }
		
        menuactive = messageLastMenuActive;
        messageToPrint = 0;
        if (messageRoutine)
           {
            messageRoutine(ch);
           }
			
        menuactive = false;
        S_StartSound(NULL,sfx_swtchx);
        return true;
       }
	
/*
    //Don't really need this anymore -- keeping it for legacy...
    if (devparm && ch == KEY_F1)
    {
	G_ScreenShot ();
	return true;
    }
*/

    if ((ch == key_screenshot) && (!setup_select))
       {
        G_ScreenShot();
        return true;
       }

    if ((ch == key_autorun) && (!setup_select))
       {
        autorun = !autorun;
        return true;
       }
    
    // F-Keys
    if (!menuactive)
       {
        if (ch == key_huddn)
           {
	        if (automapactive || chat_on)
		        return false;
	        //M_SizeDisplay(0);
            hudmode--;
            if (hudmode < 0)
               {
                hudmode += 4;
               }
	        S_StartSound(NULL,sfx_stnmov);
	        return true;
           }

        if (ch == key_hudup)
           {
            if (automapactive || chat_on)
                return false;
            //M_SizeDisplay(1);
            hudmode++;
            hudmode %= 4;
            S_StartSound(NULL,sfx_stnmov);
            return true;
           }

        if (ch == key_f1)
           {
            M_StartControlPanel();
            if ( gamemode == retail )
                currentMenu = &ReadDef2;
            else
                currentMenu = &ReadDef1;
            itemOn = 0;
            S_StartSound(NULL,sfx_swtchn);
            return true;
           }

        if (ch == key_f2)
           {
            M_StartControlPanel();
            S_StartSound(NULL,sfx_swtchn);
            M_SaveGame(0);
            return true;
           }

        if (ch == key_f3)
           {
            M_StartControlPanel();
            S_StartSound(NULL,sfx_swtchn);
            M_LoadGame(0);
            return true;
           }

        if (ch == key_f4)
           {
            M_StartControlPanel ();
            currentMenu = &SoundDef;
            itemOn = sfx_vol;
            S_StartSound(NULL,sfx_swtchn);
            return true;
           }

        if (ch == key_f5)
           {
            M_ChangeDetail(0);
            S_StartSound(NULL,sfx_swtchn);
            return true;
           }

        if (ch == key_f6)
           {
            S_StartSound(NULL,sfx_swtchn);
            M_QuickSave();
            return true;
           }

        if (ch == key_f7)
           {
            S_StartSound(NULL,sfx_swtchn);
            M_EndGame(0);
            return true;
           }

        if (ch == key_f8)
           {
            M_ChangeMessages(0);
            S_StartSound(NULL,sfx_swtchn);
            return true;
           }

        if (ch == key_f9)
           {
            S_StartSound(NULL,sfx_swtchn);
            M_QuickLoad();
            return true;
           }

        if (ch == key_f10)
           {
            S_StartSound(NULL,sfx_swtchn);
            M_QuitDOOM(0);
            return true;
           }

        if (ch == key_f10)
           {
            usegamma++;
            if (usegamma > 4)
	            usegamma = 0;
            players[consoleplayer].message = gammamsg[usegamma];
            I_SetPalette (W_CacheLumpName ("PLAYPAL",PU_CACHE));
            return true;
           }
       }

    
    // Pop-up menu?
    if (!menuactive)
       {
        if (ch == key_escape)
           {
            M_StartControlPanel();
            S_StartSound(NULL,sfx_swtchn);
            return true;
           }
        return false;
       }

    

  if (setup_active)
    {
    setup_menu_t* ptr1= current_setup_menu + set_menu_itemon;
    setup_menu_t* ptr2 = NULL;

    // phares 4/19/98:
    // Catch the response to the 'reset to default?' verification
    // screen

    if (default_verify)
      {
      if (toupper(ch) == 'Y')
        {
         //M_ResetDefaults();
         default_verify = false;
         M_SelectDone(ptr1);
        }
      else if (toupper(ch) == 'N')
        {
         default_verify = false;
         M_SelectDone(ptr1);
        }
      default_verify = false;
      M_SelectDone(ptr1);
      return true;
      }

    // Common processing for some items

    if (setup_select) // changing an entry
      {
      if (ch == key_menu_escape) // Exit key = no change
        {
        M_SelectDone(ptr1);                                 // phares 4/17/98
        return true;
        }

      if (ptr1->m_flags & S_YESNO) // yes or no setting?
        {
        if (ch == key_menu_enter)
          {
          if (*(ptr1->m_var1))
            *(ptr1->m_var1) = 0; // turn off
          else
            *(ptr1->m_var1) = 1; // turn on

          // phares 4/14/98:
          // If not in demoplayback, demorecording, or netgame,
          // and there's a second variable in var2, set that
          // as well

          if (!demoplayback && !demorecording &&
            !netgame && ptr1->m_var2)
            *(ptr1->m_var2) = *(ptr1->m_var1);
          }
        M_SelectDone(ptr1);                                 // phares 4/17/98
        return true;
        }

      if (ptr1->m_flags & S_CRITEM)
        {
        if (ch != key_menu_enter)
          {
          ch -= 0x30; // out of ascii
          if (ch < 0 || ch > 9)
            return true; // ignore
          *(ptr1->m_var1) = ch;
          }
        M_SelectDone(ptr1);                                 // phares 4/17/98
        return true;
        }

      if (ptr1->m_flags & S_NUM) // number?
        {
        if (setup_gather) // gathering keys for a value?
          {
          int value;
    
          if (ch == key_menu_enter)
            {
            M_SelectDone(ptr1);     // phares 4/17/98
            setup_gather = false; // finished gathering keys
            return true;
            }
          if (ch < 0x30 || ch > 0x39)
            return true; // ignore
          if (gather_count == MAXGATHER)
            return true; // ignore
          value = 10*(*(ptr1->m_var1)) + ch - 0x30;
          if (value < ptr1->m_low || value > ptr1->m_high)
            return true; // ignore
          gather_count++;
          *(ptr1->m_var1) = value;
          }
        return true;
        }
      }

    // Key Bindings

    if (set_keybnd_active) // on a key binding setup screen
      if (setup_select)    // incoming key or button gets bound
        {
        if (ev->type == ev_joystick)
          {
          int oldbutton,group;
          dboolean search = true;
      
          if (ptr1->m_joy == NULL)
            return true; // not a legal action here (yet)
      
          // see if the button is already bound elsewhere. if so, you
          // have to swap bindings so the action where it's currently
          // bound doesn't go dead. Since there is more than one
          // keybinding screen, you have to search all of them for
          // any duplicates. You're only interested in the items
          // that belong to the same group as the one you're changing.
      
          oldbutton = *(ptr1->m_joy);
          group  = ptr1->m_group;
          for (i = 0, ishift = 1; i < 32; i++)
             {
              if (ev->data1 & (ishift << i))
                 {
                  ch = i;
                  break;
                 }
             }
          if (i == 32)
             {
              return true;
             }
/*
          if (ev->data1 & 1)
            ch = 0;
          else if (ev->data1 & 2)
            ch = 1;
          else if (ev->data1 & 4)
            ch = 2;
          else if (ev->data1 & 8)
            ch = 3;
          else
              return true;
*/
          for (i = 0 ; keys_settings[i] && search ; i++)
            for (ptr2 = keys_settings[i] ; !(ptr2->m_flags & S_END) ; ptr2++)
              if ((ptr2->m_group == group) && (ptr1 != ptr2))
                if ((ptr2->m_flags & S_KEY) && (ptr2->m_joy))
                  if (*(ptr2->m_joy) == ch)
                    {
                    *(ptr2->m_joy) = oldbutton;
                    search = false;
                    break;
                    }
          *(ptr1->m_joy) = ch;
          }
        else if (ev->type == ev_mouse)
          {
          int oldbutton,group;
          dboolean search = true;

          if (ptr1->m_mouse == NULL)
             {
              return true; // not a legal action here (yet)
             }
          // see if the button is already bound elsewhere. if so, you
          // have to swap bindings so the action where it's currently
          // bound doesn't go dead. Since there is more than one
          // keybinding screen, you have to search all of them for
          // any duplicates. You're only interested in the items
          // that belong to the same group as the one you're changing.

          oldbutton = *(ptr1->m_mouse);
          group  = ptr1->m_group;
          if (ev->data1 & 1)
            ch = 0;
          else if (ev->data1 & 2)
            ch = 1;
          else if (ev->data1 & 4)
            ch = 2;
          else
            return true;
          for (i = 0 ; keys_settings[i] && search ; i++)
            for (ptr2 = keys_settings[i] ; !(ptr2->m_flags & S_END) ; ptr2++)
              if ((ptr2->m_group == group) && (ptr1 != ptr2))
                if ((ptr2->m_flags & S_KEY) && (ptr2->m_mouse))
                  if ((*(ptr2->m_mouse) == ch) && (ptr2->m_key != &key_use))
                    {
                     *(ptr2->m_mouse) = oldbutton;
                     search = false;
                     break;
                    }
          *(ptr1->m_mouse) = ch;
          }
        else  // keyboard key
          {
          int oldkey,group;
          dboolean search = true;
        
          // see if 'ch' is already bound elsewhere. if so, you have
          // to swap bindings so the action where it's currently
          // bound doesn't go dead. Since there is more than one
          // keybinding screen, you have to search all of them for
          // any duplicates. You're only interested in the items
          // that belong to the same group as the one you're changing.

          // if you find that you're trying to swap with an action
          // that has S_KEEP set, you can't bind ch; it's already
          // bound to that S_KEEP action, and that action has to
          // keep that key.

          oldkey = *(ptr1->m_key);
          group  = ptr1->m_group;
          for (i = 0 ; keys_settings[i] && search ; i++)
            for (ptr2 = keys_settings[i] ; !(ptr2->m_flags & S_END) ; ptr2++)
              if ((ptr2->m_flags & (S_KEY|S_KEEP)) &&
                (ptr2->m_group == group)  && 
                (ptr1 != ptr2))
                if (*(ptr2->m_key) == ch)
                  {
                  if (ptr2->m_flags & S_KEEP)
                    return true; // can't have it!
                  *(ptr2->m_key) = oldkey;
                  search = false;
                  break;
                  }
          *(ptr1->m_key) = ch;
          }

        M_SelectDone(ptr1);             // phares 4/17/98
        return true;
        }

    // Weapons

    if (set_weapon_active) // on the weapons setup screen
      if (setup_select) // changing an entry
        {
        if (ch != key_menu_enter)
          {
          int oldass;
          dboolean search = true;
      
          ch -= 0x30; // out of ascii
          if (ch < 1 || ch > 9)
            return true; // ignore
              
          // Plasma and BFG don't exist in shareware
              
          if (gamemode == shareware)
            if ((ch == (wp_plasma+1)) || (ch == (wp_bfg+1))) 
              return true;
            
          // SSG doesn't exist in shareware, registered, or
          // retail DOOM 1
            
          if (gamemode != commercial)
            if (ch == (wp_supershotgun+1))
              return true;
            
          // see if 'ch' is already assigned elsewhere. if so,
          // you have to swap assignments.
            
          oldass = *(ptr1->m_var1);
          for (i = 0 ; weap_settings[i] && search ; i++)
            for (ptr2 = weap_settings[i] ; !(ptr2->m_flags & S_END) ; ptr2++)
              if ((ptr2->m_flags & S_WEAP) && (ptr1 != ptr2))
                if (*(ptr2->m_var1) == ch)
                  {
                  *(ptr2->m_var1) = oldass;
                  search = false;
                  break;
                  }
          *(ptr1->m_var1) = ch;
          }

        M_SelectDone(ptr1);             // phares 4/17/98
        return true;
        }

    // Status Bar and Heads-Up Display

    if (set_status_active) // on the status bar & hud setup screen
      if (setup_select) // changing an entry
        {

        // Here's where status/hud-specific setup items go.

        M_SelectDone(ptr1);             // phares 4/17/98
        return true;
        }

    // Automap

    if (set_auto_active) // on the automap setup screen
      if (setup_select) // incoming key
        {
        if (ch == key_menu_down)                
          {
          if (++color_palette_y == 16)
            color_palette_y = 0;
          S_StartSound(NULL,sfx_itemup);
          return true;
          }

        if (ch == key_menu_up)                
          {
          if (--color_palette_y < 0)
            color_palette_y = 15;
          S_StartSound(NULL,sfx_itemup);
          return true;
          }

        if (ch == key_menu_left)                
          {
          if (--color_palette_x < 0)
            color_palette_x = 15;
          S_StartSound(NULL,sfx_itemup);
          return true;
          }

        if (ch == key_menu_right)                
          {
          if (++color_palette_x == 16)
            color_palette_x = 0;
          S_StartSound(NULL,sfx_itemup);
          return true;
          }

        if (ch == key_menu_enter)               
          {
          *(ptr1->m_var1) = color_palette_x + 16*color_palette_y;
          M_SelectDone(ptr1);                               // phares 4/17/98
          colorbox_active = false;
          return true;
          }
        }

    // Enemies

    if (set_enemy_active) // on the enemy setup screen
      if (setup_select) // changing an entry
        {

        // Here's where enemy-specific setup items go.
        // A good place for the Phase II AI changes, if any.

        M_SelectDone(ptr1);                                 // phares 4/17/98
        return true;
        }

    // Messages

    if (set_mess_active) // on the messages setup screen
      if (setup_select)  // changing an entry
        {

        // Here's where message-specific setup items go.

        M_SelectDone(ptr1);                                 // phares 4/17/98
        return true;
        }

    // Chat Strings

    if (set_chat_active)  // on the Chat Strings setup screen
      if (setup_select) // changing an entry
        {
        if (ptr1->m_flags & S_CHAT) // creating/editing a string?
          {
          if (ch == key_menu_backspace) // backspace and DEL
            {
            if (chat_string_buffer[chat_index] == 0)
              {
              if (chat_index > 0)
                chat_string_buffer[--chat_index] = 0;
              }
             // shift the remainder of the text one char left
            else
              strcpy(&chat_string_buffer[chat_index],
                   &chat_string_buffer[chat_index+1]);
            }
          else if (ch == key_menu_left) // move cursor left
            {
            if (chat_index > 0)
              chat_index--;
            }
          else if (ch == key_menu_right) // move cursor right
            {
            if (chat_string_buffer[chat_index] != 0)
              chat_index++;
            }
          else if ((ch == key_menu_enter) ||
               (ch == key_menu_escape))
            {
            //(char *)ptr1->m_var1 = chat_string_buffer;
            strncpy((char*)(*(ptr1->m_var1)), chat_string_buffer, CHAT_STRING_BFR_SIZE);
            M_SelectDone(ptr1);         // phares 4/17/98
            }

          // Adding a char to the text. Has to be a printable
          // char, and you can't overrun the buffer. If the
          // chat string gets larger than what the screen can hold,
          // it is dealt with when the string is drawn (above).

          else if ((ch >= 32) && (ch <= 126))
            if ((chat_index+1) < CHAT_STRING_BFR_SIZE)
              {
              if (shiftdown)
                ch = shiftxform[ch];
              if (chat_string_buffer[chat_index] == 0) 
                {
                chat_string_buffer[chat_index++] = ch;
                chat_string_buffer[chat_index] = 0;
                }
              else
                chat_string_buffer[chat_index++] = ch;
              }
          return true;
          }

        M_SelectDone(ptr1);             // phares 4/17/98
        return true;
        }

    // Not changing any items on the Setup screens. See if we're
    // navigating the Setup menus or selecting an item to change.

    if (ch == key_menu_down)
      {
      ptr1->m_flags &= ~S_HILITE;           // phares 4/17/98
      do
        if (ptr1->m_flags & S_END)
          {
          set_menu_itemon = 0;
          ptr1 = current_setup_menu;
          }
        else
          {
          set_menu_itemon++;
          ptr1++;
          }
      while (ptr1->m_flags & S_SKIP);
      M_SelectDone(ptr1);               // phares 4/17/98
      return true;
      }
  
    if (ch == key_menu_up)                 
      {
      ptr1->m_flags &= ~S_HILITE;           // phares 4/17/98
      do
        {
        if (set_menu_itemon == 0)
          do
            set_menu_itemon++;
          while(!((current_setup_menu + set_menu_itemon)->m_flags & S_END));
        set_menu_itemon--;
        }
      while((current_setup_menu + set_menu_itemon)->m_flags & S_SKIP);
      M_SelectDone(current_setup_menu + set_menu_itemon);               // phares 4/17/98
      return true;
      }

    if (ch == key_menu_enter)               
      {
      int flags = ptr1->m_flags;

      // You've selected an item to change. Highlight it, post a new
      // message about what to do, and get ready to process the
      // change.

      if (flags & S_NUM)
        {
        setup_gather = true;
        *(ptr1->m_var1) = 0; // clear so you can rebuild it
        gather_count = 0;
        }

      else if (flags & S_COLOR)
        {
        int color = *(ptr1->m_var1);
        
        if (color < 0 || color > 255) // range check the value
          color = 0;        // 'no show' if invalid
        color_palette_x = (*(ptr1->m_var1))&15;
        color_palette_y = (*(ptr1->m_var1))>>4;
        colorbox_active = true;
        }

      else if (flags & S_CHAT)
        {

        // copy chat string into working buffer; trim if needed.
        // free the old chat string memory and replace it with
        // the (possibly larger) new memory for editing purposes

        chat_string_buffer = malloc(CHAT_STRING_BFR_SIZE);
        strncpy(chat_string_buffer,(char *)(*(ptr1->m_var1)),CHAT_STRING_BFR_SIZE);
        chat_string_buffer[CHAT_STRING_BFR_SIZE-1] = 0; // gurantee null delimiter

        // set chat table pointer to working buffer
        // and free old string's memory.

        free((char *)(*(ptr1->m_var1)));
        strncpy((char*)(*(ptr1->m_var1)), chat_string_buffer, CHAT_STRING_BFR_SIZE);
        //(char *)ptr1->m_var1 = chat_string_buffer;
        chat_index = 0; // current cursor position in chat_string_buffer
        }

      else if (flags & S_RESET)
        default_verify = true;

      ptr1->m_flags |= S_SELECT;
      setup_select = true;
      S_StartSound(NULL,sfx_itemup);
      return true;
      }

    if ((ch == key_menu_escape) || (ch == key_menu_backspace))
      {
      if (ch == key_menu_escape) // Clear all menus
        M_ClearMenus();
      else // key_menu_backspace = return to Setup Menu
        if (currentMenu->prevMenu)
          {
          currentMenu = currentMenu->prevMenu;
          itemOn = currentMenu->lastOn;
          S_StartSound(NULL,sfx_swtchn);
          }
      ptr1->m_flags &= ~(S_HILITE|S_SELECT);      // phares 4/19/98
      setup_active = false;
      set_keybnd_active = false;
      set_weapon_active = false;
      set_status_active = false;
      set_auto_active = false;
      set_enemy_active = false;
      set_mess_active = false;
      set_chat_active = false;
      colorbox_active = false;
      default_verify = false;             // phares 4/19/98
      HU_Start();    // catch any message changes // phares 4/19/98
      S_StartSound(NULL,sfx_swtchx);
      return true;
      }

    // Some setup screens may have multiple screens.
    // When there are multiple screens, m_prev and m_next items need to
    // be placed on the appropriate screen tables so the user can
    // move among the screens using the left and right arrow keys.
    // The m_var1 field contains a pointer to the appropriate screen
    // to move to.

    if (ch == key_menu_left)
      {
      ptr2 = ptr1;
      do
        {
        ptr2++;
        if (ptr2->m_flags & S_PREV)
          {
          ptr1->m_flags &= ~S_HILITE;
          mult_screens_index--;
          current_setup_menu = (setup_menu_t *) ptr2->m_var1;
          set_menu_itemon = 0;
          while ((current_setup_menu + set_menu_itemon)->m_flags & S_SKIP)
            set_menu_itemon++;
          (current_setup_menu + set_menu_itemon)->m_flags |= S_HILITE;
          S_StartSound(NULL,sfx_swtchx);
          return true;
          }
        }
      while (!(ptr2->m_flags & S_END));
      }

    if (ch == key_menu_right)                
      {
      ptr2 = ptr1;
      do
        {
        ptr2++;
        if (ptr2->m_flags & S_NEXT)
          {
          ptr1->m_flags &= ~S_HILITE;
          mult_screens_index++;
          current_setup_menu = (setup_menu_t *) ptr2->m_var1;
          set_menu_itemon = 0;
          while ((current_setup_menu + set_menu_itemon)->m_flags & S_SKIP)
            set_menu_itemon++;
          (current_setup_menu + set_menu_itemon)->m_flags |= S_HILITE;
          S_StartSound(NULL,sfx_swtchx);
          return true;
          }
        }
      while (!(ptr2->m_flags & S_END));
      }

    } // End of Setup Screen processing

    
    
    
  // From here on, these navigation keys are used on the BIG FONT menus
  // like the Main Menu.

    if (ch == key_menu_down)                                   // phares 3/7/98
       {
        do
           {
            if (itemOn+1 > currentMenu->numitems-1)
                itemOn = 0;
            else
                itemOn++;
            S_StartSound(NULL,sfx_pstop);
           }
        while(currentMenu->menuitems[itemOn].status==-1);
        return true;
       }
  
    if (ch == key_menu_up)                                     // phares 3/7/98
       {
        do
           {
            if (!itemOn)
                itemOn = currentMenu->numitems-1;
            else
                itemOn--;
            S_StartSound(NULL,sfx_pstop);
           }
        while(currentMenu->menuitems[itemOn].status==-1);
        return true;
       }

    if (ch == key_menu_left)                                   // phares 3/7/98
       {
        if (currentMenu->menuitems[itemOn].routine &&
            currentMenu->menuitems[itemOn].status == 2)
           {
            S_StartSound(NULL,sfx_stnmov);
            currentMenu->menuitems[itemOn].routine(0);
           }
        return true;
       }
  
    if (ch == key_menu_right)                                  // phares 3/7/98
       {
        if (currentMenu->menuitems[itemOn].routine &&
            currentMenu->menuitems[itemOn].status == 2)
           {
            S_StartSound(NULL,sfx_stnmov);
            currentMenu->menuitems[itemOn].routine(1);
           }
        return true;
       }

    if (ch == key_menu_enter)                                  // phares 3/7/98
       {
        if (currentMenu->menuitems[itemOn].routine &&
            currentMenu->menuitems[itemOn].status)
           {
            currentMenu->lastOn = itemOn;
            if (currentMenu->menuitems[itemOn].status == 2)
               {
                currentMenu->menuitems[itemOn].routine(1);   // right arrow
                S_StartSound(NULL,sfx_stnmov);
               }
            else
               {
                currentMenu->menuitems[itemOn].routine(itemOn);
                S_StartSound(NULL,sfx_pistol);
               }
           }
        if (currentMenu==&NewDef) //jff 3/24/98 remember last skill selected
            defaultskill = currentMenu->lastOn+1;
        return true;
       }
  
    if (ch == key_menu_escape)                                 // phares 3/7/98  
       {
        currentMenu->lastOn = itemOn;
        M_ClearMenus ();
        S_StartSound(NULL,sfx_swtchx);
        return true;
       }
  
    if (ch == key_menu_backspace)                              // phares 3/7/98
       {
        currentMenu->lastOn = itemOn;

        // phares 3/30/98:
        // add checks to see if you're in the extended help screens
        // if so, stay with the same menu definition, but bump the
        // index back one. if the index bumps back far enough ( == 0)
        // then you can return to the Read_Thisn menu definitions

        if (currentMenu->prevMenu)
           {
/*
            if (currentMenu == &ExtHelpDef)
               {
                if (--extended_help_index == 0)
                   {
                    currentMenu = currentMenu->prevMenu;
                    extended_help_index = 1; // reset
                   }
               }
            else
*/
               currentMenu = currentMenu->prevMenu;
            itemOn = currentMenu->lastOn;
            S_StartSound(NULL,sfx_swtchn);
           }
    return true;
    }
  
  else
    {
    for (i = itemOn+1;i < currentMenu->numitems;i++)
      if (currentMenu->menuitems[i].alphaKey == ch)
        {
        itemOn = i;
        S_StartSound(NULL,sfx_pstop);
        return true;
        }
    for (i = 0;i <= itemOn;i++)
      if (currentMenu->menuitems[i].alphaKey == ch)
        {
        itemOn = i;
        S_StartSound(NULL,sfx_pstop);
        return true;
        }
    }
  return false;
    
    
    
    // Keys usable within menu
 //   switch (ch)
 //   {
 //     case SDL_SCANCODE_DOWN:
	//do
	//{
	//    if (itemOn+1 > currentMenu->numitems-1)
	//	itemOn = 0;
	//    else itemOn++;
	//    S_StartSound(NULL,sfx_pstop);
	//} while(currentMenu->menuitems[itemOn].status==-1);
	//return true;
	//	
 //     case SDL_SCANCODE_UP:
	//do
	//{
	//    if (!itemOn)
	//	itemOn = currentMenu->numitems-1;
	//    else itemOn--;
	//    S_StartSound(NULL,sfx_pstop);
	//} while(currentMenu->menuitems[itemOn].status==-1);
	//return true;

 //     case SDL_SCANCODE_LEFT:
	//if (currentMenu->menuitems[itemOn].routine &&
	//    currentMenu->menuitems[itemOn].status == 2)
	//{
	//    S_StartSound(NULL,sfx_stnmov);
	//    currentMenu->menuitems[itemOn].routine(0);
	//}
	//return true;
	//	
 //     case SDL_SCANCODE_RIGHT:
	//if (currentMenu->menuitems[itemOn].routine &&
	//    currentMenu->menuitems[itemOn].status == 2)
	//{
	//    S_StartSound(NULL,sfx_stnmov);
	//    currentMenu->menuitems[itemOn].routine(1);
	//}
	//return true;

 //     case SDL_SCANCODE_RETURN:
	//if (currentMenu->menuitems[itemOn].routine &&
	//    currentMenu->menuitems[itemOn].status)
	//{
	//    currentMenu->lastOn = itemOn;
	//    if (currentMenu->menuitems[itemOn].status == 2)
	//    {
	//	currentMenu->menuitems[itemOn].routine(1);      // right arrow
	//	S_StartSound(NULL,sfx_stnmov);
	//    }
	//    else
	//    {
	//	currentMenu->menuitems[itemOn].routine(itemOn);
	//	S_StartSound(NULL,sfx_pistol);
	//    }
	//}
	//return true;
	//	
 //     /*case 
 //     
 //     
 //     
 //     
 //     CONSOLE:
 //          currentMenu->lastOn = itemOn;
 //          M_ClearMenus ();
 //          return true;*/
	//	
 //     case SDL_SCANCODE_ESCAPE:
	//currentMenu->lastOn = itemOn;
	//M_ClearMenus ();
	//S_StartSound(NULL,sfx_swtchx);
	//return true;
	//	
 //     case SDL_SCANCODE_BACKSPACE:
	//currentMenu->lastOn = itemOn;
	//if (currentMenu->prevMenu)
	//{
	//    currentMenu = currentMenu->prevMenu;
	//    itemOn = currentMenu->lastOn;
	//    S_StartSound(NULL,sfx_swtchn);
	//}
	//return true;
	//
 //     default:
	//for (i = itemOn+1;i < currentMenu->numitems;i++)
	//    if (currentMenu->menuitems[i].alphaKey == ch)
	//    {
	//	itemOn = i;
	//	S_StartSound(NULL,sfx_pstop);
	//	return true;
	//    }
	//for (i = 0;i <= itemOn;i++)
	//    if (currentMenu->menuitems[i].alphaKey == ch)
	//    {
	//	itemOn = i;
	//	S_StartSound(NULL,sfx_pstop);
	//	return true;
	//    }
	//break;
	//
 //   }

 //   return false;

}



//
// M_StartControlPanel
//
void M_StartControlPanel (void)
{
    // intro might call this repeatedly
    if (menuactive)
	return;
    
    menuactive = 1;
    currentMenu = &MainDef;         // JDC
    itemOn = currentMenu->lastOn;   // JDC
}


//
// M_Drawer
// Called after the view has been rendered,
// but before it has been blitted.
//
void M_Drawer (void)
{
/*
    static short	x;
    static short	y;
    short		i;
    short		max;
    char		string[40];
    int			start;

    inhelpscreens = false;
    
    // Horiz. & Vertically center string and print it.
    if (messageToPrint)
    {
	start = 0;
	y = (SCREENHEIGHT/2) - M_StringHeight(messageString)/2;
	while(*(messageString+start))
	{
	    for (i = 0;(unsigned)i < strlen(messageString+start);i++)
		if (*(messageString+start+i) == '\n')
		{
		    memset(string,0,40);
		    strncpy(string,messageString+start,i);
		    start += i+1;
		    break;
		}
				
	    if ((unsigned)i == strlen(messageString+start))
	    {
		strcpy(string,messageString+start);
		start += i;
	    }
				
	    x = (SCREENWIDTH/2) - M_StringWidth(string)/2;
	    M_WriteText(x,y,string);
	    y += SHORT(hu_font[0]->height);
	}
	return;
    }

    if (!menuactive)
	return;

    if (currentMenu->routine)
	currentMenu->routine();         // call Draw routine
    
    // DRAW MENU
    x = ((SCREENWIDTH-320)/2)+currentMenu->x;
    y = ((SCREENHEIGHT-200)/2)+currentMenu->y;
    max = currentMenu->numitems;

    for (i=0;i<max;i++)
    {
	if (currentMenu->menuitems[i].name[0])
	    V_DrawPatchDirect (x,y,0,
			       W_CacheLumpName(currentMenu->menuitems[i].name ,PU_CACHE));
	y += LINEHEIGHT;
    }
    
    // DRAW SKULL
    V_DrawPatchDirect(x+SKULLXOFF,((SCREENHEIGHT-200)/2)+currentMenu->y - 5 + itemOn*LINEHEIGHT, 0,
		      W_CacheLumpName(skullName[whichSkull],PU_CACHE));
*/

}

void GL_DrawMenu()
   {
    static int  x, y, i, max, w, h;
    float       Left, Bottom, Right, Top, lh;
    GLTexData  *GLTex;

    char		string[40];
    int			start;

    inhelpscreens = false;

    // Horiz. & Vertically center string and print it.
    if (messageToPrint)
       {
        start = 0;
        y = 100-(M_StringHeight(messageString)/2);
        while(*(messageString+start))
           {
            for (i = 0;(unsigned)i < strlen(messageString+start);i++)
               {
                if (*(messageString+start+i) == '\n')
                   {
                    memset(string,0,40);
                    strncpy(string,messageString+start,i);
                    start += i+1;
                    break;
                   }
               }
            if ((unsigned)i == strlen(messageString+start))
               {
                strcpy(string,messageString+start);
                start += i;
               }
            x = 160 - M_StringWidth(string)/2;
            GL_WriteText(x,y,string);
            y += (int)GLFontHeight;
           }
        return;
       }

    if (!menuactive)
        return;

    GLTex = 0;

    if (currentMenu->menuitems == MainMenu)
       {
        GL_DrawMainMenu();
        GLTex = glMainMenu;
       }
    else
    if (currentMenu->menuitems == GameMenu)
       {
        GL_DrawGame();
        GLTex = glGame;
       }
    else
    if (currentMenu->menuitems == MPlayerMenu)
       {
        GL_DrawMPlayer();
        GLTex = glMultiPlayer;
       }
    else
    if (currentMenu == &JoinGameDef)
       {
        GL_DrawJoinGame();
        //GLTex = glMultiPlayer;
       }
    else
    if (currentMenu == &MusicDef)
       {
        GL_DrawMusic();
        //GLTex = glMultiPlayer;
       }
    else
    if (currentMenu == &StartGameDef)
       {
        GL_DrawStartGame();
        //GLTex = glMultiPlayer;
       }
    else
    if (currentMenu->menuitems == DisplayMenu)
       {
        GL_DrawDisplay();
        //GLTex = glDisplay;
       }
    else
    if (currentMenu->menuitems == MouseMenu)
       {
        GL_DrawMouse();
        GLTex = glMouse;
       }
    else
    if (currentMenu->menuitems == LoadGameMenu)
       {
        GL_DrawLoad();
       }
    else
    if (currentMenu->menuitems == SaveMenu)
       {
        GL_DrawSave();
       }
    else
    if (currentMenu->menuitems == SetupMenu)
       {
        GL_DrawSetup();
        GLTex = glSetupMenu;
       }
    else
    if (currentMenu == &PlayerSetupDef)
       {
        GL_DrawPlayerSetup();
       }
    else
    if (currentMenu == &KeybndDef)
       {
        GL_DrawKeybnd();
       }
    else
    if (currentMenu == &ReadDef1)
       {
        GL_DrawReadThis1();
       }
    else
    if (currentMenu == &ReadDef2)
       {
        GL_DrawReadThis2();
       }
    else
    if (currentMenu->menuitems == EpisodeMenu)
       {
        GL_DrawEpisode();
        GLTex = glEpisode;
       }
    else
    if (currentMenu->menuitems == SoundMenu)
       {
        GL_DrawSound();
        GLTex = glSound;
       }
    else
    if (currentMenu->menuitems == OptionsMenu)
       {
        GL_DrawOptions();
        GLTex = glOptions;
       }
    else
    if (currentMenu->menuitems == NewGameMenu)
       {
        GL_DrawNewGame();
        GLTex = glNewGame;
       }

    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);
    if (gl_premalpha)
       {
        glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
       }
    else
       {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       }

    if ((currentMenu->x >= 0) && (currentMenu->x <= (320-MenuSkull[whichSkull].Width)))
       {
        Left = -160.0f + (currentMenu->x+SKULLXOFF);
        Right = Left + (MenuSkull[whichSkull].Width);
        Top = (120.0f - (float)(((currentMenu->y-5) + (itemOn*LINEHEIGHT)) * 1.2f));
        Bottom = Top - (MenuSkull[whichSkull].Height * 1.2f);

        max = currentMenu->numitems;

        glBindTexture(GL_TEXTURE_2D, MenuSkull[whichSkull].TexName);
        glBegin(GL_QUADS);
           glTexCoord2f( 0.0f, 1.0f);
           glVertex3f( Left, Top, SetBack);
           glTexCoord2f( 0.0f, MenuSkull[whichSkull].YDisp);
           glVertex3f( Left, Bottom, SetBack);
           glTexCoord2f( MenuSkull[whichSkull].XDisp, MenuSkull[whichSkull].YDisp);
           glVertex3f( Right, Bottom, SetBack);
           glTexCoord2f( MenuSkull[whichSkull].XDisp, 1.0f);
           glVertex3f( Right, Top, SetBack);
        glEnd();
       }

    if (GLTex == 0)
       return;

    Top = 120.0f - ((float)currentMenu->y * 1.2f);
    Left = -160+(float)currentMenu->x;
    Bottom = Top - (GLTex[0].Height*1.2f);
    lh = ((float)LINEHEIGHT*1.2f);

    for (i = 0; i < currentMenu->numitems; i++)
       {
        if (currentMenu->menuitems[i].name[0])
           {
            Right = Left + GLTex[i].Width;
            glBindTexture(GL_TEXTURE_2D, GLTex[i].TexName);
            glBegin(GL_QUADS);
               glTexCoord2f( 0.0f, 1.0f);
               glVertex3f( Left, Top, SetBack);
               glTexCoord2f( 0.0f, GLTex[i].YDisp);
               glVertex3f( Left, Bottom, SetBack);
               glTexCoord2f( GLTex[i].XDisp, GLTex[i].YDisp);
               glVertex3f( Right, Bottom, SetBack);
               glTexCoord2f( GLTex[i].XDisp, 1.0f);
               glVertex3f( Right, Top, SetBack);
            glEnd();
           }
        Top -= lh;
        Bottom -= lh;
       }
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
   }

//
// M_ClearMenus
//
void M_ClearMenus (void)
{
    menuactive = 0;
    // if (!netgame && usergame && paused)
    //       sendpause = true;
}




//
// M_SetupNextMenu
//
void M_SetupNextMenu(menu_t *menudef)
{
    currentMenu = menudef;
    itemOn = currentMenu->lastOn;
}


//
// M_Ticker
//
void M_Ticker (void)
{
    if (--skullAnimCounter <= 0)
    {
	whichSkull ^= 1;
	skullAnimCounter = 8;
    }
}

unsigned  char tbuff[512*512];

void SaveResources()
   {
/*
    int      fn, pad;

    fn = open("m_setup.lmp", O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0666);
    pad = m_auto - m_setup;
    write(fn, m_setup, pad);
    close(fn);

    fn = open("m_music.raw", O_RDONLY | O_BINARY);
    read(fn, tbuff, (63*15));
    close(fn);
    SavePic( 63, 15, tbuff, "m_music.lmp");

    fn = open("aboxskin.raw", O_RDONLY | O_BINARY);
    read(fn, tbuff, (56*56));
    close(fn);
    SavePic( 56, 56, tbuff, "aboxskin.lmp");
*/
   }

//
// M_Init
//
void M_Init (void)
   {
    int      i;

    currentMenu = &MainDef;
    menuactive = 0;
    itemOn = currentMenu->lastOn;
    whichSkull = 0;
    skullAnimCounter = 10;
    screenSize = screenblocks - 3;
    messageToPrint = 0;
    messageString = NULL;
    messageLastMenuActive = menuactive;
    quickSaveSlot = -1;

    // Here we could catch other version dependencies,
    //  like HELP1/2, and four episodes.

  
    switch ( gamemode )
       {
        case commercial:
             // This is used because DOOM 2 had only one HELP
             //  page. I use CREDIT as second page now, but
             //  kept this hack for educational purposes.
             MainMenu[readthis] = MainMenu[quitdoom];
             MainDef.numitems--;
             MainDef.y += 8;
             NewDef.prevMenu = &MainDef;
             //ReadDef1.routine = M_DrawReadThis1;
             ReadDef1.routine = GL_DrawReadThis1;
             ReadDef1.x = 330;
             ReadDef1.y = 165;
             ReadMenu1[0].routine = M_FinishReadThis;
             break;
        case shareware:
             // Episode 2 and 3 are handled,
             //  branching to an ad screen.
        case registered:
             // We need to remove the fourth episode.
             EpiDef.numitems--;
             break;
        case retail:
             // We are fine.
        default:
             break;
       }

/*
    for (i = 0; i < episodetitle, i++)
       {
        if (gamemode == commercial) && (i == (episodetitle-1))
           {
            break;
           }
        glTitles[i].glData->TexName = GL_MakeSpriteTexture(W_CacheLumpName(glTitles[i].lumpname,PU_CACHE), glTitles[i].glData, false);
       }
*/
    GL_MakeScreenTexture(W_CacheLumpName("TITLEPIC", PU_CACHE), glTitlePic);
    GL_MakeScreenTexture(W_CacheLumpName("CREDIT", PU_CACHE), glCredit);
    if (W_CheckNumForName("HELP") != -1)
       {
        GL_MakeScreenTexture(W_CacheLumpName("HELP", PU_CACHE), glHelp);
       }
    if (W_CheckNumForName("HELP1") != -1)
       {
        GL_MakeScreenTexture(W_CacheLumpName("HELP1", PU_CACHE), glHelp1);
       }
    if (W_CheckNumForName("HELP2") != -1)
       {
        GL_MakeScreenTexture(W_CacheLumpName("HELP2", PU_CACHE), glHelp2);
       }
    GL_MakeSpriteTexture(W_CacheLumpName("M_DOOM",PU_CACHE), &GameLogo, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_PSETUP",PU_CACHE), &glPlayerSetupTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_MSENS",PU_CACHE), &glMouseTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_OPTTTL",PU_CACHE), &glOptionTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_DISOPT",PU_CACHE), &glDisplayTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_MPLTTL",PU_CACHE), &glMPlayerTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_EPISOD",PU_CACHE), &glEpisodeTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_NEWG",PU_CACHE), &glNewGameTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_SKILL",PU_CACHE), &glSkillTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_SVOL",PU_CACHE), &glSoundVolTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_LOADG",PU_CACHE), &glLoadGameTitle, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_SAVEG",PU_CACHE), &glSaveGameTitle, false);

    // Create "Skull" textures...
    GL_MakeSpriteTexture(W_CacheLumpName(skullName[0],PU_CACHE), &MenuSkull[0], false);
    GL_MakeSpriteTexture(W_CacheLumpName(skullName[1],PU_CACHE), &MenuSkull[1], false);

    for (i = 0; i < main_end; i++)
       {
        if (MainMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(MainMenu[i].name,PU_CACHE), &glMainMenu[i], false);
           }
       }
    if (gamemode != commercial)
       {
        for (i = 0; i < EpiDef.numitems; i++)
           {
            if (EpisodeMenu[i].name[0])
               {
                GL_MakeSpriteTexture(W_CacheLumpName(EpisodeMenu[i].name,PU_CACHE), &glEpisode[i], false);
               }
           }
       }
    for (i = 0; i < newg_end; i++)
       {
        if (NewGameMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(NewGameMenu[i].name,PU_CACHE), &glNewGame[i], false);
           }
       }
    for (i = 0; i < mplay_end; i++)
       {
        if (MPlayerMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(MPlayerMenu[i].name,PU_CACHE), &glMultiPlayer[i], false);
           }
       }
    for (i = 0; i < game_end; i++)
       {
        if (GameMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(GameMenu[i].name,PU_CACHE), &glGame[i], false);
           }
       }
    for (i = 0; i < display_end; i++)
       {
        if (DisplayMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(DisplayMenu[i].name,PU_CACHE), &glDisplay[i], false);
           }
       }
    for (i = 0; i < opt_end; i++)
       {
        if (OptionsMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(OptionsMenu[i].name,PU_CACHE), &glOptions[i], false);
           }
       }
    for (i = 0; i < sound_end; i++)
       {
        if (SoundMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(SoundMenu[i].name,PU_CACHE), &glSound[i], false);
           }
       }
    for (i = 0; i < mouse_end; i++)
       {
        if (MouseMenu[i].name[0])
           {
            GL_MakeSpriteTexture(W_CacheLumpName(MouseMenu[i].name,PU_CACHE), &glMouse[i], false);
           }
       }
    for (i = 0; i < 2; i++)
       {
        GL_MakeSpriteTexture(W_CacheLumpName(detailNames[i],PU_CACHE), &glDtlName[i], false);
        GL_MakeSpriteTexture(W_CacheLumpName(msgNames[i],PU_CACHE), &glMsgName[i], false);
       }

    GL_MakeSpriteTexture(W_CacheLumpName("M_THERML",PU_CACHE), &glThermL, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_THERMM",PU_CACHE), &glThermM, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_THERMR",PU_CACHE), &glThermR, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_THERMO",PU_CACHE), &glThermO, false);

    GL_MakeSpriteTexture(W_CacheLumpName("M_LSLEFT",PU_CACHE), &LSLeft, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_LSCNTR",PU_CACHE), &LSCenter, false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_LSRGHT",PU_CACHE), &LSRight, false);

    //SaveResources();

    GL_MakeSpriteTexture(W_CacheLumpName("M_SETTTL",PU_CACHE), &glSetupTitle, false);

    GL_MakeSpriteTexture(W_CacheLumpName("M_KEYBND",PU_CACHE), &glSetupMenu[0], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_WEAP",PU_CACHE), &glSetupMenu[1], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_STAT",PU_CACHE), &glSetupMenu[2], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_AUTO",PU_CACHE), &glSetupMenu[3], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_ENEM",PU_CACHE), &glSetupMenu[4], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_MESS",PU_CACHE), &glSetupMenu[5], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_CHAT",PU_CACHE), &glSetupMenu[6], false);

    GL_MakeSpriteTexture(W_CacheLumpName("M_BUTT1",PU_CACHE), &glResetBtn[0], false);
    GL_MakeSpriteTexture(W_CacheLumpName("M_BUTT2",PU_CACHE), &glResetBtn[1], false);

    GL_MakeSpriteTexture(W_CacheLumpName("ABOXSKIN",PU_CACHE), &AmmoBoxSkin, false);
   }

