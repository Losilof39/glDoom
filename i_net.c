//// Emacs style mode select   -*- C++ -*- 
////-----------------------------------------------------------------------------
////
//// $Id:$
////
//// Copyright (C) 1993-1996 by id Software, Inc.
////
//// This source is available for distribution and/or modification
//// only under the terms of the DOOM Source Code License as
//// published by id Software. All rights reserved.
////
//// The source is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
//// for more details.
////
//// $Log:$
////
//// DESCRIPTION:
////
////-----------------------------------------------------------------------------
//
static const char
rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";

//#include <process.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#else
#include <inttypes.h>
#include <unistd.h>
#endif
#include <fcntl.h>

//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <errno.h>
//#include <unistd.h>
//#include <netdb.h>
//#include <sys/ioctl.h>

#include "doomtype.h"
#include "i_system.h"
#include "d_event.h"
#include "d_net.h"
#include "m_argv.h"

#include "doomstat.h"

#ifdef __GNUG__
#pragma implementation "i_net.h"
#endif
#include "i_net.h"

//
//static struct sockaddr broadcastaddr;
//
//static int controlsocket;
//static int broadcastsocket = 0;
//
//// For some odd reason...
//#define ntohl(x) \
//        ((unsigned long int)((((unsigned long int)(x) & 0x000000ffU) << 24) | \
//                             (((unsigned long int)(x) & 0x0000ff00U) <<  8) | \
//                             (((unsigned long int)(x) & 0x00ff0000U) >>  8) | \
//                             (((unsigned long int)(x) & 0xff000000U) >> 24)))
//
//#define ntohs(x) \
//        ((unsigned short int)((((unsigned short int)(x) & 0x00ff) << 8) | \
//                              (((unsigned short int)(x) & 0xff00) >> 8))) \
//	  
//#define htonl(x) ntohl(x)
//#define htons(x) ntohs(x)
//
void	NetSend (void);
//dboolean NetListen (void);
//
//void lfprintf(char *message, ... );
//
////
//// NETWORKING
////
//
//DWORD dwPacketID;
//DWORD dwMessageID;
//
//// offset into the "message" that contains the command/request
//#define MSG_OFF   0
//// offset into the "message" where the actual message starts (PACKETID, MESSAGEID, etc.)
//#define BODY_OFF  4
//
//// FIXME
//#define IPPORT_USERRESERVED 26000
//
//int	DOOMPORT =	(IPPORT_USERRESERVED +0x1d );
//
//int			sendsocket;
//int			insocket;
//
//struct	sockaddr_in	sendaddress[MAXNETNODES];
//
//void	(*netget) (void);
//void	(*netsend) (void);
//
//extern char ipaddress[4][16];
//extern int  numplayers;
//
////
//// UDPsocket
////
//int UDPsocket (void)
//   {
//    int	 s;
//    char MsgText[256];
//	
//    // allocate a socket
//    s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (s < 0)
//       {
//        sprintf(MsgText, "ERROR:Can't create socket:%08X %s\n", errno, strerror(errno));
//        I_Error(MsgText);
//       }
//		
//    return s;
//   }
//
////
//// BindToLocalPort
////
//void BindToLocalPort( int s, int port )
//   {
//    int			v;
//    struct sockaddr_in	address;
//	
//    memset (&address, 0, sizeof(address));
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = port;
//
//    v = bind(s, (void *)&address, sizeof(address));
//    if (v == -1)
//        I_Error ("BindToPort: bind: %s", strerror(errno));
//   }
//
//
//char *AddressToString(struct sockaddr_in *addr)
//   {
//    static char string[32];
//    int         ip;
//
//    ip = ntohl(addr->sin_addr.s_addr);
//    sprintf(string, "%d.%d.%d.%d:%d", (ip & 0xFF), ((ip >> 8) & 0xFF), ((ip >> 16) & 0xFF), ((ip >> 24) & 0xFF), ntohs(addr->sin_port));
//    return string;
//   }
//
//int StringToAddress(char *string, struct sockaddr_in *addr)
//   {
//    int         ip1, ip2, ip3, ip4, port;
//    DWORD       ipaddr;
//
//    if (strrchr(string, ':') != NULL)
//       {
//        sscanf(string, "%d.%d.%d.%d:%d", &ip1, &ip2, &ip3, &ip4, &port);
//        addr->sin_port = htons(port);
//       }
//    else
//       {
//        sscanf(string, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
//       }
//
//    ipaddr = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;
//
//    addr->sin_family = AF_INET;
//    addr->sin_addr.s_addr = htonl(ipaddr);
//
//	return 1;
//   }
//
//// This takes a possibly partial IP address and substitutes the
//// digits from the local host IP for the missing digits
//dboolean BuildIPAddress(char *oldaddr, char *newaddr)
//   {
//    int    triplet = 3, i, s, d, ip;
//    char   tc[4][4], hostaddr[24], htc[4][4];
//    char   hostname[256];
//    struct hostent *hostentry;
//
//    con_printf("Test address : %s\n", oldaddr);
//    for (i = 3, s = (strlen(oldaddr)-1); (i >= 0); i--)
//       {
//        strcpy(tc[i], "   ");
//        d = 2;
//        while ((s >= 0) && (d >= 0) && (oldaddr[s] != '.'))
//           {
//            if ((oldaddr[s] >= '0') && (oldaddr[s] <= '9'))
//               {
//                tc[i][d--] = oldaddr[s];
//               }
//            s--;
//           }
//        if ((s > 0) && (oldaddr[s] == '.'))
//           {
//            s--;
//           }
//       }
//    con_printf("Expanded Address : %s.%s.%s.%s\n", tc[0], tc[1], tc[2], tc[3]);
//    gethostname(hostname, 255);
//    con_printf("Local host name: %s\n", hostname);
//    hostentry = gethostbyname(hostname);
//    if (hostentry)
//       {
//        ip = *(int *)hostentry->h_addr_list[0];
//        sprintf(htc[0], "%3d",  (ip & 0xFF));
//        sprintf(htc[1], "%3d", ((ip >> 8) & 0xFF));
//        sprintf(htc[2], "%3d", ((ip >> 16) & 0xFF));
//        sprintf(htc[3], "%3d", ((ip >> 24) & 0xFF));
//        con_printf("Local address: %s.%s.%s.%s\n", htc[0], htc[1], htc[2], htc[3]);
//       }
//    for (i = 0; i < 4; i++)
//       {
//        if ((strcmp(tc[i], "  0") == 0) || (atoi(tc[i]) > 0))
//           {
//            continue;
//           }
//        for (d = 0; d < 3; d++)
//           {
//            if ((htc[i][d] != ' ') && (tc[i][d] == ' '))
//               {
//                tc[i][d] = htc[i][d];
//               }
//           }
//       }
//    con_printf("New Address : %s.%s.%s.%s\n", tc[0], tc[1], tc[2], tc[3]);
//    memset(hostaddr, 0, 24);
//    for (i = 0, d = 0; i < 4; i++)
//       {
//        for (s = 0; s < 3; s++)
//           {
//            if (tc[i][s] != ' ')
//               {
//                hostaddr[d++] = tc[i][s];
//               }
//           }
//        if (i < 3)
//           {
//            hostaddr[d++] = '.';
//           }
//       }
//    con_printf("Completed Address : %s\n", hostaddr);
//    return true;
//   }
//
//unsigned char NetBuffIn[65536];
////
//// SendMessage
////
//// This function is to allow multiple message types to
//// be sent by the network code.
//int SendNetMessage(int socket, byte *buf, int len, struct sockaddr *addr)
//   {
//    int ret;
//
//    ret = sendto(socket, buf, len, 0, (struct sockaddr *)addr, sizeof(struct sockaddr));
//    if (ret == -1)
//       {
//        if (WSAGetLastError() == WSAEWOULDBLOCK)
//           {
//            return 0;
//           }
//       }
//    return ret;
//   }
//
////
//// GetMessage
////
//// This function is to allow multiple message types to
//// be received by the network code.
////int GetNetMessage(int socket, byte *buf, int len, struct sockaddr *addr)
//int GetNetMessage(int socket, struct sockaddr *addr)
//   {
//    int addrlen = sizeof (struct sockaddr);
//    int ret;
//
//    //ret = recvfrom(socket, buf, len, 0, (struct sockaddr *)addr, &addrlen);
//    ret = recvfrom(socket, NetBuffIn, 65536, 0, (struct sockaddr *)addr, &addrlen);
//    if (ret == SOCKET_ERROR)
//	   {
//        int errnum = WSAGetLastError();
//
//        if (errnum == WSAEWOULDBLOCK || errnum == WSAECONNREFUSED)
//           {
//            return 0;
//           }
//       }
//    return ret;
//   }
//
////
//// EnableBroadcast
////
//// This function sets the socket to allow broadcasting of packets.
//int EnableBroadcast(int socket)
//   {
//    int i = 1;
//
//    // make this socket broadcast capable
//    if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char *)&i, sizeof(i)) < 0)
//       {
//        return -1;
//       }
//    broadcastsocket = socket;
//
//    return 0;
//   }
//
//
////
//// BroadcastMessage
////
//// This function broadcasts messages to all addresses that can
//// hear it. Mainly for searching for a game to join.
//int BroadcastMessage(int socket, byte *buf, int len)
//   {
//    int ret;
//
//    if (socket != broadcastsocket)
//       {
//        if (broadcastsocket != 0)
//           {
//            I_Error("Attempted to use multiple broadcasts sockets\n");
//           }
//        ret = EnableBroadcast(socket);
//        if (ret == -1)
//           {
//            con_printf("Unable to enable broadcasting on socket\n");
//            return ret;
//           }
//       }
//    return SendNetMessage(socket, buf, len, &broadcastaddr);
//   }
//
////
//// PacketSend
////
//void PacketSend (void)
//{
//    int		c;
//    doomdata_t	sw;
//				
//    // byte swap
//    sw.checksum = htonl(netbuffer->checksum);
//    sw.player = netbuffer->player;
//    sw.retransmitfrom = netbuffer->retransmitfrom;
//    sw.starttic = netbuffer->starttic;
//    sw.numtics = netbuffer->numtics;
//    for (c=0 ; c< netbuffer->numtics ; c++)
//    {
//	sw.cmds[c].forwardmove = netbuffer->cmds[c].forwardmove;
//	sw.cmds[c].sidemove = netbuffer->cmds[c].sidemove;
//	sw.cmds[c].angleturn = htons(netbuffer->cmds[c].angleturn);
//	sw.cmds[c].consistancy = htons(netbuffer->cmds[c].consistancy);
//	sw.cmds[c].chatchar = netbuffer->cmds[c].chatchar;
//	sw.cmds[c].buttons = netbuffer->cmds[c].buttons;
//    }
//		
//    //printf ("sending %i\n",gametic);		
//    c = sendto (sendsocket , (const char far *)&sw, doomcom->datalength
//		,0,(void *)&sendaddress[doomcom->remotenode]
//		,sizeof(sendaddress[doomcom->remotenode]));
//	
//    //	if (c == -1)
//    //		I_Error ("SendPacket error: %s",strerror(errno));
//}
//
//
////
//// PacketGet
////
//void PacketGet (void)
//{
//    int			i;
//    int			c;
//    struct sockaddr_in	fromaddress;
//    int			fromlen;
//    doomdata_t		sw;
//				
//    fromlen = sizeof(fromaddress);
//    c = recvfrom (insocket, (char far *)&sw, sizeof(sw),0,(struct sockaddr *)&fromaddress, &fromlen );
//    if (c == SOCKET_ERROR )
//       {
//        c = WSAGetLastError();
//	    if (c != WSAEWOULDBLOCK)
//	        I_Error ("GetPacket: Network error.");
//	    doomcom->remotenode = -1;		// no packet
//        return;
//       }
//
//    {
//     static int first=1;
//     if (first)
//        {
//         lfprintf("len=%d:p=[0x%x 0x%x] \n", c, *(int*)&sw, *((int*)&sw+1));
//        }
//     first = 0;
//    }
//
//    // find remote node number
//    for (i = 0; i < doomcom->numnodes; i++)
//       {
//        if ( fromaddress.sin_addr.s_addr == sendaddress[i].sin_addr.s_addr )
//           {
//            break;
//           }
//       }
//
//    if (i == doomcom->numnodes)
//       {
//        // packet is not from one of the players (new game broadcast)
//// NETUPDATE
//// test for someone wanting to join here
//// if this is a join game request
////    {
////     if I am not the lowest numbered active player
////        return
////     if I can't find an open slot
////        send back a decline and return
////     send back current game information including:
////         player positions
////         player statistics
////         current level and game mode
////         other player addresses and positions
////     send "add player" message to other players
////     add player to game
////     return
////    }
//// else
////    {
////
//        doomcom->remotenode = -1;		// no packet
//        return;
////     }
//       }
//	
//    doomcom->remotenode = i;			// good packet from a game player
//    doomcom->datalength = c;
//	
//    // byte swap
//    netbuffer->checksum = ntohl(sw.checksum);
//    netbuffer->player = sw.player;
//    netbuffer->retransmitfrom = sw.retransmitfrom;
//    netbuffer->starttic = sw.starttic;
//    netbuffer->numtics = sw.numtics;
//
//    for (c=0 ; c< netbuffer->numtics ; c++)
//    {
//	netbuffer->cmds[c].forwardmove = sw.cmds[c].forwardmove;
//	netbuffer->cmds[c].sidemove = sw.cmds[c].sidemove;
//	netbuffer->cmds[c].angleturn = ntohs(sw.cmds[c].angleturn);
//	netbuffer->cmds[c].consistancy = ntohs(sw.cmds[c].consistancy);
//	netbuffer->cmds[c].chatchar = sw.cmds[c].chatchar;
//	netbuffer->cmds[c].buttons = sw.cmds[c].buttons;
//    }
//}
//
//
//
//int GetLocalAddress (void)
//{
//    char		hostname[1024];
//    struct hostent*	hostentry;	// host information entry
//    int			v;
//
//    // get local address
//    v = gethostname (hostname, sizeof(hostname));
//    if (v == -1)
//	I_Error ("GetLocalAddress : gethostname: errno %d",errno);
//	
//    hostentry = gethostbyname (hostname);
//    if (!hostentry)
//	I_Error ("GetLocalAddress : gethostbyname: couldn't get local host");
//		
//    return *(int *)hostentry->h_addr_list[0];
//}
//
//dboolean isdigits(char *s);
///*
//   {
//    int i = 0;
//
//    for (i = 0; s[i] != '\0'; i++)
//       {
//        if (((s[i] < '0') || (s[i] > '9')) && (s[i] != '.'))
//           {
//            return false;
//           }
//       }
//    return true;
//   }
//*/
//
////
//// I_InitNetwork
////
extern int numplayers;

void I_InitNetwork(void)
   {
    //dboolean		trueval = true;
    int			i;
    //struct hostent*	hostentry;	// host information entry
    //WORD wVersionRequested;  
    //WSADATA wsaData; 
    //unsigned char hostname[256];
    //DWORD       lip;

    doomcom = malloc (sizeof (*doomcom) );
    memset (doomcom, 0, sizeof(*doomcom) );
    
    // set up for network
    /*i = M_CheckParm ("-dup");
    if (i && i< myargc-1)
       {
	    doomcom->ticdup = myargv[i+1][0]-'0';
	    if (doomcom->ticdup < 1)
            doomcom->ticdup = 1;
        if (doomcom->ticdup > 9)
            doomcom->ticdup = 9;
       }
    else*/
    doomcom->ticdup = 1;
	
    if (M_CheckParm ("-extratic"))
       doomcom->extratics = 1;
    else
       doomcom->extratics = 0;
		
    /*p = M_CheckParm ("-port");
    if (p && p<myargc-1)
       {
        DOOMPORT = atoi(myargv[p+1]);
        con_printf("using alternate port %i\n",DOOMPORT);
       }*/
    
    numplayers = 0;

    /*for (i = 0; i < 4; i++)
       {
        memset(ipaddress[i], 0, 16);
       }*/

    // parse network game options,
    //  -net <consoleplayer> <host> <host> ...
    i = M_CheckParm ("-net");
    if (!i)
       {
        // single player game
        netgame = false;
        doomcom->id = DOOMCOM_ID;
        doomcom->numplayers = doomcom->numnodes = 1;
        doomcom->deathmatch = false;
        doomcom->consoleplayer = 0;
        return;
       }

//    wVersionRequested = MAKEWORD(2,2); 
//    err = WSAStartup(wVersionRequested, &wsaData); 
// 
//    netsend = PacketSend;
//    netget = PacketGet;
//    netgame = true;
//
//    // parse player number and host list
//    doomcom->consoleplayer = myargv[i+1][0]-'1';
//    con_printf("console player number : %d\n", (doomcom->consoleplayer+1));
//
//    doomcom->numnodes = 1;	// this node for sure
//
//    gethostname(hostname, 255);
//    con_printf("Local host name: %s\n", hostname);
//    hostentry = gethostbyname(hostname);
//    if (hostentry)
//       {
//        while (hostentry->h_addr_list[adaptor])
//           {
//            lip = *(int *)hostentry->h_addr_list[adaptor];
//            if (adaptor == 0)
//               {
//                sprintf(ipaddress[doomcom->consoleplayer], "%d.%d.%d.%d", (lip & 0xFF),
//                                                                         ((lip >> 8)  & 0xFF),
//                                                                         ((lip >> 16) & 0xFF),
//                                                                         ((lip >> 24) & 0xFF));
//               }
//            con_printf("Adaptor %d IP address: %d.%d.%d.%d\n", adaptor, (lip & 0xFF),
//                                                                       ((lip >> 8)  & 0xFF),
//                                                                       ((lip >> 16) & 0xFF),
//                                                                       ((lip >> 24) & 0xFF));
//            adaptor++;
//           }
//       }
//    else
//       {
//        con_printf("Get local host address FAILED...\n");
//       }
//	
//    i++;
//    while (++i < myargc && myargv[i][0] != '-')
//       {
//        if (strlen(ipaddress[numplayers]) > 0)
//           {
//            numplayers++;
//           }
//	    sendaddress[doomcom->numnodes].sin_family = AF_INET;
//	    sendaddress[doomcom->numnodes].sin_port = htons(DOOMPORT);
//	    if (isdigits(myargv[i]))
//	       {
//            strcpy(ipaddress[numplayers], myargv[i]);
//	        sendaddress[doomcom->numnodes].sin_addr.s_addr = inet_addr(myargv[i]);
//	       }
//        else
//           {
//            con_printf("Node number %d hostname %s\n", doomcom->numnodes, myargv[i]);
//            hostentry = gethostbyname(myargv[i]);
//            if (!hostentry)
//                I_Error ("gethostbyname: couldn't find %s", myargv[i]);
//            sendaddress[doomcom->numnodes].sin_addr.s_addr = *(int *)hostentry->h_addr_list[0];
//            lip = *(int *)hostentry->h_addr_list[0];
//            sprintf(ipaddress[numplayers], "%d.%d.%d.%d", (lip & 0xFF),
//                                                         ((lip >>  8) & 0xFF),
//                                                         ((lip >> 16) & 0xFF),
//                                                         ((lip >> 24) & 0xFF) );
//           }
//        numplayers++;
//        doomcom->numnodes++;
//       }
//	
//    numplayers = doomcom->numnodes;
//    con_printf("Total number of players : %d\n", doomcom->numnodes);
//    for (i = 0; i < numplayers; i++)
//       {
//        con_printf("Player number %d IP %s\n", i+1, ipaddress[i]);
//       }
//    doomcom->id = DOOMCOM_ID;
//    doomcom->numplayers = doomcom->numnodes;
//
//	((struct sockaddr_in *)&broadcastaddr)->sin_family = AF_INET;
//	((struct sockaddr_in *)&broadcastaddr)->sin_addr.s_addr = INADDR_BROADCAST;
//	((struct sockaddr_in *)&broadcastaddr)->sin_port = htons(DOOMPORT);
//
//// build message to receive
//    insocket = UDPsocket();
//    BindToLocalPort(insocket,htons(DOOMPORT));
//    ioctlsocket(insocket, FIONBIO, &trueval);
//
//    sendsocket = UDPsocket();
   }
//
//
void I_NetCmd (void)
{
 //   if (doomcom->command == CMD_SEND)
 //   {
	////netsend ();
 //   }
 //   else if (doomcom->command == CMD_GET)
 //   {
	////netget ();
 //   }
 //   else
	//I_Error ("Bad net cmd: %i\n",doomcom->command);
}
//
/////////////////////////////////////////////////////////////////////////////
//// game status information layout
/////////////////////////////////////////////////////////////////////////////
///*
//unsigned char game mode;  // shareware, registered, retail, commmercial, tnt, plutonia or other
//unsigned char current episode;    // 0 to 4 unless Doom 2 then always 1
//unsigned char current level;      // 0 to 9
//unsigned char skill level;        // game skill level if monsters present  (see nomonsters)
//unsigned char game flags;         // see game flags reference
//unsigned char frag limit;         // if frags flag is set
//unsigned char time limit;         // if timer flag is set (in minutes)
//unsigned char number of players;  // 1 to MAX_PLAYERS
//// looped for each player in the game
//{
//   DWORD          playerx;
//   DWORD          playery;
//   DWORD          playerz;
//   unsigned short viewangle;
//   unsigned char  health;
//   unsigned char  armor;
//   unsigned char  readyweapon;    
//   unsigned char  weapons         // bitfields - see reference
//   unsigned short bullets
//   unsigned short shells
//   unsigned short energy
//   unsigned short rockets
//   unsigned short kills           // number of monsters killed - only if monsters allowed (see nomonsters)
//   unsigned short items           // number of items picked up
//   unsigned short secrets         // number of of secrets found
//   // loop 1 for each powerup    
//      {
//      unsigned int powerup value;
//      }
//   unsigned char  keys[6];
//   unsigned int   frags           // number of time player killed others
//}
//*/
//
//extern dboolean levelTimer;
//extern dboolean levelFrags;
//extern int     levelTimeCount;
//extern int     levelFragCount;
//
//unsigned char *BuildGameStats(int *buffsize)
//   {
//    static unsigned char gamebuff[4096];
//    unsigned char       *p, packed;
//    unsigned short      playlist;
//    int                 i, frags, player_num, numplayers;
//
//    p = gamebuff;
//
//    *p++ = (unsigned char)gamemode;
//    *p++ = (unsigned char)gameepisode;
//    *p++ = (unsigned char)gamemap;
//    *p++ = (unsigned char)gameskill;
//    packed = 0;
//    if (deathmatch == 1)
//       packed |= 0x1;  // 00000001  in case another deathmatch mode gets added
//    if (deathmatch == 2)
//       packed |= 0x2;  // 00000002  in case another deathmatch mode gets added
//    if (nomonsters)
//       packed |= 0x4;  // 00000003
//    if (levelTimer)
//       packed |= 0x8;  // 00000004
//    if (levelFrags)
//       packed |= 0x10; // 00000010
//    *p++ = (unsigned char)packed;
//    if (levelTimer)
//       {
//        *p++ = (unsigned char)(levelTimeCount / (60 * 35));
//       }
//    if (levelFrags)
//       {
//        *p++ = (unsigned char)levelFragCount;
//       }
//    for (player_num = 0, numplayers = 0, playlist = 0; player_num < MAXPLAYERS; player_num++)
//       {
//        if (playeringame[player_num])
//           {
//            numplayers++;
//            playlist |= (1 << player_num);
//           }
//       }
//    *p++ = (unsigned char)playlist >> 8;
//    *p++ = (unsigned char)playlist;
//    for (player_num = 0; player_num < MAXPLAYERS; player_num++)
//       {
//        if (playeringame[player_num])
//           {// player_t
///*
//            if (players[player_num].mo == 0)
//               {
//                for (i = 0; i < 14; i++)
//                   {
//                    *p++ = 0;
//                   }
//               }
//            else
//               {
//*/
//                *p++ = (unsigned char)players[player_num].mo->x >> 24;
//                *p++ = (unsigned char)players[player_num].mo->x >> 16;
//                *p++ = (unsigned char)players[player_num].mo->x >> 8;
//                *p++ = (unsigned char)players[player_num].mo->x;
//                *p++ = (unsigned char)players[player_num].mo->y >> 24;
//                *p++ = (unsigned char)players[player_num].mo->y >> 16;
//                *p++ = (unsigned char)players[player_num].mo->y >> 8;
//                *p++ = (unsigned char)players[player_num].mo->y;
//                *p++ = (unsigned char)players[player_num].mo->z >> 24;
//                *p++ = (unsigned char)players[player_num].mo->z >> 16;
//                *p++ = (unsigned char)players[player_num].mo->z >> 8;
//                *p++ = (unsigned char)players[player_num].mo->z;
//                *p++ = (unsigned char)players[player_num].mo->angle >> 8;
//                *p++ = (unsigned char)players[player_num].mo->angle;
////               }
//            *p++ = (unsigned char)players[player_num].health;
//            *p++ = (unsigned char)players[player_num].armorpoints;
//            *p++ = (unsigned char)players[player_num].readyweapon;
//            for (i = 0, packed = 0; i < NUMWEAPONS; i++)
//               {
//                if (players[player_num].weaponowned[i])
//                   {
//                    packed |= (1 << i);
//                   }
//               }
//            *p++ = (unsigned char)packed;
//            for (i = 0; i < NUMAMMO; i++)
//               {
//                *p++ = (unsigned char)players[player_num].ammo[i] >> 8;
//                *p++ = (unsigned char)players[player_num].ammo[i];
//               }
//            *p++ = (unsigned char)players[player_num].killcount >> 8;
//            *p++ = (unsigned char)players[player_num].killcount;
//            *p++ = (unsigned char)players[player_num].itemcount >> 8;
//            *p++ = (unsigned char)players[player_num].itemcount;
//            *p++ = (unsigned char)players[player_num].secretcount >> 8;
//            *p++ = (unsigned char)players[player_num].secretcount;
//            for (i = 0; i < NUMPOWERS; i++)
//               {
//                *p++ = (unsigned char)players[player_num].powers[i] >> 24;
//                *p++ = (unsigned char)players[player_num].powers[i] >> 16;
//                *p++ = (unsigned char)players[player_num].powers[i] >> 8;
//                *p++ = (unsigned char)players[player_num].powers[i];
//               }
//            for (i = 0, packed = 0; i < NUMCARDS; i++)
//               {
//                if (players[player_num].cards[i])
//                   {
//                    packed |= (1 << i);
//                   }
//               }
//            if (players[player_num].backpack)
//               {
//                packed |= (1 << i);
//               }
//            *p++ = (unsigned char)packed;
//            for (i = 0, frags = 0; i < MAXPLAYERS; i++)
//               {
//                frags += players[player_num].frags[i];
//               }
//            *p++ = (unsigned char)players[player_num].frags[i] >> 8;
//            *p++ = (unsigned char)players[player_num].frags[i];
//           }
//       }
//    *buffsize = p - gamebuff;
//
//    return gamebuff;
//   }
//
///*
//void WriteGameStats()
//   {
//    int            fn, length;
//    unsigned char *buff;
//
//    buff = BuildGameStats(&length);
//
//    fn = open("gldstats.bin", O_CREAT | O_RDWR | O_TRUNC | O_BINARY, 0666);
//    write(fn, buff, length);
//    close(fn);
//   }
//*/
//
//dboolean netactive;
//HANDLE   hThread;
//DWORD    dwThreadID;
//
//typedef struct net_msg_t
//   {
//    DWORD               dwMessage;
//    DWORD               dwPacket;
//    struct sockaddr_in  fromaddress;
//    struct net_msg_t   *pNext;
//    int                 datalen;
//    unsigned char       Data[];
//   }net_msg_t;
//
//net_msg_t *InQueue, *LastMsg, *OutQueue;
//
//DWORD NetHandler(DWORD parm)
//   {
//    //struct sockaddr_in	fromaddress;
//    //int			        fromlen;
//    //net_msg_t          *NewMsg;
//				//
//    //while( netactive == true )
//    //   {
//    //    // check for inbound messages here...
//    //    if ((fromlen = GetNetMessage(insocket, (struct sockaddr *)&fromaddress)) > 0)
//    //       {
//    //        // message received! - add it to the inbound message Queue...
//    //        NewMsg = (net_msg_t *)malloc(sizeof(net_msg_t)+fromlen);
//    //        memset(NewMsg, 0, sizeof(net_msg_t)+fromlen);
//    //        if (InQueue == NULL)
//    //           {
//    //            InQueue = NewMsg;
//    //            NewMsg->pNext = NULL;
//    //           }
//    //        else
//    //           {
//    //            LastMsg->pNext = LastMsg;
//    //           }
//    //        memcpy(&NewMsg->fromaddress, &fromaddress, sizeof(struct sockaddr));
//    //        memcpy(NewMsg->Data, NetBuffIn, fromlen);
//    //        NewMsg->datalen = fromlen;
//    //        LastMsg = NewMsg;
//    //       }
//
//    //    // send any outbound messages here...
//    //    if (OutQueue != NULL)
//    //       {
//    //       }
//    //   }
//    //ExitThread(0);
//    //return 0;
//   }
//
//void StartNetHandler()
//   {
//    /*InQueue = NULL;
//    OutQueue = NULL;
//    netactive = true;
//    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NetHandler, NULL, 0, &dwThreadID);*/
//   }
//
//void StopNetHandler()
//   {
//    //netactive = false;
//   }
//
