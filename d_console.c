// d_console.c
// This is the console control program it is NOT covered
// by the Doom license agreement.
// Copyright © 1998, Bruce A. Lewis
// This code may not be re-used in a commercial product
// of any kind without express written consent from
// the author, Bruce A. Lewis.
//
#include "thirdparty/glad/include/glad/glad.h"
#include <ctype.h>
////////////////////////////////////////////////////////////////////////
// Doom defines and external data
////////////////////////////////////////////////////////////////////////
#include "doomdef.h"
#include "doomstat.h"
#include "d_event.h"
#include "d_englsh.h"
#include "doomlib.h"
#include "g_game.h"
#include "hu_stuff.h"
#include "i_system.h"
#include "m_cheat.h"
#include "p_inter.h"
#include "s_sound.h"
#include "sounds.h"
#include "r_defs.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"
#include "gldefs.h"

#define ST_MSGWIDTH        256
#define CMDLENGTH         1024

extern byte *screens[5];
extern patch_t*		hu_font[HU_FONTSIZE];

extern cheatseq_t	cheat_mus;
extern cheatseq_t	cheat_god;
extern cheatseq_t	cheat_ammo;
extern cheatseq_t	cheat_ammonokey;
extern cheatseq_t	cheat_noclip;
extern cheatseq_t	cheat_commercial_noclip;

extern cheatseq_t	cheat_powerup[7];

extern cheatseq_t	cheat_choppers;
extern cheatseq_t	cheat_clev;
extern cheatseq_t	cheat_mypos;


void M_WriteText(int x, int y, char *string);

#ifdef _WIN32
#ifdef OLD_WIN32
#define strnicmp strnicmp
#else
#define strnicmp _strnicmp
#endif
#else
#define strnicmp strncasecmp
#endif

////////////////////////////////////////////////////////////////////////
// Doom98 code
////////////////////////////////////////////////////////////////////////

#include "m_music.h"
#include "gl_utils.h"

void WriteDebug(char*);

extern unsigned char TexRaw[512*512];
extern unsigned char TexAlpha[512*512];
extern int TexWide, TexHigh;

#define CONSOLE_NAME "M_DOOM"
#define CONSOLE_FLAT "CEIL5_1"
#define CONSOLE_BORD "FLAT18"
#define CONSBORD     2
#define CONSVERS     96
#define CONSREV      'C'
#define CONSMSGS     64

char szProgName[] = "GLDOOM";
char szVersion[8];

typedef enum { asleep, sleepy, waking, awake } consolemode;

char revision = CONSREV;
int version = CONSVERS;

int iConsoleSpeed = 10;
int iConsoleHeight = 0;
int iConsoleMax;
int iConsoleMode = asleep;
int iFontHigh;
int iNamePosX, iVerPosX;
int iConsHead = 0, iConsTail = 0;

float GLConsoleHeight;

GLTexData ConsBack, ConsBord, ConsLogo;

int iConsBack, iConsBord, iConsLogo;

// Console background;
byte                  consback[64*64];
byte                  consbord[64*64];
byte                 *pconstex;
static patch_t       *consname;

static player_t      *plyr; 

static unsigned char  consmsgs[CONSMSGS][132];
static unsigned char  szCommand[1028];
int                   iCCursor = 0;

dboolean bConsoleActive = false;

dboolean ResizeMainWindow(char *resolution);

int CO_StringWidth(char *s);
int CO_StringPrintable(char *s);
int CO_WriteConsoleLine(int i, char *s, dboolean wrap);
int CO_GLWriteConsoleLine(int l, char *s, dboolean wrap);
void CO_AddConsoleMessage(char *s);
int CO_HandleCommand(char *cmd);
dboolean isdigits(char *s);

void R_DrawViewBorder(void);

char *PowerUpOn[] = { "INVULNERABILITY ON", "BERSERKER ON", "INVISIBILITY ON", "ENVIRONMENT SUIT ON", "FULL MAP ON", "LIGHT AMPLIFICATION GOGGLES ON" };
char *PowerUpOff[] = { "INVULNERABILITY OFF", "BERSERKER OFF", "INVISIBILITY OFF", "ENVIRONMENT SUIT OFF", "FULL MAP OFF", "LIGHT AMPLIFICATION GOGGLES OFF" };
char  PowerUpKey[] = "VSIRAL";

typedef enum { msg_ammo, msg_health, msg_armor, msg_keys, msg_weapons, msg_all, msg_amap, msg_pack, msg_bear };

char *Gimme[] = { "Full ammo given", "Full health given", "Full armor given", "All keys given", "All weapons given",
                  "You got it ALL!", "Full automap given", "Backpack given", "You are now a one person army!" };


extern int  key_lookup;
extern int  key_lookdown;
extern int  key_mlook;

extern int  key_right;
extern int  key_left;
extern int  key_up;
extern int  key_down; 
extern int  key_strafeleft;
extern int  key_straferight; 
extern int  key_fire;
extern int  key_use;
extern int  key_strafe;
extern int  key_speed; 

extern int  key_mvert;

extern int  usemouse;
extern int  mousebfire;
extern int  mousebstrafe;
extern int  mousebforward;

extern int  mouseb1;
extern int  mouseb2;
extern int  mouseb3;

extern int  gl_fog;
extern int  gl_poffsetf;
extern int  gl_poffsetu;
extern int  gl_modeltest;
extern int  gl_alphatest;
extern int  gl_premalpha;
extern int  gl_widetex;

extern char gamename[128];
extern int  hudmode;

extern int  usejoystick;
extern int	joybfire;
extern int	joybstrafe;
extern int	joybuse;
extern int	joybspeed;

extern int  joydead;

extern int  joyb1;
extern int  joyb2;
extern int  joyb3;
extern int  joyb4;

extern int          autorun;
extern int          swap_stereo;
extern int          nosound;
extern int          mvert;
extern int          mouse_factor;
extern int          mlook;
extern int          keylink;

extern unsigned char scan2chars[256];
extern unsigned char scan2char[256];
 
typedef enum { cv_value, cv_scan, cv_string } cv_types;

typedef enum { c_text,
               c_fire,
               c_forward,
               c_back,
               c_left,
               c_right,
               c_slide,
               c_slideleft,
               c_slideright,
               c_use,
               c_run,
               c_weapon1,
               c_weapon2,
               c_weapon3,
               c_weapon4,
               c_weapon5,
               c_weapon6,
               c_weapon7,
               c_say,
               c_wait,
               c_end
             } c_commands;

typedef struct
   {
    unsigned char  key;
    unsigned char *command;
   }Alias_t;


typedef struct
   {
    int  *var;
    char *name;
    int  cv_type;
    int  maxval;
   }KeyDef_t;

KeyDef_t cvars[] = { &key_right, "KEY_RIGHT", cv_scan, 0,
                     &key_up, "KEY_UP", cv_scan, 0,
                     &key_down, "KEY_DOWN", cv_scan, 0,
                     &key_strafeleft, "KEY_STRAFELEFT", cv_scan, 0,
                     &key_straferight, "KEY_STRAFERIGHT", cv_scan, 0,
                     &key_fire, "KEY_FIRE", cv_scan, 0,
                     &key_use, "KEY_USE", cv_scan, 0,
                     &key_strafe, "KEY_STRAFE", cv_scan, 0,
                     &key_speed, "KEY_SPEED", cv_scan, 0,
                     &joyb1, "JOYB1", cv_scan, 0,
                     &joyb2, "JOYB2", cv_scan, 0,
                     &joyb3, "JOYB3", cv_scan, 0,
                     &joyb4, "JOYB4", cv_scan, 0,
                     &mouseb1, "MOUSEB1", cv_scan, 0,
                     &mouseb2, "MOUSEB2", cv_scan, 0,
                     &mouseb3, "MOUSEB3", cv_scan, 0,
                     &key_lookup, "KEY_LOOKUP", cv_scan, 0,
                     &key_lookdown, "KEY_LOOKDOWN", cv_scan, 0,
                     &key_mlook, "KEY_MLOOK", cv_scan, 0,
                     &swap_stereo, "SWAP_STEREO", cv_value, 1,  // true/false
                     &autorun, "AUTORUN", cv_value, 1,    // true/false
                     &usemouse, "USEMOUSE", cv_value, 1,        // true/false
                     &mousebfire, "MOUSEBFIRE", cv_value, 2,
                     &mousebstrafe, "MOUSEBSTRAFE", cv_value, 2,
                     &mousebforward, "MOUSEBFORWARD", cv_value, 2,
                     &usejoystick, "USEJOYSTICK", cv_value, 1,  // true/false
                     &joybfire, "JOYBFIRE", cv_value, 3,
                     &joybstrafe, "JOYBSTRAFE", cv_value, 3,
                     &joybuse, "JOYBUSE", cv_value, 3,
                     &joybspeed, "JOYBSPEED", cv_value, 3,
                     &mvert, "MVERT", cv_value, 1,              // true/false
                     &mouse_factor, "MOUSE_FACTOR", cv_value, 4, // multiplier for mouse
                     &mlook, "MLOOK", cv_value, 1,              // true/false
                     &keylink, "KEYLINK", cv_value, 1,          // true/false
                     &gl_fog, "GL_FOG", cv_value, 1,            // true/false
                     &gl_poffsetf, "GL_POFFSETF", cv_value, 255,            // true/false
                     &gl_poffsetu, "GL_POFFSETU", cv_value, 255,            // true/false
                     &gl_modeltest, "GL_MODELTEST", cv_value, 1,// true/false
                     &gl_alphatest, "GL_ALPHATEST", cv_value, 1,// true/false
                     &gl_premalpha, "GL_PREMALPHA", cv_value, 1,// true/false
                     &gl_widetex, "GL_WIDETEX", cv_value, 1,    // true/false
                     &nosound, "NOSOUND", cv_value, 1,          // true/false
                     &hudmode, "HUDMODE", cv_value, 3,
                     &joydead, "JOYDEAD", cv_value, 5000,
                     &gamename, "GAME", cv_string, 128,// true/false
                     NULL,       "", cv_value, 0 };

typedef struct
   {
    int scancode;
    char *name;
   }KeyName_t;

char *scanname[] = {    "NULL", // no key
                        "", // <ESC>
                        "", // 1
                        "",  // 3
                        "",  // 4
                        "",  // reserved for weapons...
                        "",  // 5
                        "",  // 6
                        "",  // 7
                       "8",  // open
                       "9",  // open
                       "0",  // open
                        "",  // minus key
                        "",  // equals key
                    "BACK",  // backspace key
                        "",  // tab key
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
                   "ENTER", // enter key
                   "LCTRL", // left control key
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
                        "", // tilde - console key
                  "LSHIFT",
                      "\\",
                       "Z",
                       "X",
                       "C",
                       "V",
                       "B",
                       "N",
                       "M",
                       ",",
                       ".",
                       "/",
                  "RSHIFT",
                        "",  // print screen - sysrq
                    "LALT",  
                   "SPACE",
                    "CAPS",
                        "",  //  F1
                        "",  //  F2
                        "",  //  F3
                        "",  //  F4
                        "",  //  F5
                        "",  //  F6
                        "",  //  F7
                        "",  //  F8
                        "",  //  F9
                        "",  // F10
                 "NUMLOCK",
                  "SCROLL",
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
                        "",  // F11
                        "",  // F12
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
                "PADENTER", // 0x9C
                   "RCTRL", // 0x9D
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
                        "", // SYS RQ
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
                      "UP",
                  "PAGEUP",
                        "",
                    "LEFT",
                        "",
                   "RIGHT",
                        "",
                     "END",
                    "DOWN",
                "PAGEDOWN",
                  "INSERT",
                  "DELETE",
                       "-" };

/*
   This function cleans up sloppy command lines.
   It removes leading and trailing spaces from
   commands or parts of them
*/

char *CleanUpCommand(char *command)
   {
    char *tchar;
    int   i;

    tchar = command;

    for (i = (strlen(command)-1); command[i] == ' '; i--)
       command[i] = '\0';
    for (i = 0; command[i] == ' '; i++)
       tchar = &command[i+1];

    return tchar;
   }

/*
  This is the data structure and supporting data for the console commands.
  They take the form of:

  command     -> the text of the command the user types
  key length  -> the length of the command part we need to handle (command may have parameters)
  procedure   -> the function that should be called to do the command
  description -> the text displayed when the cmdlist command is issued (not hooked up yet)

  This is designed so that commands can have parameters which can
  cause the primary command handler to call a secondary function.

  (i.e. the give command calls GiveItems which processes it's own parameter
  list and calls the appropriate function based on the item requested)

  The give command may be modified extensively in the future to allow the player to
  request ANY item in the game.  It may also be modified to allow for multiple
  parameters (easily) in a single give command... Someone may want a set of
  items that isn't part of another command.
*/

typedef struct
   {
    char       *keyword;
    int         keylength;
    dboolean      (*command)(char *);
    char       *description;
   }con_command_t;

/* These are the console command functions. They work
   in conjunction with the command list.  They are invoked
   by searching through the command list for a match then
   calling the function associated with the command and
   passing the command string to the function.

   Commands which are not allowed in network games will
   call the NetCheat function which displays the message:
   "cheats only allowed in single player" to let them
   know that the command is not available to them and why.

   Some commands take subcommands and can themselves have
   command lists. (i.e. "give all" or "cd on")

   Some commands are just variations on a root name (like
   idbehold) with the last letter indicating what the
   command should do.  A simple loop is used to search for
   the command letter in a string to determine what the
   command should do.  GetPowerUp works this way.
*/

void NetCheat()
   {
    CO_AddConsoleMessage("cheats only allowed in single player");
   }

dboolean GodMode(char *cmd)
   {
    if (netgame == false)
       {
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
        plyr->cheats ^= CF_GODMODE;
        if (plyr->cheats & CF_GODMODE)
           {
            if (plyr->mo)
                plyr->mo->health = 100;
            plyr->health = 100;
            CO_AddConsoleMessage(STSTR_DQDON);
           }
        else 
           {
            CO_AddConsoleMessage(STSTR_DQDOFF);
           }
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean FullAmmo(char *cmd)
   {
    int i;

    if (netgame == false)
       {
        plyr->armorpoints = 200;
        plyr->armortype = 2;

        for (i = 0;i < NUMWEAPONS; i++)
           {
            plyr->weaponowned[i] = true;
	
            for (i = 0;i < NUMAMMO; i++)
                plyr->ammo[i] = plyr->maxammo[i];
            totalscore = 0; keepscore = 0; showscoreHUD = 0;
            CO_AddConsoleMessage(STSTR_FAADDED);
            return false;
           }
       }
    else
       {
        NetCheat();
		return false;
       }
	return false;
   }

dboolean FullAmmoKeys(char *cmd)
   {
    int i;

    if (netgame == false)
       {
        plyr->armorpoints = 200;
        plyr->armortype = 2;

        for (i = 0;i < NUMWEAPONS; i++)
           {
            plyr->weaponowned[i] = true;
           }
        for (i = 0;i < NUMAMMO; i++)
           {
            plyr->ammo[i] = plyr->maxammo[i];
           }
	
        for (i=0;i<NUMCARDS;i++)
           {
            plyr->cards[i] = true;
           }
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
	
        CO_AddConsoleMessage(STSTR_KFAADDED);
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean NoClipping(char *cmd)
   {
    if (netgame == false)
       {
        plyr->cheats ^= CF_NOCLIP;
	
        if (plyr->cheats & CF_NOCLIP)
           {
            CO_AddConsoleMessage(STSTR_NCON);
           }
        else
           {
            CO_AddConsoleMessage(STSTR_NCOFF);
           }
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean ChangeMusic(char *cmd)
   {
    char buf[3];
    int  musnum;
    char   maxep;
	
    CO_AddConsoleMessage(STSTR_MUS);
    strcpy(buf, cmd);

    if (gamemode == commercial)
       {
        if ((buf[0] >= '0' && buf[0] <= '3') &&
            (buf[1] >= '0' && buf[1] <= '9') &&
            (strncmp(buf, "00", 2) != 0))
           {
            musnum = mus_runnin + (buf[0]-'0')*10 + buf[1]-'0' - 1;
            // German version only has 30 levels. NO id wad has more
            // than 32. Need a switch to tell how many are "valid" or
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
    return false;
   }

dboolean GetPowerUp(char *cmd)
   {
    char c;
    int  i;

    if (netgame == false)
       {
        c = toupper(cmd[0]);
        for (i = 0;i < 6;i++)
           {
            if (PowerUpKey[i] == c)
               {
                if (!plyr->powers[i])
                   {
                    P_GivePower(plyr, i);
                    CO_AddConsoleMessage(PowerUpOn[i]);
                    if ((i == pw_strength) && (plyr->readyweapon != wp_fist))
                       {
                        plyr->pendingweapon = wp_fist;
                       }
                   }
                else
                if (i!=pw_strength)
                   {
                    plyr->powers[i] = 1;
                    CO_AddConsoleMessage(PowerUpOff[i]);
                   }
                else
                   {
                    plyr->powers[i] = 0;
                    CO_AddConsoleMessage(PowerUpOff[i]);
                   }
               }
           }
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean ConHelp(char *cmd)
   {
    if (cmd == 0)
       {
        // do basic help - command list
        CO_AddConsoleMessage("General help...");
       }
    else
       {
        // do specific command help
        CO_AddConsoleMessage("Command specific help...");
       }
    return false;
   }

void MaxHealth()
   {
    if (plyr->mo)
       {
        if (plyr->mo->health > 99)
           {
            plyr->mo->health = 200;
           }
        else
           {
            plyr->mo->health = 100;
           }
       }
    if (plyr->health > 99)
       {
        plyr->health = 200;
       }
    else
       {
        plyr->health = 100;
       }
    CO_AddConsoleMessage(Gimme[msg_health]);
   }

void MaxAmmo()
   {
    int i;

    for (i = 0;i < NUMAMMO; i++)
        plyr->ammo[i] = plyr->maxammo[i];
    CO_AddConsoleMessage(Gimme[msg_ammo]);
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
   }

void MaxArmor()
   {
    plyr->armorpoints = 200;
    plyr->armortype = 2;
    CO_AddConsoleMessage(Gimme[msg_armor]);
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
   }

void MaxWeapons()
   {
    int i;

    for (i = 0;i < NUMWEAPONS; i++)
         plyr->weaponowned[i] = true;
    CO_AddConsoleMessage(Gimme[msg_weapons]);
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
   }

void MaxKeys()
   {
    int i;

    for (i = 0; i < NUMCARDS;i++)
        plyr->cards[i] = true;
    CO_AddConsoleMessage(Gimme[msg_keys]);
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
   }

void MaxPack()
   {
    int i;

    if (!plyr->backpack)
       {
        for (i = 0; i < NUMAMMO; i++)
             plyr->maxammo[i] *= 2;
	    plyr->backpack = true;
	   }
    for (i = 0;i < NUMAMMO; i++)
         plyr->ammo[i] = plyr->maxammo[i];
    CO_AddConsoleMessage(Gimme[msg_pack]);
   }

void MaxMap()
   {
    plyr->powers[pw_allmap] = 1;
    CO_AddConsoleMessage(Gimme[msg_amap]);
   }

dboolean GiveAll(char *cmd)
   {
    int i;

    MaxWeapons();
    MaxArmor();
    MaxHealth();
    MaxKeys();
    MaxPack();
    MaxMap();
    CO_AddConsoleMessage(Gimme[msg_all]);
    totalscore = 0; keepscore = 0; showscoreHUD = 0;
    return false;
   }

dboolean GiveAmmo(char *cmd)
   {
    MaxAmmo();
    return false;
   }

dboolean GiveArmor(char *cmd)
   {
    MaxArmor();
    return false;
   }

dboolean GiveAutoMap(char *cmd)
   {
    MaxMap();
    return false;
   }

dboolean GiveBackPack(char *cmd)
   {
    MaxPack();
    return false;
   }

dboolean GiveFullPack(char *cmd)
   {
    MaxWeapons();
    MaxArmor();
    MaxPack();
    CO_AddConsoleMessage(Gimme[msg_bear]);
    return false;
   }

dboolean GiveHealth(char *cmd)
   {
    MaxHealth();
    return false;
   }

dboolean GiveKeys(char *cmd)
   {
    MaxKeys();
    return false;
   }

dboolean GiveWeapons(char *cmd)
   {
    MaxWeapons();
    return false;
   }

con_command_t give_commands[] = { "all",       0, GiveAll,          "gives (ammo, weapons, armor, health, keys, automap, backpack)",
                                  "ammo",      0, GiveAmmo,         "gives all ammo",
                                  "armor",     0, GiveArmor,        "gives full armor",
                                  "automap",   0, GiveAutoMap,      "gives the automap",
                                  "backpack",  0, GiveBackPack,     "gives a backpack and it's contents",
                                  "fullpack",  0, GiveFullPack,     "gives (ammo, weapons, armor and backpack)",
                                  "health",    0, GiveHealth,       "gives 100% health (or 200% health)",
                                  "keys",      0, GiveKeys,         "gives all six keys (key cards and skull keys)",
                                  "weapons",   0, GiveWeapons,      "gives all weapons",
                                  //"shells",    0, GiveShells,       "gives shotgun shells",
                                  //"bullets",   0, GiveBullets,      "gives bullets",
                                  //"rockets",   0, GiveRockets,      "gives rockets",
                                  //"cells",     0, GiveCells,        "gives cells",
                                  //"suit",      0, GiveSuit,         "gives radiation suit",
                                  //"berserk",   0, GiveBerserk,      "gives berserk pack",
                                  //"inviso",    0, GiveInviso,       "gives invisibility",
                                  //"invul",     0, GiveInvul,        "gives invulnerability",
                                  //"shotgun",   0, GiveShotgun,      "gives the pump shotgun",
                                  //"launcher",  0, GiveLauncher,     "gives the rocket launcher",
                                  //"plasma",    0, GivePlasma,       "gives the plasma rifle",
                                  //"bfg",       0, GiveBFG,          "gives the bfg 9000",
                                  0, 0, 0, 0 };

dboolean GiveItems(char *cmd)
   {
    int   i;
    char *s;

    if (netgame == false)
       {
        s = strtok(cmd, " ");
        while (s != NULL)
           {
            for (i = 0; give_commands[i].keyword != 0; i++)
               {
                if (give_commands[i].keylength > 0)
                   {
                    if (D_strncasecmp(give_commands[i].keyword, s, give_commands[i].keylength) == 0)
                       {
                        //s = CleanUpCommand(&s[give_commands[i].keylength]);
                        give_commands[i].command(s);
                       }
                   }
                else
                   {
                    if (D_strcasecmp(give_commands[i].keyword, s) == 0)
                       {
                        give_commands[i].command(s);
                       }
                   }
               }
            s = strtok(NULL, " ");
           }
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean MidiCommand(char *cmd)
   {
    if ((D_strcasecmp(cmd, "pause") == 0) || (D_strcasecmp(cmd, "resume") == 0))
        //PauseResumeMusic();
    return false;
   }

dboolean Choppers(char *cmd)
   {
    if (netgame == false)
       {
        // 'choppers' invulnerability & chainsaw
        plyr->weaponowned[wp_chainsaw] = true;
        plyr->powers[pw_invulnerability] = true;
        CO_AddConsoleMessage(STSTR_CHOPPERS);
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
       }
    else
       {
        NetCheat();
		return false;
       }
    return false;
   }

dboolean ShowPosition(char *cmd)
   {
    // 'mypos' for player position
    static char buf[ST_MSGWIDTH];

    sprintf(buf, "ang=0x%x;x,y=(0x%x,0x%x)",
            players[consoleplayer].mo->angle,
            players[consoleplayer].mo->x,
            players[consoleplayer].mo->y);
            CO_AddConsoleMessage(buf);
    return false;
   }

/*
con_command_t cd_commands[] = { "on",     0, PlayCDMusic,       "starts cd playback",
                                "play",   0, PlayCDMusic,       "starts cd playback",
                                "pause",  0, PauseResumeMusic,  "pauses/resumes music playback",
                                "resume", 0, PauseResumeMusic,  "pauses/resumes music playback",
                                "off",    0, PlayMidiMusic,     "stops cd playback (restarts midi)",
                                "stop",   0, PlayMidiMusic,     "stops cd playback (restarts midi)",
                                "next",   0, PlayNextSong,      "plays next cut on CD",
                                "+",      0, PlayNextSong,      "plays next cut on CD",
                                "prev",   0, PlayPrevSong,      "plays previous cut on CD",
                                "-",      0, PlayPrevSong,      "plays previous cut on CD",
                                0, 0, 0, 0 };
*/

dboolean CDCommand(char *cmd)
   {
    /*int i;

    // Control the CD Player...
    for (i = 0; cd_commands[i].keyword != 0; i++)
       {
        if (cd_commands[i].keylength > 0)
           {
            if (D_strncasecmp(cd_commands[i].keyword, cmd, cd_commands[i].keylength) == 0)
               {
                cmd = CleanUpCommand(&cmd[cd_commands[i].keylength]);
                return(cd_commands[i].command(cmd));
               }
           }
        else
           {
            if (D_strcasecmp(cd_commands[i].keyword, cmd) == 0)
               {
                return(cd_commands[i].command(cmd));
               }
           }
       }
    if (isdigits(cmd) && (strlen(cmd) < 3))
        PlayCDTrack(atoi(cmd));
    return false;*/
   }

char *NewMapSyntax[] = { "Syntax: MAP EXMY (X = episode & Y = mission)",
                         "Syntax: MAP MAPXX (XX = map number)" };

dboolean LoadNewMap(char *cmd)
   {
    if (!netgame)
       {
        // 'map' change-level cheat
        int  epsd;
        int  map;

        if (gamemode == commercial)
           {
            if (strlen(cmd) != 5)
               {
                CO_AddConsoleMessage(NewMapSyntax[1]);
                return false;
               }
           }
        else
           {
            if (strlen(cmd) != 4)
               {
                CO_AddConsoleMessage(NewMapSyntax[0]);
                return false;
               }
           }
        if (strnicmp(cmd,"map",3) == 0)
           {
            if ((cmd[3] >= '0') && (cmd[3] <= '9') &&
                (cmd[4] >= '0') && (cmd[4] <= '9'))
               {
               }
            map = (cmd[3] - '0')*10 + cmd[4] - '0';
            epsd = 1;
           }
        else
        if (((cmd[0] == 'e') || (cmd[0] == 'E')) &&
            ((cmd[2] == 'm') || (cmd[2] == 'M')))
           {
            if ((cmd[1] >= '0') && (cmd[1] <= '9'))
               {
                epsd = cmd[1] - '0';
               }
            else
               {
                epsd = 0;
               }
            if ((cmd[3] >= '0') && (cmd[3] <= '9'))
               {
                map = cmd[3] - '0';
               }
            else
               {
                map = 0;
               }
           }
        else
           {
            if (gamemode == commercial)
               {
                CO_AddConsoleMessage(NewMapSyntax[1]);
               }
            else
               {
                CO_AddConsoleMessage(NewMapSyntax[0]);
               }
            return false;
           }
        // Catch invalid maps.
        if (W_CheckNumForName(cmd) == -1)
           {
            CO_AddConsoleMessage("Invalid map name.");
            return false;
           }

        // So be it.
        CO_AddConsoleMessage(STSTR_CLEV);
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
        G_DeferedInitNew(gameskill, epsd, map);
       }
    return false;
   }

dboolean ChangeLevel(char *cmd)
   {
    char mapname[10];

    if (!netgame)
       {
        // 'clev' change-level cheat
        char buf[3];
        int  i, epsd;
        int  map;
      
        strcpy(buf, cmd);
      
        epsd = 0;
        map = 0;

        if (strlen(cmd) == 0)
           {
            if (gamemode == commercial)
               {
                CO_AddConsoleMessage("Syntax: IDCLEV XX or X (XX/X = map number)");
               }
            else
               {
                CO_AddConsoleMessage("Syntax: IDCLEV EM (E = episode, M = mission)");
               }
            return false;
           }

        if (gamemode == commercial)
           {
            epsd = 1;
            if (strlen(cmd) > 1)
               {
                if ((cmd[0] >= '0') && (cmd[0] <= '9') &&
                    (cmd[1] >= '0') && (cmd[1] <= '9'))
                   {
                    map = (cmd[0] - '0')*10 + cmd[1] - '0';
                   }
                else
                   {
                    map = 0;
                   }
               }
            else
               {
                if ((cmd[0] >= '0') && (cmd[0] <= '9'))
                   {
                    map = cmd[0] - '0';
                   }
                else
                   {
                    map = 0;
                   }
               }
           }
        else
           {
            if (strlen(cmd) < 2)
               {
                CO_AddConsoleMessage("No Map");
               }
            else
               {
                if ((cmd[0] >= '0') && (cmd[0] <= '9'))
                   {
                    epsd = cmd[0] - '0';
                   }
                else
                   {
                    epsd = 0;
                   }
                i = strlen(cmd) - 1;
                if ((cmd[i] >= '0') && (cmd[i] <= '9'))
                   {
                    map = cmd[i] - '0';
                   }
                else
                   {
                    map = 0;
                   }
               }
           }
        if (gamemode == commercial)
           {
            sprintf(mapname, "MAP%02d", map);
           }
        else
           {
            sprintf(mapname, "E%dM%d", epsd, map);
           }
        // Catch invalid maps.
        if (W_CheckNumForName(mapname) == -1)
           {
            CO_AddConsoleMessage("Invalid map requested.");
            return false;
           }

        // So be it.
        CO_AddConsoleMessage(STSTR_CLEV);
        totalscore = 0; keepscore = 0; showscoreHUD = 0;
        G_DeferedInitNew(gameskill, epsd, map);
       }
    return false;
   }


dboolean BindCommand(char *cmd)
   {
    char *ts;
    int   i, sc;

    ts = strtok(cmd, " ");
    for (i = 0; ts != NULL && cvars[i].name[0]; i++)
       {
        if (cvars[i].cv_type != cv_scan)
            continue;
        if (D_strcasecmp(ts, cvars[i].name) == 0)
           {
            ts = strtok(NULL, " ");
            if (ts != NULL)
               {
                static char buf[ST_MSGWIDTH];
                for (sc = 0; scanname[sc][0] != '-'; sc++)
                   {
                    if (!scanname[sc][0])
                       continue;
                    if (D_strcasecmp(ts, scanname[sc]) == 0)
                       {
                        sprintf(buf, "CMD %s BOUND TO SCANCODE %3d\n", cvars[i].name, sc);
                        CO_AddConsoleMessage(buf);
                        *cvars[i].var = sc;
                        break;
                       }
                   }
                if (scanname[sc][0] == '-')
                    CO_AddConsoleMessage("BIND ERROR: INVALID KEY VALUE");
               }
            break;
           }
       }
    if (!cvars[i].name[0])
        CO_AddConsoleMessage("BIND ERROR: INVALID CVAR");
    return false;
   }

// This command list will "go away" after the new command code is finished

char *cmdlist[] = { "iddqd : god mode",
                    "idfa : full ammo and all weapons",
                    "idkfa : keys full ammo and all weapons",
                    "idclev : change level",
                    "idchoppers : chainsaw and invulnerability",
                    "idclip : no clipping toggle",
                    "idspispopd : no clipping toggle",
                    "idbeholdi : invisibility",
                    "idbeholdv : invulnerability",
                    "idbeholds : strength (berserk pack)",
                    "idbeholdr : radiation/environment suit",
                    "idbeholdl : light amplification goggles",
                    "idbeholda : all map cheat",
                    "cd on : starts your cd player (switches from midi)",
                    "cd off : stops your cd player (switches to midi)",
                    "cd pause : pause/resume cd music toggle",
                    "cd resume : pause/resume cd music toggle",
                    "cd next : play next song on cd",
                    "cd + : play next song on cd",
                    "cd prev : play previous song on cd",
                    "cd - : play previous song on cd",
                    "cd XX : plays song number specified in XX 00 to 99",
                    "midi pause : pauses/resumes the midi playback (toggle)",
                    "midi resume : pauses/resumes the midi playback (toggle)",
                    "quit : quits the game (immediately)",
                    "god : enable god mode (degreelessness?) toggle",
                    "map : e?m? or map?? (Doom I or Doom II map names)",
                    "noclip : no clipping toggle",
                    "give all : gives full ammo,armor,health,weapons,keys",
                    "give ammo : gives full ammo",
                    "give armor : gives full armor",
                    "give health : give 100% health",
                    "give weapons : gives all weapons",
                    "give keys : gives all keys",
                    "give backpack : gives ammo backpack",
                    "autorun X : toggles always run 0 = off, 1 = on",
                    "swap_stereo X : toggles swap left/right 0 = off, 1 = on",
                    "bind : binds a value to a cvar",
                    "record : records a demo in new demo II format",
                    "finish : stops recording a demo II format demo",
                    "play : plays back a demo II format demo",
                    "cmdlist : lists console commands",
                    "cvarlist : lists cvars",
                    "" };


dboolean CommandList(char *cmd)
   {
    int i;

    for (i = 0; cmdlist[i][0] != '\0'; i++)
       CO_AddConsoleMessage(cmdlist[i]);
    return false;
   }

dboolean ConQuit(char *cmd)
   {
    I_Quit();
    return false;
   }

con_command_t con_commands[] = { "iddqd",      0, GodMode,      "god mode",
                                 "idfa",       0, FullAmmo,     "full ammo and weapons",
                                 "idkfa",      0, FullAmmoKeys, "full ammo, keys and weapons",
                                 "idspispopd", 0, NoClipping,   "no clipping toggle",
                                 "idclip",     0, NoClipping,   "no clipping toggle",
                                 "idchoppers", 0, Choppers,     "god mode and get the chainsaw",
                                 "idmus",      5, ChangeMusic,  "change selected music",
                                 "idbehold",   8, GetPowerUp,   "enable a powerup",
                                 "god",        0, GodMode,      "god mode",
                                 "noclip",     0, NoClipping,   "no clipping toggle",
                                 "help",       4, ConHelp,      "console command help",
                                 "quit",       0, ConQuit,      "quit the game",
                                 "give",       4, GiveItems,    "give the requested item",
                                 "midi",       4, MidiCommand,  "control midi playback",
                                 "mypos",      0, ShowPosition, "show current player position",
                                 "cd",         2, CDCommand,    "control CD music",
                                 "map ",       4, LoadNewMap,   "change to new level",
                                 "idclev",     0, ChangeLevel,  "change to new level",
                                 "idclev",     6, ChangeLevel,  "change to new level",
                                 "video",      5, ResizeMainWindow, "change video mode or window size",
                                 "bind",       4, BindCommand,  "change command bindings",
                                 0, 0, 0, 0 };

/* 
  console command information ends here
*/

unsigned char testcmd[] = { c_fire, c_forward, c_slideleft, c_slideleft, c_slideleft, c_forward, c_fire, c_end };

Alias_t aliases[256];


void CO_Ticker()
   {
    static int lasttics = 0;
    int        gametics, newtics;

    if (bConsoleActive == false)
       return;

    gametics = I_GetTime();
    if ((lasttics == 0) || (iConsoleMode == asleep))
       {
        newtics = 1;
        iConsoleMode = waking;
       }
    else
        newtics = gametics - lasttics;
    lasttics = gametics;

    switch(iConsoleMode)
       {
        case sleepy:
             if (iConsoleHeight > 0)
                 iConsoleHeight -= (iConsoleSpeed * 2 * newtics);
             else
                {
                 iConsoleMode = asleep;
                 iCCursor = 0;
                 bConsoleActive = false;
                 if (scaledviewwidth != SCREENWIDTH)
                     R_DrawViewBorder();
                }
             if (iConsoleHeight < 0)
                iConsoleHeight = 0;
             break;
        case waking:
             if (iConsoleHeight < iConsoleMax)
                 iConsoleHeight += (iConsoleSpeed * newtics);
             else
                {
                 iConsoleMode = awake;
                }
             if (iConsoleHeight > iConsoleMax)
                iConsoleHeight = iConsoleMax;
             break;
        case awake:
             // Do whatever might need to be done to an active console here...
             // Animations?
             break;
       }
   }

dboolean CO_Responder(event_t* ev)
   {
    int           i;
    unsigned char c, *szCmd;
    static dboolean bDisplayed = false;
    static dboolean bShiftState = false;

    if ((ev->type == ev_keydown) && (ev->data1 == SDL_SCANCODE_F1))
       {
        if (bConsoleActive == true)
           {
            iConsoleMode = sleepy;
            paused = false;
            return true;
           }
        else
           {
            bConsoleActive = true;
            if ((netgame == false) && (gamestate == GS_LEVEL) && (demoplayback == false))
                paused = true;
            for (i = 0; szCommand[i]; i++)
               szCommand[i] = '\0';
            iCCursor = 0;
            return false;
           }
       }

    if ((ev->type == ev_keydown) && (ev->data1 == SDL_SCANCODE_ESCAPE))
       {
        if (bConsoleActive == true)
           {
            iConsoleMode = asleep;
            bConsoleActive = false;
            return false;
           }
       }

    if (bConsoleActive == false)
       return false;

    if ((ev->type == ev_keydown) && ((ev->data1 == SDL_SCANCODE_RSHIFT) || (ev->data1 == SDL_SCANCODE_LSHIFT)))
       {
        bShiftState = true;
        return false;
       }

    if ((ev->type == ev_keyup) && ((ev->data1 == SDL_SCANCODE_RSHIFT) || (ev->data1 == SDL_SCANCODE_LSHIFT)))
       {
        bShiftState = false;
        return false;
       }

    if (ev->type != ev_keydown)
       return false;

    if (ev->type == ev_keydown)
       {
        switch(ev->data1)
           {
            /*case KEY_SCRNSHOT:
                 G_ScreenShot();
                 break;*/
            case SDL_SCANCODE_BACKSPACE:
            case SDL_SCANCODE_LEFT:
                 if (iCCursor > 0)
                    {
                     iCCursor--;
                     szCommand[iCCursor] = '\0';
                    }
                 break;
            case SDL_SCANCODE_RETURN:
                 if (iCCursor > 0)
                    {
                     szCmd = strtok(szCommand, ";");
                     while (szCmd != NULL)
                        {
                         szCmd = CleanUpCommand(szCmd);
                         CO_AddConsoleMessage(szCmd);
                         CO_HandleCommand(szCmd);
                         szCmd = strtok(NULL, ";");
                        }
                     //CO_AddConsoleMessage(szCommand);
                     //CO_HandleCommand(szCommand);
                     for (i = 0; i < CMDLENGTH; i++)
                        szCommand[i] = '\0';
                     iCCursor = 0;
                     bDisplayed = false;
                     break;
                    }
            case SDL_SCANCODE_TAB:
                 return false;
            default:
                 if (iCCursor < CMDLENGTH)
                    {
                     if (bShiftState == true)
                         c = scan2chars[ev->data1];
                     else
                         c = scan2char[ev->data1];
                     if ((c >= 32) && (c <= 127))
                        {
                         szCommand[iCCursor] = c;
                         iCCursor++;
                        }
                    }
                 break;
           }
        if ((D_strcasecmp(szCommand, "idbehold") == 0) && (bDisplayed == false))
           {
            CO_AddConsoleMessage(STSTR_BEHOLD);
            bDisplayed = true;
           }
       }

    return true;
   }

void CO_Init()
   {
    int i;

    iConsoleMax = SCREENHEIGHT/2;

    // Load the main console background "flat"
    pconstex = (byte *)W_CacheLumpName(CONSOLE_FLAT, PU_CACHE);
    memcpy(consback, pconstex, 64*64);

    memcpy(TexRaw, consback, 64*64);
    for (i = 0; i < (64*64); i++)
       TexAlpha[i] = GLD_COLORED;
    TexWide = 64;
    TexHigh = 64;
    iConsBack = MakeRGBATexture(false, false, 64, 64);
    ConsBack.TexName = iConsBack;
    ConsBack.Width = 64.0f;
    ConsBack.Height = 64.0f;
    ConsBack.XDisp = 1.0f;
    ConsBack.YDisp = 0.0f;

    // Load the console border "flat"
    pconstex = (byte *)W_CacheLumpName(CONSOLE_BORD, PU_CACHE);
    memcpy(consbord, pconstex, 64*64);
    memcpy(TexRaw, consbord, 64*64);
    for (i = 0; i < (64*64); i++)
       TexAlpha[i] = GLD_COLORED;
    TexWide = 64;
    TexHigh = 64;
    iConsBord = MakeRGBATexture(false, false, 64, 64);
    ConsBord.TexName = iConsBord;
    ConsBord.Width = 64.0f;
    ConsBord.Height = 64.0f;
    ConsBord.XDisp = 1.0f;
    ConsBord.YDisp = 0.0f;

    // Load the game logo "patch"
    consname = (patch_t *)W_CacheLumpName(CONSOLE_NAME, PU_STATIC);
    iConsLogo = GL_MakeSpriteTexture(consname, &ConsLogo, false);

    iFontHigh = hu_font[0]->height;
    sprintf(szVersion, "V%d.%02d%c", version/100,version%100,revision);
    iNamePosX = SCREENWIDTH-(CO_StringWidth(szProgName)+1);
    iVerPosX = SCREENWIDTH-(CO_StringWidth(szVersion)+1);
    for (i = 0; i < CONSMSGS; i++)
       consmsgs[i][0] = '\0';
    plyr = &players[consoleplayer];
    aliases[0].command = testcmd;
   }

void CO_Drawer()
   {
    int h, v, yoff, d, s, i, j, clines, mline;
    static char tstr[32];
    static int  cursor = 0;
    int         curpos;

    cursor++;
    cursor %= 18;

    if (bConsoleActive == false)
       return;

    if (scaledviewwidth != SCREENWIDTH)
        R_DrawViewBorder();

    // 64 is the height of a "flat"
    // flats are 64x64 bitmaps
    for (v = 0; v < (iConsoleHeight-CONSBORD); v++)
       {
        yoff = v * SCREENWIDTH;
        i = v % 64;
        for (h = 0; h < SCREENWIDTH; h++)
           {
            s = (i * 64)+(h % 64);
            d = yoff + h;
            screens[0][d] = consback[s];
           }
       }
    for (j = (64-CONSBORD)/2; v < iConsoleHeight; v++, j++)
       {
        yoff = v * SCREENWIDTH;
        i = v % 64;
        for (h = 0; h < SCREENWIDTH; h++)
           {
            s = (j * 64)+(h % 64);
            d = yoff + h;
            screens[0][d] = consbord[s];
           }
       }

    if (iConsoleHeight >= (consname->height+CONSBORD+2))
       {
        // Draw the Game logo (Doom, Doom II, TNT, Plutonia, whatever)
        V_DrawPatch(((SCREENWIDTH-consname->width)/2)-3, 2, 0, consname);
       }

    if (iConsoleHeight > 10)
       {
        clines = ((iConsoleHeight-(CONSBORD+iFontHigh+3))/(iFontHigh+1));
        M_WriteText(iNamePosX,1,szProgName);
        M_WriteText(iVerPosX,iConsoleHeight-(CONSBORD+iFontHigh+1),szVersion);
        curpos = CO_WriteConsoleLine(clines, szCommand, true);
        if (cursor/9)
            M_WriteText(curpos,clines*(iFontHigh+1),"_");
        clines = ((iConsoleHeight-(CONSBORD+iFontHigh+3))/(iFontHigh+1));
        mline = iConsHead - clines;
        if (mline < 0)
           mline += CONSMSGS;
        for (i = 0; i < clines; i++)
           {
            CO_WriteConsoleLine(i, consmsgs[mline], true);
            mline++;
            mline %= CONSMSGS;
           }
       }
   }

int M_GLDrawText( int x, int y, char *string );
extern float SetBack;

void GL_DrawConsole()
   {
    int h, v, yoff, d, s, i, j, clines, mline;
    static char tstr[32];
    static int  cursor = 0;
    int         curpos;
    float       talpha;

    if (bConsoleActive == false)
       return;

    cursor++;
    cursor %= 18;

    glEnable(GL_TEXTURE_2D);

    GLConsoleHeight = 118.0f-(120.0f*((float)iConsoleHeight/(float)iConsoleMax));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    talpha = (float)iConsoleHeight/(float)iConsoleMax;
    //glColor4f( 1.0f, 1.0f, 1.0f, (float)iConsoleHeight/(float)iConsoleMax );
    glColor4f( talpha, talpha, talpha, talpha );
    glBindTexture(GL_TEXTURE_2D, iConsBack);
    glBegin( GL_QUADS );
      glNormal3f( 0.0f, 0.0f, 1.0f);
      glTexCoord2f(  0.0f, 3.75f );
      glVertex3f( -160.00f, 120.0f, SetBack+2);
      glTexCoord2f(  0.0f, 0.0f );
      glVertex3f( -160.00f, GLConsoleHeight, SetBack+2);
      glTexCoord2f( 10.0f, 0.0f );
      glVertex3f(  160.00f, GLConsoleHeight, SetBack+2);
      glTexCoord2f( 10.0f, 3.75f );
      glVertex3f(  160.00f, 120.0f, SetBack+2);
    glEnd();

    //glColor4f( 1.0f, 1.0f, 1.0f, (float)iConsoleHeight/(float)iConsoleMax );
    glBindTexture(GL_TEXTURE_2D, iConsBord);
    glBegin( GL_QUADS );
      glNormal3f( 0.0f, 0.0f, 1.0f);
      glTexCoord2f( 0.0f, 0.55f );
      glVertex3f( -160.00f, GLConsoleHeight+2.0f, SetBack+2);
      glTexCoord2f( 0.0f, 0.45f );
      glVertex3f( -160.00f, GLConsoleHeight, SetBack+2);
      glTexCoord2f( 10.0f, 0.45f );
      glVertex3f(  160.00f, GLConsoleHeight, SetBack+2);
      glTexCoord2f( 10.0f, 0.55f );
     glVertex3f(  160.00f, GLConsoleHeight+2.0f, SetBack+2);
    glEnd();

    glDisable(GL_BLEND);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    if (iConsoleHeight >= (consname->height+CONSBORD+2))
       {
        // Draw the Game logo (Doom, Doom II, TNT, Plutonia, whatever)
        glBindTexture(GL_TEXTURE_2D, iConsLogo);
        glBegin( GL_QUADS );
          glNormal3f( 0.0f, 0.0f, 1.0f);
          glTexCoord2f( 0.0f, 1.0f );
          glVertex3f( 0.0f-(ConsLogo.Width/2.0f), 118.0f, SetBack);
          glTexCoord2f( 0.0f, ConsLogo.YDisp );
          glVertex3f( 0.0f-(ConsLogo.Width/2.0f), 118.0f-ConsLogo.Height, SetBack);
          glTexCoord2f( ConsLogo.XDisp, ConsLogo.YDisp );
          glVertex3f(  0.0f+(ConsLogo.Width/2.0f), 118.0f-ConsLogo.Height, SetBack);
          glTexCoord2f( ConsLogo.XDisp, 1.0f );
          glVertex3f(  0.0f+(ConsLogo.Width/2.0f), 118.0f, SetBack);
        glEnd();
       }

    if (iConsoleHeight > 10)
       {
        M_GLDrawText(iNamePosX,1,szProgName);
        M_GLDrawText(iVerPosX,(iConsoleHeight-(CONSBORD+iFontHigh+1))/.825f,szVersion);
        clines = ((iConsoleHeight-(CONSBORD+iFontHigh+3))/(iFontHigh+1));
        curpos = CO_GLWriteConsoleLine(clines, szCommand, true);
        if (cursor/9)
            M_GLDrawText(curpos,(clines*(iFontHigh+1))/.825f,"_");
        mline = iConsHead - clines;
        if (mline < 0)
           mline += CONSMSGS;
        for (i = 0; i < clines; i++)
           {
            CO_GLWriteConsoleLine(i, consmsgs[mline], true);
            mline++;
            mline %= CONSMSGS;
           }
       }

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
   }

// This function should return the width of the string in pixels.
int CO_StringWidth(char *s)
   {
    int           i, j;
    unsigned char c;

    j = i = 0;
    while (s[i])
       {
        c = toupper(s[i]-HU_FONTSTART);
        if ((c < 0) || (c >= HU_FONTSIZE))
           j += 4;
        else
           j += hu_font[c]->width;
        i++;
       }
    return j;
   }

// This function should return the first character in the string that will
// allow the string to be printed from there to the end.
int CO_StringPrintable(char *s)
   {
    int           i, j;
    unsigned char c;

    j = SCREENWIDTH-(CO_StringWidth(s)+5);
    i = 0;
    while (j < 0)
       {
        c = toupper(s[i]-HU_FONTSTART);
        if ((c < 0) || (c >= HU_FONTSIZE))
           j += 4;
        else
           j += hu_font[c]->width;
        i++;
       }
    return i;
   }

int CO_WriteConsoleLine(int l, char *s, dboolean wrap)
   {
    int  b;
    dboolean lwrap;

    lwrap = wrap;
    if (lwrap == true)
       {
        if (CO_StringWidth(s) < SCREENWIDTH)
           lwrap = false;
        else
           b = CO_StringPrintable(s);
       }

    if (lwrap == false)
       {
        M_WriteText(1,(iFontHigh+1)*l,s);
        return CO_StringWidth(s);
       }
    else
       {
        M_WriteText(SCREENWIDTH-(CO_StringWidth(&s[b])+5),(iFontHigh+1)*l,&s[b]);
        return CO_StringWidth(&s[b]);
       }
   }

int CO_GLWriteConsoleLine(int l, char *s, dboolean wrap)
   {
    int  b;
    dboolean lwrap;

    lwrap = wrap;
    if (lwrap == true)
       {
        if (CO_StringWidth(s) < SCREENWIDTH)
           lwrap = false;
        else
           b = CO_StringPrintable(s);
       }

    if (lwrap == false)
       {
        M_GLDrawText(1,((iFontHigh+1)*l)/.825f,s);
        return CO_StringWidth(s);
       }
    else
       {
        M_GLDrawText(SCREENWIDTH-(CO_StringWidth(&s[b])+5),((iFontHigh+1)*l)/.825f,&s[b]);
        return CO_StringWidth(&s[b]);
       }
   }

void CO_AddConsoleMessage(char *s)
   {
    strncpy(consmsgs[iConsHead], s, 128);
    if (strlen(s) > 128)
       consmsgs[iConsHead][128] = '\0';
    iConsHead++;
    iConsHead %= CONSMSGS;
   }

int CO_HandleCommand(char *cmd)
   {
    int    i, sc;
    char  *ts;

    // b. - enabled for more debug fun.
    // if (gameskill != sk_nightmare) {

    for (i = 0; con_commands[i].keyword != 0; i++)
       {
        if (con_commands[i].keylength > 0)
           {
            if (D_strncasecmp(con_commands[i].keyword, cmd, con_commands[i].keylength) == 0)
               {
                cmd = CleanUpCommand(&cmd[con_commands[i].keylength]);
                return(con_commands[i].command(cmd));
               }
           }
        else
           {
            if (D_strcasecmp(con_commands[i].keyword, cmd) == 0)
               {
                return(con_commands[i].command(cmd));
               }
           }
       }
    
    if (strnicmp(cmd, "record ", 7) == 0)
       {
        static char buf[ST_MSGWIDTH];
        if (strlen(&cmd[7]) > 0)
           {
            G_RecordDemo_II(&cmd[7]);
            sprintf(buf, "RECORDING DEMO II: %s", &cmd[7]);
            CO_AddConsoleMessage(buf);
            G_BeginRecording_II();
            bConsoleActive = false;
            paused = false;
           }
        return false;
       }

    if (D_strcasecmp(cmd, "finish") == 0)
       {
        static char buf[ST_MSGWIDTH];
        G_EndDemo_II();
        sprintf(buf, "DEMO II SAVED");
        CO_AddConsoleMessage(buf);
        return false;
       }

    if (strnicmp(cmd, "play ", 5) == 0)
       {
        static char buf[ST_MSGWIDTH];
        if (strlen(&cmd[7]) > 0)
           {
            if (G_DeferedPlayDemo_II(&cmd[5]) == true)
               {
                sprintf(buf, "PLAYING DEMO II: %s", &cmd[5]);
                CO_AddConsoleMessage(buf);
                bConsoleActive = false;
                paused = false;
               }
            else
               {
                sprintf(buf, "DEMO II [%s.dem] - NO FILE.", &cmd[5]);
                CO_AddConsoleMessage(buf);
               }
           }
        return false;
       }

    if (D_strcasecmp(cmd, "cmdlist") == 0)
       {
        i = 0;
        while (cmdlist[i][0] != '\0')
            CO_AddConsoleMessage(cmdlist[i++]);
        return false;
       }

    if (D_strcasecmp(cmd, "cvarlist") == 0)
       {
        static char buf[128];
        i = 0;
        while (cvars[i].name[0] != '\0')
           {
            sprintf(buf, "%s - %d", cvars[i].name, *cvars[i].var);
            CO_AddConsoleMessage(buf);
            i++;
           }
        return false;
       }

    ts = strtok(cmd, " ");
    for (i = 0; ts != NULL && cvars[i].name[0]; i++)
       {
        if ((cvars[i].cv_type != cv_value) && (cvars[i].cv_type != cv_string))
            continue;
        if (D_strcasecmp(ts, cvars[i].name) == 0)
           {
            static char buf[ST_MSGWIDTH];
            ts = strtok(NULL, " ");
            if (ts != NULL)
               {
                if (cvars[i].cv_type == cv_value)
                   {
                    if (ts[0] >= '0' && ts[0] <= ('0'+cvars[i].maxval))
                       {
                        *cvars[i].var = ts[0] - '0';
                        sprintf(buf, "%s SET TO %d\n", cvars[i].name, *cvars[i].var);
                        CO_AddConsoleMessage(buf);
                       }
                    else
                        CO_AddConsoleMessage("ERROR: INVALID VALUE");
                   }
                else
                   {
                    strncpy(cvars[i].var, ts, cvars[i].maxval);
                    cvars[i].var[cvars[i].maxval] = '\0';
                    sprintf(buf, "%s SET TO '%s'\n", cvars[i].name, cvars[i].var);
                    CO_AddConsoleMessage(buf);
                   }
               }
            else
               {
                sprintf(buf, "%s VALUE %d\n", cvars[i].name, *cvars[i].var);
                CO_AddConsoleMessage(buf);
               }
            break;
           }
       }
    if (!cvars[i].name[0])
        CO_AddConsoleMessage("ERROR: INVALID CVAR");

    return false;
   }

dboolean isdigits(char *s)
   {
    int i;

    for (i = 0; s[i]; i++)
       if (!isdigit(s[i]) && s[i] != '.')
           return false;
    return true;
   }