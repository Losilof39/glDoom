// win_kbrd.c
//
// This module handles the DirectInput startup code and initializes
// the keyboard interface as well as handling the keyboard input
// and shutting down the keyboard interface and DirectInput.

#include <stdio.h>
#include <SDL.h>

#include "doomtype.h"
#include "d_main.h"
#include "d_event.h"
#include "sys_sdl.h"

extern dboolean keystates[256];
unsigned char        KeyState[256]; // current keys states
short                si_Kbd[256];   // previous keys states

extern int           keylink;

void lfprintf(char *message, ... );

void I_ReleaseKeyboard()
   {
   }

void I_SetupKeyboard()
{
    int k;
    // Set the keyboard buffer to "all keys up"
    for (k = 0; k < 256; k++)
#if SDL_MAJOR_VERSION == 3
        si_Kbd[k] = SDL_EVENT_KEY_UP;
#else
        si_Kbd[k] = SDL_KEYUP;
#endif
}

void I_CheckKeyboard()
{
    static  event_t  event[256];
    int i;
    for (i = 1; i < 256; i++)
    {
#if SDL_MAJOR_VERSION == 3
        // key released
        if (!keystates[i] && (si_Kbd[i] == SDL_EVENT_KEY_DOWN))
#else
        // key released
        if (!keystates[i] && (si_Kbd[i] == SDL_KEYDOWN))
#endif
        {
            event[i].type = ev_keyup;
            event[i].data1 = i;
            D_PostEvent(&event[i]);
#if SDL_MAJOR_VERSION == 3
            si_Kbd[i] = SDL_EVENT_KEY_UP;
#else
            si_Kbd[i] = SDL_KEYUP;
#endif
        }
#if SDL_MAJOR_VERSION == 3
        // key pressed
        if (keystates[i] && (si_Kbd[i] == SDL_EVENT_KEY_UP))
#else
        // key pressed
        if (keystates[i] && (si_Kbd[i] == SDL_KEYUP))
#endif
        {
            event[i].type = ev_keydown;
            event[i].data1 = i;
            D_PostEvent(&event[i]);
#if SDL_MAJOR_VERSION == 3
            si_Kbd[i] = SDL_EVENT_KEY_DOWN;
#else
            si_Kbd[i] = SDL_KEYDOWN;
#endif
        }
    }
}
