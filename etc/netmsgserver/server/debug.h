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
 * $Log:	debug.h,v $
 * Revision 1.1  90/02/19  21:50:12  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.15  89/05/02  11:07:43  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 15-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Added optional call to syslog in ERROR.
 *
 * 19-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added INCPORTSTAT macro.
 *
 * 19-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Removed an unmatched close-curly in DL_BODY* with NET_PRINT = 0.
 *
 * 19-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added INCSTAT macro.
 *
 * 25-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added macros for debugging network ports and network addresses.
 *	Added macro for debugging sbufs.
 *	Cast all parameters to debugging macros into longs.
 *	Added extern definition of thread_lock.
 *
 * 16-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed the #ifndef checks - put them in config.h.
 *	Include config.h.
 *
 * 15-Apr-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Fixed to always use locking when invoking stdio.
 *	Delinted.
 *
 * 10-Mar-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Defined all the heavy-duty logging and debugging macros.
 *
 * 15-Nov-86  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created.
 *
 *
 */
/*
 * debug.h 
 *
 *
 * $ Header: debug.h,v 1.15 89/05/02 11:07:43 dpj Exp $ 
 *
 */

/*
 * Macros used for debugging, tracing and reporting errors. 
 *
 * NOTE: this file makes use of NET_DEBUG, NET_LOG and NET_PRINT defined
 * in config.h. To allow redefinition of these macros for a single file,
 * config.h is not automatically included, but the macros must be defined.
 */


#ifndef	_DEBUG_
#define	_DEBUG_

#include	<stdio.h>
#include	<strings.h>
#include	<cthreads.h>
#include	<syslog.h>

#include	"config.h"
#include	"ls_defs.h"
#include	"nm_defs.h"

extern	log_rec_t	*log_cur_ptr;
extern	log_rec_t	*log_end_ptr;
extern	mutex_t		log_lock;
extern	stat_t		nmstat;
extern	debug_t		debug;
extern	param_t		param;

#if	LOCK_THREADS
extern	mutex_t		thread_lock;
#endif	LOCK_THREADS

#if	NM_STATISTICS
#define INCSTAT(field) nmstat.field++
#else	NM_STATISTICS
#define INCSTAT(field)
#endif	NM_STATISTICS

#if	PORTSTAT
#define INCPORTSTAT(pr_ptr,field) (pr_ptr)->portrec_stat->field++
#else	PORTSTAT
#define INCPORTSTAT(pr_ptr,field)
#endif	PORTSTAT


/*
 * The panic() routine is to be called when an unrecoverable error occurs. 
 * It stops the whole server and dumps the log.
 */
extern	void	panic();

/*
 * DL_BODY and DL_STRING_BODY write a message to the log if cond is true.
 * In addition, if NET_PRINT is true, the message is also printed on stderr if
 * level is greater than debug.print_level. These macros are intended to be used
 * only in the definitions of other macros, and should not be used directly in
 * user code.
*/
#if	NET_DEBUG || NET_LOG
#if	NET_PRINT
#define	DL_BODY0(cond,level,dcode) {				\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d \n",	\
					cthread_self(),dcode);	\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
	}							\
}
#define	DL_BODY1(cond,level,dcode,da1) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x\n",\
					cthread_self(),dcode,	\
					da1);			\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
	}							\
}
#define	DL_BODY2(cond,level,dcode,da1,da2) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x %x\n",\
					cthread_self(),dcode,	\
					da1,da2);		\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
	}							\
}
#define	DL_BODY3(cond,level,dcode,da1,da2,da3) {		\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x %x %x\n",\
					cthread_self(),dcode,	\
					da1,da2,da3);		\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
	}							\
}
#define	DL_BODY4(cond,level,dcode,da1,da2,da3,da4) {		\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x %x %x %x\n",\
					cthread_self(),dcode,	\
					da1,da2,da3,da4);	\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
	}							\
}
#define	DL_BODY5(cond,level,dcode,da1,da2,da3,da4,da5) {	\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x %x %x %x %x\n",\
					cthread_self(),dcode,	\
					da1,da2,da3,da4,da5);	\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
		lp->a5 = (long)da5;				\
	}							\
}
#define	DL_BODY6(cond,level,dcode,da1,da2,da3,da4,da5,da6) {	\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : %x %x %x %x %x %x\n",\
					cthread_self(),dcode,	\
					da1,da2,da3,da4,da5,da6);\
			(void)fflush(stderr);			\
		}						\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
		lp->a5 = (long)da5;				\
		lp->a6 = (long)da6;				\
	}							\
}
#define	DL_STRING_BODY(cond,level,dcode,ds) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr;				\
		log_cur_ptr += 4;				\
		if (level >= debug.print_level) {		\
			fprintf(stderr,"(%d) Debug code %4d : \"%s\"\n",\
				cthread_self(),dcode,ds);	\
			(void)fflush(stderr);			\
		}						\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		(void)strncpy((char *)&lp->a1,ds,110);		\
		((char *)&lp->a1)[110] = '\0';			\
		mutex_unlock(log_lock);				\
	}							\
}
#else	NET_PRINT
#define	DL_BODY0(cond,level,dcode) {				\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
	}							\
}
#define	DL_BODY1(cond,level,dcode,da1) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
	}							\
}
#define	DL_BODY2(cond,level,dcode,da1,da2) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
	}							\
}
#define	DL_BODY3(cond,level,dcode,da1,da2,da3) {		\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
	}							\
}
#define	DL_BODY4(cond,level,dcode,da1,da2,da3,da4) {		\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
	}							\
}
#define	DL_BODY5(cond,level,dcode,da1,da2,da3,da4,da5) {	\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
		lp->a5 = (long)da5;				\
	}							\
}
#define	DL_BODY6(cond,level,dcode,da1,da2,da3,da4,da5,da6) {	\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr++;				\
		mutex_unlock(log_lock);				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		lp->a1 = (long)da1;				\
		lp->a2 = (long)da2;				\
		lp->a3 = (long)da3;				\
		lp->a4 = (long)da4;				\
		lp->a5 = (long)da5;				\
		lp->a6 = (long)da6;				\
	}							\
}
#define	DL_STRING_BODY(cond,level,dcode,ds) {			\
	if (cond) {						\
		register log_rec_t *lp;				\
		mutex_lock(log_lock);				\
		lp = log_cur_ptr;				\
		log_cur_ptr += 4;				\
		lp->code = dcode;				\
		lp->thread = (long) cthread_self();		\
		(void)strncpy((char *)&lp->a1,ds,110);		\
		((char *)&lp->a1)[110] = '\0';			\
		mutex_unlock(log_lock);				\
	}							\
}
#endif	NET_PRINT
#endif	NET_DEBUG || NET_LOG

#if	NET_DEBUG || NET_LOG
#define DL_NPORT_BODY(cond,level,nport) {			\
	/* register */ ip_addr_t ip_addr;				\
	DL_BODY4(cond,level,3,					\
		nport.np_puid.np_uid_high,			\
		nport.np_puid.np_uid_low,			\
		nport.np_sid.np_uid_high,			\
		nport.np_sid.np_uid_low);			\
	ip_addr.ia_netaddr = nport.np_receiver;			\
	DL_BODY4(cond,level,4,					\
		ip_addr.ia_bytes.ia_net_owner,			\
		ip_addr.ia_bytes.ia_net_node_type,		\
		ip_addr.ia_bytes.ia_host_high,			\
		ip_addr.ia_bytes.ia_host_low);			\
	ip_addr.ia_netaddr = nport.np_owner;			\
	DL_BODY4(cond,level,5,					\
		ip_addr.ia_bytes.ia_net_owner,			\
		ip_addr.ia_bytes.ia_net_node_type,		\
		ip_addr.ia_bytes.ia_host_high,			\
		ip_addr.ia_bytes.ia_host_low);			\
}
#define DL_NETADDR_BODY(cond,level,netaddr) {			\
	/* register */ ip_addr_t ip_addr;				\
	ip_addr.ia_netaddr = netaddr;				\
	DL_BODY4(cond,level,6,					\
		ip_addr.ia_bytes.ia_net_owner,			\
		ip_addr.ia_bytes.ia_net_node_type,		\
		ip_addr.ia_bytes.ia_host_high,			\
		ip_addr.ia_bytes.ia_host_low);			\
}
#define DL_KEY_BODY(cond,level,key) {				\
	DL_BODY4(cond,level,9,					\
			key.key_longs[0],key.key_longs[1],	\
			key.key_longs[2],key.key_longs[3]);	\
}
#define DL_SBUF_BODY(cond,level,sbuf) {				\
	/* register */ sbuf_seg_ptr_t	seg_ptr;			\
	DL_BODY5(cond,level,7,&sbuf,sbuf.end,sbuf.segs,		\
			sbuf.free,sbuf.size);			\
	for (seg_ptr = sbuf.segs; seg_ptr != sbuf.end; seg_ptr++) {\
		DL_BODY3(cond,level,8,seg_ptr,			\
				seg_ptr->p,seg_ptr->s);		\
	}							\
}
#endif	NET_DEBUG || NET_LOG


/*
 * DEBUG macros are used to enter data into the LOG, if NET_DEBUG is true.
 * They use the DL_BODY macros.
 */
#if	NET_DEBUG
#define	DEBUG0(cond,level,dcode)			DL_BODY0(cond,level,dcode)
#define	DEBUG1(cond,level,dcode,da1)			DL_BODY1(cond,level,dcode,da1)
#define	DEBUG2(cond,level,dcode,da1,da2)		DL_BODY2(cond,level,dcode,da1,da2)
#define	DEBUG3(cond,level,dcode,da1,da2,da3)		DL_BODY3(cond,level,dcode,da1,da2,da3)
#define	DEBUG4(cond,level,dcode,da1,da2,da3,da4)	DL_BODY4(cond,level,dcode,da1,da2,da3,da4)
#define	DEBUG5(cond,level,dcode,da1,da2,da3,da4,da5)	DL_BODY5(cond,level,dcode,da1,da2,da3,da4,da5)
#define	DEBUG6(cond,level,dcode,da1,da2,da3,da4,da5,da6) DL_BODY6(cond,level,dcode,da1,da2,da3,da4,da5,da6)
#define	DEBUG_STRING(cond,level,dcode,ds)		DL_STRING_BODY(cond,level,dcode,ds)
#define DEBUG_NPORT(cond,level,nport)			DL_NPORT_BODY(cond,level,nport);
#define DEBUG_NETADDR(cond,level,netaddr)		DL_NETADDR_BODY(cond,level,netaddr);
#define DEBUG_KEY(cond,level,key)			DL_KEY_BODY(cond,level,key);
#define DEBUG_SBUF(cond,level,sbuf)			DL_SBUF_BODY(cond,level,sbuf);
#else	NET_DEBUG
#define	DEBUG0(cond,level,dcode)			/**/
#define	DEBUG1(cond,level,dcode,da1)			/**/
#define	DEBUG2(cond,level,dcode,da1,da2)		/**/
#define	DEBUG3(cond,level,dcode,da1,da2,da3)		/**/
#define	DEBUG4(cond,level,dcode,da1,da2,da3,da4)	/**/
#define	DEBUG5(cond,level,dcode,da1,da2,da3,da4,da5)	/**/
#define	DEBUG6(cond,level,dcode,da1,da2,da3,da4,da5,da6) /**/
#define	DEBUG_STRING(cond,level,dcode,ds)		/**/
#define DEBUG_NPORT(cond,level,nport)			/**/
#define DEBUG_NETADDR(cond,level,netaddr)		/**/
#define DEBUG_KEY(cond,level,key)			/**/
#define DEBUG_SBUF(cond,level,sbuf)			/**/
#endif	NET_DEBUG


/*
 * LOG macros are identical to the DEBUG ones,
 * but can be enabled/disabled independently.
 */
#if	NET_LOG
#define	LOG0(cond,level,dcode)				DL_BODY0(cond,level,dcode)
#define	LOG1(cond,level,dcode,da1)			DL_BODY1(cond,level,dcode,da1)
#define	LOG2(cond,level,dcode,da1,da2)			DL_BODY2(cond,level,dcode,da1,da2)
#define	LOG3(cond,level,dcode,da1,da2,da3)		DL_BODY3(cond,level,dcode,da1,da2,da3)
#define	LOG4(cond,level,dcode,da1,da2,da3,da4)		DL_BODY4(cond,level,dcode,da1,da2,da3,da4)
#define	LOG5(cond,level,dcode,da1,da2,da3,da4,da5)	DL_BODY5(cond,level,dcode,da1,da2,da3,da4,da5)
#define	LOG6(cond,level,dcode,da1,da2,da3,da4,da5,da6)	DL_BODY6(cond,level,dcode,da1,da2,da3,da4,da5,da6)
#define	LOG_STRING(cond,level,dcode,ds)			DL_STRING_BODY(cond,level,dcode,ds)
#define LOG_NPORT(cond,level,nport)			DL_NPORT_BODY(cond,level,nport);
#define LOG_NETADDR(cond,level,netaddr)			DL_NETADDR_BODY(cond,level,netaddr);
#define LOG_KEY(cond,level,key)				DL_KEY_BODY(cond,level,key);
#define LOG_SBUF(cond,level,sbuf)			DL_SBUF_BODY(cond,level,sbuf);
#else	NET_LOG
#define	LOG0(cond,level,dcode)				/**/
#define	LOG1(cond,level,dcode,da1)			/**/
#define	LOG2(cond,level,dcode,da1,da2)			/**/
#define	LOG3(cond,level,dcode,da1,da2,da3)		/**/
#define	LOG4(cond,level,dcode,da1,da2,da3,da4)		/**/
#define	LOG5(cond,level,dcode,da1,da2,da3,da4,da5)	/**/
#define	LOG6(cond,level,dcode,da1,da2,da3,da4,da5,da6)	/**/
#define	LOG_STRING(cond,level,dcode,ds)			/**/
#define LOG_NPORT(cond,level,nport)			/**/
#define LOG_NETADDR(cond,level,netaddr)			/**/
#define LOG_KEY(cond,level,key)				/**/
#define LOG_SBUF(cond,level,sbuf)			/**/
#endif	NET_LOG


/*
 * ERROR prints a message on stderr and puts it into the log.
 */
#define	ERROR(args) {			\
	char msg[200];			\
	mutex_lock(log_lock);		\
	(void)sprintf args;		\
	(void)fprintf(stderr,msg);	\
	(void)fprintf(stderr,"\n");	\
	(void)fflush(stderr);		\
	if (param.syslog)		\
		syslog(LOG_ERR,msg);	\
	mutex_unlock(log_lock);		\
	DEBUG_STRING(1,1,1,msg);	\
}

/*
 * This is a dummy here, needed for compatibility with the Camelot
 * code.
 */
#define	PRINT_ERROR			/**/


/*
 * LOGCHECK makes sure that the log does not overflow, and resets it if 
 * necessary.
 */
#define	LOGCHECK {				\
	if (log_cur_ptr >= log_end_ptr)		\
		(void)ls_resetlog(PORT_NULL);	\
}


#endif	_DEBUG_
