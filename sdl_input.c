// sdl_inpt.c
// 
// This module starts up and handles all
// keyboard, mouse and joystick inputs.
// This module is for Win32

#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

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

dboolean keystates[256] = { false };

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
    //SDL_MouseButtonEvent mouse_button = { 0 };

    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_QUIT:
#else
        case SDL_QUIT:
#endif
        {
            bQuit = false;
            break;
        }break;

#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_KEY_DOWN:
#else
        case SDL_KEYDOWN:
#endif
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
            keystates[ev.key.keysym.scancode] = true;
        }break;
#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_KEY_UP:
#else
        case SDL_KEYUP:
#endif
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
            keystates[ev.key.keysym.scancode] = false;
        }break;

        if ((usemouse) && (mouseavail)) {
#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_MOUSE_WHEEL:
#else
        case SDL_MOUSEWHEEL:
#endif
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
#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
#else
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
#endif
            event.type = ev_mouse;
            event.data1 = I_SDLtoDoomMouseState(SDL_GetMouseState(NULL, NULL));
            event.data2 = event.data3 = 0;
            D_PostEvent(&event);

            break;
        }
#if SDL_MAJOR_VERSION == 3
        case SDL_EVENT_MOUSE_MOTION:
#else
        case SDL_MOUSEMOTION:
#endif
        {
            if (ev.motion.xrel != 0 || ev.motion.yrel != 0)
            {
                event.type = ev_mousemotion;
                mouse_motion.x = ev.motion.xrel;
                mouse_motion.y = ev.motion.yrel;
#if SDL_MAJOR_VERSION == 3
                event.data2 = (int)mouse_motion.x;
                event.data3 = (int)mouse_motion.y;
#else
                event.data2 = mouse_motion.x;
                event.data3 = mouse_motion.y;
#endif
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
#if SDL_MAJOR_VERSION == 3
static void SmoothMouse(float* x, float* y)
{
    static float x_remainder_old = 0;
    static float y_remainder_old = 0;

    fixed_t x_remainder, y_remainder;
    fixed_t correction_factor;

    const fixed_t fractic = I_TickElapsedTime();

    *x += x_remainder_old;
    *y += y_remainder_old;

    correction_factor = FixedDiv(fractic, FRACUNIT + fractic);

    x_remainder = FixedMul(*x, correction_factor);
    *x -= x_remainder;
    x_remainder_old = (float)x_remainder;

    y_remainder = FixedMul(*y, correction_factor);
    *y -= (float)y_remainder;
    y_remainder_old = (float)y_remainder;
}
#else
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
#endif

static void I_ReadMouse()
{
#if SDL_MAJOR_VERSION == 3
    float x, y;
    SDL_GetRelativeMouseState(&x, &y);
#else
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
#endif
    SmoothMouse(&x, &y);

    if (x != 0 || y != 0)
    {
        event_t event;
        event.type = ev_mousemotion;
        event.data1 = 0;
#if SDL_MAJOR_VERSION == 3
        event.data2 = (int)x << 4;
        event.data3 = (int)-y << 4;
#else
        event.data2 = x << 4;
        event.data3 = -y << 4;
#endif
        D_PostEvent(&event);
    }
}


void I_GetEvent(void)
{
    SDL_GL_SetSwapInterval(true);
    
    I_CheckInputs();

    I_ReadMouse();
}

