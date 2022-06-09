// gl_video.h
// Data type and function declarations for gl_video.c

#include <windows.h>
#include "doomtype.h"

#define GL_MAX_EXT     128                    // How many extensions will we allow?
#define VSYNC_ON	   1

typedef enum { gl_2d, gl_3d } glmode_t;
dboolean StartUpOpenGL( HWND hWnd );
void OGL_Error( int, char * );
void GetGLInfo();

