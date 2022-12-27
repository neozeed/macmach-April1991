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
 * $Log:	portrec.c,v $
 * Revision 1.1  90/02/19  21:55:40  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.28  89/05/02  11:16:21  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * Revision 1.27  89/04/24  20:42:03  dpj
 * 	Changes from NeXT as of 88/09/30
 * 
 * 	 13-Oct-87 Mary R. Thompson (mrt) at Carnegie Mellon
 * 	[89/04/19  17:57:09  dpj]
 * 	Forcefully zero newly-allocated port records.
 * 	[89/01/15  18:35:48  dpj]
 * 
 *	Changed queue_item_t to cthread_queue_item_t.
 *
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 14-Nov-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for new ipc_rec states and outgoing queue.
 *
 *  6-Oct-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Redone locking strategy for hash tables to work with the new
 *	RPC module.
 *	Modified for new port record format (RPCMOD).
 *
 * 19-Aug-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added stuff to do port statistics.
 *
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Changed the netport stuff from USE_VMTP to NETPORT, and
 *	redone the netport strategy.
 *	Fixed to use the lock_queue macros.
 *
 * 22-Jul-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Removed (void) cast from lk_lock calls.
 *
 * 12-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	USE_VMTP: added a call to netport_enter() when a port record is modified.
 *
 *  2-Jun-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Placed network port equality test inline in nport_test.
 *	Do [ln]port_lookup inline in pr_[ln]tran.
 *	Made the hashing macros better.
 *
 * 20-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some register declarations.
 *
 *  5-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made the hash indirection record inline in a port record.
 *	Made the lock inline in port record.
 *
 * 24-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Need a lock to synchronise access to the unique_id value.
 *	Allocate port_rec_queue statically.
 *	Replaced fprintf by ERROR and DEBUG/LOG macros.
 *
 * 15-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed for new definition of lq_cond_delete_from_queue.
 *	Call port_enable when a local port is allocated in pr_ntran.
 *
 *  2-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made pr_nport_equal check the SID as well as the PUID.
 *	Added pr_np_puid_lookup to look up a network port given only its PUID.
 *	Made pr_destroy deallocate the local port.
 *	Do not allocate a lock_queue for the portrec_block_queue
 *		(locking done at granularity of port record).
 *
 * 17-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed hash functions into macros.
 *	Made pq_hash_t records be allocated as MEM_PORTITEMs -
 *		this assumes that they are the same size as a port_item_t.
 *	pr_destroy dellocates the port records token list.
 *	Set the aliveness field of a new port record to PORT_ACTIVE.
 *
 *  4-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Many minor changes.  Removed np_assign; use mem_alloc[obj].
 *
 *  1-Nov-86  Healfdene Goguen (hhg) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * portrec.c
 *
 *
 */

#ifndef	lint
char portrec_rcsid[] = "$ Header: portrec.c,v 1.28 89/05/02 11:16:21 dpj Exp $";
#endif not lint

/*
 * Network port and port record operations.
 */


#include	<cthreads.h>
#include	<mach.h>
#include	<stdio.h>
#include	<mach/boolean.h>
#include	<sys/time.h>

#include	"debug.h"
#include	"lock_queue.h"
#include	"ls_defs.h"
#include 	"mem.h"
#include	"netmsg.h"
#include	"network.h"
#include	"nm_extra.h"
#include	"port_defs.h"
#include	"portrec.h"
#include	"uid.h"
#include	"ipc.h"
#include	"po_defs.h"	/* MEM_POITEM */


#define	PORT_HASH_SIZE	32	/* Must be a multiple of 2. */
static struct lock_queue	port_rec_queue[PORT_HASH_SIZE];

network_port_t		null_network_port;

PRIVATE long		unique_id;
PRIVATE struct mutex	unique_id_lock;

/*
 * Memory management definitions.
 */
PUBLIC mem_objrec_t	MEM_PORTREC;
PUBLIC mem_objrec_t	MEM_PORTITEM;



/*
 * PR_LPORTHASHFORIDX(lport)
 *	Find the bucket for this local port according to some hash algorithm.
 */
#define PR_LPORTHASHFORIDX(lport) \
	(((((lport) >> 8) & 0xff) ^ ((lport) & 0xff)) & (PORT_HASH_SIZE - 1))


/*
 * PR_NPORTHASHFORIDX(nport_ptr)
 * PR_NPORTHASHFORPUIDX(puid)
 *	Find the bucket for this network port according to some hash algorithm.
 *	We hash on the top and the bottom byte of the low word of the PUID
 *	because these are the bytes most likely to change because they are
 *	generated from the unique id value.
 */
#define PR_NPORTHASHFORIDX(nport_ptr) 								\
	(((((nport_ptr)->np_puid.np_uid_low >> 24) & 0xff)					\
			^ ((nport_ptr)->np_puid.np_uid_low & 0xff)) & (PORT_HASH_SIZE - 1))

#define PR_NPORTHASHFORPUIDX(puid)								\
	(((((puid).np_uid_low >> 24) & 0xff) ^ ((puid).np_uid_low & 0xff)) & (PORT_HASH_SIZE - 1))


/*
 * pr_eqfn
 *	See if the two port records of two port hash records are the same (equal addresses).
 *
 * Parameters:
 *	pqh_1, pqh_2	: pointers to the two hash records.
 */
PRIVATE int pr_eqfn(pqh_1, pqh_2)
register cthread_queue_item_t	pqh_1;
register int		pqh_2;
BEGIN("pr_eqfn")

	RETURN(((pq_hash_ptr_t)pqh_1)->pqh_portrec == ((pq_hash_ptr_t)pqh_2)->pqh_portrec);

END


/*
 * lport_test
 *	Test if lport is the same as the local port
 *	in the port record pointed to by this hash record.
 *
 * Parameters:
 *	pqh	: pointer to a hash record for the port record
 *	lport	: the local port
 */
PRIVATE int lport_test(pqh, lport)
register cthread_queue_item_t		pqh;
register int			lport;
BEGIN("lport_test")
	RETURN(((pq_hash_ptr_t)pqh)->pqh_portrec->portrec_local_port == (port_t)lport);
END


/*
 * nport_test
 *	Test to see if this network port is the same as the network port
 *	in the port record pointed to by this hash record.
 *
 * Parameters:
 *	pqh		: pointer to the port hash record
 *	nport_ptr	: pointer to the network port
 */
PRIVATE int nport_test(pqh, nport_ptr)
register cthread_queue_item_t		pqh;
register int			nport_ptr;
BEGIN("nport_test")
	register network_port_ptr_t	other_nport_ptr;

	other_nport_ptr = &((pq_hash_ptr_t)pqh)->pqh_portrec->portrec_network_port;
	RETURN(
		(((network_port_ptr_t)nport_ptr)->np_puid.np_uid_high
						== other_nport_ptr->np_puid.np_uid_high)
		&&
		(((network_port_ptr_t)nport_ptr)->np_puid.np_uid_low == other_nport_ptr->np_puid.np_uid_low)
		&&
		(((network_port_ptr_t)nport_ptr)->np_sid.np_uid_low == other_nport_ptr->np_sid.np_uid_low)
		&&
		(((network_port_ptr_t)nport_ptr)->np_sid.np_uid_low == other_nport_ptr->np_sid.np_uid_low)
	);
END

/*
 * np_puid_test
 *	Test to see if this PUID is equal to the PUID in the port record.
 *
 * Parameters:
 *	pqh		: pointer to the port hash record
 *	puid_ptr	: pointer to the PUID
 *
 */
PRIVATE np_puid_test(pqh, puid_ptr)
register cthread_queue_item_t		pqh;
register int			puid_ptr;
BEGIN("np_puid_test")
	RETURN((((pq_hash_ptr_t)pqh)->pqh_portrec->portrec_network_port.np_puid.np_uid_high
						== ((np_uid_t *)puid_ptr)->np_uid_high)
		&& (((pq_hash_ptr_t)pqh)->pqh_portrec->portrec_network_port.np_puid.np_uid_low
						== ((np_uid_t *)puid_ptr)->np_uid_low));
END



/*
 * pr_init()
 *	Initialize variables:
 *		the unique identifier used to construct network port PUIDs
 *		the hash queues
 */
EXPORT boolean_t
pr_init()
BEGIN("pr_init")
	register int	i;
	struct timeval	now;
	struct timezone	zone;

	/*
	 * Initialize the memory management facilities.
	 */
	mem_initobj(&MEM_PORTREC,"Port record",sizeof(port_rec_t),
								FALSE,100,10);
	mem_initobj(&MEM_PORTITEM,"Port item",sizeof(port_item_t),
								FALSE,500,50);

	(void)gettimeofday(&now, &zone);
	unique_id = now.tv_sec;

	mutex_init(&unique_id_lock);

	for (i = 0; i < PORT_HASH_SIZE; ++i)
		lq_init(&port_rec_queue[i]);

	RETURN(TRUE);
END



/*
 * pr_create
 *	Create a new port record.
 *	Put it on the hash tables and allocate lock and condition variables.
 *
 * Parameters:
 *	lport		: the local port
 *	nport_ptr	: pointer to the network port
 *
 * Returns:
 *	Pointer to the new record locked.
 *
 */
PRIVATE port_rec_ptr_t
pr_create(lport, nport_ptr)
port_t			lport;
network_port_ptr_t	nport_ptr;
BEGIN("pr_create")
	register port_rec_ptr_t	newport_rec;
	register int		networkidx, localidx;

	MEM_ALLOCOBJ(newport_rec,port_rec_ptr_t,MEM_PORTREC);
	bzero(newport_rec,sizeof(port_rec_t));
	newport_rec->portrec_local_port = lport;
	newport_rec->portrec_network_port = *nport_ptr;

	localidx = PR_LPORTHASHFORIDX(lport);
	networkidx = PR_NPORTHASHFORIDX(nport_ptr);

	lk_init(&newport_rec->portrec_lock);

#if	RPCMOD
	newport_rec->portrec_refcount = 1;

	newport_rec->portrec_retry_level = 0;
	newport_rec->portrec_waiting_count = 0;
	newport_rec->portrec_transit_count = 0;
	sys_queue_init(&newport_rec->portrec_out_ipcrec);
	newport_rec->portrec_lazy_ipcrec = (pointer_t)0;
	newport_rec->portrec_reply_ipcrec = (pointer_t)0;
#endif	RPCMOD
	newport_rec->portrec_block_queue = (pointer_t)0;

	newport_rec->portrec_networkitem.pqh_portrec = newport_rec;
	newport_rec->portrec_localitem.pqh_portrec = newport_rec;

	newport_rec->portrec_aliveness = PORT_ACTIVE;

	if (networkidx > localidx) {
		mutex_lock(&port_rec_queue[networkidx].lock);
		mutex_lock(&port_rec_queue[localidx].lock);
	} else {
		mutex_lock(&port_rec_queue[localidx].lock);
		if (networkidx != localidx) {
			mutex_lock(&port_rec_queue[networkidx].lock);
		}
	}

	lqn_prequeue(&port_rec_queue[networkidx], (cthread_queue_item_t) &newport_rec->portrec_networkitem);
	lqn_prequeue(&port_rec_queue[localidx], (cthread_queue_item_t) &newport_rec->portrec_localitem);

	lk_lock(&newport_rec->portrec_lock, PERM_READWRITE, BLOCK);

	mutex_unlock(&port_rec_queue[networkidx].lock);
	if (networkidx != localidx) {
		mutex_unlock(&port_rec_queue[localidx].lock);
	}

#if	PORTSTAT
	/*
	 * Allocate a port statistics record for this port.
	 */
	mutex_lock(&port_stat_lock);
	if (port_stat_cur == port_stat_end) newport_rec->portrec_stat = port_stat_end;
	else newport_rec->portrec_stat = port_stat_cur++;
	mutex_unlock(&port_stat_lock);
#endif	PORTSTAT

	RETURN(newport_rec);
END



/*
 * pr_destroy
 *	Destroy a port record.
 *
 * Parameters:
 *	port_rec_ptr	: pointer to the port record to be destroyed
 *
 * Assumes:
 *	PERM_READWRITE has already been acquired.
 *
 */
EXPORT void
pr_destroy(port_rec_ptr)
register port_rec_ptr_t	port_rec_ptr;
BEGIN("pr_destroy")
	register int	networkidx, localidx;
	pointer_t	temp_ptr, next_ptr;
	cthread_queue_item_t	network_ret, local_ret;

	if (port_rec_ptr == PORT_REC_NULL)
		RET;

#if	PORTSTAT
	port_rec_ptr->portrec_stat->alive = FALSE;
#endif	PORTSTAT

	if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
		/*
		 * Some other thread is in the process of destroying
		 * this port record.
		 */
		lk_unlock(&port_rec_ptr->portrec_lock);
		RET;
	}
	port_rec_ptr->portrec_info |= PORT_INFO_NOLOOKUP;

	localidx = PR_LPORTHASHFORIDX(port_rec_ptr->portrec_local_port);
	networkidx = PR_NPORTHASHFORIDX(&(port_rec_ptr->portrec_network_port));

	(void)port_deallocate(task_self(), port_rec_ptr->portrec_local_port);
	port_rec_ptr->portrec_local_port = PORT_NULL;

	/*
	 * Deallocate the portrec_po_host_list.
	 */
	temp_ptr = port_rec_ptr->portrec_po_host_list;
	while (temp_ptr != (pointer_t)0) {
		next_ptr = *(pointer_t *)temp_ptr;
		MEM_DEALLOCOBJ(temp_ptr, MEM_POITEM);
		temp_ptr = next_ptr;
	}
	port_rec_ptr->portrec_po_host_list = NULL;

	/*
	 * At this point, we are still holding one reference in the port record.
	 * The NOLOOKUP flag guarantees that nobody else will be able to
	 * lookup the port record and use that same reference. All other threads
	 * that still use this port record have their own reference for it.
	 */

	lk_unlock(&port_rec_ptr->portrec_lock);

	if (networkidx > localidx) {
		mutex_lock(&port_rec_queue[networkidx].lock);
		mutex_lock(&port_rec_queue[localidx].lock);
	} else {
		mutex_lock(&port_rec_queue[localidx].lock);
		if (networkidx != localidx) {
			mutex_lock(&port_rec_queue[networkidx].lock);
		}
	}

	lk_lock(&port_rec_ptr->portrec_lock,PERM_READWRITE,TRUE);

	lqn_cond_delete_macro(&port_rec_queue[localidx], pr_eqfn,
				(int)&port_rec_ptr->portrec_localitem,local_ret);
	lqn_cond_delete_macro(&port_rec_queue[networkidx], pr_eqfn,
				(int)&port_rec_ptr->portrec_networkitem,network_ret);

	mutex_unlock(&port_rec_queue[networkidx].lock);
	if (networkidx != localidx) {
		mutex_unlock(&port_rec_queue[localidx].lock);
	}

	if (!local_ret) {
		LOG0(TRUE, 5, 1060);
	}
	if (!network_ret) {
		LOG0(TRUE, 5, 1061);
	}

#if	RPCMOD
	pr_release(port_rec_ptr);
#else	RPCMOD
	lk_clear(&port_rec_ptr->portrec_lock);
	MEM_DEALLOCOBJ(port_rec_ptr, MEM_PORTREC);
#endif	RPCMOD

	RET;

END



/*
 * pr_np_puid_lookup
 *	Look up a network port given its PUID.
 *
 * Parameters:
 *	np_puid		: the PUID for the network port.
 *
 * Returns:
 *	PORT_REC_NULL if no such port record exists.
 *	otherwise pointer to the appropriate port record.
 *	Locks the port record.
 *
 */
EXPORT port_rec_ptr_t pr_np_puid_lookup(np_puid)
np_uid_t	np_puid;
BEGIN("pr_np_puid_lookup")
	register int		idx;
	register port_rec_ptr_t	port_rec_ptr;
	register cthread_queue_item_t	p;

	idx = PR_NPORTHASHFORPUIDX(np_puid);

	mutex_lock(&port_rec_queue[idx].lock);

	lqn_find_macro(&port_rec_queue[idx], np_puid_test, (int)&np_puid, p);

	if (p == (cthread_queue_item_t)NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		RETURN(PORT_REC_NULL);
	}

	if ((port_rec_ptr = ((pq_hash_ptr_t)p)->pqh_portrec) == PORT_REC_NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		panic("pr_np_puid_lookup");
	}

	lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, BLOCK);

	if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
		lk_unlock(&port_rec_ptr->portrec_lock);
		port_rec_ptr = NULL;
	}

	mutex_unlock(&port_rec_queue[idx].lock);

	RETURN(port_rec_ptr);
END



/*
 * pr_nportlookup
 *	Look up a network port.
 *
 * Parameters:
 *	nport_ptr	: pointer to the network port
 *
 * Returns:
 *	PORT_REC_NULL if no such port record exists.
 *	otherwise pointer to the appropriate port record.
 *	Locks the port record.
 *
 */
EXPORT port_rec_ptr_t pr_nportlookup(nport_ptr)
register network_port_ptr_t	nport_ptr;
BEGIN("pr_nportlookup")
	register int		idx;
	register port_rec_ptr_t	port_rec_ptr;
	register cthread_queue_item_t	p;

	idx = PR_NPORTHASHFORIDX(nport_ptr);

	mutex_lock(&port_rec_queue[idx].lock);

	lqn_find_macro(&port_rec_queue[idx], nport_test, (int)nport_ptr, p);

	if (p == (cthread_queue_item_t)NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		RETURN(PORT_REC_NULL);
	}

	if ((port_rec_ptr = ((pq_hash_ptr_t)p)->pqh_portrec) == PORT_REC_NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		panic("pr_nportlookup");
	}

	lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, BLOCK);

	if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
		lk_unlock(&port_rec_ptr->portrec_lock);
		port_rec_ptr = NULL;
	}

	mutex_unlock(&port_rec_queue[idx].lock);

	RETURN(port_rec_ptr);
END



/*
 * pr_ntran
 *	Return the port record associated with a network port.
 *	If no such record exists, create a new one.
 *
 * Parameters:
 *	nport_ptr	: pointer to the network port
 *
 * Returns:
 *	Pointer to the (new) record locked (by pr_create or pr_nportlookup).
 *
 */
EXPORT port_rec_ptr_t pr_ntran(nport_ptr)
register network_port_ptr_t	nport_ptr;
BEGIN("pr_ntran")
	port_t			lport;
	kern_return_t		kr;
	register int		idx;
	register port_rec_ptr_t	port_rec_ptr;
	register cthread_queue_item_t	p;

	if (nport_ptr == NULL) RETURN(PORT_REC_NULL);
	if ((nport_ptr->np_puid.np_uid_high == null_network_port.np_puid.np_uid_high)
		&& (nport_ptr->np_puid.np_uid_low == null_network_port.np_puid.np_uid_low)
		&& (nport_ptr->np_sid.np_uid_low == null_network_port.np_sid.np_uid_low)
		&& (nport_ptr->np_sid.np_uid_low == null_network_port.np_sid.np_uid_low))
	{
		RETURN(PORT_REC_NULL);
	}

	idx = PR_NPORTHASHFORIDX(nport_ptr);

	mutex_lock(&port_rec_queue[idx].lock);

	lqn_find_macro(&port_rec_queue[idx], nport_test, (int)nport_ptr, p);
	if (p == (cthread_queue_item_t)NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);

		/*
		 * Allocate a new local port and create a port record.
		 */
		if ((kr = port_allocate(task_self(), &lport)) != KERN_SUCCESS) {
			ERROR((msg, "pr_ntran.port_allocate fails, kr = %d.", kr));
			RETURN(PORT_REC_NULL);
		}
		(void)port_enable(task_self(), lport);

		port_rec_ptr = pr_create(lport, nport_ptr);

#if	PORTSTAT
		port_rec_ptr->portrec_stat->port_id = (u_int)lport;
		port_rec_ptr->portrec_stat->nport_id_high = (u_int)nport_ptr->np_puid.np_uid_high;
		port_rec_ptr->portrec_stat->nport_id_low = (u_int)nport_ptr->np_puid.np_uid_low;
		port_rec_ptr->portrec_stat->nport_receiver = (u_int)nport_ptr->np_receiver;
		port_rec_ptr->portrec_stat->nport_owner = (u_int)nport_ptr->np_owner;
		port_rec_ptr->portrec_stat->alive = TRUE;
#endif	PORTSTAT

		port_rec_ptr->portrec_info = 0;
#if	RPCMOD
#else	RPCMOD
		port_rec_ptr->portrec_current_ipcrec = NULL;
		port_rec_ptr->portrec_backlog_ipcrec = NULL;
#endif	RPCMOD
	}
	else if ((port_rec_ptr = ((pq_hash_ptr_t)p)->pqh_portrec) == PORT_REC_NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		panic("pr_nportlookup");
	}
	else {
		lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, BLOCK);
		mutex_unlock(&port_rec_queue[idx].lock);
		if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
			lk_unlock(&port_rec_ptr->portrec_lock);
			panic("pr_ntran trying to resuscitate a port");
		}
	}

	RETURN(port_rec_ptr);
END



/*
 * pr_lportlookup
 *	Look up a local port.
 *
 * Parameters:
 *	lport	: the local port
 *
 * Returns:
 *	PORT_REC_NULL if no such port record exists.
 *	otherwise pointer to the appropriate port record.
 *	Locks the port record.
 *
 */
EXPORT port_rec_ptr_t pr_lportlookup(lport)
register port_t	lport;
BEGIN("pr_lportlookup")
	register int		idx;
	register port_rec_ptr_t	port_rec_ptr;
	register cthread_queue_item_t	p;

	idx = PR_LPORTHASHFORIDX(lport);

	mutex_lock(&port_rec_queue[idx].lock);

	lqn_find_macro(&port_rec_queue[idx], lport_test, (int) lport, p);
	if (p == (cthread_queue_item_t)NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		RETURN(PORT_REC_NULL);
	}

	if ((port_rec_ptr = ((pq_hash_ptr_t)p)->pqh_portrec) == NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		panic("pr_lportlookup");
	}

	lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, BLOCK);

	if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
		lk_unlock(&port_rec_ptr->portrec_lock);
		port_rec_ptr = NULL;
	}

	mutex_unlock(&port_rec_queue[idx].lock);

	RETURN(port_rec_ptr);
END



/*
 * pr_ltran
 *	Return the port record associated with a local port.
 *	If no such port record exists, create one.
 *
 * Parameters:
 *	lport	: the local port
 *
 * Returns:
 *	Pointer to the (new) port record locked by (pr_create or pr_lportlookup).
 */
EXPORT port_rec_ptr_t pr_ltran(lport)
register port_t	lport;
BEGIN("pr_ltran")
	network_port_t		new_nport;
	register int		idx;
	register port_rec_ptr_t	port_rec_ptr;
	register cthread_queue_item_t	p;

	if (lport == PORT_NULL) RETURN(PORT_REC_NULL);

	idx = PR_LPORTHASHFORIDX(lport);
	mutex_lock(&port_rec_queue[idx].lock);
	lqn_find_macro(&port_rec_queue[idx], lport_test, (int)lport, p);

	if (p == (cthread_queue_item_t)NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		/*
		 * Create a new network port and port record.
		 */
		new_nport.np_owner = my_host_id;
		new_nport.np_receiver = my_host_id;
		new_nport.np_puid.np_uid_high = my_host_id;
		mutex_lock(&unique_id_lock);
		new_nport.np_puid.np_uid_low = ++unique_id;
		mutex_unlock(&unique_id_lock);
		new_nport.np_sid.np_uid_high = uid_get_new_uid();
		new_nport.np_sid.np_uid_low = uid_get_new_uid();

		port_rec_ptr = pr_create(lport, (network_port_ptr_t)&new_nport);

#if	NETPORT
		ipc_netport_enter(port_rec_ptr->portrec_network_port,
			port_rec_ptr->portrec_local_port,
			(port_rec_ptr->portrec_network_port.np_receiver == my_host_id));
#endif	NETPORT

#if	PORTSTAT
		port_rec_ptr->portrec_stat->port_id = (u_int)lport;
		port_rec_ptr->portrec_stat->nport_id_high = (u_int)new_nport.np_puid.np_uid_high;
		port_rec_ptr->portrec_stat->nport_id_low = (u_int)new_nport.np_puid.np_uid_low;
		port_rec_ptr->portrec_stat->nport_receiver = (u_int)my_host_id;
		port_rec_ptr->portrec_stat->nport_owner = (u_int)my_host_id;
		port_rec_ptr->portrec_stat->alive = TRUE;
#endif	PORTSTAT

		port_rec_ptr->portrec_info = 0;
#if	RPCMOD
#else	RPCMOD
		port_rec_ptr->portrec_current_ipcrec = NULL;
		port_rec_ptr->portrec_backlog_ipcrec = NULL;
#endif	RPCMOD
	}
	else if ((port_rec_ptr = ((pq_hash_ptr_t)p)->pqh_portrec) == PORT_REC_NULL) {
		mutex_unlock(&port_rec_queue[idx].lock);
		panic("pr_lportlookup");
	}
	else {
		lk_lock(&port_rec_ptr->portrec_lock, PERM_READWRITE, BLOCK);
		mutex_unlock(&port_rec_queue[idx].lock);
		if (port_rec_ptr->portrec_info & PORT_INFO_NOLOOKUP) {
			lk_unlock(&port_rec_ptr->portrec_lock);
			panic("pr_ltran trying to resuscitate a port");
		}
	}

	RETURN(port_rec_ptr);
END




/*
 * pr_nport_equal
 *	Check for equality of two network ports.
 *
 * Parameters:
 *	nport_ptr1, nport_ptr2	: pointers to the two network ports
 *
 * Returns:
 *	TRUE if the SIDs and PUIDs of the network ports are equal.
 *	FALSE otherwise.
 *
 */
EXPORT boolean_t pr_nport_equal(nport_ptr1, nport_ptr2)
register network_port_ptr_t	nport_ptr1, nport_ptr2;
BEGIN("pr_nport_equal")
	RETURN
	(
		(nport_ptr1->np_puid.np_uid_high == nport_ptr2->np_puid.np_uid_high)
		&&
		(nport_ptr1->np_puid.np_uid_low == nport_ptr2->np_puid.np_uid_low)
		&&
		(nport_ptr1->np_sid.np_uid_low == nport_ptr2->np_sid.np_uid_low)
		&&
		(nport_ptr1->np_sid.np_uid_low == nport_ptr2->np_sid.np_uid_low)
	);
END



/*
 * pr_nporttostring
 *	Print contents of the network port into a string.
 *
 * Parameters:
 *	nport_str	: char * pointer
 *	nport_ptr	: pointer to the network port
 */
EXPORT void pr_nporttostring(nport_str, nport_ptr)
char			*nport_str;
network_port_ptr_t	nport_ptr;
BEGIN("pr_nporttostring")
	char		receiver_string[30];
	char		owner_string[30];

	ipaddr_to_string(receiver_string, nport_ptr->np_receiver);
	ipaddr_to_string(owner_string, nport_ptr->np_owner);
	(void)sprintf(nport_str, "%d.%d-%d.%d-%s-%s",
			nport_ptr->np_puid.np_uid_high, nport_ptr->np_puid.np_uid_low,
			nport_ptr->np_sid.np_uid_high, nport_ptr->np_sid.np_uid_low,
			receiver_string, owner_string);
	RET;

END



/*
 * pr_list_subfn
 *	Do gathering for pr_list.
 *
 * Parameters:
 *	p	: pointer to a hash entry
 *	lq	: lock queue that is being used to form a port list
 *
 * Note:
 *	The value returned is ignored.
 */

PRIVATE int pr_list_subfn(p, lq)
register cthread_queue_item_t	p;
register int		lq;
BEGIN("pr_list_subfn")
	register port_item_ptr_t	portitem;

	/*
	 * Only add this port to the list if the hash entry is the network hash entry.
	 * This stops us from adding the port twice.
	 */
	if ((&((pq_hash_ptr_t)p)->pqh_portrec->portrec_networkitem) != (pq_hash_ptr_t)p)
		RETURN(1);

	MEM_ALLOCOBJ(portitem,port_item_ptr_t,MEM_PORTITEM);
	portitem->pi_port = ((pq_hash_ptr_t)p)->pqh_portrec->portrec_local_port;

	lq_prequeue((lock_queue_t)lq, (cthread_queue_item_t)portitem);
	RETURN(1);
END


/*
 * pr_list()
 *	Return a list of all local ports that are elements of port records.
 */
EXPORT lock_queue_t pr_list()
BEGIN("pr_list")
	register lock_queue_t	portlist;
	register int		index;

	portlist = lq_alloc();

	for (index = 0; index < PORT_HASH_SIZE; index++) {
		/*
		 * Avoid call lq_map_queue if there is nothing there.
		 */
		if (port_rec_queue[index].head != (cthread_queue_item_t)0) {
			lq_map_macro(&port_rec_queue[index], pr_list_subfn, (int)portlist);
		}
	}

	RETURN(portlist);
END
