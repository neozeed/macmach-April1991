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
 *	File: mac2emul/macexception.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <mac2/pcb.h>
#include <mac2/psl.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#include <mac2emul/macinst.h>
#include <mac2emul/macuser.h>

/*
 * mac exception handling
 */

/*
 * take_exception causes the exception
 * specified in the exception frame to
 * be simulated for the current thread.
 */
take_exception(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    register struct pcb *pcb = current_thread_pcb();
    register struct mac_internal *mac = pcb->pcb_emul;
    register unsigned long sp;
    register frame_size;

    /*
     * determine format and size of exception
     * frame.
     */
    switch (frame->f_fmt) {
      case STKFMT_NORMAL:
	frame_size = sizeof (normal_exception_frame_t);
	break;

      case STKFMT_SPECIAL:
	frame_size = sizeof (special_exception_frame_t);
	break;

      case STKFMT_SHORT_BUSERR:
	frame_size = sizeof (short_buserr_exception_frame_t);
	break;

      case STKFMT_LONG_BUSERR:
	frame_size = sizeof (long_buserr_exception_frame_t);
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
    sp = regs->r_sp - frame_size;
    if (copyout(frame, sp, frame_size)) {
	frame->f_sr &= ~SR_USERCLR;
	pcb->pcb_ast &= ~TRACE_USER;
	return (0);
    }
    regs->r_sp = sp;

    frame->f_sr &= ~SR_USERCLR;
    pcb->pcb_ast &= ~TRACE_USER;

    /*
     * setup return frame
     */
    if (frame->f_fmt != STKFMT_NORMAL && frame->f_fmt != STKFMT_SPECIAL) {
	register normal_exception_frame_t *return_frame;

	return_frame = &pcb->pcb_return_frame_data.f_normal;
	pcb->pcb_return_frame_size = sizeof (normal_exception_frame_t);
	return_frame->f_sr = frame->f_sr;
	FETCH(mac->ctrl[R_VBR]+frame->f_vector, &return_frame->f_pc, long);
	return_frame->f_fmt = STKFMT_NORMAL;
	return_frame->f_vector = 0;
	pcb->pcb_frame = return_frame;
	pcb->pcb_flags |= RET_FRAME;
    }
    else
	FETCH(mac->ctrl[R_VBR]+frame->f_vector, &frame->f_pc, long);

    return (1);
}

return_from_exception(frame, regs)
register normal_exception_frame_t *frame;
register struct regs *regs;
{
    register normal_exception_frame_t *return_frame;
    register struct pcb *pcb = current_thread_pcb();
    struct mac_internal *mac = pcb->pcb_emul;
    register unsigned long sp = regs->r_sp;
    register frame_size;

    /*
     * fetch normal part of stack
     * frame.
     */
    return_frame = &pcb->pcb_return_frame_data.f_normal;
    if (copyin(sp, return_frame, sizeof (normal_exception_frame_t)))
	return (0);

    /*
     * determine frame fmt and
     * size of extension.
     */
    switch (return_frame->f_fmt) {
      case STKFMT_NORMAL:
	frame_size = 0;
	break;

      case STKFMT_SPECIAL:
	regs->r_sp += sizeof (special_exception_frame_t) -
	    sizeof (normal_exception_frame_t);
	frame_size = 0;
	break;

      case STKFMT_SHORT_BUSERR:
	frame_size = sizeof (short_buserr_exception_frame_t) -
	    sizeof (normal_exception_frame_t);
	break;

      case STKFMT_LONG_BUSERR:
	frame_size = sizeof (long_buserr_exception_frame_t) -
	    sizeof (normal_exception_frame_t);
	break;

      default:
	return (0);
    }

    /*
     * fetch frame extension.
     */
    if (frame_size > 0) {
	if (copyin(sp+sizeof (normal_exception_frame_t),
		   return_frame+1,
		   frame_size))
	    return (0);

	frame_size += sizeof (normal_exception_frame_t);
	regs->r_sp += frame_size;
	
	/*
	 * setup return frame.
	 */
	pcb->pcb_return_frame_size = frame_size;
	pcb->pcb_frame = return_frame;
	pcb->pcb_flags |= (RET_SPECIAL|RET_FRAME);

	/*
	 * set system byte of
	 * software sr from frame.
	 */
	((struct status_reg *)&mac->sr)->sr_sys =
	    ((struct status_reg *)&return_frame->f_sr)->sr_sys;

	((struct status_reg *)&mac->sr)->sr_s = 0;
	
	return_frame->f_sr &= ~(SR_USERCLR&~SR_TRACE);
	pcb->pcb_ast &= ~TRACE_USER;
    }
    else {
	frame->f_sr = return_frame->f_sr;
	frame->f_pc = return_frame->f_pc;
	frame->f_fmt = STKFMT_NORMAL;
	frame->f_vector = 0;

	regs->r_sp += sizeof (normal_exception_frame_t);

	/*
	 * set system byte of
	 * software sr from frame.
	 */
	((struct status_reg *)&mac->sr)->sr_sys =
	    ((struct status_reg *)&frame->f_sr)->sr_sys;
	
	((struct status_reg *)&mac->sr)->sr_s = 0;

	frame->f_sr &= ~(SR_USERCLR&~SR_TRACE);
	pcb->pcb_ast &= ~TRACE_USER;
    }

    return (1);
}
