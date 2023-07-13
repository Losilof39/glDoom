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
// DESCRIPTION:
//	Networking stuff.
//
//-----------------------------------------------------------------------------


#ifndef __D_NET__
#define __D_NET__

#include "d_player.h"


#ifdef __GNUG__
#pragma interface
#endif


//
// Network play related stuff.
// There is a data struct that stores network
//  communication related stuff, and another
//  one that defines the actual packets to
//  be transmitted.
//

#define DOOMCOM_ID		0x12345678l

// Max computers/players in a game.
#define MAXNETNODES		8


// Networking and tick handling related.
#define BACKUPTICS		12

typedef enum
{
    CMD_SEND	= 1,
    CMD_GET	= 2

} command_t;


//
// Network packet data.
//
typedef struct
{
    // High bit is retransmit request.
    unsigned		checksum;
    // Only valid if NCMD_RETRANSMIT.
    byte		retransmitfrom;
    
    byte		starttic;
    byte		player;
    byte		numtics;
    ticcmd_t		cmds[BACKUPTICS];

} doomdata_t;




typedef struct
{
    // Supposed to be DOOMCOM_ID?
    int		id;
    
    // DOOM executes an int to execute commands.
    short		intnum;		
    // Communication between DOOM and the driver.
    // Is CMD_SEND or CMD_GET.
    short		command;
    // Is dest for send, set by get (-1 = no packet).
    short		remotenode;
    
    // Number of bytes in doomdata to be sent
    short		datalength;

    // Info common to all nodes.
    // Console is allways node 0.
    short		numnodes;
    // Flag: 1 = no duplication, 2-5 = dup for slow nets.
    short		ticdup;
    // Flag: 1 = send a backup tic in every packet.
    short		extratics;
    // Flag: 1 = deathmatch.
    short		deathmatch;
    // Flag: -1 = new game, 0-5 = load savegame
    short		savegame;
    short		episode;	// 1-3
    short		map;		// 1-9
    short		skill;		// 1-5

    // Info specific to this node.
    short		consoleplayer;
    short		numplayers;
    
    // These are related to the 3-display mode,
    //  in which two drones looking left and right
    //  were used to render two additional views
    //  on two additional computers.
    // Probably not operational anymore.
    // 1 = left, 0 = center, -1 = right
    short		angleoffset;
    // 1 = drone
    short		drone;		

    // The packet data to be sent.
    doomdata_t		data;
    
} doomcom_t;



// Create any new ticcmds and broadcast to other players.
void NetUpdate (void);

// Broadcasts special packets to other players
//  to notify of game exit
void D_QuitNetGame (void);

//? how many ticks to run?
void TryRunTics (void);

// These are the game type flags
// There can be up to 32 flags in the DLONG (32 bits)
// Since the flag is NOT an integer value but is instead
// a binary 32 bit field, it "shouldn't" need to be SWABBED.
typedef enum
   {
    gf_deathmatch   = 0x1,  // use deathmatch finale screens
    gf_altdeath     = 0x3,  // respawn items
    gf_nomonsters   = 0x4,  // no monsters in game
    gf_respawnparm  = 0x8,  // nightmare mode
    gf_allowcheat   = 0x10, // allow cheat codes in game?
    gf_allowjump    = 0x20, // allow jumping in game
    gf_allowcrouch  = 0x40, // allow crouching in game
    gf_allowother   = 0x80  // allow something else...
   }game_flags_t;

typedef enum
   {
    gr_deny   = 0x1,
    gr_accept = 0x2
   }game_reply_t;

typedef enum
   {
    gd_position    = 0x1,
    gd_direction   = 0x2,
    gd_speed       = 0x4,
    gd_weapons     = 0x8,
    gd_health      = 0x10,
    gd_armor       = 0x20,
    gd_ammo        = 0x40,
    gd_powerups    = 0x80,
    gd_kills       = 0x100,
    gd_frags       = 0x200,
    gd_time        = 0x400
   }game_data_t;

typedef struct
   {
    uint64_t         dwPacketID;
    uint64_t         dwReturnAddress;
    uint64_t         dwMessageType;
    uint64_t         dwMessageSize;
    unsigned char cMessage[];
   }net_packet_t;

// Message for requesting to connect to a game
// The game flags is 32 bit field that uses game_flags_t
typedef struct
   {
    uint64_t  dwGameFlags;   // see game_flags_t
   }game_req_t;

typedef struct
   {
    uint64_t  dwGameReply;
    uint64_t  dwGameFlags;
    uint64_t  dwNumPlayers;
    uint64_t  dwIPAddress[];
   }game_rep_t;

typedef struct
   {
    uint64_t  dwDataRequest;
   }status_req_t;

typedef struct
   {
    uint64_t  dwForwardMove;
    uint64_t  dwSideMove;
    uint64_t  dwAngleTurn;
    uint64_t  dwConsistancy;
    uint64_t  dwChatChar;
    uint64_t  dwButtons;
   }tic_data_t;

typedef struct
   {
    uint64_t      dwCheckSum;
    uint64_t      dwPlayer;
    uint64_t      dwRetransmitFrom;
    uint64_t      dwStartTic;
    uint64_t      dwNumTics;
    tic_data_t TicData[];
   }tic_header_t;

typedef enum
   {
    game_req,
    game_rep,
    status_req,
    status_rep,
    tic_update,
    net_msg_end
   }net_message_t;

#endif

//-----------------------------------------------------------------------------
//
// $Log:$
//
//-----------------------------------------------------------------------------

