// Emacs style mode select   -*- C++ -*- 
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
// DESCRIPTION:
//  Nil.
//    
//-----------------------------------------------------------------------------


#ifndef __M_ARGV__
#define __M_ARGV__

#define ARG_MAX 256

//
// MISC
//
extern  int	  myargc;
extern  char *myargv[ARG_MAX];

// Sets the value of the argument counter to 0
void M_InitParms(void);

// Returns the position of the given parameter
// in the arg list (0 if not found).
int  M_CheckParm (char* check);

// Adds an argument to the *my_argv[] list
int  M_AddParm( char *parm );

// Frees the storage for arguments loaded by M_AddParm
void M_FreeParms(void);

#endif
//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------
