// sdl_video.c
// This module handles the video interface to Windows

#include <stdio.h>
#include "v_video.h"
#include "sdl_video.h"
#include "gl_video.h"
#include "i_system.h"

void lfprintf(char *message, ... );

extern video_t      video;

int                 iModeCount = 0, iCurrMode = 0;
dboolean               *bNoReboot = 0;

devinfo_t           DevInfo;
char               *szYesNo[] = { "Yes", "No" };

void GetVideoInfo()
   {
    SDL_DisplayMode         mode;
    static int              display_in_use = 0;
    Uint32                  f;
#if SDL_MAJOR_VERSION == 3
    SDL_GetCurrentDisplayMode(display_in_use);
#else
    SDL_GetCurrentDisplayMode(display_in_use, &mode);
#endif
    f = mode.format;

    DevInfo.width = mode.w;
    DevInfo.height = mode.h;
    DevInfo.bpp    = SDL_BITSPERPIXEL(f);

   }

void GetModeList(char *dbgname)
   {
    FILE*                   fn;
    static int              display_in_use = 0; /* Only using first display */
    int                     i, display_mode_count;
    SDL_DisplayMode         mode;
    Uint32                  f;
    
    fn = fopen(dbgname, "a+");
#if SDL_MAJOR_VERSION == 3
    display_mode_count = SDL_GetFullscreenDisplayModes(display_in_use, 1);
#else
    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
#endif
    if (display_mode_count < 1) {
        I_Error("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
    }
    fprintf(fn, "Mode count : %d\n", display_mode_count);
    fprintf(fn, "General Video Mode List\n");
    fprintf(fn, "Mode Width x Height x Color Bits - Refresh\n");

    for (i = 0; i < display_mode_count; ++i) 
    {
#if SDL_MAJOR_VERSION == 3
        if ((SDL_GetCurrentDisplayMode(display_in_use)) != 0) {
#else
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
#endif
            I_Error("SDL_GetDisplayMode failed: %s", SDL_GetError());
            
        }

        f = mode.format;

        printf("%d - %d x %d\tBpp: %d - %d Hz\n",
            i, mode.w, mode.h,
            SDL_BITSPERPIXEL(f),
            mode.refresh_rate );

        fprintf(fn, "%d - %d x %d\tBpp: %d - %d Hz\n",
            i, mode.w, mode.h,
            SDL_BITSPERPIXEL(f),
            mode.refresh_rate);
    }
   }

dboolean SetVideoMode()
   {
    SDL_DisplayMode mode;
    dboolean success = true;
#if SDL_MAJOR_VERSION == 3
    video.fullscreen = SDL_SetWindowFullscreenMode(pWindow, &mode);
#else
    video.fullscreen = SDL_SetWindowDisplayMode(pWindow, &mode);
#endif
    if (!video.fullscreen)
    {
        printf("Failed to load into fullscreen mode");
        success = false;
    }
    //   {
    //    if (/*(WinData.bChangeBPP == FALSE) && */(DevInfo.bpp != video.bpp))
    //       {
    //        lfprintf("This version of Windows cannot change color depth.\n"
    //                 "Please request different video mode settings or adjust\n"
    //                 "your desktop color depth.\n");
    //        return FALSE;
    //       }
    //    for (mode = 0; mode < iModeCount; mode++)
    //       {
    //        if ((pModeList[mode].dmPelsWidth == video.width) &&
    //            (pModeList[mode].dmPelsHeight == video.height) &&
    //            (pModeList[mode].dmBitsPerPel == video.bpp))
    //           {
    //            //WinData.iVidMode = mode;

    //            memset(&dm, 0, sizeof(dm));
    //            dm.dmSize = sizeof(dm);

    //            dm.dmPelsWidth  = video.width;
    //            dm.dmPelsHeight = video.height;
    //            dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;

    //            if (video.bpp != DevInfo.bpp)
    //               {
    //                dm.dmBitsPerPel = video.bpp;
    //                dm.dmFields |= DM_BITSPERPEL;
    //               }

    //            if ( ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
    //               {
    //                lfprintf("SetVideoMode - ChangeDisplaySettings failed.\n"
    //                         "Switching to windowed mode.\n");
    //                video.fullscreen = FALSE;
    //                return TRUE;
    //               }
    //            return TRUE;
    //           }
    //       }
    //    lfprintf("Your requested video mode is unavailable.\n"
    //             "Please request different video mode settings.\n");
    //    return FALSE;
    //   }
    //else
    //   {
    //    if (DevInfo.bpp != video.bpp)
    //       {
    //        lfprintf("Your requested color depth and desktop do not match.\n"
    //                 "Using your current desktop color depth.\n");
    //       }
    //   }
    return success;
   }

