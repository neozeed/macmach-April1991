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
 * $Log:	pcb.c,v $
 * Revision 2.2.1.1  90/09/07  00:56:17  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:23:02  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pcb.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <kern/thread.h>

#include <vm/vm_kern.h>

#include <mach/thread_status.h>
#include <mach/vm_param.h>

#include <mach/mac2/kern_return.h>

#include <mac2/pcb.h>
#include <mac2/psl.h>

void
pcb_init(thread, ksp)
register struct thread *thread;
register vm_offset_t ksp;
{
    register struct pcb *pcb = thread->pcb;
    register generic_exception_frame_t *frame;
    int thread_bootstrap();
    
    bzero((caddr_t)pcb, sizeof(struct pcb));

    pcb->pcb_thread = thread;
    
    /*
     *	Set up thread to start at user bootstrap.  The user
     *	register block is put at the top of the kernel stack as
     *	if the thread had just taken a SYSCALL trap.  Thread-
     *	bootstrap will simulate returning from a trap.
     *
     */
    pcb->pcb_kernel.r_pc = (int) thread_bootstrap;
    pcb->pcb_kernel.r_sr = SR_LOW;	/* kernel mode, interrupts on */
    pcb->pcb_frame = frame = (generic_exception_frame_t *)
	(ksp + KERNEL_STACK_SIZE - NORMAL_EXCEPTION_FRAME_SIZE);
    /*
     *	Leave a word on the kernel stack for the return address from
     *	load_context.
     */
    pcb->pcb_kernel.r_ksp = ((unsigned long)frame) - sizeof(long);
    
    /*
     *    Guarantee that the bootstrapped thread will be in user
     *	mode (this psl assignment above executes the bootstrap
     *	code in kernel mode.  Note, this is the only user register
     *	that we set.  All others are assumed to be random unless
     *	the user sets them.
     *
     *	Also set up the rte frame to return from.
     */
    frame->f_normal.f_sr = SR_USER;
    frame->f_normal.f_fmt = STKFMT_NORMAL;
    frame->f_normal.f_vector = 0;
}

/*
 *      Set up the context for the very first thread to execute
 *      (which is passed in as the parameter).
 */
void initial_context(thread)
thread_t thread;
{
    active_threads[cpu_number()] = thread;
    pmap_activate(vm_map_pmap(thread->task->map), thread, cpu_number());
    load_context_data();
}

/*
 *	thread_start:
 *
 *	Start a thread at the specified routine.  The thread must be in a
 *	suspended state.
 */
thread_start(thread, start, mode)
thread_t thread;
void (*start)();
int mode;
{
    register struct pcb *pcb = thread->pcb;
    
    pcb->pcb_kernel.r_pc = (int) start;	/* start at the beginning */
    if (mode == THREAD_USERMODE)
	pcb->pcb_kernel.r_sr = SR_USER; /* user mode */
    else
	pcb->pcb_kernel.r_sr = SR_LOW; /* kernel mode,
					* all interrupts enabled */
    /*
     *	Leave a word at the top of the kernel stack for the
     *	return address from load_context.
     */
    pcb->pcb_kernel.r_ksp = thread->kernel_stack+
	KERNEL_STACK_SIZE-sizeof (long);
    pcb->pcb_kernel.r_kfp = thread->kernel_stack;
}

load_context_data()
{
    U_ADDRESS.uthread = active_threads[cpu_number()]->u_address.uthread;
    U_ADDRESS.utask   = active_threads[cpu_number()]->u_address.utask;
}

/*
 *	thread_setstatus:
 *
 *	Set the status of the specified thread.
 */
kern_return_t
thread_setstatus(thread, flavor, tstate, count)
thread_t	thread;
int		flavor;
thread_state_t	tstate;
unsigned int	count;
{
    register struct pcb *pcb = thread->pcb;
    
    switch (flavor) {
      case THREAD_STATE_REGS:
	{
	    register thread_state_regs_t *state;
	    
	    if (count < THREAD_STATE_REGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_regs_t *) tstate;
	    
	    /*
	     * copy machine registers
	     */
	    bcopy(state, &pcb->pcb_user, sizeof (regs_t));
	}
	break;
	
      case THREAD_STATE_FPREGS:
	{
	    register thread_state_fpregs_t *state;
	    
	    if (count < THREAD_STATE_FPREGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_fpregs_t *)tstate;

	    pcb->pcb_fp_state = *state;
	    pcb->pcb_flags |= FP_RESTORE;
	    pcb->pcb_fp_frame.fpf_format = 0;
	}
	break;
	
      case THREAD_STATE_FRAME:
	{
	    register thread_state_frame_t *state;
	    register frame_size;
	    
	    state = (thread_state_frame_t *)tstate;

	    switch (state->f_normal.f_fmt) {
	      case STKFMT_NORMAL:
		frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SPECIAL:
		frame_size = SPECIAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_COPROC:
		frame_size = COPROC_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SHORT_BUSERR:
		frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_LONG_BUSERR:
		frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      default:
		return (KERN_INVALID_ARGUMENT);
	    }
	    /*
	     * This code knows that frame_size
	     * always represents an integral
	     * number of long words.
	     */
	    if ((count << 2) < frame_size)
		return (KERN_INVALID_ARGUMENT);

	    bcopy(state, &pcb->pcb_return_frame_data, frame_size);

	    pcb->pcb_frame = &pcb->pcb_return_frame_data;
	    pcb->pcb_return_frame_size = frame_size;
	    pcb->pcb_flags |= (RET_SPECIAL|RET_FRAME);
	    
	    /*
	     *	Enforce user mode status register:
	     *	must have user mode, user stack, interrupt priority 0.
	     *	User may set trace single bit.
	     */
	    pcb->pcb_frame->f_normal.f_sr &= ~(SR_T0|SR_SUPR|SR_MASTER|SR_IPL);
	}
	break;

      case THREAD_STATE_FPFRAME:
	{
	    register thread_state_fpframe_t *state;
	    register frame_size;

	    state = (thread_state_fpframe_t *)tstate;

	    switch (state->fpf_size) {
	      case FPF_NULL_SIZE:
		frame_size = sizeof (state->fpf_format);
		break;

	      case FPF_IDLE_SIZE:
		frame_size = FPF_IDLE_SIZE + sizeof (state->fpf_format);
		break;

	      case FPF_BUSY_SIZE:
		frame_size = FPF_BUSY_SIZE + sizeof (state->fpf_format);
		break;

	      default:
		return (KERN_INVALID_ARGUMENT);
	    }
	    /*
	     * This code knows that frame_size
	     * always represents an integral
	     * number of long words.
	     */
	    if ((count << 2) < frame_size)
		return (KERN_INVALID_ARGUMENT);

	    if (frame_size != sizeof (state->fpf_format))
		bcopy(state, &pcb->pcb_fp_frame, frame_size);
	    else
		pcb->pcb_fp_frame.fpf_format = state->fpf_format;

	    pcb->pcb_flags &= ~FP_RESTORE;
	}
	break;
	
      default:
	return (KERN_INVALID_ARGUMENT);
    }
    
    return(KERN_SUCCESS);
}

/*
 *	thread_getstatus:
 *
 *	Get the status of the specified thread.
 */
kern_return_t
thread_getstatus(thread, flavor, tstate, count)
register thread_t thread;
int flavor;
thread_state_t tstate;	/* pointer to OUT array */
unsigned int *count;		/* IN/OUT */
{
    register struct pcb *pcb = thread->pcb;
    
    switch (flavor) {
      case THREAD_STATE_REGS:
	{
	    register thread_state_regs_t *state;
	    
	    if (*count < THREAD_STATE_REGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_regs_t *) tstate;
	    
	    /*
	     * copy machine registers
	     */
	    bcopy(&pcb->pcb_user, state, sizeof (regs_t));
	    *count = THREAD_STATE_REGS_COUNT;
	}
	break;
	
      case THREAD_STATE_FPREGS:
	{
	    register thread_state_fpregs_t *state;
	    
	    if (*count < THREAD_STATE_FPREGS_COUNT)
		return (KERN_INVALID_ARGUMENT);
	    
	    state = (thread_state_fpregs_t *)tstate;

	    *state = pcb->pcb_fp_state;
	    *count = THREAD_STATE_FPREGS_COUNT;
	}
	break;
	
      case THREAD_STATE_FRAME:
	{
	    register thread_state_frame_t *state;
	    register frame_size;

	    state = pcb->pcb_frame;
	    switch (state->f_normal.f_fmt) {
	      case STKFMT_NORMAL:
		frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SPECIAL:
		frame_size = SPECIAL_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_COPROC:
		frame_size = COPROC_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_SHORT_BUSERR:
		frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      case STKFMT_LONG_BUSERR:
		frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
		break;
		
	      default:
		return (KERN_INVALID_ARGUMENT);
	    }
	    /*
	     * This code knows that frame_size
	     * always represents an integral
	     * number of long words.
	     */
	    if ((*count << 2) < frame_size)
		return (KERN_INVALID_ARGUMENT);

	    bcopy(state, tstate, frame_size);
	    *count = (frame_size >> 2);
	}
	break;

      case THREAD_STATE_FPFRAME:
	{
	    thread_state_fpframe_t *state;
	    fp_frame_t *fp_frame;
	    register frame_size;

	    state = (thread_state_fpframe_t *)tstate;

	    fp_frame = &pcb->pcb_fp_frame;
	    switch (fp_frame->fpf_size) {
	      case FPF_NULL_SIZE:
		frame_size = sizeof (fp_frame->fpf_format);
		break;

	      case FPF_IDLE_SIZE:
		frame_size = FPF_IDLE_SIZE + sizeof (fp_frame->fpf_format);
		break;

	      case FPF_BUSY_SIZE:
		frame_size = FPF_BUSY_SIZE + sizeof (fp_frame->fpf_format);
		break;

	      default:
		return (KERN_INVALID_ARGUMENT);
	    }
	    /*
	     * This code knows that frame_size
	     * always represents an integral
	     * number of long words.
	     */
	    if ((*count << 2) < frame_size)
		return (KERN_INVALID_ARGUMENT);

	    if (frame_size != sizeof (fp_frame->fpf_format))
		bcopy(fp_frame, state, frame_size);
	    else
		state->fpf_format = fp_frame->fpf_format;
	    *count = (frame_size >> 2);
	}
	break;
	
      default:
	return (KERN_INVALID_ARGUMENT);
    }
    
    return(KERN_SUCCESS);
}

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>

/*
 *	thread_dup:
 *
 *	Duplicate the user's state of a thread.  This is only used to perform
 *	the Unix fork operation.
 *
 *	Assumes that the child thread's pcb and user registers have been
 *	initialized as by pcb_init.
 */
thread_dup(parent, child)
register thread_t parent, child;
{
    register struct regs *child_regs;
    
    child->pcb->pcb_user = parent->pcb->pcb_user;
    
    child_regs = &child->pcb->pcb_user;
    child_regs->r_r0 = proc[child->task->proc_index].p_pid;
    child_regs->r_r1 = 1;
    child->pcb->pcb_frame->f_normal.f_pc =
	parent->pcb->pcb_frame->f_normal.f_pc;
    child->pcb->pcb_frame->f_normal.f_sr =
	parent->pcb->pcb_frame->f_normal.f_sr&~(SR_T1|SR_T0|SR_CC);
    
    /*
     *	Don't forget to pop the system call number from the
     *	child's stack.
     */
    child_regs->r_sp += sizeof(long);
    
    /*
     *	Copy any floating-point state.
     */
    pcb_synch(parent);	/* force parent's FP state to memory */
    if (parent->pcb->pcb_fp_frame.fpf_size) {
	/*
	 *	Copy floating point state
	 */
	child->pcb->pcb_fp_state = parent->pcb->pcb_fp_state;
	child->pcb->pcb_fp_frame = parent->pcb->pcb_fp_frame;
    } else {
	/*
	 *	No floating point state to save - zero it.
	 */
	child->pcb->pcb_fp_frame.fpf_format = 0;
    }
}

void pcb_terminate(thread)
thread_t	thread;
{
}

/*
 * pcb_synch() stores special user context
 * into the pcb.
 */
pcb_synch(thread)
register thread_t thread;
{
    if (thread != current_thread())
	return;

    fp_sync();
}
