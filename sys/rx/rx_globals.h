/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx_globals.h,v $
 * Revision 2.4  89/08/02  08:08:34  jsb
 * 	Change rxi_Alloc* macros to use osi_Zalloc instead of osi_Alloc.
 * 	[89/07/31  17:28:02  jsb]
 * 
 * Revision 2.3  89/06/03  15:39:46  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:24:53  jsb]
 * 
 * Revision 2.2  89/04/22  15:29:05  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
****************************************************************************
*        Copyright IBM Corporation 1988, 1989 - All Rights Reserved        *
*                                                                          *
* Permission to use, copy, modify, and distribute this software and its    *
* documentation for any purpose and without fee is hereby granted,         *
* provided that the above copyright notice appear in all copies and        *
* that both that copyright notice and this permission notice appear in     *
* supporting documentation, and that the name of IBM not be used in        *
* advertising or publicity pertaining to distribution of the software      *
* without specific, written prior permission.                              *
*                                                                          *
* IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL IBM *
* BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY      *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  *
* IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   *
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    *
****************************************************************************
*/

/* RX:  Globals for internal use, basically */

#ifdef	KERNEL
#include <rx/rx.h>
#else	KERNEL
# include "rx.h"
#endif	KERNEL

#ifndef INIT
#define INIT(x)
#define	EXT extern
#endif	INIT

/* The array of installed services.  Null terminated. */
EXT struct rx_service *rx_services[RX_MAX_SERVICES+1];

/* Incoming calls wait on this queue when there are no available server processes */
EXT struct queue rx_incomingCallQueue;

/* Server processes wait on this queue when there are no appropriate calls to process */
EXT struct queue rx_idleServerQueue;

/* Constant delay time before sending an acknowledge of the last packet received.  This is to avoid sending an extra acknowledge when the client is about to make another call, anyway, or the server is about to respond. */
EXT struct clock rx_lastAckDelay;

/* Variable to allow introduction of network unreliability */
#ifdef RXDEBUG
EXT int rx_intentionallyDroppedPacketsPer100 INIT(0);	/* Dropped on Send */
#endif

EXT int rx_stackSize INIT(RX_DEFAULT_STACK_SIZE);

EXT int	rx_connDeadTime	INIT(12);	    /* Time until an unresponsive connection is declared dead */
EXT int rx_idleConnectionTime INIT(700);    /* Time until we toss an idle connection */
EXT int	rx_idlePeerTime	INIT(60);	    /* Time until we toss a peer structure, after all connections using it have disappeared */

/* These definitions should be in one place */
#define	RX_REAP_TIME	60	    /* Check for tossable connections every 60 seconds */

EXT int rx_Window INIT(15); /* Temporary HACK:  transmit/receive window */
EXT int rx_ACKHACK INIT(4); /* Temporary HACK:  how often to send request for acknowledge */
EXT int zork;
#define	ACKHACK(p)  (((zork++ % rx_ACKHACK) == 0) && ((p)->header.flags |= RX_REQUEST_ACK))
EXT int rx_nPackets INIT(100);	/* Set by the user to the number of packets desired; this default is a little high, right now so we can avoid deadlock problems temporarily (because packet allocation is all wrong right now) */
#define	RX_MIN_PACKETS	15  /* Minimum number of packets */

#ifndef	MACH
/* List of free packets */
EXT struct queue rx_freePacketQueue;
#endif

/* Number of free packets */
EXT int rx_nFreePackets;

/* List of free queue entries */
EXT struct rx_serverQueueEntry *rx_FreeSQEList INIT(0);

/* List of free call structures */
EXT struct queue rx_freeCallQueue;

/* Basic socket for client requests; other sockets (for receiving server requests) are in the service structures */
EXT osi_socket rx_socket;

/* Port requested at rx_Init.  If this is zero, the actual port used will be different--but it will only be used for client operations.  If non-zero, server provided services may use the same port. */
EXT u_short rx_port;

/* 32-bit select Mask for rx_Listener.  We use 32 bits because IOMGR_Select only supports 32 */
EXT u_long rx_selectMask;
EXT int rx_maxSocketNumber;	    /* Maximum socket number represented in the select mask */

/* This is actually the minimum number of packets that must remain free, overall, immediately after a packet of the requested class has been allocated.  *WARNING* These must be assigned with a great deal of care. */
#define	RX_PACKET_QUOTAS {1, 10, 0}
#define	RX_MIN_RECLAIM	14	/* min packets must have after a reclaim; must be 2 larger than RX_PACKET_QUOTAS[i] so that next quota alloc works */
EXT int rx_packetQuota[RX_N_PACKET_CLASSES] INIT(RX_PACKET_QUOTAS);

EXT int rx_nextCid;	    /* Next connection call id */
EXT int rx_epoch;	    /* Initialization time of rx */
EXT char rx_waitingForPackets; /* Processes set and wait on this variable when waiting for packet buffers */

EXT struct {		    /* General rx statistics */
    int	packetRequests;	    /* Number of packet allocation requests */
    int noPackets[RX_N_PACKET_CLASSES]; /* Number of failed packet requests, per allocation class */
    int	socketGreedy;	    /* Whether SO_GREEDY succeeded */
    int bogusPacketOnRead;  /* Number of inappropriately short packets received */
    int	bogusHost;	    /* Host address from bogus packets */
    int	noPacketOnRead;	    /* Number of read packets attempted when there was actually no packet to read off the wire */
    int	noPacketBuffersOnRead; /* Number of dropped data packets due to lack of packet buffers */
    int	selects;	    /* Number of selects waiting for packet or timeout */
    int	sendSelects;	    /* Number of selects forced when sending packet */
    int	packetsRead[RX_N_PACKET_TYPES]; /* Total number of packets read, per type */
    int	dataPacketsRead;    /* Number of unique data packets read off the wire */
    int	ackPacketsRead;	    /* Number of ack packets read */
    int	dupPacketsRead;	    /* Number of duplicate data packets read */	    
    int	spuriousPacketsRead;/* Number of inappropriate data packets */
    int	packetsSent[RX_N_PACKET_TYPES]; /* Number of rxi_Sends: packets sent over the wire, per type */
    int	ackPacketsSent;	    /* Number of acks sent */
    int	pingPacketsSent;    /* Total number of ping packets sent */
    int	abortPacketsSent;   /* Total number of aborts */
    int	busyPacketsSent;    /* Total number of busies sent received */
    int	dataPacketsSent;    /* Number of unique data packets sent */
    int	dataPacketsReSent;  /* Number of retransmissions */
    int	dataPacketsPushed;  /* Number of retransmissions pushed early by a NACK */
    int ignoreAckedPacket;  /* Number of packets with acked flag, on rxi_Start */
    struct clock totalRtt;  /* Total round trip time measured (use to compute average) */
    struct clock minRtt;    /* Minimum round trip time measured */
    struct clock maxRtt;    /* Maximum round trip time measured */
    int	nRttSamples;	    /* Total number of round trip samples */
    int	nServerConns;	    /* Total number of server connections */
    int	nClientConns;	    /* Total number of client connections */
    int	nPeerStructs;	    /* Total number of peer structures */
    int	nCallStructs;	    /* Total number of call structures allocated */
    int	nFreeCallStructs;   /* Total number of previously allocated free call structures */
} rx_stats;

EXT struct rx_peer **rx_peerHashTable;
EXT struct rx_connection **rx_connHashTable;
EXT u_long rx_hashTableSize INIT(256);	/* Power of 2 */
EXT u_long rx_hashTableMask INIT(255);	/* One less than rx_hashTableSize */

#define CONN_HASH(host, port, cid, epoch, type) ((((cid)>>RX_CIDSHIFT)&rx_hashTableMask))

#define PEER_HASH(host, port)  ((host ^ port) & rx_hashTableMask)

#define	rxi_Free(addr, size)	osi_Free(addr, size)
#define	rxi_Zfree(addr, size)	osi_Zfree(addr, size)

#define rxi_AllocSecurityObject() (struct rx_securityClass *) rxi_Zalloc(rx_securityClass_zone)
#define	rxi_FreeSecurityObject(obj) rxi_Zfree(rx_securityClass_zone, obj)
#define	rxi_AllocService()	(struct rx_service *) rxi_Zalloc(rx_service_zone)
#define	rxi_AllocPeer()		(struct rx_peer *) rxi_Zalloc(rx_peer_zone)
#define	rxi_FreePeer(peer)	rxi_Zfree(rx_peer_zone, peer)
#define	rxi_AllocConnection()	(struct rx_connection *) rxi_Zalloc(rx_connection_zone)
#define rxi_FreeConnection(conn) (rxi_Zfree(rx_connection_zone, conn))

/* Forward definitions of internal procedures */
struct rx_packet *rxi_AllocPacket();
struct rx_packet *rxi_AllocSendPacket();
char *rxi_Alloc();
char *rxi_Zalloc();
struct rx_peer *rxi_FindPeer();
struct rx_call *rxi_NewCall();
void rxi_FreeCall();
struct rx_call *rxi_FindCall();
void rxi_Listener();
int rxi_ReadPacket();
struct rx_packet *rxi_ReceivePacket();
struct rx_packet *rxi_ReceiveDataPacket();
struct rx_packet *rxi_ReceiveAckPacket();
struct rx_packet *rxi_ReceiveResponsePacket();
struct rx_packet *rxi_ReceiveChallengePacket();
void rx_ServerProc();
void rxi_AttachServerProc();
void rxi_ChallengeOn();
#define	rxi_ChallengeOff(conn)	rxevent_Cancel((conn)->challengeEvent);
void rxi_ChallengeEvent();
struct rx_packet *rxi_SendAck();
void rxi_ClearTransmitQueue();
void rxi_ClearReceiveQueue();
void rxi_ResetConnection();
void rxi_InitCall();
void rxi_ResetCall();
void rxi_ResetConnection();
void rxi_CallError();
void rxi_ConnectionError();
void rxi_QueuePackets();
void rxi_Start();
void rxi_CallIsIdle();
void rxi_CallTimedOut();
void rxi_ComputeRoundTripTime();
void rxi_ScheduleKeepAliveEvent();
void rxi_KeepAliveEvent();
void rxi_KeepAliveOn();
#define rxi_KeepAliveOff(call) rxevent_Cancel((call)->keepAliveEvent)
void rxi_AckAll();
void rxi_SendDelayedAck();
struct rx_packet *rxi_SendSpecial();
struct rx_packet *rxi_SendCallAbort();
struct rx_packet *rxi_SendConnectionAbort();
void rxi_ScheduleDecongestionEvent();
void rxi_CongestionWait();
void rxi_ReapConnections();
void rxi_EncodePacketHeader();
void rxi_DecodePacketHeader();
void rxi_DebugPrint();
void rxi_SendDelayedAck();
void rxi_PrepareSendPacket();

#ifdef RXDEBUG
/* Some debugging stuff */
EXT FILE *rx_debugFile;	/* Set by the user to a stdio file for debugging output */

#define Log rx_debugFile
#define dpf(args) if (rx_debugFile) rxi_DebugPrint args; else

EXT char *rx_packetTypes[RX_N_PACKET_TYPES] INIT(RX_PACKET_TYPES); /* Strings defined in rx.h */

#endif	RXDEBUG
