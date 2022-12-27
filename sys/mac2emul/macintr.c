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
 *	File: mac2emul/macintr.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/user.h>

#include <mach/exception.h>

#include <mac2/pcb.h>
#include <mac2/psl.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macinst.h>
#include <mac2emul/macuser.h>

/*
 * mac software interrupt handling
 */

static inline
int
ffsl(l)
unsigned long l;
{
    int bit;

    asm("bfffo %1{#0:#0},%0; bne 0f; moveq #-1,%0; 0:" :
	"=d" (bit) :
	"d" (l));

    return (bit);
}

/*
 * Check for a pending interrupt at
 * an ipl greater than that of the
 * processor.  Schedule an AST if
 * one is found.
 */
check_mac_interrupts(pcb)
register struct pcb *pcb;
{
    register struct mac_internal *mac = pcb->pcb_emul;
    register bit, s;

    if ((pcb->pcb_flags&MAC_EMULATION) == 0)
	return;

    s = splhigh();

    bit = ffsl(mac->softintr_pend);
    if (bit >= 0 &&
	mac->softintr[bit].ipl > ((struct status_reg *)&mac->sr)->sr_ipl)
	    pcb->pcb_ast |= TRACE_AST;

    splx(s);
}

/*
 * Check for a pending interrupt at
 * an ipl greater than that of the
 * processor.  Simulate the interrupt
 * if one is found.
 */
perform_mac_interrupts()
{
    register struct mac_internal *mac = current_thread_pcb()->pcb_emul;
    register bit, s;

    s = splhigh();

    bit = ffsl(mac->softintr_pend);
    if (bit >= 0 &&
	(bit == MAC_INTR_INTR ||
	 mac->softintr[bit].ipl > ((struct status_reg *)&mac->sr)->sr_ipl))
	    take_mac_interrupt(bit);

    splx(s);
}

/*
 * Simulate an interrupt.
 */
take_mac_interrupt(intr)
int	intr;
{
    register struct pcb *pcb = current_thread_pcb();
    register normal_exception_frame_t *frame = pcb->pcb_frame;
    register struct mac_internal *mac = pcb->pcb_emul;
    register struct softintr *si = &mac->softintr[intr];
    register unsigned long sp;
    register frame_size;
    register special;

    if (intr == MAC_INTR_INTR) {
	if (si->flags&MAC_IF_ASYNC)
	    mac->softintr_pend &= ~MAC_INTR_PEND(intr);

	thread_doexception(pcb->pcb_thread,
			   EXC_BREAKPOINT, EXC_MAC2_TRACE, 0);
	return (1);
    }

    /*
     * determine format and size of exception
     * frame.
     */
    switch (frame->f_fmt) {
      case STKFMT_NORMAL:
	special = 0;
	frame_size = sizeof (normal_exception_frame_t);
	break;

      case STKFMT_SPECIAL:
	special = 1;
	frame_size = sizeof (normal_exception_frame_t);
	break;

      default:
	return (0);
    }

    /*
     * set system byte of frame status reg
     * to software copy.
     */
    ((struct status_reg *)&frame->f_sr)->sr_sys =
	((struct status_reg *)&mac->sr)->sr_sys;

    /*
     * if tracing, clear trace mode for
     * exception handler.
     */
    mac->sr &= ~(SR_T1|SR_T0);

    /*
     * save exception frame on stack.
     */
    sp = pcb->pcb_user.r_sp - frame_size;
    if (special)
	frame->f_fmt = STKFMT_NORMAL;
    if (copyout(frame, sp, frame_size)) {
	if (special)
	    frame->f_fmt = STKFMT_SPECIAL;
	frame->f_sr &= ~SR_USERCLR;
	pcb->pcb_ast &= ~TRACE_USER;
	return (0);
    }
    pcb->pcb_user.r_sp = sp;

    if (special)
	frame->f_fmt = STKFMT_SPECIAL;
    frame->f_sr &= ~SR_USERCLR;
    pcb->pcb_ast &= ~TRACE_USER;

    /*
     * clear interrupt pending
     * flag.
     */
    if (si->flags&MAC_IF_ASYNC)
	mac->softintr_pend &= ~MAC_INTR_PEND(intr);

    /*
     * set the ipl in the software
     * status register to the ipl of
     * this interrrupt.
     */
    ((struct status_reg *)&mac->sr)->sr_ipl = si->ipl;

    /*
     * since we know that we have a normal or special
     * return frame, only need to set the
     * return pc.
     */
    if (si->vector >= &((unsigned long *)0)[256])
	FETCH(si->vector, &frame->f_pc, long);
	
    else
	FETCH(mac->ctrl[R_VBR]+si->vector, &frame->f_pc, long);

    return (1);
}
