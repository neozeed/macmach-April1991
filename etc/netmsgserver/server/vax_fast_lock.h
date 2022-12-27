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
 * $Log:	vax_fast_lock.h,v $
 * Revision 1.1  90/02/19  21:57:57  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.7  89/05/02  11:19:14  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 27-Jul-87  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Created history, code courtesy of rfr.
 */
/*
 * vax_fast_lock.h
 *
 *
 * $ Header: vax_fast_lock.h,v 1.7 89/05/02 11:19:14 dpj Exp $
 *
 */

/*
 * Fast inline locking routines for Vax.
 */


#ifndef _VAX_FAST_LOCK_
#define _VAX_FAST_LOCK_ 1

#ifndef	vax
	Sorry, this only works on a Vax.
#endif	vax

#undef spin_lock
#define spin_lock(p_crit) {					\
	extern	int	cthread_critical;			\
	register int not_done = 1;				\
	while (not_done) {					\
		asm("7:	bbssi	$0,_cthread_critical,7b");	\
		if (*(int *)(p_crit) == 0) {			\
			*(int *)(p_crit) = 1; not_done = 0; 	\
		asm("8:	bbcci	$0,_cthread_critical,8b");	\
		} else {					\
		asm("9:	bbcci	$0,_cthread_critical,9b"); }	\
	}							\
}

#undef spin_unlock
#define	spin_unlock(p_crit) {				\
	extern	int	cthread_critical;		\
	asm("7:	bbssi	$0,_cthread_critical,7b");	\
	*(int *)(p_crit) = 0;				\
	asm("8:	bbcci	$0,_cthread_critical,8b");	\
}

#undef mutex_lock
#define mutex_lock(m_crit) {					\
	extern	int	cthread_critical;			\
	extern 	int *	swtch_cnt_ptr; 				\
	register int 	not_done = 1;				\
	while (not_done) {					\
		asm("7:	bbssi	$0,_cthread_critical,7b");	\
		if (*(int *)(m_crit) == 0) {			\
			*(int *)(m_crit) = 1; not_done = 0; 	\
		asm("8:	bbcci	$0,_cthread_critical,8b");	\
		} else {					\
		asm("9:	bbcci	$0,_cthread_critical,9b"); }	\
		if (not_done) { (*swtch_cnt_ptr)++; swtch_pri(127); }	\
	}							\
}

#undef mutex_unlock
#define	mutex_unlock(m_crit) {				\
	extern	int	cthread_critical;		\
	asm("7:	bbssi	$0,_cthread_critical,7b");	\
	*(int *)(m_crit) = 0;				\
	asm("8:	bbcci	$0,_cthread_critical,8b");	\
}


#endif _VAX_FAST_LOCK_
