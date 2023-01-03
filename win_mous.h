// win_mous.h
// This file contains the defines and declarations
// for access to the mouse under Win32
#include "doomtype.h"

// This releases the SDL2 interface to the mouse
void I_ReleaseMouse(void);
// This sets up the SDL2 interface to the mouse
dboolean I_SetupMouse(void);
// This checks the mouse for input events
void I_CheckMouse(void);