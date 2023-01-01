// win_kbrd.h
// This file contains the keyboard defines and declarations for the
// keyboard interface for Win32 using DirectInput.

#include "doomtype.h"

// Releases the COM interface (DirectInput) to the keyboard
// and then shuts down DirectInput.
void I_ReleaseKeyboard(void);
// This starts up DirectInput and initializes the keyboard
// interface through DirectInput
dboolean I_SetupKeyboard(void);
// This checks the keyboard for input.
void I_CheckKeyboard(void);
