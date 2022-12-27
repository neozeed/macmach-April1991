/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	softint.c,v $
 * Revision 2.2  90/08/30  11:03:03  bohman
 * 	Created.
 * 	[90/08/29  11:45:59  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/softint.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <mac2/act.h>

#include <mac2dev/via.h>

#include <cputypes.h>

struct actlist	actsoft;

#define	SOFTACT_SOFT_LIST	0

unsigned long	softflags;
#define	    SF_RUN	0x01	    /* software interrupt impending */
#define	    SF_CUR	0x02	    /* software interrupt occurring */

struct act *
makesoftact(func, ipl)
register int (*func)(), ipl;
{
    register struct act *ap;

    ap = makeact(func, ipl, 1);
    if (ap)
	addact(SOFTACT_SOFT_LIST, ap, &actsoft);

    return (ap);
}

/*
 * Call activity ap with argument arg
 * at some later time as a software interrupt.
 */
softact(ap, arg)
register struct act	*ap;
{
    register s = splhigh();

    runact(SOFTACT_SOFT_LIST, ap, arg, 0);

    if ((softflags&SF_RUN) == 0) {
	softint_trigger();
	softflags |= SF_RUN;
    }
    splx(s);
}

/*
 * Called from IPL7 to process
 * software interrupts.
 */
softint()
{
    if (softflags&SF_CUR || !CHECKACTLIST(actsoft))
	return;

    softflags |= SF_CUR;
    doactlist(&actsoft);

    if (CHECKACTLIST(actsoft))
	softint_trigger();
    else
	softflags &= ~SF_RUN;

    softflags &= ~SF_CUR;
}

/*
 * cause an interrupt to occur soon (within 1.2766 us)
 */
softint_trigger()
{
    register via_reg_t vp = VIA2_ADDR;

    vp->t2cl = 1;   /* one pulse */
    vp->t2ch = 0;
}

/*
 * Acknowledge a softint interrupt.
 */
softint_intr()
{
    unsigned char x;

    x = VIA2_ADDR->t2cl;
}
