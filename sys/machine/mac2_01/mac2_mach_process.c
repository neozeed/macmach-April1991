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
 * $Log:	mac2_mach_process.c,v $
 * Revision 2.2.1.1  90/09/07  00:56:33  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/08/30  11:01:52  bohman
 * 	Created.
 * 	[90/08/29  11:38:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/mac2_mach_process.c
 */

#include <cputypes.h>

#include <machine/reg.h>
#include <machine/psl.h>
#include <machine/vmparam.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/proc.h>

#include <mach/vm_param.h>

#include <vm/vm_kern.h>
#include <vm/vm_map.h>

#include <kern/thread.h>

#include <mac2/ptrace.h>

#define	NIPCREG 16
int ipcreg[NIPCREG] =
	{R0,R1,R2,R3,R4,R5,R6,R7,AR0,AR1,AR2,AR3,AR4,AR5,AR6,AR7};

#define	PHYSOFF(p, o) \
	((physadr)(p)+((o)/sizeof (((physadr)0)->r[0])))

/*
 * sys-trace system call.
 */
ptrace()
{
    register struct proc *p;
    register struct a {
	enum ptracereq req;
	int	pid;
	caddr_t	addr;
	int	data;
    } *uap;
    
    thread_t	thread;
    struct uthread	*uthread;
    int		*locr0;
    struct pcb	*pcb;

    int		i;
    int		*up;
    
    uap = (struct a *)u.u_ap;
    
    /*
     *	Intercept and deal with "please trace me" request.
     */
    if (uap->req == PTRACE_TRACEME) {
	u.u_procp->p_flag |= STRC;
	return;
    }
    
    /*
     *	Locate victim, and make sure it is traceable.
     */
    p = pfind(uap->pid);
    if (p == NULL) {
	u.u_error = ESRCH;
	return;
    }
    if (p->task->user_stop_count == 0 ||
	p->p_ppid != u.u_procp->p_pid ||
	!(p->p_flag & STRC)) {
	u.u_error = ESRCH;
	return;
    }
    
    /*
     *	Identify victim.
     */
    thread = p->thread;
    pcb = thread->pcb;
    uthread = thread->u_address.uthread;
    locr0 = uthread->uu_ar0;
    
    u.u_r.r_val1 = 0;

    switch (uap->req) {

      case PTRACE_PEEKTEXT:
      case PTRACE_PEEKDATA:
	u.u_error = ptrace_read_data(p,
				     (vm_offset_t)uap->addr,
				     sizeof(int),
				     (caddr_t)&u.u_r.r_val1);
	break;
		
      case PTRACE_POKETEXT:
      case PTRACE_POKEDATA:
	u.u_error = ptrace_write_data(p,
				      (vm_offset_t)uap->addr,
				      sizeof(int),
				      (caddr_t)&uap->data,
				      (caddr_t) 0,
				      FALSE);
	break;
		
      case PTRACE_PEEKUSER:
	/*
	 *	Read victim's U area or registers.
	 *	Offsets are into BSD kernel stack, and must be
	 *	faked to match MACH.
	 */
	i = (int)uap->addr;
	if (i < 0 || i >= ctob(UPAGES))
	    goto error;
	if (i < sizeof(struct user)) {
	    /*
	     *	We want data from the U area.  Fake it up,
	     *	then pull out the desired int.
	     */
	    struct user	fake_uarea;
	    
	    bzero((caddr_t)&fake_uarea, sizeof(struct user));
	    fake_u(&fake_uarea, thread);
	    u.u_r.r_val1 = *(int *)PHYSOFF(&fake_uarea, i);
	}
	else {
	    /*
	     *	Assume we want data from the kernel stack, most
	     *	likely the user's registers.
	     */
	    u.u_r.r_val1 = *(int *)
		PHYSOFF(thread->kernel_stack
			+ (KERNEL_STACK_SIZE - ctob(UPAGES)), i);
	}
	break;
	
      case PTRACE_POKEUSER:
	i = (int) uap->addr;
	
	/*
	 *	Write one of the user's registers.
	 *	Convert the offset (in old-style Uarea/kernel stack)
	 *	into the corresponding offset into the saved
	 *	register set.
	 */
	up = (int *)PHYSOFF(thread->kernel_stack
			    + (KERNEL_STACK_SIZE - ctob(UPAGES)), i);
	for (i = 0; i < NIPCREG; i++)
	    if (up == &locr0[ipcreg[i]])
		goto ok;
	goto error;
	
      ok:
	*up = uap->data;
	goto error;
	break;
	
	/*
	 * read general registers.
	 */
      case PTRACE_GETREGS:
	if (copyout((caddr_t)locr0,
		    uap->addr,
		    sizeof(struct regs)) != 0) {
	    u.u_error = EIO;
	}
	break;

	/*
	 * write general registers.
	 */
      case PTRACE_SETREGS:
	{
	    struct regs	ip_regs;
	    
	    if (copyin(uap->addr, (caddr_t)&ip_regs,
		       sizeof(ip_regs)) != 0) {
		u.u_error = EIO;
		break;
	    }
	    bcopy((caddr_t)&ip_regs, (caddr_t)locr0, sizeof(struct regs));
	    break;
	}

	/*
	 * read frame registers.
	 */
      case PTRACE_GETSREGS:
	{
	    struct {
		unsigned long sr;
		unsigned long pc;
	    } ip_regs;
	    
	    if (pcb->pcb_frame == 0) {
		u.u_error = EIO;
		break;
	    }
	    ip_regs.sr = pcb->pcb_frame->f_normal.f_sr;
	    ip_regs.pc = pcb->pcb_frame->f_normal.f_pc;
	    if (copyout((caddr_t)&ip_regs, uap->addr, sizeof (ip_regs)) != 0) {
		u.u_error = EIO;
		break;
	    }
	    break;
	}
	
	/*
	 * write frame registers.
	 */
      case PTRACE_SETSREGS:
	{
	    struct {
		unsigned long sr;
		unsigned long pc;
	    } ip_regs;
	    generic_exception_frame_t *frame = pcb->pcb_frame;
	    
	    if (frame == 0 ||
		copyin(uap->addr, (caddr_t)&ip_regs, sizeof (ip_regs)) != 0) {
		u,u_error = EIO;
		break;
	    }
	    if (frame->f_normal.f_fmt != STKFMT_NORMAL) {
		frame = &pcb->pcb_return_frame_data;
		pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
		frame->f_normal.f_fmt = STKFMT_NORMAL;
		frame->f_normal.f_vector = 0;
		pcb->pcb_frame = frame;
		pcb->pcb_flags |= RET_FRAME;
	    }
	    frame->f_normal.f_pc = ip_regs.pc;
	    frame->f_normal.f_sr = ip_regs.sr;
	    frame->f_normal.f_sr &= ~(SR_T0|SR_SUPR|SR_MASTER|SR_IPL);
	    break;
	}
	
	/*
	 * Read floating point registers
	 */
      case PTRACE_GETFPREGS:
	if (copyout((caddr_t)&pcb->pcb_fp_state,
		    uap->addr,
		    sizeof(pcb->pcb_fp_state)) != 0) {
	    u.u_error = EIO;
	}
	break;
	
	/*
	 * Write floating point registers
	 */
      case PTRACE_SETFPREGS:
	{
	    fp_state_t	ip_fp_state;
	    
	    if (copyin(uap->addr,
		       (caddr_t)&ip_fp_state,
		       sizeof(ip_fp_state)) != 0) {
		u.u_error = EIO;
		break;
	    }
	    pcb->pcb_fp_state = ip_fp_state;
	    pcb->pcb_flags |= FP_RESTORE;
	    pcb->pcb_fp.fp_frame.fpf_format = 0;
	    break;
	}
	
	/*
	 * set signal and continue
	 */

	/*
	 * one version causes a trace-trap
	 */
      case PTRACE_SINGLESTEP:

	/*
	 * one version stops tracing
	 */
      case PTRACE_CONT:
	{
	    generic_exception_frame_t *frame = pcb->pcb_frame;
	    
	    if (frame == 0)
		goto error;
	    
	    if ((int)uap->addr != 1) {
		if (frame->f_normal.f_fmt != STKFMT_NORMAL) {
		    generic_exception_frame_t *old_frame = frame;
		    
		    frame = &pcb->pcb_return_frame_data;
		    pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
		    frame->f_normal.f_fmt = STKFMT_NORMAL;
		    frame->f_normal.f_vector = 0;
		    frame->f_normal.f_sr = old_frame->f_normal.f_sr;
		    pcb->pcb_frame = frame;
		    pcb->pcb_flags |= RET_FRAME;
		}
		frame->f_normal.f_pc = (int)uap->addr;
	    }
	    
	    if ((unsigned)uap->data > NSIG)
		goto error;
	    
	    if (sigmask(p->p_cursig) & threadmask)
		uthread->uu_cursig = 0;
	    
	    p->p_cursig = uap->data;	/* see issig */
	    
	    if (sigmask(uap->data) & threadmask)
		uthread->uu_cursig = uap->data;
	    
	    if (uap->req == PTRACE_SINGLESTEP) 
		frame->f_normal.f_sr |= PSL_T;
	  resume:
	    p->p_stat = SRUN;
	    task_resume(p->task);
	    break;
	}

	/* force exit */
      case PTRACE_KILL:
	p->p_cursig += NSIG;
	goto resume;
		
      default:
      error:
	u.u_error = EIO;
    }
}
