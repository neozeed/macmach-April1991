/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	rx.h,v $
 * Revision 2.3  89/06/03  15:35:15  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  22:11:27  jsb]
 * 
 * Revision 2.2  89/04/22  15:18:55  gm0w
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

#ifndef	_RX_
#define _RX_

/* Substitute VOID (char) for void, because some compilers are confused by void in some situations */
#define	VOID	char

#ifdef	KERNEL
#include <rx/rx_kernel.h>
#include <rx/rx_clock.h>
#include <rx/rx_event.h>
#else	KERNEL
# include <sys/types.h>
# include <stdio.h>
# include "rx_user.h"
# include "rx_clock.h"
# include "rx_event.h"
#endif	KERNEL

/* Configurable parameters */
#define	RX_IDLE_DEAD_TIME	60	/* default idle dead time */
#define	RX_MAX_SERVICES		20	/* Maximum number of services that may be installed */
#define	RX_DEFAULT_STACK_SIZE	4096	/* Default process stack size; overriden by rx_SetStackSize */

/* This parameter should not normally be changed */
#define	RX_PROCESS_PRIORITY	LWP_NORMAL_PRIORITY

/* Exported interfaces XXXX clean this up:  not all of these are exported*/
int rx_Init();
struct rx_service *rx_NewService();
struct rx_connection *rx_NewConnection();
struct rx_call *rx_NewCall();
struct rx_call *rx_GetCall();  /* Not normally used, but not obsolete */
long rx_EndCall();
int rx_AllocPackets();
struct rx_packet *rx_AllocPacket();
void rx_FreePackets();
int rx_WriteProc();
int rx_ReadProc();
void rx_FlushWrite();
void rx_PrintStats();
void rx_PrintPeerStats();
void rx_SetArrivalProc();
void rx_Finalize();

#define	RX_WAIT	    1
#define	RX_DONTWAIT 0

#define	rx_ConnectionOf(call)		((call)->conn)
#define	rx_PeerOf(conn)			((conn)->peer)
#define	rx_HostOf(peer)			((peer)->host)
#define	rx_PortOf(peer)			((peer)->port)
#define	rx_SetLocalStatus(call, status)	((call)->localStatus = (status))
#define rx_GetLocalStatus(call, status) ((call)->localStatus)
#define	rx_GetRemoteStatus(call)	((call)->remoteStatus)
#define	rx_Error(call)			((call)->error)
#define	rx_IsServerConn(conn)		((conn)->type == RX_SERVER_CONNECTION)
#define	rx_IsClientConn(conn)		((conn)->type == RX_CLIENT_CONNECTION)
/* Don't use these; use the IsServerConn style */
#define	rx_ServerConn(conn)		((conn)->type == RX_SERVER_CONNECTION)
#define	rx_ClientConn(conn)		((conn)->type == RX_CLIENT_CONNECTION)
/* Set and get rock is applicable to both connections and calls.  It's used by multi rx macros for calls. */
#define	rx_SetRock(obj, newrock)	((obj)->rock = (VOID *)(newrock))
#define	rx_GetRock(obj,	type)		((type)(obj)->rock)
#define	rx_SecurityClassOf(conn)	((conn)->securityIndex)

/* Macros callable by the user to further define attributes of a service.  Must be called before rx_StartServer */

/* Set the service stack size.  This currently just sets the stack size for all processes to be the maximum seen, so far */
#define rx_SetStackSize(service, stackSize) rx_stackSize = (((stackSize) > rx_stackSize)? stackSize: rx_stackSize)

/* Set minimum number of processes guaranteed to be available for this service at all times */
#define rx_SetMinProcs(service, min) ((service)->minProcs = (min))

/* Set maximum number of processes that will be made available to this service (also a guarantee that this number will be made available if there is no competition) */
#define rx_SetMaxProcs(service, max) ((service)->maxProcs = (max))

/* Define a procedure to be called just before a server connection is destroyed */
#define rx_SetDestroyConnProc(service,proc) ((service)->destroyConnProc = (proc))

/* Define procedure to set service dead time */
#define rx_SetIdleDeadTime(service,time) ((service)->idleDeadTime = (time))

/* Define procedures for getting and setting before and after execute-request procs */
#define rx_SetAfterProc(service,proc) ((service)->afterProc = (proc))
#define rx_SetBeforeProc(service,proc) ((service)->beforeProc = (proc))
#define rx_GetAfterProc(service) ((service)->afterProc)
#define rx_GetBeforeProc(service) ((service)->beforeProc)

/* Define a procedure to be called when a server connection is created */
#define rx_SetNewConnProc(service, proc) ((service)->newConnProc = (proc))

/* NOTE:  We'll probably redefine the following three routines, again, sometime. */

/* Set the connection dead time for any connections created for this service (server only) */
#define rx_SetServiceDeadTime(service, seconds) ((service)->secondsUntilDead = (seconds))

/* Set connection dead time, for a specific client or server connection */
#define rx_SetConnDeadTime(conn, seconds) (rxi_SetConnDeadTime(conn, seconds))
extern void rxi_SetConnDeadTime();

/* Set connection hard timeout for a connection */
#define rx_SetConnHardDeadTime(conn, seconds) ((conn)->hardDeadTime = (seconds))

/* Set rx default connection dead time; set on both services and connections at creation time */
extern int rx_connDeadTime;
#define	rx_SetRxDeadTime(seconds)   (rx_connDeadTime = (seconds))

extern int rx_nPackets;

/* Write nbytes of data to the call.  Returns the number of bytes written */
/* If it returns 0, the call status should be checked with rx_Error. */
#define	rx_Write(call, buf, nbytes)				\
    ((call)->nFree > (nbytes) ?					\
	bcopy((buf), (call)->bufPtr, (nbytes)),			\
	(call)->nFree -= (nbytes),				\
	(call)->bufPtr += (nbytes), (nbytes)			\
      : rx_WriteProc((call), (buf), (nbytes)))

/* Read nbytes of data from the call.  Returns the number of bytes read */
/* If it returns less than requested, the call status should be checked with rx_Error */
#define	rx_Read(call, buf, nbytes)					\
    ((call)->nLeft > (nbytes) ?						\
    bcopy((call)->bufPtr, (buf), (nbytes)),				\
    (call)->nLeft -= (nbytes), (call)->bufPtr += (nbytes), (nbytes)	\
   : rx_ReadProc((call), (buf), (nbytes)))

/* Macros callable by security modules, to set header/trailer lengths, set actual packet size, and find the beginning of the security header (or data) */
#define rx_SetSecurityHeaderSize(conn, length) ((conn)->securityHeaderSize = (length))
#define rx_SetSecurityMaxTrailerSize(conn, length) ((conn)->securityMaxTrailerSize = (length))
#define rx_GetSecurityHeaderSize(conn) ((conn)->securityHeaderSize)
#define rx_GetSecurityMaxTrailerSize(conn) ((conn)->securityMaxTrailerSize)

/* This is the address of the data portion of the packet.  Any encryption headers will be at this address, the actual data, for a data packet, will start at this address + the connection's security header size. */
#define	rx_DataOf(packet)		((char *) (packet)->wire.data)
#define	rx_GetDataSize(packet)		((packet)->length)
#define	rx_SetDataSize(packet, size)	((packet)->length = (size))

/* The offset of the actual user's data in the packet, skipping any security header */
#define	rx_UserDataOf(conn, packet)	(((char *) (packet)->wire.data) + (conn)->securityHeaderSize)
/* This is the maximum size data packet that can be sent on this connection, accounting for security module-specific overheads. */
#define	rx_MaxUserDataSize(conn)		((conn)->peer->packetSize - RX_HEADER_SIZE - (conn)->securityHeaderSize - (conn)->securityMaxTrailerSize)

/* XXXX (rewrite this description) A security class object contains a set of procedures and some private data to implement a security model for rx connections.  These routines are called by rx as appropriate.  Rx knows nothing about the internal details of any particular security model, or about security state.  Rx does maintain state per connection on behalf of the security class.  Each security class implementation is also expected to provide routines to create these objects.  Rx provides a basic routine to allocate one of these objects; this routine must be called by the class. */
struct rx_securityClass {
    struct rx_securityOps {
	int (*op_Close)(/* obj */);
	int (*op_NewConnection)(/* obj, conn */);
	int (*op_PreparePacket)(/* obj, call, packet */);
	int (*op_SendPacket)(/*obj, call, packet */);
	int (*op_CheckAuthentication)(/*obj,conn*/);
	int (*op_CreateChallenge)(/*obj,conn*/);
	int (*op_GetChallenge)(/*obj,conn,packet*/);
	int (*op_GetResponse)(/*obj,conn,packet*/);
	int (*op_CheckResponse)(/*obj,conn,packet*/);
	int (*op_CheckPacket) (/*obj,call,packet*/);
	int (*op_DestroyConnection)(/*obj, conn*/);
	int (*op_Spare1)();
	int (*op_Spare2)();
	int (*op_Spare3)();
	int (*op_Spare4)();
    } *ops;
    VOID *privateData;
    int refCount;
};

#if defined(__STDC__) && !defined(__HIGHC__)
#define RXS_OP(obj,op,args) ((obj->ops->op_ ## op) ? (*(obj)->ops->op_ ## op)args : 0)
#else
#define RXS_OP(obj,op,args) ((obj->ops->op_/**/op) ? (*(obj)->ops->op_/**/op)args : 0)
#endif

#define RXS_Close(obj) RXS_OP(obj,Close,(obj))
#define RXS_NewConnection(obj,conn) RXS_OP(obj,NewConnection,(obj,conn))
#define RXS_PreparePacket(obj,call,packet) RXS_OP(obj,PreparePacket,(obj,call,packet))
#define RXS_SendPacket(obj,call,packet) RXS_OP(obj,SendPacket,(obj,call,packet))
#define RXS_CheckAuthentication(obj,conn) RXS_OP(obj,CheckAuthentication,(obj,conn))
#define RXS_CreateChallenge(obj,conn) RXS_OP(obj,CreateChallenge,(obj,conn))
#define RXS_GetChallenge(obj,conn,packet) RXS_OP(obj,GetChallenge,(obj,conn,packet))
#define RXS_GetResponse(obj,conn,packet) RXS_OP(obj,GetResponse,(obj,conn,packet))
#define RXS_CheckResponse(obj,conn,packet) RXS_OP(obj,CheckResponse,(obj,conn,packet))
#define RXS_CheckPacket(obj,call,packet) RXS_OP(obj,CheckPacket,(obj,call,packet))
#define RXS_DestroyConnection(obj,conn) RXS_OP(obj,DestroyConnection,(obj,conn))

/* A service is installed by rx_NewService, and specifies a service type that is exported by this process.  Incoming calls are stamped with the service type, and must match an installed service for the call to be accepted.  Each service exported has a (port,serviceId) pair to uniquely identify it.  It is also named:  this is intended to allow a remote statistics gathering program to retrieve per service statistics without having to know the local service id's.  Each service has a number of security objects (instances of security classes) which implement various types of end-to-end security protocols for connections made to this service.  Finally, there are two parameters controlling the number of requests which may be executed in parallel by this service:  minProcs is the number of requests to this service which are guaranteed to be able to run in parallel at any time; maxProcs has two meanings:  it limits the total number of requests which may execute in parallel and it also guarantees that that many requests may be handled in parallel if no other service is handling any requests. */

struct rx_service {
    u_short serviceId;		    /* Service number */
    u_short servicePort;	    /* UDP port for this service */
    char *serviceName;		    /* Name of the service */
    osi_socket socket;		    /* socket structure or file descriptor */
    u_short nSecurityObjects;	    /* Number of entries in security objects array */
    struct rx_securityClass **securityObjects;  /* Array of security class objects */
    long (*executeRequestProc)();   /* Routine to call when an rpc request is received */
    VOID (*destroyConnProc)();	    /* Routine to call when a server connection is destroyed */
    VOID (*newConnProc)();	    /* Routine to call when a server connection is created */
    VOID (*beforeProc)();	    /* routine to call before a call is executed */
    VOID (*afterProc)();	    /* routine to call after a call is executed */
    short nRequestsRunning;	    /* Number of requests currently in progress */
    short maxProcs;		    /* Maximum procs to be used for this service */
    short minProcs;		    /* Minimum # of requests guaranteed executable simultaneously */
    short connDeadTime;		    /* Seconds until a client of this service will be declared dead, if it is not responding */
    short idleDeadTime;		    /* Time a server will wait for I/O to start up again */
};

/* A server puts itself on an idle queue for a service using an instance of the following structure.   When a call arrives, the call structure pointer is placed in "newcall", the routine to execute to service the request is placed in executeRequestProc, and the process is woken up.  The queue entry's address is used for the sleep/wakeup. */
struct rx_serverQueueEntry {
    struct queue queueItemHeader;
    struct rx_call *newcall;
};

/* Bottom n-bits of the Call Identifier give the call number */
#define	RX_MAXCALLS 4	/* Power of 2; max async calls per connection */
#define	RX_CIDSHIFT 2	/* Log2(RX_MAXCALLS) */
#define	RX_CHANNELMASK (RX_MAXCALLS-1)
#define	RX_CIDMASK  (~RX_CHANNELMASK)

/* A peer refers to a peer process, specified by a (host,port) pair.  There may be more than one peer on a given host. */
struct rx_peer {
    struct rx_peer *next;	    /* Next in hash conflict or free list */
    u_long host;		    /* Remote IP address, in net byte order */
    u_short port;		    /* Remote UDP port, in net byte order */
    short packetSize;		    /* Max packet size, if known, for this host */

    /* For garbage collection */
    u_long idleWhen;		    /* When the refcountwent to zero */
    short refCount;		    /* Reference count for this structure */

    /* Congestion control parameters */
    u_char burstSize;		    /* Reinitialization size for the burst parameter */
    u_char burst;		    /* Number of packets that can be transmitted right now, without pausing */
    struct clock burstWait;	    /* Delay until new burst is allowed */
    struct queue congestionQueue;   /* Calls that are waiting for non-zero burst value */
    int	rtt;			    /* Round trip time, measured in milliseconds */
    struct clock timeout;	    /* Current retransmission delay */
    int	nSent;			    /* Total number of distinct data packets sent, not including retransmissions */
    int	reSends;		    /* Total number of retransmissions for this peer, since this structure was created */

/* Skew:  if a packet is received N packets later than expected (based on packet serial numbers), then we define it to have a skew of N.  The maximum skew values allow us to decide when a packet hasn't been received yet because it is out-of-order, as opposed to when it is likely to have been dropped. */
    u_long inPacketSkew;	    /* Maximum skew on incoming packets */
    u_long outPacketSkew;	    /* Peer-reported maximum skew on our outgoing packets */

};

/* A connection is an authenticated communication path, allowing multiple asynchronous conversations. */
struct rx_connection {
    struct rx_connection *next;	    /* Next on hash chain or free list */
    struct rx_peer *peer;
    u_long epoch;		    /* Process start time of client side of connection */
    u_long cid;			    /* Connection id (call channel is bottom bits) */
    VOID *rock;			    /* User definable rock */
    struct rx_call *call[RX_MAXCALLS];
    u_long callNumber[RX_MAXCALLS]; /* Current call numbers */
    int	timeout;		    /* Overall timeout per call (seconds) for this connection */
    u_char flags;		    /* Defined below */
    u_char type;		    /* Type of connection, defined below */
    u_short serviceId;		    /* The service ID that should be stamped on requests (clients only) */
    struct rx_service *service;	    /* The service structure associated with this connection (servers only) */
    u_long serial;		    /* Next outgoing packet serial number */
    u_long lastSerial;		    /* Serial number of last packet received, for computing skew */
    u_short secondsUntilDead;	    /* Maximum time of silence from peer before call will be terminated with an RX_CALL_DEAD */
    u_char secondsUntilPing;	    /* A ping should be made every this number of seconds when a call is active */
    u_char securityIndex;	    /* The security index for this service corresponding to the security class of the object implementing security for this connection */
    long error;			    /* If this connection is in error, this is it */
    struct rx_securityClass *securityObject; /* Security object for this connection */
    VOID *securityData;		    /* Private security data, for this connection's security class */
    u_short securityHeaderSize;	    /* Length of security module's packet header data */
    u_short securityMaxTrailerSize; /* Length of security module's packet trailer data */
    struct rxevent *challengeEvent;   /* Scheduled when the server is challenging a client--to retransmit the challenge */
    int	lastSendTime;		    /* Last send time for this connection */
    long maxSerial;		    /* largest serial number seen on incoming packets */
    u_short hardDeadTime;	    /* hard max for call execution */
};

/* Flags for connection structure */
#define	RX_CONN_MAKECALL_WAITING    1	/* rx_MakeCall is waiting for a channel */
#define	RX_CONN_DESTROY_ME	    2	/* Destroy *client* connection after last call */

/* Type of connection, client or server */
#define	RX_CLIENT_CONNECTION	0
#define	RX_SERVER_CONNECTION	1

/* Call structure:  only instantiated for active calls and dallying server calls.  The permanent call state (i.e. the call number as well as state shared with other calls associated with this connection) is maintained in the connection structure. */
struct rx_call {
    struct queue queue_item_header; /* Call can be on various queues (one-at-a-time) */
    struct queue tq;		    /* Transmit packet queue */
    struct queue rq;		    /* Receive packet queue */
    char *bufPtr;		    /* Next byte to fill or read in current send/read packet */
    u_short nLeft;		    /* Number of bytes left in first receive queue packet */
    u_short nFree;		    /* Number of bytes free in last send packet */
    struct rx_packet *currentPacket;/* Current packet being assembled or being read */
    struct rx_connection *conn;	    /* Parent connection for this call */
    u_long *callNumber;		    /* Pointer to call number field within connection */
    u_char channel;		    /* Index of call, within connection */
    u_char dummy1, dummy2;	    /* Spares */
    u_char state;		    /* Current call state as defined below */
    u_char mode;		    /* Current mode of a call in ACTIVE state */
    u_char flags;		    /* Some random flags */
    u_char localStatus;		    /* Local user status sent out of band */
    u_char remoteStatus;	    /* Remote user status received out of band */ 
    long error;			    /* Error condition for this call */
    u_long timeout;		    /* High level timeout for this call */
    u_long rnext;		    /* Next sequence number expected to be read by rx_ReadData */
    u_long rprev;	    	    /* Previous packet received; used for deciding what the next packet to be received should be, in order to decide whether a negative acknowledge should be sent */
    u_long rwind;		    /* The receive window:  the peer must not send packets with sequence numbers >= rnext+rwind */
    u_long tfirst;		    /* First unacknowledged transmit packet number */
    u_long tnext;		    /* Next transmit sequence number to use */
    u_long twind;		    /* The transmit window:  we cannot assign a sequence number to a packet >= tfirst + twind */
    struct rxevent *resendEvent;	    /* If this is non-Null, there is a retransmission event pending */
    struct rxevent *timeoutEvent;	    /* If this is non-Null, then there is an overall timeout for this call */
    struct rxevent *keepAliveEvent;   /* Scheduled periodically in active calls to keep call alive */
    struct rxevent *delayedAckEvent;  /* Scheduled after all packets are received to send an ack if a reply or new call is not generated soon */
    int	lastSendTime;		    /* Last time a packet was sent on this call */
    int	lastReceiveTime;	    /* Last time a packet was received for this call */
    VOID (*arrivalProc)();	    /* Procedure to call when reply is received */
    VOID *arrivalProcHandle;	    /* Handle to pass to replyFunc */
    VOID *arrivalProcArg;	    /* Additional arg to pass to reply Proc */
    u_long lastAcked;		    /* last packet "hard" acked by receiver */
    u_long startTime;		    /* time the call started running */
    u_long startWait;		    /* time server began waiting for input data/send quota */
};

/* Major call states */
#define	RX_STATE_NOTINIT  0    /* Call structure has never been initialized */
#define	RX_STATE_PRECALL  1    /* Server-only:  call is not in progress, but packets have arrived */
#define	RX_STATE_ACTIVE	  2    /* An active call; a process is dealing with this call */
#define	RX_STATE_DALLY	  3    /* Dallying after process is done with call */

/* Call modes:  the modes of a call in RX_STATE_ACTIVE state (process attached) */
#define	RX_MODE_SENDING	  1    /* Sending or ready to send */
#define	RX_MODE_RECEIVING 2    /* Receiving or ready to receive */
#define	RX_MODE_ERROR	  3    /* Something in error for current conversation */
#define	RX_MODE_EOF	  4    /* Server has flushed (or client has read) last reply packet */

/* Flags */
#define	RX_CALL_READER_WAIT	   1   /* Reader is waiting for next packet */
#define	RX_CALL_WAIT_WINDOW_ALLOC  2   /* Sender is waiting for window to allocate buffers */
#define	RX_CALL_WAIT_WINDOW_SEND   4   /* Sender is waiting for window to send buffers */
#define	RX_CALL_WAIT_PACKETS	   8   /* Sender is waiting for packet buffers */
#define	RX_CALL_WAIT_PROC	  16   /* Waiting for a process to be assigned */
#define	RX_CALL_RECEIVE_DONE	  32   /* All packets received on this call */
#define	RX_CALL_CLEARED		  64   /* Receive queue cleared in precall state */

/* The rx part of the header of a packet, in host form */
struct rx_header {
    u_long epoch;	/* Start time of client process */
    u_long cid;		/* Connection id (defined by client) */
    u_long callNumber;	/* Current call number */
    u_long seq;		/* Sequence number of this packet, within this call */
    u_long serial;	/* Serial number of this packet:  a new serial number is stamped on each packet sent out */
    u_char type;	/* RX packet type */
    u_char flags;	/* Flags, defined below */
    u_char userStatus;	/* User defined status information, returned/set by macros rx_Get/SetLocal/RemoteStatus */
    u_char securityIndex; /* Which service-defined security method to use */
    u_short serviceId;	/* The id of the server-provided service that this packet is directed to */
    u_short spare;	/* Reserved */
};

/* Flags for rx_header flags field */
#define	RX_CLIENT_INITIATED	1   /* Packet is sent/received from client side of call */
#define	RX_REQUEST_ACK		2   /* Peer requests acknowledgement */
#define	RX_LAST_PACKET		4   /* This is the last packet from this side of the call */
#define	RX_MORE_PACKETS		8   /* There are more packets following this, i.e. the next sequence number seen by the receiver should be greater than this one, rather than a retransmission of an earlier sequence number */
/* The following flags are preset per packet, i.e. they don't change on retransmission of the packet */
#define	RX_PRESET_FLAGS		(RX_CLIENT_INITIATED | RX_LAST_PACKET)


#define	RX_IPUDP_SIZE		28
#define	RX_MAX_PACKET_SIZE	(1500-RX_IPUDP_SIZE) /* Ether MTU minus IP & UDP headers */
#define	RX_HEADER_SIZE		sizeof (struct rx_header) /* Size of rx header */
#define	RX_MAX_PACKET_DATA_SIZE	(RX_MAX_PACKET_SIZE-RX_HEADER_SIZE)
#define	RX_LOCAL_PACKET_SIZE	RX_MAX_PACKET_SIZE  /* For hosts on same net */
#define	RX_REMOTE_PACKET_SIZE	(576-RX_IPUDP_SIZE) /* For Internet hosts */

struct rx_packet {
    struct queue queueItemHeader;   /* Packets are chained using the queue.h package */
    u_char acked;		    /* This packet has been *tentatively* acknowledged */
    short length;		    /* Data length */
    u_char packetType;		    /* For rx_AllocPacket/rx_FreePacket */
    struct clock retryTime;	    /* When this packet should NEXT be re-transmitted */
    struct clock timeSent;	    /* When this packet was transmitted last */
    struct rx_header header;	    /* The internal packet header */
    struct {
	u_long head[RX_HEADER_SIZE/sizeof(long)];
	u_long data[RX_MAX_PACKET_DATA_SIZE/sizeof(long)]; /* Max == Ether MTU, for now */
    } wire;			    /* The wire form of the packet */
};

/* Packet types, for rx_packet.type */
#define	RX_PACKET_TYPE_DATA	    1    /* A vanilla data packet */
#define	RX_PACKET_TYPE_ACK	    2    /* Acknowledge packet */
#define	RX_PACKET_TYPE_BUSY	    3    /* Busy: can't accept call immediately; try later */
#define	RX_PACKET_TYPE_ABORT	    4    /* Abort packet.  No response needed. */
#define	RX_PACKET_TYPE_ACKALL	    5    /* Acknowledges receipt of all packets */
#define	RX_PACKET_TYPE_CHALLENGE    6    /* Challenge client's identity: request credentials */
#define	RX_PACKET_TYPE_RESPONSE	    7    /* Respond to challenge packet */
#define	RX_PACKET_TYPE_DEBUG	    8	 /* Get debug information */

#define	RX_PACKET_TYPES	    {"data", "ack", "busy", "abort", "ackall", "challenge", "response", "debug"}
#define	RX_N_PACKET_TYPES	    9	    /* Must agree with above list; counts 0 */

/* Packet classes, for rx_AllocPacket */
#define	RX_PACKET_CLASS_RECEIVE	    0
#define	RX_PACKET_CLASS_SEND	    1
#define	RX_PACKET_CLASS_SPECIAL	    2

#define	RX_N_PACKET_CLASSES	    3	    /* Must agree with above list */


/* Maximum number of acknowledgements in an acknowledge packet */
#define	RX_MAXACKS	    255

/* The structure of the data portion of an acknowledge packet:  An acknowledge packet is in network byte order at all times.  An acknowledgement is always prompted for a specific reason by a specific incoming packet.  This reason is reported in "reason" and the packet's sequence number in the packet header.seq.  In addition to this information, all of the current acknowledgement information about this call is placed in the packet.  "FirstPacket" is the sequence number of the first packet represented in an array of bytes, "acks", containing acknowledgement information for a number of consecutive packets.  All packets prior to FirstPacket are implicitly acknowledged:  the sender need no longer be concerned about them.  Packets from firstPacket+nAcks and on are not acknowledged.  Packets in the range [firstPacket,firstPacket+nAcks) are each acknowledged explicitly.  The acknowledgement may be RX_NACK if the packet is not (currently) at the receiver (it may have never been received, or received and then later dropped), or it may be RX_ACK if the packet is queued up waiting to be read by the upper level software.  RX_ACK does not imply that the packet may not be dropped before it is read; it does imply that the sender should stop retransmitting the packet until notified otherwise.   The field previousPacket identifies the previous packet received by the peer.  This was used in a previous version of this software, and could be used in the future.  The serial number in the data part of the ack packet corresponds to the serial number oof the packet which prompted the acknowledge.  Any packets which are explicitly not acknowledged, and which were last transmitted with a serial number less than the provided serial number, should be retransmitted immediately.   Actually, this is slightly inaccurate:  packets are not necessarily received in order.  When packets are habitually transmitted out of order, this is allowed for in the retransmission algorithm by introducing the notion of maximum packet skew:  the degree of out-of-orderness of the packets received on the wire.   This number is communicated from the receiver to the sender in ack packets. */

struct rx_ackPacket {
    u_short bufferSpace;    /* Number of packet buffers available.  That is:  the number of buffers that the sender of the ack packet is willing to provide for data, on this or subsequent calls.  Lying is permissable. */
    u_short maxSkew;	    /* Maximum difference between serial# of packet acknowledged and highest packet yet received */
    u_long  firstPacket;    /* The first packet in the list of acknowledged packets */
    u_long  previousPacket; /* The previous packet number received (obsolete?) */
    u_long  serial;	    /* Serial number of the packet which prompted the acknowledge */
    u_char  reason;	    /* Reason for the acknowledge of ackPacket, defined below */
    u_char  nAcks;	    /* Number of acknowledgements */
    u_char  acks[RX_MAXACKS]; /* Up to RX_MAXACKS packet acknowledgements, defined below */
    /* Packets <firstPacket are implicitly acknowledged and may be discarded by the sender.  Packets >= firstPacket+nAcks are implicitly NOT acknowledged.  No packets with sequence numbers >= firstPacket should be discarded by the sender (they may thrown out at any time by the receiver) */
};

/* Reason for acknowledge message */
#define	RX_ACK_REQUESTED	1   /* Peer requested an ack on this packet */
#define	RX_ACK_DUPLICATE	2   /* Duplicate packet */
#define	RX_ACK_OUT_OF_SEQUENCE	3   /* Packet out of sequence */
#define	RX_ACK_EXCEEDS_WINDOW	4   /* Packet sequence number higher than window; discarded */
#define	RX_ACK_NOSPACE		5   /* No buffer space at all */
#define	RX_ACK_PING		6   /* This is a keep-alive ack */
#define	RX_ACK_PING_RESPONSE	7   /* Ack'ing because we were pinged */
#define	RX_ACK_DELAY		8   /* Ack generated since nothing has happened since receiving packet */

/* Packet acknowledgement type */ 
#define	RX_ACK_TYPE_NACK	0   /* I Don't have this packet */
#define	RX_ACK_TYPE_ACK		1   /* I have this packet, although I may discard it later */

/* The packet size transmitted for an acknowledge is adjusted to reflect the actual size of the acks array.  This macro defines the size */
#define rx_AckDataSize(nAcks) (sizeof(struct rx_ackPacket) - RX_MAXACKS + (nAcks))

#define	RX_CHALLENGE_TIMEOUT	2   /* Number of seconds before another authentication request packet is generated */

/* RX error codes.  RX uses error codes from -1 to -64.  Rxgen may use other error codes < -64; user programs are expected to return positive error codes */

/* Something bad happened to the connection; temporary loss of communication */
#define	RX_CALL_DEAD		    (-1)

/* An invalid operation, such as a client attempting to send data after having received the beginning of a reply from the server */
#define	RX_INVALID_OPERATION	    (-2)

/* An optional timeout per call may be specified */
#define	RX_CALL_TIMEOUT		    (-3)

/* End of data on a read */
#define	RX_EOF			    (-4)

/* Some sort of low-level protocol error */
#define	RX_PROTOCOL_ERROR	    (-5)

/* Generic user abort code; used when no more specific error code needs to be communicated.  For example, multi rx clients use this code to abort a multi rx call */
#define	RX_USER_ABORT		    (-6)


/* structures for debug input and output packets */
struct rx_debugIn {
    long type;
    long index;
};

struct rx_debugStats {
    long nFreePackets;
    long packetReclaims;
    long callsExecuted;
    char waitingForPackets;
};

/* debug input types */
#define	RX_DEBUGI_GETSTATS	1	/* get basic rx stats */
#define	RX_DEBUGI_GETCONN	2	/* get connection info */

struct rx_debugConn {
    long host;
    long cid;
    long serial;
    long callNumber[RX_MAXCALLS];
    long error;
    short port;
    char flags;
    char type;
    char securityIndex;
    char callState[RX_MAXCALLS];
    char callMode[RX_MAXCALLS];
    char callFlags[RX_MAXCALLS];
    char callOther[RX_MAXCALLS];
};

#define	RX_OTHER_IN	1	/* packets avail in in queue */
#define	RX_OTHER_OUT	2	/* packets avail in out queue */

#endif _RX_	/* End of rx.h */
