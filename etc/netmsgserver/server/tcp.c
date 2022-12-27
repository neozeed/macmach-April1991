/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	tcp.c,v $
 * Revision 1.1  90/02/19  21:57:07  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.15  89/05/02  11:18:11  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.14  89/04/24  20:42:49  dpj
 * 	Changes from NeXT as of 88/09/30
 * 	[89/04/19  17:58:11  dpj]
 * 
 * 23-Jun-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added a LOGCHECK.
 *
 *  3-Apr-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Increased TCPBUFSZ to 8K to catch all inline messages.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 *  2-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed horrible pointer arithmetic bug in cptoix.
 *
 * 26-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Allow tcp_sendreply() to transmit a simple reply code and no data.
 *
 * 16-Feb-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified tcp_xmit_data to use writev() instead of flattening
 *	the sbuf.
 *
 * 24-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed re-alignment of data in receive buffer.
 *	Ignore SIGPIPE.
 *
 * 21-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added an option to enable socket debugging (TCP_DBG_SOCKET).
 *	Fixed to byte-swap the TCP port number.
 *
 * 19-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified delivery of responses to avoid passing an empty sbuf
 *	when there is no data.
 *
 * 15-Jan-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed a typecast in tcp_xmit_data.
 *
 *  9-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined TCP_COPY_THRESH. Added TCP stats. Modified tcp_xmit_data()
 *	to try to aggregate the first few small segments for a single
 *	send() operation.
 *
 *  7-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Made sure that all socket creation/destruction operations are
 *	serialized with conn_lock.
 *
 *  3-Dec-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 */
/*
 * tcp.c
 *
 *
 */
#ifndef	lint
char tcp_rcsid[] = "$ Header: tcp.c,v 1.15 89/05/02 11:18:11 dpj Exp $";
#endif not lint
/*
 * TCP transport module.
 *
 * This is a simple interface to the TCP facility in the kernel. It uses
 * normal Unix sockets to communicate with the kernel.
 */


#include	"netmsg.h"
#include	"nm_defs.h"

#if	USE_TCP

#if	RPCMOD
#else	RPCMOD
 ! You lose !
#endif	RPCMOD

#include	<cthreads.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<sys/signal.h>
#include	<sys/uio.h>

#include	"sbuf.h"
#include	"sys_queue.h"
#include	"transport.h"
#include	"disp_hdr.h"
#include	"dispatcher.h"
#include	"tcp_defs.h"

extern int	errno;

#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))

/*
 * Forward declarations.
 */
void	tcp_conn_handler();


/*
 * Size for a small (inline) receive buffer.
 *
 * Must be a multiple of 4.
 */
#define	TCP_BUFSZ		8192


/*
 * Size of iovec for data transmission.
 */
#define	TCP_IOVLEN		16

/*
 * TCP port to be used by the Mach network service.
 */
#define	TCP_NETMSG_PORT	2453


/*
 * Debugging flags.
 */
#define	TCP_DBG_MAJOR	(debug.tcp & 0x1)	/* major events */
#define	TCP_DBG_CRASH	(debug.tcp & 0x2)	/* host crashes */
#define	TCP_DBG_VERBOSE	(debug.tcp & 0x4)	/* verbose output */
#define	TCP_DBG_SOCKET	(debug.tcp & 0x8)	/* kernel SO_DEBUG */


/*
 * Control messages.
 */
#define	TCP_CTL_REQUEST		1
#define	TCP_CTL_REPLY		2
#define	TCP_CTL_CLOSEREQ	3
#define	TCP_CTL_CLOSEREP	4

typedef struct tcp_ctl {
	int		ctl;
	unsigned long	trid;
	int		code;
	unsigned long	size;
	int		crypt_level;
} tcp_ctl_t, *tcp_ctl_ptr_t;

/*
 * Connection records.
 */
typedef	struct tcp_conn {
	int			state;	/* see defines below */
	int			sock;	/* socket descriptor */
	cthread_t		th;	/* service thread */
	struct mutex		lock;	/* lock for this record */
	struct condition	cond;	/* to wake up the service thread */
	netaddr_t		peer;	/* peer for current connection */
	netaddr_t		to;	/* destination for active-side conn */
	sys_queue_head_t	trans;	/* list of pending/waiting transactions */
	int			count;	/* number of pending/waiting trans */
	sys_queue_chain_t	connq;	/* list of records */
	unsigned long		incarn;	/* incarnation number */
	struct iovec		iov[TCP_IOVLEN]; /* for writev() */
	tcp_ctl_t		ctlbuf;	/* for xmit control header */
} tcp_conn_t, *tcp_conn_ptr_t;

#define	TCP_INVALID	0
#define	TCP_FREE	1
#define	TCP_CONNECTED	2
#define	TCP_OPENING	3
#define	TCP_CLOSING	4
#define	TCP_CLOSED	5

/*
 * Static declarations.
 */
PRIVATE tcp_conn_t		conn_vec[32];	/* connection records */

PRIVATE sys_queue_head_t	conn_lru;	/* LRU list of active conn */
PRIVATE int			conn_num;	/* number of active conn */
PRIVATE sys_queue_head_t	conn_free;	/* list of free conn */
PRIVATE struct condition	conn_cond;	/* to wake up listener */
PRIVATE int			conn_closing;	/* number of conn in TCP_CLOSING */
PRIVATE struct mutex		conn_lock;	/* lock for conn_lru & conn_free */


/*
 * Transport IDs are composed of 16 bits for the client side and 16 bits
 * for the server side. The client side is just a counter, to be matched
 * between the message and the transaction record. The server side is composed
 * of 8 bits of index of the connection record in the conn_vec array and
 * 8 bits of incarnation number for this connection record.
 *
 * We can afford not to protect the counter for client-side IDs with a lock,
 * because transaction records for one connection are protected by the lock
 * that connection, and they never move from one connection to another.
 *
 * XXX This is not completely foolproof if there is A LOT of traffic,
 * but it's cheap.
 */
PRIVATE unsigned long			trid_counter;
#define	cptoix(cp)			((((unsigned long)(cp)) - \
		((unsigned long)conn_vec))/sizeof(tcp_conn_t))
#define	ixtocp(id)			((tcp_conn_ptr_t)&(conn_vec[(id)]))
#define TRID_SET_CLIENT(trid)		{ trid = (trid_counter++) & 0xffff; }
#define	TRID_GET_CLIENT(trid,cl)	{ (cl) = (trid) & 0xffff; }
#define	TRID_SET_SERVER(trid,sv)	{ (trid) |= \
					(cptoix(sv) << 24) | ((sv)->incarn << 16);}
#define	TRID_GET_SERVER(trid,sv)	{ (sv) = ixtocp((trid) >> 24); \
		if ((((trid) >> 16) & 0xff) != (sv)->incarn) (sv) = NULL; }



/*
 * Limits on connected sockets.
 */
#define	TCP_CONN_STEADY		param.tcp_conn_steady	/* steady-state max [16] */
#define	TCP_CONN_OPENING	param.tcp_conn_opening	/* max open/opening [18] */
#define	TCP_CONN_MAX		param.tcp_conn_max	/* absolute maximum [20] */


/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_TCPTRANS;



/*
 * Macro for transmission of data.
 *
 * cp->lock must be held throughout.
 */
#define	tcp_xmit_data(cp,ctlcode,a_trid,a_code,a_data,a_crypt,ret) {	\
	unsigned long	_size;						\
	sbuf_seg_ptr_t	_sp;						\
	sbuf_seg_ptr_t	_end;						\
	int		_iovc;						\
									\
	(cp)->ctlbuf.ctl = htonl(ctlcode);				\
	(cp)->ctlbuf.trid = htonl(a_trid);				\
	(cp)->ctlbuf.code = htonl(a_code);				\
	SBUF_GET_SIZE((a_data),_size);					\
	(cp)->ctlbuf.size = htonl(_size);				\
	(cp)->ctlbuf.crypt_level = htonl(a_crypt);			\
									\
	DEBUG6(TCP_DBG_VERBOSE,0,2800,cp,ctlcode,a_trid,		\
				a_code,&a_data,a_crypt);		\
									\
	/*								\
	 * XXX Worry about data encryption.				\
	 */								\
									\
	/*								\
	 * Fill in the iovec and send all the data possible.		\
	 */								\
	(cp)->iov[0].iov_base = (caddr_t)&((cp)->ctlbuf);		\
	(cp)->iov[0].iov_len = sizeof(tcp_ctl_t);			\
	_sp = (a_data).segs;						\
	_end = (a_data).end;						\
	_iovc = 1;							\
	while ((_sp != _end) && (_iovc < TCP_IOVLEN)) {			\
		if (_sp->s != 0) {					\
			(cp)->iov[_iovc].iov_base = (caddr_t)_sp->p;	\
			(cp)->iov[_iovc].iov_len = _sp->s;		\
			_iovc++;					\
		}							\
		_sp++;							\
	}								\
	ret = writev((cp)->sock,(cp)->iov,_iovc);			\
	INCSTAT(tcp_send);						\
	DEBUG3(TCP_DBG_VERBOSE,0,2801,_iovc,ret,errno);			\
									\
	/*								\
	 * Send the remaining segments one by one.			\
	 */								\
	while ((ret >= 0) && (_sp != _end)) {				\
		if (_sp->s != 0) {					\
			ret = send((cp)->sock,(void *)_sp->p,_sp->s,0);	\
			INCSTAT(tcp_send);				\
			DEBUG3(TCP_DBG_VERBOSE,0,2802,_sp->s,ret,errno);\
		}							\
		_sp++;							\
	}								\
}

/*
 * Macro for transmission of a simple control message.
 *
 * cp->lock must be held throughout.
 */
#define	tcp_xmit_control(cp,ctlcode,a_trid,a_code,ret) {	\
	(cp)->ctlbuf.ctl = htonl(ctlcode);			\
	(cp)->ctlbuf.trid = htonl(a_trid);			\
	(cp)->ctlbuf.code = htonl(a_code);			\
	(cp)->ctlbuf.size = 0;					\
	(cp)->ctlbuf.crypt_level = 0;				\
	ret =							\
	send((cp)->sock,&((cp)->ctlbuf),sizeof(tcp_ctl_t),0);	\
	INCSTAT(tcp_send);					\
	DEBUG6(TCP_DBG_VERBOSE,0,2803,cp,ctlcode,a_trid,	\
					a_code,ret,errno);	\
}



/*
 * tcp_init_conn --
 *
 * Allocate and initialize a new TCP connection record.
 *
 * Parameters:
 *
 * Results:
 *
 * pointer to the new record.
 *
 * Side effects:
 *
 * Starts a new thread to handle the connection.
 *
 * Note:
 *
 * conn_lock must be acquired before calling this routine.
 * It is held throughout its execution.
 */
PRIVATE tcp_conn_ptr_t tcp_init_conn()
BEGIN("tcp_init_conn")
	tcp_conn_ptr_t	cp;
	int		i;
	cthread_t	th;
	char		name[40];

	/*
	 * Find an unused connection record in the conn_vec array.
	 * We could have used the global memory allocator for that,
	 * but since there are few connection records, why bother...
	 *
	 * conn_lock guarantees mutual exclusion.
	 */
	cp = NULL;
	for (i = 0; i < 32; i++) {
		if (conn_vec[i].state == TCP_INVALID) {
			cp = &conn_vec[i];
			break;
		}
	}
	if (cp == NULL) {
		panic("The TCP module cannot allocate a new connection record");
	}

	cp->state = TCP_FREE;
	cp->sock = 0;
	cp->count = 0;
	cp->peer = 0;
	cp->to = 0;
	mutex_init(&cp->lock);
	mutex_lock(&cp->lock);
	condition_init(&cp->cond);
	sys_queue_init(&cp->trans);
	th = cthread_fork(tcp_conn_handler,cp);
	cp->th = th;
	sprintf(name,"tcp_conn_handler(0x%x)",cp);
	cthread_set_name(th,name);
	cthread_detach(th);

	DEBUG2(TCP_DBG_MAJOR,0,2805,cp,th);

	mutex_unlock(&cp->lock);

	RETURN(cp);
END



/*
 * tcp_close_conn --
 *
 * Arrange to close down one TCP connection as soon as possible.
 *
 * Parameters:
 *
 * Results:
 *
 * Side effects:
 *
 * Note:
 *
 * conn_lock must be acquired before calling this routine.
 * It is held throughout its execution.
 */
PRIVATE void tcp_close_conn()
BEGIN("tcp_close_conn")
	tcp_conn_ptr_t		first;
	tcp_conn_ptr_t		cp;
	int			ret;

	/*
	 * Look for an old connection to recycle.
	 */
	first = (tcp_conn_ptr_t)sys_queue_first(&conn_lru);
	cp = (tcp_conn_ptr_t)sys_queue_last(&conn_lru);
	while (cp != first) {
		if (cp->count == 0) {
			mutex_lock(&cp->lock);
			if ((cp->count == 0) && (cp->state == TCP_CONNECTED)) {
				break;
			} else {
				mutex_unlock(&cp->lock);
			}
		}
		cp = (tcp_conn_ptr_t)sys_queue_prev(&cp->connq);
	}
	if (cp == first) {
		/*
		 * We are over-committed. We will try again
		 * to close something at the next request or
		 * reply.
		 *
		 * XXX We could also set a timer to kill someone at
		 * random, to give new clients a chance.
		 */
		DEBUG2(TCP_DBG_MAJOR,0,2838,conn_num,conn_closing);
	} else {
		/*
		 * Close this unused connection.
		 */
		DEBUG4(TCP_DBG_MAJOR,0,2839,cp,cp->peer,conn_num,conn_closing);
		cp->state = TCP_CLOSING;
		conn_closing++;
		tcp_xmit_control(cp,TCP_CTL_CLOSEREQ,0,0,ret);
		mutex_unlock(&cp->lock);
	}

	RET;
END



/*
 * tcp_sendrequest --
 *
 * Send a request through the TCP interface.
 *
 * Parameters:
 *
 *	client_id	: an identifier assigned by the client to this transaction
 *	data		: the data to be sent
 *	to		: the destination of the request
 *	crypt_level	: whether the data should be encrypted
 *	reply_proc	: a function to be called to handle the response
 *
 * Results:
 *
 *	TR_SUCCESS or a specific failure code.
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT int tcp_sendrequest(client_id,data,to,crypt_level,reply_proc)
int		client_id;
sbuf_ptr_t	data;
netaddr_t	to;
int		crypt_level;
int		(*reply_proc)();
BEGIN("tcp_sendrequest")
	tcp_conn_ptr_t		first;
	tcp_conn_ptr_t		cp;
	tcp_trans_ptr_t		tp;
	int			ret;

	mutex_lock(&conn_lock);
	DEBUG4(TCP_DBG_VERBOSE,0,2837,to,client_id,conn_num,conn_closing);
	INCSTAT(tcp_requests_sent);

	/*
	 * Find an open connection to the destination.
	 */
	first = (tcp_conn_ptr_t)sys_queue_first(&conn_lru);
	cp = first;
	while (!sys_queue_end(&conn_lru,(sys_queue_entry_t)cp)) {
		if (cp->to == to) {
			break;
		}
		cp = (tcp_conn_ptr_t)sys_queue_next(&cp->connq);
	}

	if (sys_queue_end(&conn_lru,(sys_queue_entry_t)cp)) {
		/*
		 * Could not find an open connection.
		 */
		if (conn_num < TCP_CONN_OPENING) {
			/*
			 * Immediately start a new connection.
			 */
			if (sys_queue_empty(&conn_free)) {
				/*
				 * Initialize a new connection record.
				 */
				cp = tcp_init_conn();
			} else {
				cp = (tcp_conn_ptr_t)sys_queue_first(&conn_free);
				sys_queue_remove(&conn_free,cp,
							tcp_conn_ptr_t,connq);
			}
			mutex_lock(&cp->lock);
			DEBUG2(TCP_DBG_MAJOR,0,2840,cp,to);
			sys_queue_enter_first(&conn_lru,cp,tcp_conn_ptr_t,connq);
			conn_num++;
			cp->peer = to;
			cp->to = to;
			cp->state = TCP_OPENING;
			cp->count = 1;
#ifdef	notdef
			/*
			 * This is done when placing cp on the free list.
			 */
			sys_queue_init(&cp->trans);
#endif	notdef
			condition_signal(&cp->cond);
			mutex_unlock(&cp->lock);
			if ((conn_num - conn_closing) > TCP_CONN_STEADY) {
				tcp_close_conn();
			}
			mutex_unlock(&conn_lock);
		} else {
			/*
			 * We are over-committed. Tell the caller to wait.
			 */
			DEBUG0(TCP_DBG_MAJOR,0,2841);
			if ((conn_num - conn_closing) > TCP_CONN_STEADY) {
				tcp_close_conn();
			}
			mutex_unlock(&conn_lock);
			RETURN(TR_OVERLOAD);
		}
	} else {
		/*
		 * Found an open connection. Use it!
		 */
		DEBUG2(TCP_DBG_VERBOSE,0,2842,cp,cp->peer);
		if (cp != first) {
			/*
			 * Place the record at the head of the queue.
			 */
			sys_queue_remove(&conn_lru,cp,tcp_conn_ptr_t,connq);
			sys_queue_enter_first(&conn_lru,cp,tcp_conn_ptr_t,connq);
		}
		if ((conn_num - conn_closing) > TCP_CONN_STEADY) {
			tcp_close_conn();
		}
		mutex_lock(&cp->lock);
		cp->count++;
		mutex_unlock(&conn_lock);
	}

	/*
	 * At this point, we have a lock on a connection record for the
	 * right destination. See if we can transmit the data.
	 */

	/*
	 * Link the transaction record in the connection record.
	 */
	MEM_ALLOCOBJ(tp,tcp_trans_ptr_t,MEM_TCPTRANS);
	tp->client_id = client_id;
	tp->reply_proc = reply_proc;
	TRID_SET_CLIENT(tp->trid);

	DEBUG4(TCP_DBG_VERBOSE,0,2843,cp,cp->state,tp,tp->trid);

	if (cp->state == TCP_FREE) {
		panic("TCP module trying to transmit on a free connection");
	}

	if (cp->state == TCP_CONNECTED) {
		/*
		 * Send all the data on the socket.
		 */
		tp->state = TCP_TR_PENDING;
		tcp_xmit_data(cp,TCP_CTL_REQUEST,tp->trid,0,(*data),crypt_level,ret);
		if (ret < 0) {
			/*
			 * Something went wrong. Most probably, the client is dead.
			 */
			DEBUG2(TCP_DBG_CRASH,0,2844,cp->peer,errno);
			cp->count--;
			mutex_unlock(&cp->lock);
			MEM_DEALLOCOBJ(tp,MEM_TCPTRANS);
			RETURN(TR_FAILURE);
		}
	} else {
		tp->state = TCP_TR_WAITING;
		tp->data = data;
		tp->crypt_level = crypt_level;
	}
	sys_queue_enter(&cp->trans,tp,tcp_trans_ptr_t,transq);
	mutex_unlock(&cp->lock);

	RETURN(TR_SUCCESS);
END



/*
 * tcp_sendreply --
 *
 * Send a response through the TCP interface.
 *
 * Parameters:
 *
 *	trid		: transport-level ID for a previous operation on this
 *			  transaction
 *	code		: a return code to be passed to the client.
 *	data		: the data to be sent
 *	crypt_level	: whether the data should be encrypted
 *
 * Results:
 *
 *	TR_SUCCESS or a specific failure code.
 *
 * Side effects:
 *
 * Design:
 *
 * Note:
 *
 */
EXPORT int tcp_sendreply(trid,code,data,crypt_level)
int		trid;
int		code;
sbuf_ptr_t	data;
int		crypt_level;
BEGIN("tcp_sendreply")
	tcp_conn_ptr_t	cp;
	int		ret;

	TRID_GET_SERVER(trid,cp);

	/*
	 * If the client has died, the connection record may
	 * already have been reused, and we may be sending this reply
	 * to the wrong machine. This should be detected by the 
	 * incarnation number in the trid.
	 */
	if (cp == NULL) {
		DEBUG1(TCP_DBG_CRASH,0,2847,trid);
		RETURN(TR_FAILURE);
	}

	mutex_lock(&cp->lock);

	DEBUG4(TCP_DBG_VERBOSE,0,2845,trid,cp,cp->peer,cp->state);
	INCSTAT(tcp_replies_sent);

	if (cp->state != TCP_CONNECTED) {
		/*
		 * The client has died or the connection has just
		 * been dropped. Drop the reply.
		 */
		mutex_unlock(&cp->lock);
		RETURN(TR_FAILURE);
	}

	cp->count--;
	if (data) {
		tcp_xmit_data(cp,TCP_CTL_REPLY,trid,code,(*data),crypt_level,ret);
	} else {
		tcp_xmit_control(cp,TCP_CTL_REPLY,trid,code,ret);
	}

	if (ret < 0) {
		/*
		 * Something went wrong. Most probably, the client is dead.
		 */
		DEBUG2(TCP_DBG_CRASH,0,2846,cp->peer,errno);
		mutex_unlock(&cp->lock);
		RETURN(TR_FAILURE);
	}

	mutex_unlock(&cp->lock);

	/*
	 * Update the LRU list of active connections and check for
	 * excess connections.
	 */
	mutex_lock(&conn_lock);
	if (cp != (tcp_conn_ptr_t)sys_queue_first(&conn_lru)) {
		/*
		 * Place the record at the head of the queue.
		 */
		sys_queue_remove(&conn_lru,cp,tcp_conn_ptr_t,connq);
		sys_queue_enter_first(&conn_lru,cp,tcp_conn_ptr_t,connq);
	}
	if ((conn_num - conn_closing) > TCP_CONN_STEADY) {
		tcp_close_conn();
	}
	mutex_unlock(&conn_lock);

	RETURN(TR_SUCCESS);
END



/*
 * tcp_conn_handler_open --
 *
 * Handler for one connection - opening phase.
 *
 * Parameters:
 *
 * cp: pointer to the connection record.
 *
 * Results:
 *
 * TRUE if the connection was successfully opened, FALSE otherwise.
 *
 * Side effects:
 *
 * Transactions waiting in the connection record are initiated.
 *
 * Note:
 *
 * cp->lock must be locked on entry. It is also locked on exit, but
 * it may be unlocked during the execution of this procedure.
 */
PRIVATE boolean_t tcp_conn_handler_open(cp)
	tcp_conn_ptr_t	cp;
BEGIN("tcp_conn_handler_open")
	tcp_trans_ptr_t		tp;
	int			cs;
	struct sockaddr_in	sname;
	netaddr_t		peeraddr;
	int			ret;

	sname.sin_family = AF_INET;
	sname.sin_port = htons(TCP_NETMSG_PORT);
	sname.sin_addr.s_addr = (u_long)(cp->peer);
	peeraddr = cp->peer;

	/*
	 * Unlock the record while we are waiting for the connection
	 * to be established.
	 */
	mutex_unlock(&cp->lock);

	mutex_lock(&conn_lock);
	cs = socket(AF_INET,SOCK_STREAM,0);
	mutex_unlock(&conn_lock);
	if (cs < 0) {
		ERROR((msg,"tcp_conn_handler.socket failed: errno=%d",errno));
		panic("tcp");
	}

	if (TCP_DBG_SOCKET) {
		int	optval;

		optval = 1;
		setsockopt(cs,SOL_SOCKET,SO_DEBUG,&optval,sizeof(int));
	}

	ret = connect(cs,(struct sockaddr *)&sname,sizeof(struct sockaddr_in));
	if (ret < 0) {
		DEBUG2(TCP_DBG_CRASH,0,2815,peeraddr,errno);
		mutex_lock(&cp->lock);
		RETURN(FALSE);
	}
	INCSTAT(tcp_connect);

	mutex_lock(&cp->lock);
	cp->sock = cs;
	cp->state = TCP_CONNECTED;
	DEBUG3(TCP_DBG_VERBOSE,0,2816,cp,cs,peeraddr);

	/*
	 * Look for transactions waiting to be transmitted.
	 */
	tp = (tcp_trans_ptr_t)sys_queue_first(&cp->trans);
	while (!sys_queue_end(&cp->trans,(sys_queue_entry_t)tp)) {
		DEBUG2(TCP_DBG_VERBOSE,0,2817,tp,tp->state);
		if (tp->state == TCP_TR_WAITING) {
			tp->state = TCP_TR_PENDING;
			tcp_xmit_data(cp,TCP_CTL_REQUEST,tp->trid,0,
						(*(tp->data)),tp->crypt_level,ret);
			if (ret < 0) {
				RETURN(FALSE);
			}
		}
		tp = (tcp_trans_ptr_t)sys_queue_next(&tp->transq);
	}

	RETURN(TRUE);
END



/*
 * tcp_conn_handler_active --
 *
 * Handler for one connection - active phase.
 *
 * Parameters:
 *
 * cp: pointer to the connection record.
 *
 * Results:
 *
 * Exits when the connection should be closed.
 *
 * Note:
 *
 * For now, the data received on the connection is only kept until the
 * higher-level handler procedure (disp_in_request or reply_proc) returns.
 * This allows the use of a data buffer on the stack.
 *
 */
PRIVATE void tcp_conn_handler_active(cp)
	tcp_conn_ptr_t	cp;
BEGIN("tcp_conn_handler_active")
	int			cs;
	netaddr_t		peeraddr;
	tcp_trans_ptr_t		tp;
	int			ret;
	pointer_t		bufp;		/* current location in data */
	tcp_ctl_ptr_t		ctlbufp;	/* tcp control header in buf */
	int			buf_count;	/* data available in buf */
	int			buf_free;	/* free space in buf */
	pointer_t		bigbufp;	/* large (variable) data buffer */
	unsigned long		bigbuf_size;	/* size of bigbuf */
	unsigned long		bigbuf_count;	/* data in bigbuf */
	sbuf_t			sb;		/* sbuf for data received */
	unsigned long		data_size;	/* size of user data */
	unsigned long		seg_size;
	unsigned long		trid;
	int			disp_ret;
	/*
	 * The following buffers are used to receive small amounts
	 * of data inline, and to realign that data on a long word
	 * boundary if needed. They are declared as int in order
	 * to have the correct alignment.
	 */
	int			buf[TCP_BUFSZ / 4];
	int			abuf[TCP_BUFSZ / 4];
	int			curbuf;	/* 0 = buf, 1 = abuf */

	/*
	 * Prepare the sbuf for the handoff to the higher-level.
	 * There will always be at most two segments, from buf and bigbuf.
	 */
	SBUF_INIT(sb,2);

	peeraddr = cp->peer;	/* OK not to lock at this point */
	cs = cp->sock;
	buf_count = 0;

	/*
	 * Enter the recv loop.
	 */
	for (;;) {
		/*
		 * Recycle the data buffer whenever needed.
		 */
		if (buf_count > 0) {
			ctlbufp = (tcp_ctl_ptr_t)bufp;
		} else {
			bufp = (pointer_t)buf;
			ctlbufp = (tcp_ctl_ptr_t)bufp;
			curbuf = 0;
			buf_count = 0;
			buf_free = TCP_BUFSZ - sizeof(tcp_ctl_t);
		}
		
		DEBUG5(TCP_DBG_VERBOSE,0,2818,peeraddr,ctlbufp,bufp,
							buf_count,buf_free);

		/*
		 * Get at least a tcp control header in the
		 * buffer. We always keep some space at the
		 * end of buf in case we had received part of
		 * the header in a previous pass. If we receive
		 * the header in that extra space, we do not
		 * receive more than that, so we can always
		 * recycle the buffer after that.
		 */
		while (buf_count < sizeof(tcp_ctl_t)) {
			ret = recv(cs,(void *)bufp,
			    MAX(buf_free,sizeof(tcp_ctl_t)),0);
			if (ret <= 0) {
				if (ret < 0) {
					DEBUG2(TCP_DBG_CRASH,0,2819,
								peeraddr,errno);
				}
				RET;
			}
			INCSTAT(tcp_recv);
			DEBUG2(TCP_DBG_VERBOSE,0,2820,ret,peeraddr);
			buf_count += ret;
			buf_free -= ret;
			bufp += ret;
		}

		/*
		 * Realign the data if needed.
		 */
		if (((int)ctlbufp) & 0x3) {
			if (curbuf) {
				bcopy((caddr_t)ctlbufp,(caddr_t)buf,
								buf_count);
				curbuf = 0;
				ctlbufp = (tcp_ctl_ptr_t)buf;
				bufp = ((pointer_t)buf) + buf_count;
			} else {
				bcopy((caddr_t)ctlbufp,(caddr_t)abuf,
								buf_count);
				curbuf = 1;
				ctlbufp = (tcp_ctl_ptr_t)abuf;
				bufp = ((pointer_t)abuf) + buf_count;
			}
		}

		/*
		 * Do all the required byte-swapping (Sigh!).
		 */
		ctlbufp->ctl		= ntohl(ctlbufp->ctl);
		ctlbufp->trid		= ntohl(ctlbufp->trid);
		ctlbufp->code		= ntohl(ctlbufp->code);
		ctlbufp->size		= ntohl(ctlbufp->size);
		ctlbufp->crypt_level	= ntohl(ctlbufp->crypt_level);

		/*
		 * Read any user data from the small buffer
		 * and put it in the sbuf.
		 * Advance the current data pointer.
		 */
		bufp = (pointer_t)(ctlbufp + 1);
		buf_count -= sizeof(tcp_ctl_t);
		data_size = ctlbufp->size;
		if (data_size > 0) {
			SBUF_REINIT(sb);
			seg_size = MIN(data_size,buf_count);
			SBUF_APPEND(sb,bufp,seg_size);
			buf_count -= seg_size;
			bufp += seg_size;
		}
		DEBUG6(TCP_DBG_VERBOSE,0,2821,peeraddr,data_size,seg_size,
							ctlbufp,bufp,buf_count);

		/*
		 * Get more data in a large buffer if needed.
		 */
		if (data_size > seg_size) {
			bigbuf_size = data_size - seg_size;
			MEM_ALLOC(bigbufp,pointer_t,bigbuf_size,FALSE);
			bigbuf_count = 0;
			bufp = bigbufp;
			DEBUG3(TCP_DBG_VERBOSE,0,2822,peeraddr,
							bigbufp,bigbuf_size);
			while (bigbuf_count < bigbuf_size) {
				DEBUG3(TCP_DBG_VERBOSE,0,2823,peeraddr,
							bufp,bigbuf_count);
				ret = recv(cs,(void *)bufp,
					(bigbuf_size - bigbuf_count),0);
				if (ret <= 0) {
					if (ret < 0) {
						DEBUG2(TCP_DBG_CRASH,0,2824,
								peeraddr,errno);
					}
					MEM_DEALLOC(bigbufp,bigbuf_size);
					RET;
				}
				DEBUG2(TCP_DBG_VERBOSE,0,2825,ret,peeraddr);
				INCSTAT(tcp_recv);
				bigbuf_count += ret;
				bufp += ret;
			}
			SBUF_APPEND(sb,bigbufp,bigbuf_size);
		} else {
			bigbufp = NULL;
		}

		/*
		 * XXX Worry about encryption.
		 */

		/*
		 * Now process the message.
		 */
 		DEBUG5(TCP_DBG_VERBOSE,0,2826,ctlbufp->ctl,ctlbufp->size,
						ctlbufp->trid,ctlbufp->code,
						ctlbufp->crypt_level);
		switch(ctlbufp->ctl) {
			case TCP_CTL_REQUEST:
				INCSTAT(tcp_requests_rcvd);
				mutex_lock(&cp->lock);
				cp->count++;
				if (cp->state == TCP_CLOSING) {
					cp->state = TCP_CONNECTED;
					mutex_unlock(&cp->lock);
					mutex_lock(&conn_lock);
					conn_closing--;
					mutex_unlock(&conn_lock);
				} else {
					mutex_unlock(&cp->lock);
				}
				trid = ctlbufp->trid;
				TRID_SET_SERVER(trid,cp);
				disp_ret = disp_in_request(TR_TCP_ENTRY,trid,
						&sb,peeraddr,
						ctlbufp->crypt_level,FALSE);
				/*
				 * Clean up the big buffer if there is one.
				 */
				if (bigbufp != NULL) {
					DEBUG1(TCP_DBG_VERBOSE,0,2832,bigbufp);
					MEM_DEALLOC(bigbufp,bigbuf_size);
					bigbufp = NULL;
				}
				if (disp_ret != DISP_WILL_REPLY) {
					mutex_lock(&cp->lock);
					DEBUG3(TCP_DBG_VERBOSE,0,2827,peeraddr,
								trid,disp_ret);
					tcp_xmit_control(cp,TCP_CTL_REPLY,trid,
								disp_ret,ret);
					cp->count--;
					mutex_unlock(&cp->lock);
					if (ret < 0) {
						RET;
					}
				}
				break;

			case TCP_CTL_REPLY:
				INCSTAT(tcp_replies_rcvd);
				mutex_lock(&cp->lock);
				if (cp->state == TCP_CLOSING) {
					cp->state = TCP_CONNECTED;
					mutex_unlock(&cp->lock);
					mutex_lock(&conn_lock);
					conn_closing--;
					mutex_unlock(&conn_lock);
					mutex_lock(&cp->lock);
				}
				/*
				 * Find the transaction record.
				 */
				TRID_GET_CLIENT(ctlbufp->trid,trid);
				tp = (tcp_trans_ptr_t)sys_queue_first(&cp->trans);
				while (!sys_queue_end(&cp->trans,(sys_queue_entry_t)tp)) {
					if (tp->trid == trid) {
						break;
					}
					tp = (tcp_trans_ptr_t)sys_queue_next(&tp->transq);
				}
				if (sys_queue_end(&cp->trans,(sys_queue_entry_t)tp)) {
					ERROR((msg,
"tcp_conn_handler_active: cannot find the transaction record for a reply (trid = 0x%x)", trid));
					mutex_unlock(&cp->lock);
				} else {
					sys_queue_remove(&cp->trans,tp,
							tcp_trans_ptr_t,transq);
					cp->count--;
					mutex_unlock(&cp->lock);
					DEBUG1(TCP_DBG_VERBOSE,0,2828,tp);
					if (data_size) {
						(*(tp->reply_proc))(tp->client_id,
								ctlbufp->code,&sb);
					} else {
						(*(tp->reply_proc))(tp->client_id,
								ctlbufp->code,0);
					}
					MEM_DEALLOCOBJ(tp,MEM_TCPTRANS);
				}
				/*
				 * Clean up the big buffer if there is one.
				 */
				if (bigbufp != NULL) {
					DEBUG1(TCP_DBG_VERBOSE,0,2833,bigbufp);
					MEM_DEALLOC(bigbufp,bigbuf_size);
					bigbufp = NULL;
				}
				break;

			case TCP_CTL_CLOSEREQ:
				mutex_lock(&cp->lock);
				if (cp->count == 0) {
					/*
					 * Send CLOSEREP.
					 */
					DEBUG1(TCP_DBG_MAJOR,0,2829,cp->peer);
					tcp_xmit_control(cp,TCP_CTL_CLOSEREP,
									0,0,ret);
					if (cp->state != TCP_CLOSING) {
						cp->state = TCP_CLOSED;
					}
					mutex_unlock(&cp->lock);
					RET;
				} else {
					/*
					 * We have some data in
					 * transit. Nothing more
					 * should be needed.
					 */
					DEBUG2(TCP_DBG_MAJOR,0,2830,cp->peer,
									cp->count);
					cp->state = TCP_CONNECTED;
					mutex_unlock(&cp->lock);
				}
				break;

			case TCP_CTL_CLOSEREP:
				mutex_lock(&cp->lock);
				DEBUG1(TCP_DBG_MAJOR,0,2831,cp->peer);
				/*
				 * cp->state can only be TCP_CLOSING:
				 *
				 * We have sent a CLOSEREQ, and set the
				 * state to TCP_CLOSING then. If the state
				 * has changed since then, it must be because
				 * we have received data. But this data can only
				 * be a request, because we had nothing going on
				 * when we sent the CLOSEREQ. This CLOSEREQ must
				 * arrive at the other end before our reply
				 * because TCP does not reorder messages. But
				 * then the CLOSEREQ will be rejected because
				 * of the pending transaction.
				 */
				mutex_unlock(&cp->lock);
				RET;

			default:
				ERROR((msg,
		"tcp_conn_handler_active: received an unknown ctl code: %d",
								ctlbufp->ctl));
				break;
		}
	}

END



/*
 * tcp_conn_handler_close --
 *
 * Handler for one connection - closing phase.
 *
 * Parameters:
 *
 * cp: pointer to the connection record.
 *
 * Results:
 *
 * none.
 *
 * Note:
 *
 */
PRIVATE void tcp_conn_handler_close(cp)
	tcp_conn_ptr_t	cp;
BEGIN("tcp_conn_handler_close")
	tcp_trans_ptr_t		tp;

	/*
	 * Some transactions might be initiated after the active phase exits
	 * and before this phase starts. Hopefully, they will be stopped by
	 * the TCP_CLOSING or TCP_CLOSED states, or the send will fail.
	 */
	mutex_lock(&conn_lock);
	mutex_lock(&cp->lock);
	close(cp->sock);
	INCSTAT(tcp_close);
	if (cp->state == TCP_CLOSING) {
		conn_closing--;
	}
	cp->state = TCP_FREE;

	/*
	 * Go down the list of waiting/pending transactions
	 * and abort them.
	 * The client is of course free to retry them later.
	 */
	while (!sys_queue_empty(&cp->trans)) {
		tp = (tcp_trans_ptr_t)sys_queue_first(&cp->trans);
		DEBUG3(TCP_DBG_VERBOSE,0,2834,tp,tp->state,tp->client_id);
		if (tp->state == TCP_TR_WAITING) {
			(*tp->reply_proc)
				(tp->client_id,TR_SEND_FAILURE,0,0);
		} else {
			(*tp->reply_proc)
				(tp->client_id,TR_FAILURE,0,0);
		}
		sys_queue_remove(&cp->trans,tp,tcp_trans_ptr_t,transq);
		MEM_DEALLOCOBJ(tp,MEM_TCPTRANS);
	}
	sys_queue_init(&cp->trans);
	cp->count = 0;
	cp->to = 0;
	sys_queue_remove(&conn_lru,cp,tcp_conn_ptr_t,connq);
	sys_queue_enter(&conn_free,cp,tcp_conn_ptr_t,connq);
	mutex_unlock(&cp->lock);
	conn_num--;
	DEBUG1(TCP_DBG_MAJOR,0,2835,conn_num);
	if (conn_num == (TCP_CONN_MAX - 1)) {
		/*
		 * OK to start accepting connections again.
		 */
		DEBUG0(TCP_DBG_MAJOR,0,2836);
		condition_signal(&conn_cond);
	}
	mutex_unlock(&conn_lock);

	RET;
END



/*
 * tcp_conn_handler --
 *
 * Handler for one connection.
 *
 * Parameters:
 *
 * cp: pointer to the connection record.
 *
 * Results:
 *
 *	Should never exit.
 *
 * Note:
 *
 * For clarity, this code is split into three different procedures handling
 * the opening, active and closing phases of the life of the connection.
 *
 */
PRIVATE void tcp_conn_handler(cp)
	tcp_conn_ptr_t	cp;
BEGIN("tcp_conn_handler")
	boolean_t	active;

	for (;;) {
		/*
		 * First wait to be activated.
		 */
		mutex_lock(&cp->lock);
		while(cp->state == TCP_FREE) {
			DEBUG0(TCP_DBG_VERBOSE,0,2811);
			condition_wait(&cp->cond,&cp->lock);
		}

		/*
		 * At this point, the state is either TCP_OPENING (local open)
		 * or TCP_CONNECTED (remote open).
		 */
		DEBUG3(TCP_DBG_VERBOSE,0,2812,cp,cp->state,cp->peer);

		if (cp->state == TCP_OPENING) {
			/*
			 * Open a new connection.
			 */
			active = tcp_conn_handler_open(cp);
		} else {
			active = TRUE;
		}
		cp->incarn = (cp->incarn++) & 0xff;
		mutex_unlock(&cp->lock);

		if (active) {
			DEBUG3(TCP_DBG_MAJOR,0,2813,cp,cp->sock,cp->peer);
			tcp_conn_handler_active(cp);
			DEBUG3(TCP_DBG_MAJOR,0,2814,cp,cp->sock,cp->peer);
		}

		/*
		 * Close the connection.
		 */
		tcp_conn_handler_close(cp);

		LOGCHECK;
	}
END



/*
 * tcp_listener --
 *
 * Handler for the listener socket.
 *
 * Parameters:
 *
 * Results:
 *
 *	Should never exit.
 *
 * Note:
 *
 */
PRIVATE void tcp_listener()
BEGIN("tcp_listener")
	int			s;
	int			ret;
	struct sockaddr_in	sname;
	int			snamelen;
	tcp_conn_ptr_t		cp;

	/*
	 * First create the listener socket.
	 */
	mutex_lock(&conn_lock);
	s = socket(AF_INET,SOCK_STREAM,0);
	mutex_unlock(&conn_lock);
	if (s < 0) {
		ERROR((msg,"tcp_listener.socket failed: errno=%d",errno));
		panic("tcp");
	}
	sname.sin_family = AF_INET;
	sname.sin_port = htons(TCP_NETMSG_PORT);
	sname.sin_addr.s_addr = INADDR_ANY;
	ret = bind(s,(struct sockaddr *)&sname,sizeof(struct sockaddr_in));
	if (ret < 0) {
		ERROR((msg,"tcp_listener.bind failed: errno=%d",errno));
		panic("tcp");
	}
	ret = listen(s,2);
	if (ret < 0) {
		ERROR((msg,"tcp_listener.listen failed: errno=%d",errno));
		panic("tcp");
	}
	DEBUG1(TCP_DBG_VERBOSE,0,2806,s);

	/*
	 * Loop forever accepting connections.
	 */
	for (;;) {
		mutex_lock(&conn_lock);
		while (conn_num >= TCP_CONN_MAX) {
			DEBUG1(TCP_DBG_VERBOSE,0,2810,conn_num);
			condition_wait(&conn_cond,&conn_lock);
		}

		mutex_unlock(&conn_lock);
		DEBUG0(TCP_DBG_VERBOSE,0,2807);
		snamelen = sizeof(struct sockaddr_in);
		ret = accept(s,(struct sockaddr *)&sname,&snamelen);
		if (ret < 0) {
			ERROR((msg,
				"tcp_listener.accept failed: errno=%d",errno));
			continue;
		}
		INCSTAT(tcp_accept);
		DEBUG0(TCP_DBG_VERBOSE,0,2808);

		if (TCP_DBG_SOCKET) {
			int	optval;

			optval = 1;
			setsockopt(ret,SOL_SOCKET,SO_DEBUG,&optval,sizeof(int));
		}

		mutex_lock(&conn_lock);
		if (sys_queue_empty(&conn_free)) {
			/*
			 * Initialize a new connection record.
			 */
			cp = tcp_init_conn();
		} else {
			cp = (tcp_conn_ptr_t)sys_queue_first(&conn_free);
			sys_queue_remove(&conn_free,cp,tcp_conn_ptr_t,connq);
		}
		mutex_lock(&cp->lock);
		DEBUG4(TCP_DBG_MAJOR,0,2809,ret,cp,
					sname.sin_addr.s_addr,sname.sin_port);
		sys_queue_enter_first(&conn_lru,cp,tcp_conn_ptr_t,connq);
		conn_num++;
		cp->sock = ret;
		cp->peer = (netaddr_t)(sname.sin_addr.s_addr);
		cp->to = 0;
		cp->state = TCP_CONNECTED;
		cp->count = 0;
#ifdef	notdef
		/*
		 * This is done when placing cp on the free list.
		 */
		sys_queue_init(&cp->trans);
#endif	notdef
		condition_signal(&cp->cond);
		mutex_unlock(&cp->lock);
		if ((conn_num - conn_closing) > TCP_CONN_STEADY) {
			tcp_close_conn();
		}
		mutex_unlock(&conn_lock);
		LOGCHECK;
	}

END



/*
 * tcp_init --
 *
 * Initialises the TCP transport protocol.
 *
 * Parameters:
 *
 * Results:
 *
 *	FALSE : we failed to initialise the TCP transport protocol.
 *	TRUE  : we were successful.
 *
 * Side effects:
 *
 *	Initialises the TCP protocol entry point in the switch array.
 *	Allocates the listener port and creates a thread to listen to the network.
 *
 */
EXPORT boolean_t tcp_init()
BEGIN("tcp_init")
	int		i;
	tcp_conn_ptr_t	cp;
	cthread_t	th;
	struct sigvec	svec, osvec;

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_TCPTRANS,"TCP transaction record",sizeof(tcp_trans_t),
								FALSE,120,50);

	/*
	 * Initialize the set of connection records and the lists.
	 */
	for (i = 0; i < 32; i++) {
		conn_vec[i].state = TCP_INVALID;
		conn_vec[i].incarn = 0;
	}
	mutex_init(&conn_lock);
	mutex_lock(&conn_lock);
	condition_init(&conn_cond);
	sys_queue_init(&conn_lru);
	sys_queue_init(&conn_free);
	conn_num = 0;
	conn_closing = 0;
	trid_counter = 10;

	/*
	 * Create a first connection record (just a test).
	 */
	cp = tcp_init_conn();
	sys_queue_enter(&conn_free,cp,tcp_conn_ptr_t,connq);

	/*
	 * Set up the entry in the transport switch.
	 */
	transport_switch[TR_TCP_ENTRY].sendrequest = tcp_sendrequest;
	transport_switch[TR_TCP_ENTRY].sendreply = tcp_sendreply;

	/*
	 * Start the listener.
	 */
	th = cthread_fork(tcp_listener,0);
	cthread_set_name(th,"tcp_listener");
	cthread_detach(th);

	/*
	 * Under certain conditions, the kernel may raise a
	 * SIGPIPE when a send() fails. We want none of that...
	 */
	 svec.sv_handler = SIG_IGN;
	 svec.sv_mask = 0;
	 svec.sv_flags = 0;
	 sigvec(SIGPIPE,&svec,&osvec);

	/*
	 * Get the show on the road...
	 */
	DEBUG0(TCP_DBG_MAJOR,0,2804);
	mutex_unlock(&conn_lock);
	RETURN(TRUE);

END


#else	USE_TCP
	/*
	 * Just a dummy to keep the loader happy.
	 */
static int	dummy;
#endif	USE_TCP
