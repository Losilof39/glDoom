// sys_win.h - OpenGL adaptation of Doom
//
// Windows support data structures and definitions
#include <windows.h>

typedef struct
   {
    HWND      hWnd;
    HINSTANCE hInstance;
    int       iCmdShow;
    BOOL      bChangeBPP;
    HACCEL    hAccel;
    int       iVidMode;
    int       argc;
    char    **argv;
    HWND      hWndgdi;
    FARPROC   wndproc;
   }windata_t;


void dprintf(char *message,...);
void cprintf(char *message,...);
