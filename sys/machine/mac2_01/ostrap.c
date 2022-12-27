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
 * $Log:	ostrap.c,v $
 * Revision 2.2.1.1  90/09/07  00:57:01  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/08/30  11:02:10  bohman
 * 	Created.
 * 	[90/08/29  11:41:15  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ostrap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/param.h>

#include <kern/thread.h>

#include <mac2/pcb.h>
#include <mac2/psl.h>

#define OSTRAP_TBOX_TYPE	0x0800
#define OSTRAP_TBOX_APOP	0x0400
#define OSTRAP_TBOX_CODE	0x03ff

#define	OSTRAP_OS_FLAGS		0x0600
#define OSTRAP_OS_A0SPEC	0x0100
#define OSTRAP_OS_CODE		0x00ff

extern	(*mac_os_traps[])();
extern	(*mac_tbox_traps[])();

/*
 * Handle a kernel mode
 * MacOS trap.
 */
mac_ostrap(frame, regs)
register generic_exception_frame_t *frame;
register regs_t *regs;
{
    register (*vector)(), pop = 0;
    unsigned short inst;
    register struct inst_ostrap {
	unsigned short :4,
		   code:12;
    } *i = (struct inst_ostrap *)&inst;

    inst = *(unsigned short *)frame->f_normal.f_pc;

    if (i->code&OSTRAP_TBOX_TYPE) {
	/*
	  * ToolBox traps
	  */
	if (i->code&OSTRAP_TBOX_APOP) {
	    frame->f_normal.f_pc = *(unsigned long *)regs->r_sp;
	    regs->r_sp += sizeof (long);
	    pop = sizeof (long);
	}
	else
	    frame->f_normal.f_pc += sizeof (inst);

	vector = mac_tbox_traps[i->code & OSTRAP_TBOX_CODE];
	if (vector)
	    (*vector)(frame, regs);
    }
    else {
	/*
	 * Operating System traps
	 */
	frame->f_normal.f_pc += sizeof (inst);

	vector = mac_os_traps[i->code & OSTRAP_OS_CODE];
	if (vector)
	    (*vector)(frame, regs, (i->code & OSTRAP_OS_FLAGS));
    }

    /*
     * If trap is not implemented,
     * longjmp through pcb_label,
     * or if that is not set, return
     * funny error code.
     */
    if (vector == 0) {
	thread_t	thread = current_thread();

	printf("unimplemented ostrap %x\n", inst);

	if (thread != THREAD_NULL && thread->pcb->pcb_flags&JMP_FAULT) {
	    thread->pcb->pcb_flags &= ~JMP_FAULT;
	    longjmp(&thread->pcb->pcb_label);
	}
	else
	    regs->r_r0 = 32768; /* XXX really -32768 */
    }

    /*
     * Set condition codes based on
     * result like the ROM trap
     * handler does.
     */
    frame->f_normal.f_sr &= (~SR_CC|SR_X);
    if ((short)regs->r_r0 == 0)
	frame->f_normal.f_sr |= SR_Z;
    else
    if ((short)regs->r_r0 < 0)
	frame->f_normal.f_sr |= SR_N;

    return (pop);
}

/*
 * Trap linkage routines.
 */
mac_ostrap_null(frame, regs)
struct regs *regs;
{
    regs->r_r0 = 0;
}

get_zone(frame, regs)
struct regs *regs;
{
    regs->r_areg[0] = *(unsigned long *)0x118;	/* TheZone */
    regs->r_r0 = 0;
}

set_zone_x(frame, regs)
struct regs *regs;
{
    *(unsigned long *)0x118 = regs->r_areg[0];
    regs->r_r0 = 0;
}

stack_space(frame, regs)
struct regs *regs;
{
    regs->r_r0 = 4096;	/* XXX */
}

swap_mmu_mode(frame, regs)
struct regs *regs;
{
    char mode = *(unsigned char *)0xcb2;

    *(unsigned char *)0xcb2 = regs->r_r0;
    regs->r_r0 = mode;
}

strip_address(frame, regs)
struct regs *regs;
{
    char mode = *(unsigned char *)0xcb2;

    if (!mode)
	regs->r_r0 &= 0x00ffffff;
}

new_handle(frame, regs)
register struct regs *regs;
{
    register vm_offset_t p;
    register vm_size_t size = regs->r_r0;

    regs->r_areg[0] = p = NewHandle(size);
    if (p) {
	bzero(*(vm_offset_t *)p, size);
	regs->r_r0 = 0;
    }
    else
	regs->r_r0 = -108;	/* memFullErr */
}

dispos_handle(frame, regs)
register struct regs *regs;
{
    DisposHandle(regs->r_areg[0]);
    regs->r_r0 = 0;
}

new_ptr(frame, regs)
register struct regs *regs;
{
    register vm_offset_t p;
    register vm_size_t size = regs->r_r0;

    regs->r_areg[0] = p = NewPtr(size);
    if (p) {
	bzero(p, size);
	regs->r_r0 = 0;
    }
    else
	regs->r_r0 = -108;	/* memFullErr */
}

dispos_ptr(frame, regs)
register struct regs *regs;
{
    DisposPtr(regs->r_areg[0]);
    regs->r_r0 = 0;
}

s_int_install(frame, regs)
register struct regs *regs;
{
    regs->r_r0 = SIntInstall(regs->r_areg[0], (unsigned char)regs->r_r0);
}

s_int_remove(frame, regs)
register struct regs *regs;
{
    regs->r_r0 = SIntRemove(regs->r_areg[0], (unsigned char)regs->r_r0);
}

block_move(frame, regs)
register struct regs *regs;
{

    ovbcopy(regs->r_areg[0], regs->r_areg[1], regs->r_r0);
    regs->r_r0 = 0;
}
