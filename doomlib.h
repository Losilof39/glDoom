//
// doomlib.h
//
// 28 Nov, 1998 - Bruce Lewis
//
// This source file contains functions that are not supported by the
// ANSI 'C' specification but which appear in some form in most
// implementations.  These are provided to make the software more
// portable by using only ANSI standard library functions.

char *D_strupper( char *str );
char *D_strlower( char *str );
#ifdef ANSI_C
int strncasecmp(char *s1, char *s2, int count);
int strcasecmp(char *s1, char *s2);
#endif