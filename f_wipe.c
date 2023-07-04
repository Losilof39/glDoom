/* Emacs style mode select   -*- C -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *      Mission begin melt/wipe screen special effect.
 *
 *-----------------------------------------------------------------------------
 */

#include "z_zone.h"
#include "doomdef.h"
#include "v_video.h"
#include "m_random.h"
#include "f_wipe.h"

 //
 // SCREEN WIPE PACKAGE
 //

 // Parts re-written to support true-color video modes. Column-major
 // formatting removed. - POPE


// e6y: resolution limitation is removed
static int* y_lookup = NULL;
extern dboolean wasWiped;
extern video_t video;

// e6y: resolution limitation is removed
void R_InitMeltRes(void)
{
    if (y_lookup) 
        free(y_lookup);

    y_lookup = calloc(1, video.width * sizeof(*y_lookup));
}

static int wipe_initMelt(int ticks)
{
    int i;

    // setup initial column positions (y<0 => not ready to scroll yet)
    y_lookup[0] = -(M_Random() % 16);
    for (i = 1; i <= video.width; i++)
    {
        int r = (M_Random() % 3) - 1;
        y_lookup[i] = y_lookup[i - 1] + r;
        if (y_lookup[i] > 0)
            y_lookup[i] = 0;
        else
            if (y_lookup[i] == -16)
                y_lookup[i] = -15;
    }
    return 0;
}

static int wipe_doMelt(int ticks)
{
    dboolean done = true;
    int i;

    while (ticks--) {
        for (i = 0; i <= (video.width); i++) {
            if (y_lookup[i] < 0) {
                y_lookup[i]++;
                done = false;
                continue;
            }
            if (y_lookup[i] < video.height) {
                int dy;

                /* cph 2001/07/29 -
                  *  The original melt rate was 8 pixels/sec, i.e. 25 frames to melt
                  *  the whole screen, so make the melt rate depend on SCREENHEIGHT
                  *  so it takes no longer in high res
                  */
                dy = (y_lookup[i] < 16) ? y_lookup[i] + 1 : video.height / 25;
                if (y_lookup[i] + dy >= video.height)
                    dy = video.height - y_lookup[i];

                y_lookup[i] += dy;
                done = false;
            }
        }
    }

    gld_wipe_doMelt(ticks, y_lookup);

    return done;
}

// CPhipps - modified to allocate and deallocate screens[2 to 3] as needed, saving memory

static int wipe_exitMelt(int ticks)
{

    gld_wipe_exitMelt(ticks);
    return 0;
}

int wipe_StartScreen(void)
{
    if(wasWiped) return 0;//e6y
    wasWiped = true;//e6y

    gld_wipe_StartScreen();
    return 0;
}

int wipe_EndScreen(void)
{
    if(!wasWiped) return 0;//e6y
    wasWiped = false;//e6y

    gld_wipe_EndScreen();
    return 0;
}

// killough 3/5/98: reformatted and cleaned up
int wipe_ScreenWipe(int ticks)
{
    static dboolean go;                               // when zero, stop the wipe
    if (!go)                                         // initial stuff
    {
        go = 1;
        wipe_initMelt(ticks);
    }
    // do a piece of wipe-in
    if (wipe_doMelt(ticks))     // final stuff
    {
        wipe_exitMelt(ticks);
        go = 0;
    }
    return !go;
}
