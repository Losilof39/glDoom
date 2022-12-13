// win_video.c
// This module handles the video interface to Windows

#include <stdio.h>
#include "v_video.h"
#include "sdl_video.h"
#include "sys_sdl.h"
#include "gconsole.h"

void lfprintf(char *message, ... );

extern video_t      video;

int                 iModeCount = 0, iCurrMode = 0;
dboolean               *bNoReboot = 0;
//DEVMODE            *pModeList = 0;

devinfo_t           DevInfo;
char               *szYesNo[] = { "Yes", "No" };

void GetVideoInfo()
   {
    SDL_DisplayMode         mode;
    static int              display_in_use = 0;
    Uint32                  f;

    SDL_GetCurrentDisplayMode(display_in_use, &mode);
    f = mode.format;

    DevInfo.bpp    = SDL_BITSPERPIXEL(f);
    DevInfo.width  = mode.w;
    DevInfo.height = mode.h;
   }

void GetModeList(char *dbgname)
   {
    FILE*                   fn;
    static int              display_in_use = 0; /* Only using first display */
    int                     i, display_mode_count;
    SDL_DisplayMode         mode;
    Uint32                  f;
    
    fn = fopen(dbgname, "a+");

    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        con_printf("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return 1;
    }
    fprintf(fn, "Mode count : %d\n", display_mode_count);
    fprintf(fn, "General Video Mode List\n");
    fprintf(fn, "Mode Width x Height x Color Bits - Refresh - Reboot?\n");

    for (i = 0; i < display_mode_count; ++i) 
    {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            con_printf("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return 1;
        }

        f = mode.format;

        con_printf(fn, "%2d    %4d    %4d        %2d          %2d\n",
            i, SDL_BITSPERPIXEL(f),
            SDL_GetPixelFormatName(f),
            mode.w, mode.h);
    }
    /*DEVMODE devmode;
    int     iVideoMode;
    FILE *fn;

    fn = fopen(dbgname, "a+");

    iCurrMode = -1;
    iModeCount = 0;
    while(EnumDisplaySettings(NULL, iModeCount, &devmode))
       iModeCount++;

    pModeList = (DEVMODE *)malloc(iModeCount*sizeof(DEVMODE));
    bNoReboot = (BOOL *)malloc(iModeCount*sizeof(BOOL));

    fprintf(fn, "Mode count : %d\n", iModeCount);
    fprintf(fn, "General Video Mode List\n");
    fprintf(fn, "Mode Width x Height x Color Bits - Refresh - Reboot?\n");
    for (iVideoMode = 0; iVideoMode < iModeCount; iVideoMode++)
       {
        EnumDisplaySettings(NULL, iVideoMode, &pModeList[iVideoMode]);
        if (ChangeDisplaySettings(&pModeList[iVideoMode], CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
            bNoReboot[iVideoMode] = TRUE;
        else
            bNoReboot[iVideoMode] = FALSE;
        fprintf(fn, "%2d    %4d    %4d        %2d          %2d       %s\n",
                iVideoMode,
                pModeList[iVideoMode].dmPelsWidth,
                pModeList[iVideoMode].dmPelsHeight,
                pModeList[iVideoMode].dmBitsPerPel,
                pModeList[iVideoMode].dmDisplayFrequency,
                szYesNo[bNoReboot[iVideoMode]]);
        if ((pModeList[iVideoMode].dmPelsWidth == video.width) &&
            (pModeList[iVideoMode].dmPelsHeight == video.height) &&
            (pModeList[iVideoMode].dmBitsPerPel == video.bpp))
            iCurrMode = iVideoMode;
       }
    fclose(fn);
    if (iModeCount == 0)
       {
        con_printf("Warning: no video mode list available - unable to change resolution.\n");
       }*/
   }

dboolean SetVideoMode()
   {
    
    //if (video.fullscreen)
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
    return true;
   }

void ResetVideoMode()
   {
   }

