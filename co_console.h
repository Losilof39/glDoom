// d_console.h
// This is the header file for the glDoom console

#include "doomtype.h"
#include "d_event.h"


#ifndef __DOOMCON__
#define __DOOMCON__

#define CONSOLE_NAME "M_DOOM"
#define CONSOLE_FLAT "CEIL5_1"
#define CONSOLE_BORD "FLAT18"
#define CONSBORD     2
#define CONSVERS     93
#define CONSMSGS     64

extern dboolean bConsoleActive;

void CO_Ticker(void);
dboolean CO_Responder(event_t* ev);
#if 0 //Unused.
void CO_Drawer(void);
#endif
void CO_Init(void);
void CO_AddConsoleMessage(char *s);
#if 0 //Unused.
void CO_WriteConsoleLine(int i, char *s, dboolean wrap);
#endif
#endif

