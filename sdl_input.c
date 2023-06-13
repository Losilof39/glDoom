// sdl_inpt.c
// 
// This module starts up and handles all
// keyboard, mouse and joystick inputs.
// This module is for Win32

#include "thirdparty/SDL2/include/SDL.h"
#include "d_main.h"
#include "sys_sdl.h"
#include "sdl_kbrd.h"
#include "d_event.h"
#include "v_video.h"
#include "i_system.h"

dboolean         joystickavail = false;
dboolean         mouseavail    = false;

extern int       usemouse;
extern SDL_Window* pWindow;
extern video_t   video;

Uint8* keystates[256] = { false };

extern dboolean      bQuit;
extern int keylink;
extern int gametic;

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

/* cph - pulled out common button code logic */
//e6y static 
static int I_SDLtoDoomMouseState(Uint32 buttonstate)
{
    return 0
        | (buttonstate & SDL_BUTTON(1) ? 1 : 0)
        | (buttonstate & SDL_BUTTON(2) ? 2 : 0)
        | (buttonstate & SDL_BUTTON(3) ? 4 : 0)
        | (buttonstate & SDL_BUTTON(6) ? 8 : 0)
        | (buttonstate & SDL_BUTTON(7) ? 16 : 0)
        | (buttonstate & SDL_BUTTON(4) ? 32 : 0)
        | (buttonstate & SDL_BUTTON(5) ? 64 : 0)
        | (buttonstate & SDL_BUTTON(8) ? 128 : 0)
        ;
}

void I_CheckInputs(void)
   {
    event_t event;
    SDL_Event ev;
    static int mwheeluptic = 0, mwheeldowntic = 0;

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

        case SDL_MOUSEWHEEL:
        {
            if (ev.wheel.y > 0)
            {
                event.type = ev_keydown;
                event.data1 = KEYD_MWHEELUP;
                mwheeluptic = gametic;
                D_PostEvent(&event);
            }
            else if (ev.wheel.y < 0)
            {
                event.type = ev_keydown;
                event.data1 = KEYD_MWHEELDOWN;
                mwheeldowntic = gametic;
                D_PostEvent(&event);
            }
        }break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            event.type = ev_mouse;
            event.data1 = I_SDLtoDoomMouseState(SDL_GetMouseState(NULL, NULL));
            event.data2 = event.data3 = 0;
            D_PostEvent(&event);

            break;
        }

        case SDL_MOUSEMOTION:
        {
            if (ev.motion.xrel != 0 || ev.motion.yrel != 0)
            {
                event.type = ev_mousemotion;
                mouse_motion.x = ev.motion.xrel;
                mouse_motion.y = ev.motion.yrel;

                event.data2 = mouse_motion.x;
                event.data3 = mouse_motion.y;

                D_PostEvent(&event);
            }

        }break;

        default:
            break;
        }


    }

    if (mwheeluptic && mwheeluptic + 1 < gametic)
    {
        event.type = ev_keyup;
        event.data1 = KEYD_MWHEELUP;
        D_PostEvent(&event);
        mwheeluptic = 0;
    }

    if (mwheeldowntic && mwheeldowntic + 1 < gametic)
    {
        event.type = ev_keyup;
        event.data1 = KEYD_MWHEELDOWN;
        D_PostEvent(&event);
        mwheeldowntic = 0;
    }

    I_CheckKeyboard();
   }

static void SmoothMouse(int* x, int* y)
{
    static int x_remainder_old = 0;
    static int y_remainder_old = 0;

    int x_remainder, y_remainder;
    fixed_t correction_factor;

    const fixed_t fractic = I_TickElapsedTime();

    *x += x_remainder_old;
    *y += y_remainder_old;

    correction_factor = FixedDiv(fractic, FRACUNIT + fractic);

    x_remainder = FixedMul(*x, correction_factor);
    *x -= x_remainder;
    x_remainder_old = x_remainder;

    y_remainder = FixedMul(*y, correction_factor);
    *y -= y_remainder;
    y_remainder_old = y_remainder;
}

static void I_ReadMouse()
{
    int x, y;

    SDL_GetRelativeMouseState(&x, &y);
    SmoothMouse(&x, &y);

    if (x != 0 || y != 0)
    {
        event_t event;
        event.type = ev_mousemotion;
        event.data1 = 0;
        event.data2 = x << 4;
        event.data3 = -y << 4;

        D_PostEvent(&event);
    }
}


void I_GetEvent(void)
{
    I_CheckInputs();

    I_ReadMouse();
}

