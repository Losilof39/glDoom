// sdl_inpt.c
// 
// This module starts up and handles all
// keyboard, mouse and joystick inputs.
// This module is for Win32

#include "thirdparty/SDL2/include/SDL.h"
#include "sys_sdl.h"
#include "sdl_kbrd.h"
#include "d_event.h"
#include "v_video.h"

dboolean         joystickavail = false;
dboolean         mouseavail    = false;

extern int       usemouse;
extern SDL_Window* pWindow;
extern video_t   video;

Uint8* keystates[256] = { false };

extern dboolean      bQuit;
extern int keylink;

void lfprintf(char *message, ... );

dboolean I_InitInputs(void)
   {
    I_SetupKeyboard();

    if ((usemouse) && (!mouseavail))
       {
         mouseavail = true;
       }
    return true;
   }

void I_CheckInputs(void)
   {
    event_t event;
    SDL_Event ev;

    SDL_MouseMotionEvent mouse_motion = { 0 };
    SDL_MouseButtonEvent mouse_button = { 0 };

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

        if ((usemouse) && (mouseavail)) {

        case SDL_MOUSEMOTION:
        {

            event.type = ev_mouse;
            mouse_motion.x = ev.motion.xrel;
            mouse_motion.y = ev.motion.yrel;

            event.data2 = mouse_motion.x;
            event.data3 = mouse_motion.y;

            D_PostEvent(&event);

        }break;

        case SDL_MOUSEBUTTONDOWN:
        {
            event.type = ev_mouse;
            mouse_motion.x = ev.motion.xrel;
            mouse_motion.y = ev.motion.yrel;

            event.data1 = ev.button.button;

            event.data2 = mouse_motion.x;
            event.data3 = mouse_motion.y;

            D_PostEvent(&event);
        }break;

        case SDL_MOUSEBUTTONUP:
        {
            event.type = ev_mouse;
            mouse_motion.x = ev.motion.xrel;
            mouse_motion.y = ev.motion.yrel;

            event.data1 = 0;

            event.data2 = mouse_motion.x;
            event.data3 = mouse_motion.y;

            D_PostEvent(&event);
        }break;
        }

        default:
            break;
        }


    }

    I_CheckKeyboard();
   }


void I_GetEvent(void)
   {
    I_CheckInputs();
   }

