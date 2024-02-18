// Emacs style mode select   -*- C -*- 
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

#include <glad/glad.h>
#include <SDL.h>
#include <stdlib.h>
#include <math.h>

#include "doomtype.h"
#include "sys_sdl.h"
#include "gl_video.h"
#include "sdl_input.h"
#include "sdl_video.h"
#include "renderer.h"

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

unsigned char dlm[16 * 16];

int      MonoLightMap, WhiteLightMap, WhiteColorMap;

dboolean		grabMouse;

static int	multiply = 1;

// print to log file...
void lfprintf(char* message, ...);

// Game stuff

extern SDL_Window* pWindow;
SDL_GLContext glContext;
extern video_t     video;

extern int         starttime;
extern int         vsync;

// OpenGL renderer stuff
char* GL_Vendor;               // The company responsible for this GL implementation
char* GL_Renderer;             // The name of the type of renderer
char* GL_Version;              // The version and/or release number
int                  GL_EXT_Count;            // How many extensions are there?
char* GL_EXT_List[GL_MAX_EXT]; // Here's a list of their names
dboolean             GL_3Dlabs = false;

dboolean             software = false;

glmode_t             glmode;

float               glFovY;    // Rendering field of view
float                SetBack;   // 3D setback for 2D displays
float                glLeft, glTop, glRight, glBottom, glAspect;

// Windows stuff
extern int           iCurrMode;
//extern DEVMODE      *pModeList;

void ShutdownOpenGL(void)
{
    SDL_GL_MakeCurrent(NULL, NULL);
    SDL_GL_DeleteContext(glContext);
}

void I_ShutdownGraphics(void)
{
    ShutdownOpenGL();
}

//
// I_StartTic
//
void I_StartTic(void)
{
    // If the mouse has been captured (windowed mode)
    // return the mouse to the center of the screen here

    I_GetEvent();
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit(void)
{
    // what is this?
}

void GL_WriteText(int x, int y, char* msg);
extern int loadtime;
int        framecnt = 0;


//
// I_ReadScreen
//
void I_ReadScreen(dbyte* scr)
{
    memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
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
    MY_PAL* buffer;

    buffer = (MY_PAL*)W_CacheLumpName("PLAYPAL", PU_CACHE);
    memcpy(statpal, buffer, 768);
}

//
// Palette stuff.
//
//
// I_SetPalette
//
void I_SetPalette(dbyte* palette)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        gamepal[i].r = gammatable[usegamma][*palette++];
        gamepal[i].g = gammatable[usegamma][*palette++];
        gamepal[i].b = gammatable[usegamma][*palette++];
    }
}

void OGL_Error(int GL_Code, char* msg)
{
    char msgtext[128];
    char errmsg[128];
    switch (GL_Code)
    {
    default:
        sprintf(errmsg, "Unknown Error Code: %04X", GL_Code);
        strcpy(errmsg, msgtext);
        break;
    }

    lfprintf("GLERR: %s\n", errmsg);
    lfprintf("%s FAILED\n", msg);
}

//  StartUpOpenGL sets the pixel format and a rendering context then returns true/false
dboolean StartUpOpenGL()
{

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(pWindow);

    if (!glContext)
    {
        printf("Failed to create an OpenGL context!\n");
        I_Quit();
    }

    SDL_GL_MakeCurrent(pWindow, glContext);

    if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) < 0)
    {
        printf("Failed to load OpenGL library!\n");
        I_Quit();
    }

    R_InitRenderer();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glmode = gl_2d;

    GetGLInfo();

    return true;
}

void I_InitGraphics(void)
{

}

void GetGLInfo()
{
    /*if (gl_core)
        */return;

        char* szu, * tempstr;
        const GLubyte* szt;

        szt = glGetString(GL_VENDOR);
        GL_Vendor = (char*)malloc(strlen(szt) + 1);
        strcpy(GL_Vendor, szt);
        printf("OpenGL Vendor   : %s\n", GL_Vendor);
        if (strcasecmp(GL_Vendor, "3DLABS") == 0)
            GL_3Dlabs = true;

        szt = glGetString(GL_RENDERER);
        GL_Renderer = (char*)malloc(strlen(szt) + 1);
        strcpy(GL_Renderer, szt);
        printf("OpenGL Renderer : %s\n", GL_Renderer);

        //if ((D_strcasecmp(GL_Vendor, "Microsoft Corporation") == 0) &&
        //    (D_strcasecmp(GL_Renderer, "GDI Generic") == 0))
        //   {
        //    // oh CRAP, it's the dreaded Microsoft Software OpenGL Renderer...
        //    con_printf("SOFTWARE OpenGL Renderer!!!\n");
        //    software = true;
        //   }

        szt = glGetString(GL_VERSION);
        GL_Version = (char*)malloc(strlen(szt) + 1);
        strcpy(GL_Version, szt);
        printf("OpenGL Version  : %s\n", GL_Version);

        szt = glGetString(GL_EXTENSIONS);
        tempstr = (char*)malloc(strlen(szt) + 2);
        strcpy(tempstr, szt);
        if (tempstr != NULL)
        {
            szu = strtok((char*)tempstr, " ");
            GL_EXT_Count = 0;
            while ((szu != NULL) && (GL_EXT_Count < GL_MAX_EXT))
            {
                GL_EXT_List[GL_EXT_Count] = (char*)malloc(strlen(szu) + 1);
                strcpy(GL_EXT_List[GL_EXT_Count], szu);
                printf("OpenGL Extension : %s\n", GL_EXT_List[GL_EXT_Count]);
                szu = strtok(NULL, " ");
                GL_EXT_Count++;
            }
        }
        free(tempstr);
        printf("End of OpenGL extensions...\n");
}