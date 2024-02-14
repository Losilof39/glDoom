/*
** File: co_utils.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 09/08/2023
** Description:
** Cleanup the utils used on co_console and other files
** as well.
** License: Public domain/ID License 
*/

#ifndef __CO_UTILS__
#define __CO_UTILS__

dboolean CO_isdigits(char* s);
char* CO_CleanUpCommand(char* command);
int CO_StringWidth(char* s);
int CO_StringPrintable(char* s);


#endif