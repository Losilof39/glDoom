//// sdl_video.c
//// This module handles the video interface to Windows
//
//#include <VersionHelpers.h>
//#include <stdio.h>
//#include "v_video.h"
//#include "sdl_video.h"
//#include "sys_sdl.h"
//#include "gconsole.h"
//
//void lfprintf(char* message, ...);
//
//extern video_t      video;
//extern windata_t    WinData;
//
//int                 iModeCount = 0, iCurrMode = 0;
//BOOL* bNoReboot = 0;
//DEVMODE* pModeList = 0;
//
//devinfo_t           DevInfo;
//char* szYesNo[] = { "Yes", "No" };
//
//void GetVideoInfo()
//{
//    HDC         hdc;
//
//    // Get the current display device info
//    hdc = GetDC(NULL);
//    DevInfo.bpp = GetDeviceCaps(hdc, BITSPIXEL);
//    DevInfo.width = GetSystemMetrics(SM_CXSCREEN);
//    DevInfo.height = GetSystemMetrics(SM_CYSCREEN);
//    ReleaseDC(NULL, hdc);
//}
//
//void GetModeList(char* dbgname)
//{
//    DEVMODE devmode;
//    int     iVideoMode;
//    FILE* fn;
//
//    fn = fopen(dbgname, "a+");
//
//    iCurrMode = -1;
//    iModeCount = 0;
//    while (EnumDisplaySettings(NULL, iModeCount, &devmode))
//        iModeCount++;
//
//    pModeList = (DEVMODE*)malloc(iModeCount * sizeof(DEVMODE));
//    bNoReboot = (BOOL*)malloc(iModeCount * sizeof(BOOL));
//
//    fprintf(fn, "Mode count : %d\n", iModeCount);
//    fprintf(fn, "General Video Mode List\n");
//    fprintf(fn, "Mode Width x Height x Color Bits - Refresh - Reboot?\n");
//    for (iVideoMode = 0; iVideoMode < iModeCount; iVideoMode++)
//    {
//        EnumDisplaySettings(NULL, iVideoMode, &pModeList[iVideoMode]);
//        if (ChangeDisplaySettings(&pModeList[iVideoMode], CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
//            bNoReboot[iVideoMode] = TRUE;
//        else
//            bNoReboot[iVideoMode] = FALSE;
//        fprintf(fn, "%2d    %4d    %4d        %2d          %2d       %s\n",
//            iVideoMode,
//            pModeList[iVideoMode].dmPelsWidth,
//            pModeList[iVideoMode].dmPelsHeight,
//            pModeList[iVideoMode].dmBitsPerPel,
//            pModeList[iVideoMode].dmDisplayFrequency,
//            szYesNo[bNoReboot[iVideoMode]]);
//        if ((pModeList[iVideoMode].dmPelsWidth == video.width) &&
//            (pModeList[iVideoMode].dmPelsHeight == video.height) &&
//            (pModeList[iVideoMode].dmBitsPerPel == video.bpp))
//            iCurrMode = iVideoMode;
//    }
//    fclose(fn);
//    if (iModeCount == 0)
//    {
//        con_printf("Warning: no video mode list available - unable to change resolution.\n");
//    }
//}
//
//BOOL SetVideoMode()
//{
//    int             mode;
//    DEVMODE         dm;
//
//    WinData.bChangeBPP = FALSE;
//
//    if (!IsWindows8OrGreater())
//    {
//        MessageBox(NULL, "You need at least Windows 8 to use this software", "Version Not Supported", MB_OK);
//        return FALSE;
//    }
//
//    if (video.fullscreen)
//    {
//        if ((WinData.bChangeBPP == FALSE) && (DevInfo.bpp != video.bpp))
//        {
//            lfprintf("This version of Windows cannot change color depth.\n"
//                "Please request different video mode settings or adjust\n"
//                "your desktop color depth.\n");
//            return FALSE;
//        }
//        for (mode = 0; mode < iModeCount; mode++)
//        {
//            if ((pModeList[mode].dmPelsWidth == video.width) &&
//                (pModeList[mode].dmPelsHeight == video.height) &&
//                (pModeList[mode].dmBitsPerPel == video.bpp))
//            {
//                WinData.iVidMode = mode;
//
//                memset(&dm, 0, sizeof(dm));
//                dm.dmSize = sizeof(dm);
//
//                dm.dmPelsWidth = video.width;
//                dm.dmPelsHeight = video.height;
//                dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
//
//                if (video.bpp != DevInfo.bpp)
//                {
//                    dm.dmBitsPerPel = video.bpp;
//                    dm.dmFields |= DM_BITSPERPEL;
//                }
//
//                if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
//                {
//                    lfprintf("SetVideoMode - ChangeDisplaySettings failed.\n"
//                        "Switching to windowed mode.\n");
//                    video.fullscreen = FALSE;
//                    return TRUE;
//                }
//                return TRUE;
//            }
//        }
//        lfprintf("Your requested video mode is unavailable.\n"
//            "Please request different video mode settings.\n");
//        return FALSE;
//    }
//    else
//    {
//        if (DevInfo.bpp != video.bpp)
//        {
//            lfprintf("Your requested color depth and desktop do not match.\n"
//                "Using your current desktop color depth.\n");
//        }
//    }
//    return TRUE;
//}
//
//void ResetVideoMode()
//{
//    //ChangeDisplaySettings(NULL, 0);
//}
//
