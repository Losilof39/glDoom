// win_kbrd.c
//
// This module handles the DirectInput startup code and initializes
// the keyboard interface as well as handling the keyboard input
// and shutting down the keyboard interface and DirectInput.

#include <stdio.h>
#include "thirdparty/SDL2/include/SDL.h"

#include "doomtype.h"
#include "d_main.h"
#include "d_event.h"
#include "sys_sdl.h"

#undef RELEASE
#ifdef __cplusplus
#define RELEASE(x) if (x != NULL) {x->Release(); x = NULL;}
#else
#define RELEASE(x) if (x != NULL) {x->lpVtbl->Release(x); x = NULL;}
#endif

extern Uint8* keystates[256];
unsigned char        KeyState[256]; // current keys states
short                si_Kbd[256];   // previous keys states

extern int           keylink;


void lfprintf(char *message, ... );

void I_ReleaseKeyboard()
   {
   }

dboolean I_SetupKeyboard()
{
    // Set the keyboard buffer to "all keys up"
    for (int k = 0; k < 256; k++)
        si_Kbd[k] = SDL_KEYUP;

    return true;
}

char t_text[2048];

void I_CheckKeyboard()
{
    static  event_t  event[256];

    for (int i = 1; i < 256; i++)
    {
        // key released
        if (!keystates[i] && (si_Kbd[i] == SDL_KEYDOWN))
        {
            event[i].type = ev_keyup;
            event[i].data1 = i;
            D_PostEvent(&event[i]);
            si_Kbd[i] = SDL_KEYUP;
        }

        // key pressed
        if (keystates[i] && (si_Kbd[i] == SDL_KEYUP))
        {
            event[i].type = ev_keydown;
            event[i].data1 = i;
            D_PostEvent(&event[i]);
            si_Kbd[i] = SDL_KEYDOWN;
        }
    }
}
