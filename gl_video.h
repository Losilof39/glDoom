// gl_video.h
// Data type and function declarations for gl_video.c

//#include <windows.h>
#include "doomtype.h"

#define GL_MAX_EXT     128                    // How many extensions will we allow?
#define VSYNC_ON	   1

extern SDL_Window* pWindow;

typedef enum { gl_2d, gl_3d } glmode_t;
dboolean StartUpOpenGL();
void OGL_Error( int, char * );
void GetGLInfo();
void I_Start2DFrame();

