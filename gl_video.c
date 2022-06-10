// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <glad/glad.h>
#include <SDL.h>

#include <stdlib.h>
#include <math.h>

#include "doomtype.h"
#include "gconsole.h"
#include "sys_sdl.h"
#include "gl_video.h"
#include "sdl_inpt.h"
#include "sdl_video.h"

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "w_wad.h"
#include "z_zone.h"

#include "doomdef.h"
#include "doomlib.h"

// Dynamic light map base pattern
void GeneratePointPattern(void);

unsigned char dlm[16*16];

int      MonoLightMap, WhiteLightMap, WhiteColorMap;

dboolean		grabMouse;

static int	multiply = 1;

// print to log file...
void lfprintf(char *message, ... );

// Game stuff

extern SDL_Window* pWindow;
SDL_GLContext glContext;
extern video_t     video;

extern int         starttime;

extern int         vsync;

// OpenGL renderer stuff

HGLRC                hRC = 0;                 // A global rendering context
HDC                  hGDC = 0;                // A global device context
char                *GL_Vendor;               // The company responsible for this GL implementation
char                *GL_Renderer;             // The name of the type of renderer
char                *GL_Version;              // The version and/or release number
int                  GL_EXT_Count;            // How many extensions are there?
char                *GL_EXT_List[GL_MAX_EXT]; // Here's a list of their names
dboolean             GL_3Dlabs = false;

dboolean             software = false;

glmode_t             glmode;

double               glFovY;    // Rendering field of view
float                SetBack;   // 3D setback for 2D displays
float                glLeft, glTop, glRight, glBottom, glAspect;

// Windows stuff
extern int           iCurrMode;
extern DEVMODE      *pModeList;
extern windata_t     WinData;

void ShutdownOpenGL(void)
   {
    /*wglMakeCurrent(NULL, NULL);
    wglDeleteContext( hRC );*/
    SDL_GL_MakeCurrent(pWindow, NULL);
    SDL_GL_DeleteContext(glContext);
   }

void I_ShutdownGraphics(void)
   {
    ShutdownOpenGL();
    //ReleaseDC(WinData.hWnd, hGDC);
    //ResetVideoMode();
   }

void I_Start2DFrame()
   {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0, video.width, 0, video.height, 0.5, 10.0);
    glViewport( 0, 0, video.width, video.height);

    glTranslatef( 0.0f, 0.0f, -1.0f );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef( 0.0f, 0.0f, 0.0f );

   }


void I_Start3DFrame()
{
    glFovY   = video.fovy;
    glAspect = (float)video.width / (float)video.height;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective((double)glFovY, (double)glAspect, (double)video.nearclip, (double)video.farclip );
    glViewport( 0, 0, video.width, video.height);

    glTranslatef( 0.0f, 0.0f, 2.0f );

    // Define the modelview transformation.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef( 0.0f, 0.0f, 0.0f );

    SetBack  = -120.0f / tanf(DEG2RAD(glFovY * 0.5f));
    SetBack -= 2.0f;

    glTop    = 120.0f;
    glBottom = -glTop;

    glRight = glTop * glAspect;
    glLeft = -glRight;
}


//
// I_StartFrame
//
void I_StartFrame (void)
   {
    // er?
   // Will put the OpenGL frame start code in here...
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    I_Start3DFrame();

    glmode = gl_3d;
   }

//
// I_StartTic
//
void I_StartTic (void)
   {
    // If the mouse has been captured (windowed mode)
    // return the mouse to the center of the screen here

	I_GetEvent();
   }


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
   {
    // what is this?
   }

void GL_WriteText(int x, int y, char *msg);
extern int loadtime;
int        framecnt = 0;

void I_Finish3D()
   {
    glPopMatrix();
   }

//
// I_FinishUpdate
//
void I_FinishUpdate(void)
   {
    static DWORD lasttime = 0;
    DWORD        now, ms;
    float        fps;
    char         fps_str[16];

    if (devparm)  // Draw the frame rate on the screen
       {          // which will slow it down...
        now = timeGetTime();
        ms = now - lasttime;
        if ((ms > 0) && (ms < 1000))
           {
            fps = 1000.0f/(float)ms;
            sprintf(fps_str, "%.1f FPS", fps);
            GL_WriteText(260, 155, fps_str);
           }
        lasttime = now;
       }

    if (glmode == gl_3d)
       {
        //I_Finish3D();
        //glPopMatrix();
       }
    else
       {
        glOrtho(0, video.width, 0, video.height, 0.5, 10.0);
        glViewport( 0, 0, video.width, video.height);

        glTranslatef( 0.0f, 0.0f, -1.0f );
       }

    // Swap the rendering buffers...
    //SwapBuffers(hGDC);
    SDL_GL_SwapWindow(pWindow);
   }


//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}

typedef struct
   {
    unsigned char r;
    unsigned char g;
    unsigned char b;
   }MY_PAL;

MY_PAL  gamepal[256];
extern MY_PAL  statpal[256];

void CreateGLPalette()
   {
    MY_PAL *buffer;

    buffer = (MY_PAL *)W_CacheLumpName("PLAYPAL", PU_CACHE);
    memcpy(statpal, buffer, 768);
   }

//
// Palette stuff.
//
//
// I_SetPalette
//
void I_SetPalette(byte* palette)
   {
    int i;

    for (i = 0; i < 256; i++)
       {
        gamepal[i].r = gammatable[usegamma][*palette++];
        gamepal[i].g = gammatable[usegamma][*palette++];
        gamepal[i].b = gammatable[usegamma][*palette++];
       }
   }

void OGL_Error( int GL_Code, char *msg )
   {
    char msgtext[128];
    char errmsg[128];
    switch(GL_Code)
       {
        default:
             sprintf(errmsg,"Unknown Error Code: %04X", GL_Code);
             strcpy(errmsg, msgtext);
             break;
       }

    lfprintf("GLERR: %s\n", errmsg);
    lfprintf("%s FAILED\n", msg);
   }

//  StartUpOpenGL sets the pixel format and a rendering context then returns true/false
dboolean StartUpOpenGL( HWND hWnd )
   {
    //int      nMyPixelFormatID;
    //static PIXELFORMATDESCRIPTOR pfd = {
    //    sizeof (PIXELFORMATDESCRIPTOR), // struct size 
    //    1,                              // Version number
    //    PFD_DRAW_TO_WINDOW |    // Flags, draw to a window,
    //    PFD_SUPPORT_OPENGL |    // use OpenGL
    //    PFD_DOUBLEBUFFER,       // want double buffering
    //    PFD_TYPE_RGBA,          // RGBA pixel values
    //    32,                     // 32-bit color (or max video mode allows)
    //    0, 0, 0, 0, 0, 0,       // RGB bits & shift sizes. Don't care about them
    //    0,                      // No alpha buffer info
    //    0,                      // shift bit ignored
    //    0,                      // no accumulation buffer
    //    0, 0, 0, 0,             // accum bits ignored
    //    32,                     // 32-bit depth buffer if we can get it...
    //    0,                      // No stencil buffer
    //    0,                      // No auxiliary buffers
    //    PFD_MAIN_PLANE,         // main layer
    //    0,                      // Reserved (must be 0)
    //    0, 0, 0                 // No layer, visible or damage masks
    //   };

    //lfprintf("Attempting to ChoosePixelFormat...\n");
    //hGDC = GetDC( hWnd );
    //nMyPixelFormatID = ChoosePixelFormat( hGDC, &pfd );
    //if (nMyPixelFormatID == 0)
    //   {
    //    ReleaseDC( hWnd, hGDC );
    //    OGL_Error( 1, "ChoosePixelFormat");
    //    return false;
    //   }

    //if (SetPixelFormat( hGDC, nMyPixelFormatID, &pfd ) == true)
    //   {
    //    DescribePixelFormat(hGDC, nMyPixelFormatID, sizeof(PIXELFORMATDESCRIPTOR), &pfd); 
    //    lfprintf("Pixel Format    : %d\n", nMyPixelFormatID);
    //    lfprintf("Version         : %X\n", pfd.nVersion);
    //    if (pfd.dwFlags & PFD_DRAW_TO_WINDOW)
    //       lfprintf("Flags           : PFD_DRAW_TO_WINDOW\n");
    //    if (pfd.dwFlags & PFD_DRAW_TO_BITMAP)
    //       lfprintf("Flags           : PFD_DRAW_TO_BITMAP\n");
    //    if (pfd.dwFlags & PFD_SUPPORT_GDI)
    //       lfprintf("Flags           : PFD_SUPPORT_GDI\n");
    //    if (pfd.dwFlags & PFD_SUPPORT_OPENGL)
    //       lfprintf("Flags           : PFD_SUPPORT_OPENGL\n");
    //    if (pfd.dwFlags & PFD_GENERIC_ACCELERATED)
    //       lfprintf("Flags           : PFD_GENERIC_ACCELERATED\n");
    //    if (pfd.dwFlags & PFD_GENERIC_FORMAT)
    //       lfprintf("Flags           : PFD_GENERIC_FORMAT\n");
    //    if (pfd.dwFlags & PFD_NEED_PALETTE)
    //       lfprintf("Flags           : PFD_NEED_PALETTE\n");
    //    if (pfd.dwFlags & PFD_NEED_SYSTEM_PALETTE)
    //       lfprintf("Flags           : PFD_NEED_SYSTEM_PALETTE\n");
    //    if (pfd.dwFlags & PFD_DOUBLEBUFFER)
    //       lfprintf("Flags           : PFD_DOUBLEBUFFER\n");
    //    if (pfd.dwFlags & PFD_STEREO)
    //       lfprintf("Flags           : PFD_STEREO\n");
    //    if (pfd.dwFlags & PFD_SWAP_LAYER_BUFFERS)
    //       lfprintf("Flags           : PFD_SWAP_LAYER_BUFFERS\n");

    //    lfprintf("ColorBits       : %d\n", pfd.cColorBits);
    //    lfprintf("RedBits         : %d\n", pfd.cRedBits);
    //    lfprintf("RedShift        : %d\n", pfd.cRedShift);
    //    lfprintf("GreenBits       : %d\n", pfd.cGreenBits);
    //    lfprintf("GreenShift      : %d\n", pfd.cGreenShift);
    //    lfprintf("BlueBits        : %d\n", pfd.cBlueBits);
    //    lfprintf("BlueShift       : %d\n", pfd.cBlueShift);
    //    lfprintf("AlphaBits       : %d\n", pfd.cAlphaBits);
    //    lfprintf("AlphaShift      : %d\n", pfd.cAlphaShift);
    //    lfprintf("AccumBits       : %d\n", pfd.cAccumBits);
    //    lfprintf("AccumRedBits    : %d\n", pfd.cAccumRedBits);
    //    lfprintf("AccumGreenBits  : %d\n", pfd.cAccumGreenBits);
    //    lfprintf("AccumBlueBits   : %d\n", pfd.cAccumBlueBits);
    //    lfprintf("AccumAlphaBits  : %d\n", pfd.cAccumAlphaBits);
    //    lfprintf("DepthBits       : %d\n", pfd.cDepthBits);
    //    lfprintf("StencilBits     : %d\n", pfd.cStencilBits);
    //    lfprintf("AuxBuffers      : %d\n", pfd.cAuxBuffers);
    //    lfprintf("LayerType       : %d\n", pfd.iLayerType);
    //   }
    //else
    //   {
    //    ReleaseDC( hWnd, hGDC );
    //    OGL_Error( 1, "SetPixelFormat");
    //    return false;
    //   }

    //if ((!(pfd.dwFlags & PFD_GENERIC_ACCELERATED)) &&
    //     ((pfd.dwFlags & PFD_GENERIC_FORMAT) > 0))
    //   {
    //    con_printf("\nno OpenGL acceleration found...\n");

    //    if (video.allowsoft == false)
    //       {
    //        ChangeDisplaySettings(0, 0);
    //        MessageBox(NULL, "OpenGL Driver is not accelerated\n", "FATAL ERROR", MB_OK);
    //        return false;
    //       }

    //   }

    /*hRC = wglCreateContext( hGDC );
    if (hRC == (HGLRC)NULL)
       {
        ReleaseDC( hWnd, hGDC );
        OGL_Error( 1, "wglCreateContext");
        return false;
       }*/

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    glContext = SDL_GL_CreateContext(pWindow);
    SDL_GL_MakeCurrent(pWindow, glContext);

    if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) < 0)
    {
        printf("[ERROR]: Failed to load OpenGL library");
        exit(-1);
    }

    //wglMakeCurrent(hGDC, hRC);
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glDisable( GL_DEPTH_TEST );

    glmode = gl_2d;

    con_SetupOpenGL();

    //GetGLInfo();

    return true;
}

void I_InitGraphics(void)
{
}

void GetGLInfo()
   {
    char *szu, *tempstr;
    const GLubyte *szt;

    szt = glGetString(GL_VENDOR);
    GL_Vendor = (char *)malloc(strlen(szt)+1);
    strcpy(GL_Vendor, szt);
    con_printf("OpenGL Vendor   : %s\n", GL_Vendor);
    if (D_strcasecmp(GL_Vendor, "3DLABS") == 0)
       GL_3Dlabs = true;

    szt = glGetString(GL_RENDERER);
    GL_Renderer = (char *)malloc(strlen(szt)+1);
    strcpy(GL_Renderer, szt);
    con_printf("OpenGL Renderer : %s\n", GL_Renderer);

    if ((D_strcasecmp(GL_Vendor, "Microsoft Corporation") == 0) &&
        (D_strcasecmp(GL_Renderer, "GDI Generic") == 0))
       {
        // oh CRAP, it's the dreaded Microsoft Software OpenGL Renderer...
        con_printf("SOFTWARE OpenGL Renderer!!!\n");
        software = true;
       }

    szt = glGetString(GL_VERSION);
    GL_Version = (char *)malloc(strlen(szt)+1);
    strcpy(GL_Version, szt);
    con_printf("OpenGL Version  : %s\n", GL_Version);

    szt = glGetString(GL_EXTENSIONS);
    tempstr = (char *)malloc(strlen(szt)+2);
    strcpy(tempstr, szt);
    if (tempstr != NULL)
       {
        szu = strtok( (char *)tempstr, " " );
        GL_EXT_Count = 0;
        while (( szu != NULL ) && (GL_EXT_Count < GL_MAX_EXT))
           {
            GL_EXT_List[GL_EXT_Count] = (char *)malloc(strlen(szu)+1);
            strcpy(GL_EXT_List[GL_EXT_Count], szu);
            con_printf("OpenGL Extension : %s\n", GL_EXT_List[GL_EXT_Count]);
            szu = strtok( NULL, " " );
            GL_EXT_Count++;
           }
       }
    free(tempstr);
    con_printf("End of OpenGL extensions...\n");
   }
