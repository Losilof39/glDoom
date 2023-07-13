// Emacs style mode select   -*- C -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: m_argv.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

#include <string.h>
#include <malloc.h>
#include "doomtype.h"
#include "m_argv.h"

// need to change **myargv to a fixed allocation of pointers
// 256 "should" be plenty. No "clean" way to allocate additional
// argument pointers once the initial lot have been allocated.

int     myargc;
char   *myargv[ARG_MAX];

//
// M_CheckParm
// Checks for the given parameter
// in the program's command line arguments.
// Returns the argument number (1 to argc-1)
// or 0 if not present

void M_InitParms()
   {
    myargc = 0;
   }

int M_CheckParm (char *check)
   {
    int		i;

    for (i = 1;i<myargc;i++)
    {
	if ( !strcasecmp(check, myargv[i]) )
	    return i;
    }

    return 0;
   }

int M_AddParm(char *newarg)
   {
    if (myargc < ARG_MAX)
       {
        myargv[myargc] = (char *)malloc(strlen(newarg)+1);
        strcpy(myargv[myargc], newarg);
        myargc++;
        return myargc;
       }
    else
       {
        return -1;
       }
   }

void M_FreeParms()
   {
    while (myargc > 0)
       {
        myargc--;
        free(myargv[myargc]);
       }
   }


