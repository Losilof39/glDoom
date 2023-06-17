// d_input.h
// This is the defines and declarations for d_input.c
// 
#include "doomtype.h"

typedef enum { dev_keyboard, dev_mouse, dev_joystick, dev_devices } devtype_t;

// This function initializes all input devices
dboolean I_InitInputs(void);
// This function checks for input on all input devices
void I_CheckInputs(void);
// This function calls I_CheckInputs (vestigial)
void I_GetEvent(void);