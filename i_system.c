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
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdarg.h>
#include <time.h>
//#include <unistd.h>
#include <windows.h>
#include <mmsystem.h>

#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"

#include "sdl_inpt.h"

#include "d_net.h"
#include "g_game.h"
#include "m_music.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"
#include "sys_sdl.h"

extern windata_t WinData;

int	mb_used = 6;

void lfprintf(char *message, ... );

void
I_Tactile
( int	on,
  int	off,
  int	total )
{
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
    *size = mb_used*1024*1024;
    return (byte *) malloc (*size);
}



//
// I_GetTime
// returns time in 1/70th second tics
//
int  I_GetTime (void)
{

//    struct timeval	tp;
//    struct timezone	tzp;
    int			newtics;
    DWORD       currtime;
    //static int		basetime=0;
    static DWORD    basetime = 0;
  
//    gettimeofday(&tp, &tzp);
//    if (!basetime)
//	basetime = tp.tv_sec;
//    newtics = (tp.tv_sec-basetime)*TICRATE + tp.tv_usec*TICRATE/1000000;
    currtime = timeGetTime();
    if (!basetime)
       basetime = currtime;
    newtics = ((currtime-basetime)/(1000/TICRATE));
    return newtics;
}



//
// I_Init
//
void I_Init (void)
{
// FIXME
    I_InitSound();
    GetCDInfo(WinData.hWnd);
    I_InitInputs();
    //  I_InitGraphics();
}

//
// I_Quit
//
void glDoomExit(void);
extern int GameMode;

#define GAME_QUIT   5

void I_DeferQuit(void)
   {
    GameMode = GAME_QUIT;
   }

void I_Quit(void)
   {
    lfprintf("Exiting glDoom...\n");
    D_QuitNetGame();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults();
    //I_ShutdownGraphics();
    glDoomExit();
    exit(0);
   }

void I_WaitVBL(int count)
{
/* FIXME
#ifdef SGI
    sginap(1);                                           
#else
#ifdef SUN
    sleep(0);
#else
    usleep (count * (1000000/70) );                                
#endif
#endif
*/
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte*	I_AllocLow(int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}


//
// I_Error
//
extern dboolean demorecording;
extern int GameMode;
#define GAME_QUIT 5

void I_Error (char *error, ...)
{
    va_list	argptr;
    char    MsgText[512];

    if (GameMode == GAME_QUIT)
       return;

    // Message first.
    va_start (argptr,error);
    //fprintf (stderr, "Error: ");
    lfprintf("Error: ");
    //vfprintf (stderr,error,argptr);
    vsprintf (MsgText,error,argptr);
    lfprintf(MsgText);
    //fprintf (stderr, "\n");
    lfprintf("\n");
    va_end (argptr);

    //fflush( stderr );

    // Shutdown. Here might be other errors.
    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame();

    I_Quit();

    //I_ShutdownGraphics();
    
    GameMode = GAME_QUIT;
    //exit(-1);
}
