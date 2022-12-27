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
 * $Log:	via.c,v $
 * Revision 2.2.1.1  90/09/07  00:58:19  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:34:50  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/via.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Macintosh II VIA driver.
 */

#include <macos_emul.h>

#include <sys/types.h>

#include <mach/mac2/frame.h>

#include <mac2/autoconf.h>

#include <mac2dev/via.h>

#include <machine/ffsl_inline.c>

extern void	clock_intr(), fdb_intr(), SInterrupt(), mscsiirq();
extern void	onesec_intr(), softint_intr();
#if	MACOS_EMUL
#define	MACTIMER1_INTR	mactimer1_intr
#define MACTIMER2_INTR	mactimer2_intr
#define MACSOUND_INTR	macsound_intr
extern void	MACTIMER1_INTR(), MACTIMER2_INTR(), MACSOUND_INTR();
#else
#define	MACTIMER1_INTR	0
#define MACTIMER2_INTR	0
#define MACSOUND_INTR	0
#endif

via_info_t	via[] = {
    {
	/* VIA 1 */
	{
	    /* v_intr_tbl */
	    onesec_intr,		/* CA 2 */
	    clock_intr,			/* CA 1 */
	    fdb_intr,			/* Shift Register */
	    0,				/* CB 2 */	
	    0,				/* CB 1 */
	    MACTIMER2_INTR,		/* Timer 2 */
	    MACTIMER1_INTR,		/* Timer 1 */
	}
    },
    {
	/* VIA 2 */
	{
	    /* v_intr_tbl */
	    0,				/* CA 2 */
	    SInterrupt,			/* CA 1 */
	    0,				/* Shift Register */
	    mscsiirq,			/* CB 2 */
	    MACSOUND_INTR,		/* CB 1 */
	    softint_intr,		/* Timer 2 */
	    0,				/* Timer 1 */
	}
    }
};

via_reg_t	*VIA1_ADDR_PTR = &via[0].v_reg_ptr;
via_reg_t	*VIA2_ADDR_PTR = &via[1].v_reg_ptr;

unsigned char	*VIA1_SOFT_PTR = &via[0].v_softw_ifr;
unsigned char	*VIA2_SOFT_PTR = &via[1].v_softw_ifr;

/*
 * via1init -- perform initialization.
 */
via1init(dp)
struct devconf *dp;
{
    register via_info_t *v = &via[0];
    register via_reg_t vp;

    v->v_reg_ptr = (via_reg_t)dp->dc_csr;

    vp = v->v_reg_ptr;
    vp->ier = VIA_IE_CLEAR_ALL;
    vp->pcr = 0;
    vp->acr = 0;
    fdb_init();		/* XXX */
    return (1);
}

via1intr(frame)
struct generic_exception_frame *frame;
{
    register long bit;
    register via_info_t *v = &via[0];
    register via_reg_t vp = v->v_reg_ptr;
    register void (*vector)();
    
    for (;;) {
	bit = ffsl((v->v_softw_ifr|vp->ifr) & vp->ier & 0x7f);
	if (bit < 0)
	  break;

	vector = v->v_intr_tbl[bit];
	if (vector)
	    (*vector)(frame);
	else {
	    printf("via1intr: %x interrupt not serviced\n", 1 << bit);
	    vp->ifr = (1 << bit);
	}
    }
}

/*
 * via2init -- perform initialization.
 */
via2init(dp)
struct devconf *dp;
{
    register via_info_t *v = &via[1];
    register via_reg_t vp;

    v->v_reg_ptr = (via_reg_t)dp->dc_csr;

    vp = v->v_reg_ptr;
    vp->ier = VIA_IE_CLEAR_ALL;
    vp->pcr = VIA_PC_CB2_POS_INTR | VIA_PC_CA2_POS_INTR;
    vp->acr = 0;
    vp->regb |= VIA_PORT_v2BusLk;
    vp->ddrb |= VIA_PORT_v2BusLk;
    vp->ier = VIA_IE_SET | VIA_IE_TIMER2 | VIA_IE_CA1;
    return (1);
}

via2intr(frame)
struct generic_exception_frame *frame;
{
    register long bit;
    register via_info_t *v = &via[1];
    register via_reg_t vp = v->v_reg_ptr;
    register void (*vector)();
    
    for (;;) {
	bit = ffsl((v->v_softw_ifr|vp->ifr) & vp->ier & 0x7f);
	if (bit < 0)
	  break;

	vector = v->v_intr_tbl[bit];
	if (vector)
	    (*vector)(frame);
	else {
	    printf("via2intr: %x interrupt not serviced\n", 1 << bit);
	    vp->ifr = (1 << bit);
	}
    }
}
