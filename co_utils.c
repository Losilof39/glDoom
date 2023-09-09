/*
** File: co_utils.c
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 09/08/2023
** Description:
** Cleanup the utils used on co_console and other files
** as well.
** License: Public domain/ID License 
*/

#include <ctype.h>

#include "doomdef.h"
#include "doomstat.h"
#include "hu_stuff.h"
#include "r_defs.h"
#include "co_utils.h"

extern patch_t* hu_font[HU_FONTSIZE];

dboolean CO_isdigits(char* s)
{
    int i;

    for (i = 0; s[i]; i++)
        if (!isdigit(s[i]) && s[i] != '.')
            return false;
    return true;
}

/*
   This function cleans up sloppy command lines.
   It removes leading and trailing spaces from
   commands or parts of them
*/

char* CO_CleanUpCommand(char* command)
{
    char* tchar;
    size_t   i;

    tchar = command;

    for (i = (strlen(command) - 1); command[i] == ' '; i--)
        command[i] = '\0';
    for (i = 0; command[i] == ' '; i++)
        tchar = &command[i + 1];

    return tchar;
}

/* This function should return the width of the string in pixels. */
int CO_StringWidth(char* s)
{
    int           i, j;
    unsigned char c;

    j = i = 0;
    while (s[i])
    {
        c = toupper(s[i] - HU_FONTSTART);
        if ((c < 0) || (c >= HU_FONTSIZE))
            j += 4;
        else
            j += hu_font[c]->width;
        i++;
    }
    return j;
}

/* 
** This function should return the first character in the string that will
** allow the string to be printed from there to the end.
*/
int CO_StringPrintable(char* s)
{
    int           i, j;
    unsigned char c;

    j = SCREENWIDTH - (CO_StringWidth(s) + 5);
    i = 0;
    while (j < 0)
    {
        c = toupper(s[i] - HU_FONTSTART);
        if ((c < 0) || (c >= HU_FONTSIZE))
            j += 4;
        else
            j += hu_font[c]->width;
        i++;
    }
    return i;
}
