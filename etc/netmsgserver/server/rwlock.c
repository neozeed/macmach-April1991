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
 * $Log:	rwlock.c,v $
 * Revision 1.1  90/02/19  21:56:09  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.11  89/05/02  11:16:57  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	VAX_FAST_LOCK: use rfr's inline locking package.
 *	Available only on the Vax. See rwlock.h.
 *
 * 10-Jun-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Modified for X_LOCKS option (see rwlock.h).
 *
 * 20-May-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Added some register declarations.
 *
 * 27-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made the mutex and condition inline in the lock structure.
 *	Added lk_init and lk_clear.
 *	Go back to using cthreads malloc & free.
 *
 * 25-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed block_t to rw_block_t and perm_t to rw_perm_t.
 *
 *  6-Jan-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Changed to use mem_allocobj.
 *
 */
/*
 * rwlock.c
 *
 *
 */

#ifndef	lint
char rwlock_rcsid[] = "$ Header: rwlock.c,v 1.11 89/05/02 11:16:57 dpj Exp $";
#endif not lint

/*
 * Function for read/write lock module.
 */


#include	<stdio.h>
#include	<cthreads.h>

#include	"netmsg.h"
#include	"nm_extra.h"
#include	"rwlock.h"

extern char	*malloc();

#if	VAX_FAST_LOCK
int	cthread_critical= 0;
int	swtch_cnt 	= 0;
int *	swtch_cnt_ptr 	= &swtch_cnt;
#endif	VAX_FAST_LOCK

/*
 * lk_alloc()
 *	Allocate a lock and initialize it.
 */

lock_t
lk_alloc()
BEGIN("lk_alloc")
	register lock_t	lock;

	if ((lock = (lock_t)malloc(sizeof(struct lock))) == (lock_t)0) {
	    panic("lk_alloc.malloc");
	}
	mutex_init(&lock->lk_mutex);
	condition_init(&lock->lk_condition);
	lock->lk_users = 0;
	lock->lk_permission = PERM_READ;

	RETURN(lock);
END

#if	X_LOCKS
#else	X_LOCKS

/*
 * lk_init()
 *	Initialize a lock.
 */

void
lk_init(lock)
register lock_t	lock;
BEGIN("lk_init")

	mutex_init(&lock->lk_mutex);
	condition_init(&lock->lk_condition);
	lock->lk_users = 0;
	lock->lk_permission = PERM_READ;
	RET;

END

#endif	X_LOCKS

/*
 * lk_free(lock)
 *	Free the read/write lock.
 */

void
lk_free(lock)
register lock_t	lock;
BEGIN("lk_free")

	condition_clear(&lock->lk_condition);
	mutex_clear(&lock->lk_mutex);
	free((char *)lock);
	RET;
END

#if	X_LOCKS
#else	X_LOCKS

/*
 * lk_clear(lock)
 *	Clear the read/write lock.
 */

void
lk_clear(lock)
register lock_t	lock;
BEGIN("lk_clear")

	condition_clear(&lock->lk_condition);
	mutex_clear(&lock->lk_mutex);
	RET;
END


/*
 * lk_lock(lock, perm, block)
 *	Get a lock of type lock.  If block is
 *	BLOCK, wait until the lock is gotten; if it's NOBLOCK, return
 *	0 if we can't get it.
 */

int
lk_lock(lock, perm, block)
register lock_t		lock;
register rw_perm_t	perm;
register rw_block_t	block;
BEGIN("lk_lock")
	if (lock == NULL)
		RETURN(0);

	mutex_lock(&lock->lk_mutex);

	if (lock->lk_permission == PERM_READ) {
		if (perm == PERM_READWRITE) {
			if (lock->lk_users == 0) {
				lock->lk_permission = PERM_READWRITE;
				lock->lk_users = 1;
				mutex_unlock(&lock->lk_mutex);

				RETURN(1);
			} else if (block == BLOCK) {
				while (lock->lk_users != 0)
					condition_wait(&lock->lk_condition, &lock->lk_mutex);

				lock->lk_users = 1;
				lock->lk_permission = PERM_READWRITE;
				mutex_unlock(&lock->lk_mutex);

				RETURN(1);
			}
		} else {
			++lock->lk_users;
			mutex_unlock(&lock->lk_mutex);

			RETURN(1);
		}
	} else if (block == BLOCK) {
		if (perm == PERM_READ) {
			while (lock->lk_permission == PERM_READWRITE)
				condition_wait(&lock->lk_condition, &lock->lk_mutex);

			++lock->lk_users;
			lock->lk_permission = PERM_READ;
			mutex_unlock(&lock->lk_mutex);

			RETURN(1);
		} else {
			while (lock->lk_users != 0)
				condition_wait(&lock->lk_condition, &lock->lk_mutex);

			lock->lk_users = 1;
			lock->lk_permission = PERM_READWRITE;
			mutex_unlock(&lock->lk_mutex);

			RETURN(1);
		}
	}

	mutex_unlock(&lock->lk_mutex);

	RETURN(0);
END

/*
 * lk_unlock(lock)
 *	Release the read or read/write lock.  Signal to the other
 *	threads that this lock has been released.
 */

void
lk_unlock(lock)
register lock_t	lock;
BEGIN("lk_unlock")
	mutex_lock(&lock->lk_mutex);

	if (lock->lk_permission == PERM_READWRITE) {
		lock->lk_permission = PERM_READ;
		lock->lk_users = 0;
	} else
		--lock->lk_users;

	condition_signal(&lock->lk_condition);
	mutex_unlock(&lock->lk_mutex);

	RET;
END

#endif	X_LOCKS
