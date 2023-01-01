// win_inpt.c
// 
// This module starts up and handles all
// keyboard, mouse and joystick inputs.
// This module is for Win32

#include "thirdparty/SDL2/include/SDL.h"
#include "sys_sdl.h"
#include "gconsole.h"
#include "sdl_kbrd.h"
#include "d_event.h"

dboolean         joystickavail = false;
dboolean         mouseavail    = false;

extern int       usejoystick;
extern int       usemouse;

Uint8* keystates[256] = { false };

extern dboolean      bQuit;
extern int keylink;

void lfprintf(char *message, ... );

dboolean I_InitInputs(void)
   {
    if (!I_SetupKeyboard())
       {
        return false;
       }
    /*if ((usejoystick) && (!joystickavail))
       {
        if (I_SetupJoysticks())
           {
            joystickavail = true;
           }
        else
           {
            con_printf("Joystick not detected...\n");
           }
       }
    if ((usemouse) && (!mouseavail))
       {
        if (I_SetupMouse())
           {
            mouseavail = true;
           }
        else
           {
            con_printf("Mouse initialization failed...\n");
           }
       }*/
    return true;
   }

void I_CheckInputs(void)
   {
    event_t event;
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT:
        {
            bQuit = false;            
            break;
        }break;


        case SDL_KEYDOWN:
        {
            // catch first these 4 keys
            switch (ev.key.keysym.scancode)
            {
            case SDL_SCANCODE_PAUSE:
                event.type = ev_keyup;
                event.data1 = SDL_SCANCODE_PAUSE;
                D_PostEvent(&event);
                break;
            case SDL_SCANCODE_RSHIFT:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_RSHIFT;
                    D_PostEvent(&event);
                }
                break;
            case SDL_SCANCODE_RCTRL:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_RCTRL;
                    D_PostEvent(&event);
                }
                break;
            case SDL_SCANCODE_MENU:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_MENU;
                    D_PostEvent(&event);
                }
                break;
            default:
                break;
            }

            // catch the other keys
            keystates[ev.key.keysym.scancode] = 1;
        }break;
        
        case SDL_KEYUP:
        {
            // catch first these 4 keys
            switch (ev.key.keysym.scancode)
            {
            case SDL_SCANCODE_PAUSE:
                event.type = ev_keyup;
                event.data1 = SDL_SCANCODE_PAUSE;
                D_PostEvent(&event);
                break;
            case SDL_SCANCODE_RSHIFT:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_RSHIFT;
                    D_PostEvent(&event);
                }
                break;
            case SDL_SCANCODE_LCTRL:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_LCTRL;
                    D_PostEvent(&event);
                }
                break;
            case SDL_SCANCODE_MENU:
                if (keylink == true)
                {
                    event.type = ev_keyup;
                    event.data1 = SDL_SCANCODE_MENU;
                    D_PostEvent(&event);
                }
                break;
            }

            // catch the other keys
            keystates[ev.key.keysym.scancode] = 0;
        }break;

        default:
            break;
        }
    }

    /*if ((usejoystick) && (joystickavail))
       {
        I_CheckJoysticks();
       }*/
    /*if ((usemouse) && (mouseavail))
       {
        I_CheckMouse();
       }*/
    I_CheckKeyboard();
   }

void I_ShutdownInputs(void)
   {
    I_ReleaseKeyboard();
   }


void I_GetEvent(void)
   {
    I_CheckInputs();
   }

