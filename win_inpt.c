// win_inpt.c
// 
// This module starts up and handles all
// keyboard, mouse and joystick inputs.
// This module is for Win32

#include <windows.h>
#include <SDL.h>
#include "sys_win.h"
#include "gconsole.h"
//#include "win_joy.h"
//#include "win_mous.h"
#include "win_kbrd.h"

dboolean         joystickavail = false;
dboolean         mouseavail    = false;

extern int       usejoystick;
extern int       usemouse;

Uint8* keystates[256] = { false };

extern dboolean      bQuit;
extern windata_t WinData;

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
    //MSG  msg;

    //// it seems that through PeekMessage we get key messages for shift, escape, alt, etc..
    //// and the rest we get through DirectInput. Why not getting every single key with DI??
    //while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //   {
    //    if (msg.message == WM_QUIT)
	   //    {
 	  //     bQuit = true;
    //        break;
    //       }
    //    if (!TranslateAccelerator(msg.hwnd, WinData.hAccel, &msg))
    //       {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //       }
    //   }

    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT:
        {
            bQuit = true;
            break;
        }break;


        case SDL_KEYDOWN:
        {
            keystates[ev.key.keysym.scancode] = 1;
        }break;
        
        case SDL_KEYUP:
        {
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
    /*if ((usejoystick) && (joystickavail))
       {
        I_CheckJoysticks();
       }*/
    /*if ((usemouse) && (mouseavail))
       {
        I_ReleaseMouse();
       }*/
    I_ReleaseKeyboard();
   }


void I_GetEvent(void)
   {
    I_CheckInputs();
   }

