/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2emul/macinput.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/types.h>
#include <sys/tty.h>

#include <kern/thread.h>

#include <mac2/pcb.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macserver_types.h>

/*
 * Keyboard and mouse handling.
 */

extern	struct tty	kbtty[];
extern	unsigned char	*nextc();

kern_return_t
mac_get_input(thread, event)
thread_t		thread;
mac_input_event_t	*event;
{
    register struct tty		*tp = &kbtty[0];	/* XXX */
    register struct clist	*tq = &tp->t_rawq;
    register unsigned char	*cp;
    register kern_return_t	result = KERN_SUCCESS;
    register			s = spltty();

    if ((tp->t_state&TS_ISOPEN) == 0 || (tp->t_flags&RAW) == 0)
	result = KERN_FAILURE;
    else if (tq->c_cc <= 0)
	result = KERN_FAILURE;
    else {
	cp = tq->c_cf;
	if (*cp == 0x7e) {		/* XXX */
	    if (tq->c_cc < 3)
		result = KERN_FAILURE;
	    else {
		event->e_code = *cp;	cp = nextc(tq, cp);
		event->m_X = *cp;	cp = nextc(tq, cp);
		event->m_Y = *cp;
		ndflush(tq, 3);
	    }
	}
	else
	    event->e_code = getc(tq);

	if (tq->c_cc > 0) {
	    register struct mac_internal *mac = thread->pcb->pcb_emul;

	    splhigh();
	    if (mac->softintr[MAC_INTR_INP].flags&MAC_IF_ENB)
		mac->softintr_pend |= MAC_INTR_PEND(MAC_INTR_INP);
	}
    }

    splx(s);

    return (result);
}
