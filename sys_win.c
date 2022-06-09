/////////////////////////////////////////////////////////////////////////////////////
// Windows Includes...
/////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <winuser.h>
#include <zmouse.h>
#include <time.h>
#include <GL/gl.h>     // OpenGL interface
#include <GL/glu.h>    // OpenGL utility Library interface
#include <SDL.h>

/////////////////////////////////////////////////////////////////////////////////////
// DirectX Includes...
/////////////////////////////////////////////////////////////////////////////////////
#include <direct.h>
#include <dinput.h>
#include <dsound.h>

/////////////////////////////////////////////////////////////////////////////////////
// Application Includes...
/////////////////////////////////////////////////////////////////////////////////////
#include "resource.h"  // Required for Win32 Resources
#include "sys_win.h"
#include "gconsole.h"

/////////////////////////////////////////////////////////////////////////////////////
// "GLDoom" Includes...
/////////////////////////////////////////////////////////////////////////////////////
#include "doomdef.h"
#include "doomstat.h"
#include "d_event.h"
#include "d_main.h"
#include "g_game.h"
#include "m_argv.h"
#include "i_system.h"
#include "m_music.h"
#include "i_cd.h"
#include "i_midi.h"
#include "st_lib.h"
#include "dxerr.h"
#include "v_video.h"
#include "z_zone.h"
#include "gldefs.h"
#include "r_main.h"

#include "gl_video.h"
#include "mathlib.h"
#include "win_inpt.h"
#include "win_video.h"

/////////////////////////////////////////////////////////////////////////////////////
// OpenGL Defines and Data
/////////////////////////////////////////////////////////////////////////////////////

extern HDC    hGDC;
extern HGLRC  hRC;
extern BOOL   software;

// Functions
void   I_ShutdownGraphics( void );

// Data

FILE       *dbg_file;

int             SkyWide, SkyHigh, GLSkyTex;

extern int       iCurrMode;

/////////////////////////////////////////////////////////////////////////////////////
// Application Defines and Data
/////////////////////////////////////////////////////////////////////////////////////

windata_t WinData;

extern devinfo_t DevInfo;

#define MUSIC_NONE      0
#define MUSIC_CDAUDIO   1
#define MUSIC_MIDI      2

#define RENDER_GL       0
#define RENDER_D3D      1

#define KS_KEYUP        0
#define KS_KEYDOWN    255

/////////////////////////////////////////////////////////////////////////////////////
// Game states -- these are the modes in which the outer game loop can be
/////////////////////////////////////////////////////////////////////////////////////
#define GAME_START  0
#define GAME_SPLASH 1
#define GAME_MENU   2
#define GAME_PLAY   3
#define GAME_EXIT   4
#define GAME_QUIT   5
#define GAME_LIMBO  6
#define GAME_PAUSE  7

int     GameMode = GAME_START;

extern  int  version;
extern  char revision;

extern byte *screens[5];
extern GameMode_t gamemode;

char         szMsgText[2048];

extern devinfo_t DevInfo;

extern       CD_Data_t   CDData;
extern       MIDI_Data_t MidiData;
extern       video_t     video;

char        szMidiFile[] = "doomsong.mid";

int         MusicType = MUSIC_MIDI;
int         RenderType = RENDER_GL;
BOOL        bQuit = FALSE;

void  Cleanup(void);
void  InitData(HINSTANCE, int);
void  ParseCommand(PSTR);
void  EvaluateParameters(PSTR);
BOOL  CreateMainWindow( int, int, int, BOOL);

void ClearLog(char *szFileName);
void lfprintf(char *message, ... );

int   glDoomAC;
char *glDoomAV[256];

void  CheckInputs(void);

void  D_DoomMain(void);
void  MY_DoomSetup(void);
void  MY_DoomLoop(void);
void  glDoomExit(void);

unsigned char szBadWadMessage[] = { "glDoom is unable to determine the game type\n"
                                    "of the game WAD file. Please make sure that:\n"
                                    "1. You have your game wad in the current directory,\n"
                                    "2. You have it named one of the standard wad names OR\n"
                                    "   you have used the -game parameter to specify it,\n"
                                    "3. That you have not moved the game without changing\n"
                                    "   its location in the gldoom.ini file in your windows\n"
                                    "   system directory." };
void TestAlt(void);

POINT mPoint;

int   tvalue = 0;
BOOL  notop = FALSE;

/////////////////////////////////////////////////////////////////////////////////////
// Windows Defines and Data
/////////////////////////////////////////////////////////////////////////////////////

extern int    keylink;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char      szAppName[] = "glDoom - v0.96c";
char      szDbgName[] = "glDoom.dbg";
char      szCfgName[] = "glDoom.cfg";
HACCEL    ghAccel;
HICON     g_hIcon = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
   {
    MSG         msg;
    HWND        hwnd;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("Failed to init SDL");

    if ((hwnd = FindWindow(szAppName, szAppName)) != NULL)
       {
        SetForegroundWindow(hwnd);
        return 0;
       }

    g_hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GLDOOM));

    ClearLog(szDbgName);

    // parse up the command line...
    ParseCommand(szCmdLine);

    FindResponseFile();

    InitData(hInstance, iCmdShow);

    // look at the command line parameters
    EvaluateParameters(szCmdLine);
    
    // why would you create a "temp window" ???
  
    //CreateTempWindow();
    //ChangeDisplaySettings(0, 0);

    // We get the current video setup here.
    GetVideoInfo();

    DestroyWindow(WinData.hWnd);
    WinData.hWnd = NULL;

    lfprintf("Current resolution: %d x %d x %d bpp\n",DevInfo.width,DevInfo.height,DevInfo.bpp);

    lfprintf("Resolution requested: %d x %d x %d bpp\n",video.width,video.height,video.bpp);

    // This builds up the list of available video modes for the OpenGL renderer
    GetModeList(szDbgName);
    if (video.fullscreen == TRUE)
       {
        if ((iCurrMode == -1) && ((video.width != DevInfo.width) || (video.height != DevInfo.height) || (video.bpp != DevInfo.bpp)))
           {
            MessageBox(hwnd, "The video resolution you have requested is not available.", "Bad Video Mode", MB_OK);
            Cleanup();
            return 0;
           }
       }

    if (video.fullscreen == TRUE)
       {
        if (SetVideoMode() == FALSE)
           {
            Cleanup();
            return 0;
           }
       }

    // Create the main program window, start up OpenGL and create our viewport
    if (CreateMainWindow( video.width, video.height, video.bpp, video.fullscreen) != TRUE)
       {
        ChangeDisplaySettings(0, 0);
        MessageBox(NULL, "Unable to create main window.\nProgram will now end.", "FATAL ERROR", MB_OK);
        Cleanup();
        return 0;
       }

    if (video.fullscreen == FALSE)
       {
        mPoint.x = video.width/2;
        mPoint.y = video.height/2;
        ClientToScreen( WinData.hWnd, &mPoint);
        SetCursorPos(mPoint.x, mPoint.y);
        SetCapture(WinData.hWnd);
        con_printf("Display: %dx%d\n", video.width, video.height);
       }
    else
       {
        con_printf("Display: %dx%dx%d bpp\n", video.width, video.height, video.bpp);
       }

    bQuit = FALSE;

    con_printf("Beginning DOOM code startup...\n");
    D_DoomMain();
    if ((gamemode == netabort) || (gamemode == undetermined))
       {
        I_Quit();
        //StopMusic();
        //I_ShutdownGraphics();
        if (gamemode == undetermined)
           {
            MessageBox(hwnd, szBadWadMessage, "glDoom Game Data Error", MB_OK);
           }
        return 0;
       }

    con_printf("Command line: %s\n", szCmdLine);
    con_printf("Beginning DOOM data setup...\n");
    MY_DoomSetup();

    GameMode = GAME_START;

    con_printf("Starting game loop...\n");
    while (!bQuit)
	   {

        // check for user input
        I_GetEvent();

        if (GameMode == GAME_PLAY)
           MY_DoomLoop();
        else
        if (GameMode == GAME_QUIT)
           I_Quit();
        else
        if (GameMode == GAME_START)
           GameMode = GAME_PLAY;
       }
    Cleanup();
    return msg.wParam;
   }

BOOL ResizeMainWindow(char *mode)
   {
    char *parm;
    int   width, height;
    int   t_width, t_height;
    int   x, y, sx, sy, ex, ey;

    if ((parm = strtok(mode, " ")) != NULL)
       {
        width = atoi(parm);
        if ((parm = strtok(NULL, " ")) != NULL)
           {
            height = atoi(parm);
            t_width = video.width;
            t_height = video.height;
            video.width = width;
            video.height = height;
            if (video.fullscreen == TRUE)
               {
                // can't do this right without shutting down OpenGL
                // and restarting it.  But don't want to reload ALL
                // the game graphics used so far.  Later on maybe...
                return FALSE;
//                wglMakeCurrent(NULL, NULL);
//                ReleaseDC(WinData.hWnd, hGDC);
                if (SetVideoMode() == FALSE)
                   {
                    video.width = t_width;
                    video.height = t_height;
                   }
                else
                   {
                    x = y = 0;
                    sx = video.width;
                    sy = video.height;
                    MoveWindow(WinData.hWnd, x, y, sx, sy, TRUE);
                    UpdateWindow(WinData.hWnd);
                    SetForegroundWindow(WinData.hWnd);
                    SetFocus(WinData.hWnd);
                    return TRUE;
                   }
//                hGDC = GetDC(WinData.hWnd);
//                would need to release rendering context here
//                and create new one then reload all GL graphics... ugh...
//                wglMakeCurrent(hGDC, hRC);
               }
            else
               {
                ex = GetSystemMetrics(SM_CXFIXEDFRAME)*2;
                ey = (GetSystemMetrics(SM_CYFIXEDFRAME)*2)+GetSystemMetrics(SM_CYCAPTION);
                // Center the window on the screen
                x = (DevInfo.width / 2) - ((video.width+ex) / 2);
                y = (DevInfo.height / 2) - ((video.height+ey) / 2);
                sx = video.width+ex;
                sy = video.height+ey;
                /*
                  Check to be sure the requested window size fits on the screen and
                  adjust each dimension to fit if the requested size does not fit.
                */
                if ((sx <= DevInfo.width) && (sy <= DevInfo.height))
                   {
                    MoveWindow(WinData.hWnd, x, y, sx, sy, TRUE);
                    return TRUE;
                   }
                else
                   {
                    video.width = t_width;
                    video.height = t_height;
                   }
               }
            R_InitViewData();
           }
       }
    return FALSE;
   }


// Make sure the cursor is visible and release
// the memory used for the parameter list
void Cleanup()
   {
    int  i;
    char DoomDir[_MAX_PATH];
    char tstr[16];

    // Write out the current video resolution to re-use next time
    GetProfileString("GLDOOM", "DIRECTORY", "", DoomDir, _MAX_PATH );
    if (strlen(DoomDir) != 0)
       {
        sprintf(tstr, "%d", video.width);
        WriteProfileString("GLDOOM", "width", tstr);
        sprintf(tstr, "%d", video.height);
        WriteProfileString("GLDOOM", "height", tstr);
       }

    ShowCursor(TRUE);
    if (video.fullscreen == FALSE)
       {
        ReleaseCapture();
       }

    for (i = 4; i >= 0; i--)
        free(screens[i]);
    M_FreeParms();
   }


BOOL CreateMainWindow(int width, int height, int bpp, BOOL fullscreen)
{
    WNDCLASSEX  wndclass;
    DWORD       dwStyle, dwExStyle;
    int         x, y, sx, sy, ex, ey;

    wndclass.cbSize        = sizeof (wndclass);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc   = (WNDPROC)WinData.wndproc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = WinData.hInstance;
    wndclass.hIcon         = g_hIcon;
    wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = (void *)COLOR_GRAYTEXT;
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm       = 0;


    if (!RegisterClassEx (&wndclass))
    {
        MessageBox(NULL, "Window class registration failed.", "FATAL ERROR", MB_OK);
        return FALSE;
    }

    if (fullscreen)
    {
        if (notop)
            dwExStyle = WS_EX_TOPMOST;
   
        dwExStyle = 0;
        dwStyle = WS_POPUP | WS_VISIBLE;
        x = y = 0;
        sx = video.width;
        sy = video.height;
    }
    else
    {
        dwExStyle = 0;
        //dwStyle = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;  // Use this if you want a "normal" window
        dwStyle = WS_CAPTION;
        ex = GetSystemMetrics(SM_CXFIXEDFRAME)*2;
        ey = (GetSystemMetrics(SM_CYFIXEDFRAME)*2)+GetSystemMetrics(SM_CYCAPTION);
        // Center the window on the screen
        x = (DevInfo.width / 2) - ((video.width+ex) / 2);
        y = (DevInfo.height / 2) - ((video.height+ey) / 2);
        sx = video.width+ex;
        sy = video.height+ey;
        //   Check to be sure the requested window size fits on the screen and
        //   adjust each dimension to fit if the requested size does not fit.
        if (sx >= DevInfo.width)
        {
            x = 0;
            sx = DevInfo.width-ex;
        }
        if (sy >= DevInfo.height)
        {
            y = 0;
            sy = DevInfo.height-ey;
        }
    }

    if ((WinData.hWnd = CreateWindowEx (dwExStyle,
                    szAppName,               // window class name
		            szAppName,               // window caption
                    dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // window style
                    x,           // initial x position
                    y,           // initial y position
                    sx,           // initial x size
                    sy,           // initial y size
                    NULL,                    // parent window handle
                    NULL,                    // window menu handle
                    WinData.hInstance,       // program instance handle
		            NULL))                   // creation parameters
                    == NULL)
    {
        ChangeDisplaySettings(0, 0);
        MessageBox(NULL, "Window creation failed.", "FATAL ERROR", MB_OK);
        return FALSE;
    }

    SendMessage(WinData.hWnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hIcon);
    SendMessage(WinData.hWnd, WM_SETICON, ICON_BIG, (LPARAM)g_hIcon);

    WinData.hAccel = LoadAccelerators(WinData.hInstance,"AppAccel");

    R_InitViewData();

    con_printf("glDoom Version %d.%d%c\n", version/100, version%100, revision);
    con_printf("Starting OpenGL...\n");
    ShowCursor(FALSE);
    if (StartUpOpenGL(WinData.hWnd) == FALSE)
    {
        WinData.hWnd = NULL;
        return FALSE;
    }

    if ((software == TRUE) && (video.allowsoft == FALSE))
    {
        I_ShutdownGraphics();
        return FALSE;
    }

    ShowWindow(WinData.hWnd, WinData.iCmdShow);
    UpdateWindow(WinData.hWnd);

    SetForegroundWindow(WinData.hWnd);
    SetFocus(WinData.hWnd);

    return TRUE;
}


void InitData(HINSTANCE hInstance, int iCmdShow)
   {
    char  DoomDir[_MAX_PATH];
	int   iWidth, iHeight;
	float fRatio = 0.0f;

    // Set the "Doom" screen variables to default
    SCREENWIDTH = 320;
    SCREENHEIGHT = 200;

    video.nearclip = 1.5f;
    video.farclip = 5000.0f;

    // Setup CD sub-system
    CDData.CDStatus = cd_empty;
    CDData.CDMedia = FALSE;
    CDData.CDPosition = 0;
    CDData.CDCode[0] = '\0';

    // Capture Windows variables
    WinData.hInstance  = hInstance;
    WinData.iCmdShow   = iCmdShow;
    WinData.wndproc    = (FARPROC)WndProc;

    // Set user "definable" data
    video.allowsoft     = FALSE;
    video.width         = DEF_WIDTH;
    video.height        = DEF_HEIGHT;
    video.bpp           = DEF_COLORB;
    video.fullscreen    = FALSE;
	video.wide          = FALSE;

	iWidth = GetSystemMetrics(SM_CXSCREEN);
	iHeight = GetSystemMetrics(SM_CYSCREEN);

    // See if these need to be overridden by the defaults
    GetProfileString("GLDOOM", "DIRECTORY", ".", DoomDir, _MAX_PATH );
    if (strlen(DoomDir) != 0)
       {
        video.width  = GetProfileInt("GLDOOM", "width", DEF_WIDTH);
        video.height = GetProfileInt("GLDOOM", "height", DEF_HEIGHT);
       }

	fRatio = (float)((double)iHeight / (double)iWidth);

	if  ((0.801f >= fRatio) && (0.749 <= fRatio))
	{
		video.wide = FALSE;
	}
	else
	{
		video.wide = TRUE;
	}

    video.bpp  = DEF_COLORB;
    video.fov  = 90;
    video.fovy = ml_MakeFovY(video.fov, (float)video.width / (float)video.height);
}


// could just do myargc = __argc and myargv = __argv
void ParseCommand(PSTR szCmdLine)
   {
    char* s;

    M_InitParms();
    
    M_AddParm(__argv[0]);

    s = strtok(szCmdLine, " ");
    while (s != NULL)
       {
        M_AddParm(s);
        s = strtok(NULL, " ");
       }
   }

void EvaluateParameters(PSTR szCmdLine)
   {
    int  p;

    if (myargc < 2)
       {
        return;
       }

    p = M_CheckParm("-notop");
    if (p != 0)
       {
        notop = TRUE;
       }

    p = M_CheckParm("-window");
    if (p != 0)
       {
        video.fullscreen = FALSE;
       }

    p = M_CheckParm("-width");
    if (p && p < myargc-1)
       {
        video.width = atoi(myargv[p+1]);
       }

    p = M_CheckParm("-height");
    if (p && p < myargc-1)
       {
        video.height = atoi(myargv[p+1]);
       }

    p = M_CheckParm("-bpp");
    if (p && p < myargc-1)
       {
        video.bpp = atoi(myargv[p+1]);
        if (video.bpp != 15 && video.bpp != 16 && video.bpp != 24 && video.bpp != 32)
           {
            video.bpp = 16;
           }
       }

    p = M_CheckParm("-allowsoft");
    if (p != 0)
       {
        video.allowsoft = TRUE;
       }

    p = M_CheckParm("-wide");
    if (p != 0)
       {
        video.wide = TRUE;
       }

	// 04/03/2010 - BAL Need to be able to support wide monitors
	if (TRUE != video.wide)
	{
	    if ((int)(video.width * 3) != (int)(video.height * 4))
		{
			con_printf("Invalid aspect ratio requested...resetting to %dx%d.\n", DEF_WIDTH, DEF_HEIGHT);
			video.width = DEF_WIDTH;
			video.height = DEF_HEIGHT;
		}
	}

    p = M_CheckParm("-fov");
    if (p && p < myargc-1)
    {
        video.fov = atof(myargv[p+1]);
        if (video.fov < FOV_MIN || video.fov > FOV_MAX)
            video.fov = FOV_DEF;

    }

    video.fovy = ml_MakeFovY(video.fov, (float)video.width / (float)video.height);
}

void glDoomExit()
   {
    GameMode = GAME_LIMBO;
    if (demorecording)
       {
        if (demotype == DEMO_I)
           G_EndDemo();
        else
           G_EndDemo_II();
       }
    I_ShutdownInputs();
    StopMusic();
    I_ShutdownGraphics();
    SendMessage(WinData.hWnd, WM_CLOSE, 0, 0);
   };

LRESULT CALLBACK DummyProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
   {
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
   }

extern dboolean paused;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
   {
    static event_t       event;
    static unsigned char KeyPress;
    static int           scancode;
    static int           lastmousex = 0, lastmousey = 0;
    static BOOL          first = TRUE;
    static BOOL          waspaused = FALSE;

    switch(iMsg)
       {
        case WM_CREATE:
             tvalue = 1;
             con_setup(hwnd, video.width, video.height);
             break;

        case MM_MCINOTIFY:
             if (wParam == MCI_NOTIFY_SUCCESSFUL)
                {
                 if (MidiData.MidiStatus == midi_play)
                    MidiReplay(hwnd, &MidiData);
                 if (CDData.CDStatus == cd_play)
                    CDTrackPlay(hwnd, &CDData);
                }
             if (wParam == MCI_NOTIFY_FAILURE)
                {
                 if (CDData.CDStatus == cd_play)
                    {
                     MidiPlay(hwnd, &MidiData);
                     CDData.CDStatus == cd_stop;
                    }
                }
             break;

       case WM_MOUSEWHEEL:
            lfprintf("WM_MOUSEWHEEL message...\n");
            return 0;
            break;

       case WM_KEYDOWN:
            if ((lParam & 0x40000000) != 0)  // This "debounces" the keys so that we only process
               break;                        // the message when the key is first pressed and not after.

            switch(wParam)
               {
                case VK_PAUSE:
                     event.type = ev_keydown;
                     event.data1 = KEY_PAUSE;
                     D_PostEvent(&event);
                     break;
                case VK_SHIFT:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keydown;
                         event.data1 = DIK_RSHIFT;
                         D_PostEvent(&event);
                        }
                     break;
                case VK_CONTROL:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keydown;
                         event.data1 = DIK_RCONTROL;
                         D_PostEvent(&event);
                        }
                     break;
                case VK_MENU:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keydown;
                         event.data1 = DIK_RMENU;
                         D_PostEvent(&event);
                        }
                     break;
               }
            break;

       case WM_ACTIVATE:
            if (LOWORD(wParam) != WA_INACTIVE)
               {
                if ((!first) && (!waspaused))
                   {
                    event.type = ev_keydown;
                    event.data1 = KEY_PAUSE;
                    D_PostEvent(&event);
                   }
                first = FALSE;
                SetForegroundWindow( WinData.hWnd );
                SetFocus( WinData.hWnd );
                ShowWindow( WinData.hWnd, SW_RESTORE );
                UpdateWindow( WinData.hWnd );
               }
            else
               {
                waspaused = paused;
                if (!paused)
                   {
                    event.type = ev_keydown;
                    event.data1 = KEY_PAUSE;
                    D_PostEvent(&event);
                   }
                if ( video.fullscreen )
                    ShowWindow( WinData.hWnd, SW_MINIMIZE );
               }
            break;

       case WM_KEYUP:
            switch(wParam)
               {
                case VK_PAUSE:
                     event.type = ev_keyup;
                     event.data1 = KEY_PAUSE;
                     D_PostEvent(&event);
                     break;
                case VK_SHIFT:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keyup;
                         event.data1 = DIK_RSHIFT;
                         D_PostEvent(&event);
                        }
                     break;
                case VK_CONTROL:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keyup;
                         event.data1 = DIK_RCONTROL;
                         D_PostEvent(&event);
                        }
                     break;
                case VK_MENU:
                     if (keylink == TRUE)
                        {
                         event.type = ev_keyup;
                         event.data1 = DIK_RMENU;
                         D_PostEvent(&event);
                        }
                     break;
               }
            break;

        case WM_DESTROY:
             con_shutdown();
             PostQuitMessage(0);
             return 0;
       }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
   }

void ClearLog(char *szFileName)
   {
    FILE       *fn;

    fn = fopen(szFileName, "w");
    fclose(fn);
   }

void lfprintf(char *message, ... )
   {
    va_list	argptr;
    FILE *log_file;

    log_file = fopen(szDbgName, "a+");
    va_start (argptr,message);
    vfprintf (log_file,message,argptr);
    va_end (argptr);
    fclose(log_file);
   }

char cmsg[4096];

void dprintf(char *message, ... )
   {
    va_list	argptr;

    va_start (argptr,message);
    vfprintf (dbg_file,message,argptr);
    va_end (argptr);
   }
