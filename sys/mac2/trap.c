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
 * $Log:	trap.c,v $
 * Revision 2.2.1.1  90/09/07  00:58:02  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:29:49  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/trap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach_rfs.h>

#include <vice.h>
#include <xpr_debug.h>

#include <macos_emul.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/vm.h>
#include <sys/kernel.h>
#include <bsd/syscalls.c>

#include <kern/xpr.h>

#include <mac2/pmap.h>
#include <machine/cpu.h>
#include <machine/psl.h>
#include <machine/reg.h>
#include <machine/trap.h>
#include <machine/call_frame.h>

#include <kern/thread.h>
#include <kern/task.h>
#include <vm/vm_kern.h>
#include <vm/vm_map.h>
#include <mach/vm_param.h>
#include <mach/kern_return.h>
#include <kern/syscall_sw.h>

#include <mach/exception.h>
#include <builtin/ux_exception.h>

#include <mac2/pmmu_inline.c>

#if	MACOS_EMUL
#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>
#endif

extern struct	sysent	cmusysent[];
extern int ncmusysent;
extern int nallsysent;
extern struct sysent sysent[];
extern int nsysent;

struct	u_address	U_ADDRESS;

label_t *nofault;

int trapdebug = 0;

/*
 * Handle traps from user mode.
 */
user_trap(type, frame)
int type;
register generic_exception_frame_t *frame;
{
    register struct pcb *pcb = current_thread_pcb();
    register regs_t *regs;
    register struct proc *p;
    register exception, code, subcode = 0;
#ifdef notyet
    struct timeval syst;
#endif

    regs = &pcb->pcb_user;
    pcb->pcb_frame = frame;
#ifdef notyet
    syst = u.u_ru.ru_stime;
#endif
    u.u_ar0 = &regs->r_r0;

    if (trapdebug)
	printf("user_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);

    if ((pcb->pcb_ast&TRACE_USER) == 0)
	frame->f_normal.f_sr &= ~SR_TRACE;

    pcb->pcb_flags &= ~(RET_SPECIAL|RET_FRAME);

    switch (type) {
      case TRAP_BAD_ACCESS:
	{
	    register unsigned short status;
	    register fault = 0;
	    
	    switch (frame->f_normal.f_fmt) {
	      case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
		/*
		 * Data fault
		 */
		if (short_buserr_frame->f_dfault) {
#if	MACOS_EMUL
		    if (pcb->pcb_flags&MAC_EMULATION) {
			if (macdevice_cycle(frame, regs))
			    break;
		    }
#endif
		    status =
			pmmu_test_user_data(short_buserr_frame->f_fault);
		    /*
		     * RMW cycle w/o status bits
		     * means that ATC entry is
		     * missing; load it.
		     */
		    if (short_buserr_frame->f_rmw &&
			(status&
			 (PMMU_SR_WRPROT|PMMU_SR_INVALID|PMMU_SR_BUSERR)) == 0)
			pmmu_load_user(short_buserr_frame->f_fault);
		    else
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			/*
			 * Call pagein only for
			 * invalid entries or protection
			 * faults; never for physical memory
			 * faults.
			 */
			code = pagefault(short_buserr_frame->f_fault,
					 (short_buserr_frame->f_rmw ||
					  !short_buserr_frame->f_rw)?
					 VM_PROT_READ|VM_PROT_WRITE:
					 VM_PROT_READ,
					 short_buserr_frame->f_fcode);
			if (code == KERN_SUCCESS) {
			    if (short_buserr_frame->f_rmw)
				pmmu_load_user(short_buserr_frame->f_fault);
			}
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_fault;
			break;
		    }
		}
		/*
		 * Instruction fault on StageC
		 */
		if (short_buserr_frame->f_faultc) {
		    status =
			pmmu_test_user_text(short_buserr_frame->f_pc+2);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(short_buserr_frame->f_pc+2,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_pc+2;
			break;
		    }
		}
		/*
		 * Instruction fault on StageB
		 */
		else if (short_buserr_frame->f_faultb) {
		    status =
			pmmu_test_user_text(short_buserr_frame->f_pc+4);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(short_buserr_frame->f_pc+4,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = short_buserr_frame->f_pc+4;
			break;
		    }
		}
#undef short_buserr_frame
		break;
		
	      case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
		if (long_buserr_frame->f_dfault) {
#if	MACOS_EMUL
		    if (pcb->pcb_flags&MAC_EMULATION) {
			if (macdevice_cycle(frame, regs))
			    break;
		    }
#endif
		    status =
			pmmu_test_user_data(long_buserr_frame->f_fault);
		    if (long_buserr_frame->f_rmw &&
			(status&
			 (PMMU_SR_WRPROT|PMMU_SR_INVALID|PMMU_SR_BUSERR)) == 0)
			pmmu_load_user(long_buserr_frame->f_fault);
		    else
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_fault,
					 (long_buserr_frame->f_rmw ||
					  !long_buserr_frame->f_rw)?
					 VM_PROT_READ|VM_PROT_WRITE:
					 VM_PROT_READ,
					 long_buserr_frame->f_fcode);
			if (code == KERN_SUCCESS) {
			    if (long_buserr_frame->f_rmw)
				pmmu_load_user(long_buserr_frame->f_fault);
			}
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_fault;
			break;
		    }
		}
		if (long_buserr_frame->f_faultc) {
		    status =
			pmmu_test_user_text(long_buserr_frame->f_stageb-2);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_stageb-2,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_stageb-2;
			break;
		    }
		}
		else if (long_buserr_frame->f_faultb) {
		    status =
			pmmu_test_user_text(long_buserr_frame->f_stageb);
		    if ((status&(PMMU_SR_INVALID|PMMU_SR_WRPROT)) &&
			(status&PMMU_SR_BUSERR) == 0) {
			code = pagefault(long_buserr_frame->f_stageb,
					 VM_PROT_READ, FC_UP);
			if (code == KERN_SUCCESS)
			    ;
			else
			    fault = 1;
		    }
		    else
			fault = 1;
		    
		    if (fault) {
			subcode = long_buserr_frame->f_stageb;
			break;
		    }
		}
#undef long_buserr_frame
		break;
		
	      default:
		user_trap_error(frame, regs);
		panic("user_trap: BUSERR unknown frame type");
	    }

	    if (!fault)
		return (0);

#if	MACOS_EMUL
	    if ((pcb->pcb_flags&MAC_EMULATION) &&
		(macdebug & MAC_DEBUG_BERR) == 0) {
		    if (macram_cycle(frame, regs))
			return (0);
		    else
			if (take_exception(frame, regs))
			    return (0);
	    }
#endif

	    exception = EXC_BAD_ACCESS;
	}
	break;

      case TRAP_TRACE:
	dotrace();
	goto out;

      case TRAP_EMULATION_1010:
      case TRAP_EMULATION_1111:
	exception = EXC_EMULATION;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_BAD_INSTRUCTION:
#if	MACOS_EMUL
	if ((pcb->pcb_flags&MAC_EMULATION) &&
	    (macdebug & MAC_DEBUG_EXCEP) == 0) {
	    if (take_exception(frame, regs))
		return (0);
	}
#endif
      case TRAP_PRIV_INSTRUCTION:
	exception = EXC_BAD_INSTRUCTION;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_BREAKPOINT:
	frame->f_normal.f_pc -= 2;
	exception = EXC_BREAKPOINT;
	code = frame->f_normal.f_vector;
	break;

      case TRAP_ARITHMETIC:
#if	MACOS_EMUL
	if ((pcb->pcb_flags&MAC_EMULATION) &&
	    (macdebug & MAC_DEBUG_EXCEP) == 0) {
	    if (take_exception(frame, regs))
		    return (0);
	}
#endif
	code = frame->f_normal.f_vector;
	if (code == EXC_MAC2_FLT_BSUN || code == EXC_MAC2_FLT_OPERAND_ERROR)
	    exception = EXC_BAD_INSTRUCTION;
	else
	    exception = EXC_ARITHMETIC;
	break;

      case TRAP_SOFTWARE:
#if	MACOS_EMUL
	if ((pcb->pcb_flags&MAC_EMULATION) &&
	    (macdebug & MAC_DEBUG_EXCEP) == 0) {
	    if (take_exception(frame, regs))
		    return (0);
	}
#endif
	exception = EXC_SOFTWARE;
	code = frame->f_normal.f_vector;
	break;
	
      default:
	user_trap_error(frame, regs);
	panic("user_trap: unknown trap type");
    }

    /*
     * Send a MACH exception to our
     * exception port.
     */
    pcb->pcb_ast &= ~TRACE_PENDING;
    thread_doexception(current_thread(), exception, code, subcode);

out:
    if (pcb->pcb_ast&TRACE_PENDING)
	dotrace();

    /*
     * U**X signals.
     */
    p = u.u_procp;
    if (p) {
	thread_t thread;
	
	thread = current_thread();
	
	if (CHECK_SIGNALS(p, thread, thread->u_address.uthread)) {
	    /*
	     * If RET_SPECIAL is set
	     * then our return frame
	     * has special info which would
	     * be lost if we took a signal now.
	     * Instead we schedule an AST to
	     * cause another trap soon.
	     */
	    if ((pcb->pcb_flags&RET_SPECIAL) == 0) {
		if (p->p_cursig || issig())
		    psig();
	    }
	    else
		pcb->pcb_ast |= AST_SCHED;
	}
    }
    
    if (trapdebug)
	printf("exit user_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);

    /*
     * Preempt thread?
     */
    if (csw_needed(current_thread(), current_processor())) {
	u.u_ru.ru_nivcsw++;
	thread_block();
    }
#if	MACOS_EMUL
    check_mac_interrupts(pcb);
#endif

#ifdef notyet
    if (u.u_prof.pr_scale) {
	int ticks;
	struct timeval *tv = &u.u_ru.ru_stime;
	
	ticks = ((tv->tv_sec - syst.tv_sec) * 1000 +
		 (tv->tv_usec - syst.tv_usec) / 1000) / (tick / 1000);
	if (ticks)
	    addupc(locregs->r_pc, &u.u_prof, ticks);
    }
#endif
    return (0);
}

user_trap_error(frame, regs)
generic_exception_frame_t *frame;
regs_t *regs;
{
    cons_force(); dumptrap("user_trap", frame, regs);
}

/*
 * Handle traps from kernel mode.
 */
kernel_trap(type, frame, regs)
register type;
register generic_exception_frame_t *frame;
register regs_t *regs;
{
    register code;
    register unsigned long frame_size;
    thread_t		thread = current_thread();
    
    if (trapdebug)
	printf("kernel_trap: type %x pc %x sr %x\n",
	       type, frame->f_normal.f_pc, frame->f_normal.f_sr);
    
#define ADJUST_SP(f, t) (unsigned long)(((t *)(f))+1)
    /*
     * Adjust the saved ksp to the
     * value it had when the trap
     * occurred; also calculate
     * the size of the stack frame.
     */
    switch (frame->f_normal.f_fmt) {
      case STKFMT_NORMAL:
	regs->r_sp = ADJUST_SP(frame, normal_exception_frame_t);
	frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_SPECIAL:
	regs->r_sp = ADJUST_SP(frame, special_exception_frame_t);
	frame_size = SPECIAL_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_COPROC:
	regs->r_sp = ADJUST_SP(frame, coproc_exception_frame_t);
	frame_size = COPROC_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_SHORT_BUSERR:
	regs->r_sp = ADJUST_SP(frame, short_buserr_exception_frame_t);
	frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_LONG_BUSERR:
	regs->r_sp = ADJUST_SP(frame, long_buserr_exception_frame_t);
	frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
	break;
    }
#undef ADJUST_SP

    switch (type) {
      case TRAP_BAD_ACCESS:
	/*
	 * nofault is used for catching
	 * traps during system initialization
	 * and in code that is not synchronized
	 * with the execution of any particular
	 * thread.
	 */
	if (nofault) {
	    label_t *ltmp;
	    
	    ltmp = nofault;
	    nofault = 0;
	    longjmp(ltmp);
	}

	/*
	 * pcb_label is used for catching
	 * faults in code that is synchronized
	 * with a particular thread.
	 */
	if (thread != THREAD_NULL && thread->pcb->pcb_flags&JMP_FAULT) {
	    thread->pcb->pcb_flags &= ~JMP_FAULT;
	    longjmp(&thread->pcb->pcb_label);
	}	    

	/*
	 * Fault code is short and sweet;
	 * only data faults are allowed
	 * but no RMW cycles!
	 */
	switch (frame->f_normal.f_fmt) {
	  case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
	    if (short_buserr_frame->f_dfault) {
		code = pagefault(short_buserr_frame->f_fault,
				 short_buserr_frame->f_rw?VM_PROT_READ:
				 VM_PROT_READ|VM_PROT_WRITE,
				 short_buserr_frame->f_fcode);
		if (code == KERN_SUCCESS)
		    return (0);
	    }
#undef short_buserr_frame
	    break;
	    
	  case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
	    if (long_buserr_frame->f_dfault) {
		code = pagefault(long_buserr_frame->f_fault,
				 long_buserr_frame->f_rw?VM_PROT_READ:
				 VM_PROT_READ|VM_PROT_WRITE,
				 long_buserr_frame->f_fcode);
		if (code == KERN_SUCCESS)
		    return (0);
	    }
#undef long_buserr_frame
	    break;
	    
	  default:
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: BUSERR unknown frame type");
	}

	if (thread == THREAD_NULL) {
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: no thread");
	}

	if (thread->recover == 0) {
	    kernel_trap_error(frame, regs);
	    panic("kernel_trap: BUSERR no recover");
	}
	
	frame->f_normal.f_pc = thread->recover;
	thread->recover = 0;
	break;

      case TRAP_TRACE:
	/*
	 * A traced, user mode
	 * TRAP #N instruction
	 * causes the trace trap
	 * to occur in kernel mode.
	 */
	current_thread_pcb()->pcb_ast |= TRACE_PENDING;
	frame->f_special.f_sr &= ~SR_TRACE;
	break;

      case TRAP_EMULATION_1010:
	return (mac_ostrap(frame, regs));
	
      default:
	kernel_trap_error(frame, regs);
	panic("kernel_trap: unknown trap type");
    }
    return (frame_size - NORMAL_EXCEPTION_FRAME_SIZE);
}

kernel_trap_error(frame, regs)
generic_exception_frame_t *frame;
regs_t *regs;
{
    cons_force(); dumptrap("kernel_trap", frame, regs);
    dumpstack(regs->r_areg[6], regs->r_sp);
}

int pagefaultdebug = 0;

/*
 * Handle page fault.
 */
kern_return_t
pagefault(faultaddr, faulttype, fcode)
register vm_offset_t faultaddr;
vm_prot_t faulttype;
register fcode;
{
    register uerror = 0;
    register vm_map_t map;
    register kern_return_t result;
    register task_t task;
    register thread_t thread;

    if (pagefaultdebug)
	printf("pagefault: addr %x type %x fcode %x ",
	       faultaddr, faulttype, fcode);

    thread = current_thread();
    if (thread != THREAD_NULL) {
	uerror = u.u_error;
	u.u_error = 0;
	task = thread->task;
    }
    else
	task = TASK_NULL;

    /*
     * Determine which map to "fault" on.
     */
    switch (fcode) {
      case FC_UP:
      case FC_UD:
#if	MACOS_EMUL
	/*
	 * Special address map.
	 */
	if (thread->pcb->pcb_flags&MAC_24BIT) {
	    map = mac_task_compat_map(thread);
	    faultaddr &= 0x00ffffff;
	}
	else
#endif
	    map = task->map;
	break;
	
      case FC_SD:
	/*
	 * If this is a kernel space
	 * task, try to fault on the
	 * task's own map, if any.  Holding
	 * locks on the kernel_map is risky and
	 * can lead to deadlocks.
	 */
	if (task != TASK_NULL && task->kernel_vm_space)
	    map = task->map;
	else
	    map = kernel_map;
	break;
	
      default:
	panic("pagefault: illegal function code");
    }
    
    if (pagefaultdebug)
	printf("thread %x map %x (%s): ",
	       thread,
	       map,
	       (map == kernel_map)? "kernel":
	       (thread->pcb->pcb_flags&MAC_24BIT)? "task_compat": "task");
    
    result = vm_fault(map, trunc_page(faultaddr), faulttype, FALSE);

    /*
     * As a last resort, try the kernel_map
     * for kernel space tasks.
     */
    if (result != KERN_SUCCESS &&
	task != TASK_NULL && task->kernel_vm_space && map != kernel_map)
	result = vm_fault(kernel_map, trunc_page(faultaddr), faulttype, FALSE);

    if (thread != THREAD_NULL)
	u.u_error = uerror;
    
    if (pagefaultdebug)
	printf("returns %d\n", result);
    
    return (result);
}

int syscalldebug = 0;

/*
 * Called from the trap handler when a system call occurs
 */
syscall(code, frame)
register code;
register generic_exception_frame_t *frame;
{
    register struct pcb *pcb = current_thread_pcb();
    register regs_t *regs = &pcb->pcb_user;
    register struct uthread *uthread = u.uthread;
    register struct proc *p = u.u_procp;
    register struct sysent *callp;
#ifdef notyet
    struct timeval syst;
    short int syst_flag;
#endif
    
    if (syscalldebug)
	printf("syscall: code %d pc %x sr %x usp %x\n",
	       code,
	       frame->f_normal.f_pc, frame->f_normal.f_sr, pcb->pcb_user.r_sp);

#ifdef notyet    
    if (u_prof.pr_scale) {
	syst = u_ru.ru_stime;
	syst_flag = 1;
    } else
	syst_flag = 0;
#endif
    
    /*
     *	If this is a MACH-only thread, we cannot do U**X system
     *	calls.
     */
    if (p == 0) {
	thread_doexception(current_thread(), EXC_SOFTWARE,
			   EXC_UNIX_BAD_SYSCALL, 0);	/* XXX */
	return;
    }

    pcb->pcb_frame = frame;
    u_ar0 = &regs->r_r0;
    
    if ((pcb->pcb_ast&TRACE_USER) == 0)
	frame->f_normal.f_sr &= ~SR_TRACE;
    
    pcb->pcb_flags &= ~(RET_SPECIAL|RET_FRAME);
	
    /*
     *	CMU system calls have negative numbers
     */
    if (code < 0) {
	if (code < -ncmusysent && code >= (-ncmusysent - mach_trap_count)) {
	    regs->r_r0 = do_mach_syscall(code, regs);
	    regs->r_sp += sizeof (long);	/* pop syscall # */
	    goto out;
	} else if (code < -ncmusysent)
	    callp = &sysent[63];	/* undefined */
	else
	    callp = &cmusysent[code + ncmusysent];
    } else
	callp = (code >= nsysent) ? &sysent[63] : &sysent[code];
    u_error = 0;
    if (callp->sy_narg) {
	if (copyin((caddr_t)regs->r_sp + 2 * NBPW, (caddr_t)u_arg,
		   callp->sy_narg*sizeof(int))) {
	    u_error = EFAULT;
	    goto bad;
	}
    }
    u_ap = u_arg;
    u_r.r_val1 = 0;
    u_r.r_val2 = regs->r_r1;

    if (setjmp(&u_qsave)) {
	if (u_error == 0 && u_eosys != RESTARTSYS)
	    u_error = EINTR;
    } else {
	u_eosys = NORMALRETURN;
	XPR(XPR_SYSCALLS, ("syscall start: %c%c%c%c%c%c\n",
			   u_comm[0], u_comm[1], u_comm[2],
			   u_comm[3], u_comm[4], u_comm[5]));
	XPR(XPR_SYSCALLS,
	    ("syscall start %d: args=0x%x, 0x%x, 0x%x, 0x%x\n",
	     code, u_arg[0], u_arg[1], u_arg[2],
	     u_arg[3], u_arg[4]));
#if	MACH_RFS
	/*
	 *  Remember the system call we are executing so that it
	 *  can be handled remotely if need be.
	 */
	u_rfsncnt = 0;
	u_rfscode = code;
#endif	MACH_RFS
#if	VICE
	u_rmt_ncnt = 0;
	u_rmt_code = code;
#endif	VICE
	/*
	 *  Show no resource pause conditions.
	 */
	u_rpswhich &= URPW_NOTIFY;
	u_rpsfs = 0;
	(*(callp->sy_call))();
    }

#if	MACH_RFS
    u_rfscode = 0;
#endif	MACH_RFS
#if	VICE
    u_rmt_code = 0;
#endif	VICE
#if	MACH_RFS
    /*
     * The special error number EREMOTE is used by the remote
     * system call facility to short-circuit standard system call
     * processing when the equivalent has already been done remotely.
     * It serves simply to unwind the call stack back to this point
     * when the call has actually been completed successfully.
     * It is not an error and should not be relected back to the
     * user process.
     *
     * Also, clear the system call code value to indicate that we are
     * no longer in a system call.  I don't think this is actually
     * necessary since any calls on namei() will probably have to
     * have come through a system call and remote processing must
     * have been specifically enabled by the caller of namei().
     * Nevertheless, namei() will still check this value before
     * actually making a remote call and it nevers hurts to be safe.
     */
    if (u_error == ERFSREMOTE)
	u_error = 0;
    u_rfscode = 0;
#endif	MACH_RFS
#if	VICE
    if (u_error == EVICEOP)
	u_error = 0;
    u_rmt_code = 0;
#endif	VICE
    if (u_error)
	switch (u_error) {
	  case EDQUOT:		/* temp == ENOSPC */
	  case ENOSPC:
	    {
		/*
		 *  The error number ENOSPC indicates disk block or inode
		 *  exhaustion on a file system.  When this occurs during a
		 *  system call, the fsfull() routine will record the file
		 *  system pointer and type of failure (1=disk block, 2=inode)
		 *  in the U area.  If we return from a system call with this
		 *  error set, invoke the fspause() routine to determine
		 *  whether or not to enter a resource pause.  It will check
		 *  that the resource pause fields have been set in the U area
		 *  (then clearing them) and that the process has enabled such
		 *  waits before clearing the error number and pausing.  If a
		 *  signal occurs during the sleep, we will return with a false
		 *  value and the error number set back to ENOSPC.  If the wait
		 *  completes successfully, we return here with a true value.
		 *  In this case, we simply restart the current system call to
		 *  retry the operation.
		 *
		 *  Note:  Certain system calls can not be restarted this
		 *  easily since they may partially complete before running
		 *  into a resource problem.  At the moment, the read() and
		 *  write() calls and their variants have this characteristic
		 *  when performing multiple block operations.  Thus, the
		 *  resource exhaustion processing for these calls must be
		 *  handled directly within the system calls themselves.  When
		 *  they return to this point (even with ENOSPC set), the
		 *  resource pause fields in the U area will have been cleared
		 *  by their previous calls on fspause() and no action will be
		 *  taken here.
		 */
		if (fspause(0))
		    u_eosys = RESTARTSYS;
		break;
	    }
	    /*
	     *  TODO:  Handle these error numbers, also.
	     */
	  case EAGAIN:
	  case ENOMEM:
	  case ENFILE:
	    break;
	}
    
    if (u_eosys == NORMALRETURN) {
	regs->r_sp += sizeof (long);	/* pop syscall # */
	if (u_error) {
bad:
	    regs->r_r0 = u_error;
	    frame->f_normal.f_sr |= SR_CC;	/* carry bit */
	} else {
	    frame->f_normal.f_sr &= ~SR_CC;
	    regs->r_r0 = u_r.r_val1;
	    regs->r_r1 = u_r.r_val2;
	}
    }
    else if (u_eosys == RESTARTSYS)
	frame->f_normal.f_pc -= 2;	/* re-execute TRAP #0 */

out:
    if (pcb->pcb_ast & TRACE_PENDING)
	dotrace();

    XPR(XPR_SYSCALLS,
	("syscall end: pid=%d, code=%d, dreg0=%d, dreg1=%d\n",
	 u_procp->p_pid, code, regs->r_dreg[0], regs->r_dreg[1]));

    if (CHECK_SIGNALS(p, current_thread(), uthread)) {
	if ((pcb->pcb_flags&RET_SPECIAL) == 0) {
	    if (p->p_cursig || issig())
		psig();
	}
	else
	    pcb->pcb_ast |= AST_SCHED;
    }
    
    if (csw_needed(current_thread(), current_processor())) {
	u.u_ru.ru_nivcsw++;
	thread_block();
    }
#if	MACOS_EMUL
    check_mac_interrupts(pcb);
#endif

#ifdef	notyet
    if (syst_flag) {
	int ticks;
	struct timeval *tv = &u_ru.ru_stime;
	
	ticks = ((tv->tv_sec - syst.tv_sec) * 1000 +
		 (tv->tv_usec - syst.tv_usec) / 1000) / (tick / 1000);
	if (ticks)
	    addupc(regs.r_pc, &u_prof, ticks);
    }
#endif
}

/*
 *      This handles a machsyscall (one whose number is less than the cmu
 *      extra syscalls in this implementation).
 */
do_mach_syscall(info, locregs)
register info;
regs_t *locregs;
{
    int syscall_no, nargs, retval;
    register mach_trap_t *trap_entry;
    
    /*
     *	Index into table is positive
     */
    syscall_no = -info;
    trap_entry = &mach_trap_table[syscall_no];
    
    /*
     *	Get the number of arguments from the table, and
     *	copy them into u.u_arg.  Undo the VAX-specific
     *	hackery.
     */
    nargs = (trap_entry->mach_trap_length - 1);
    if (nargs) {
	if (copyin((caddr_t)locregs->r_sp + 2 * NBPW,
		   (caddr_t)u.u_arg, nargs)) {
	    u.u_error = EFAULT;
	    return(-1);
	}
    }
    /*
     *	Call as a normal function.... since C can't
     *	push a variable number of arguments on the stack,
     *	we have to push all possible arguments (7 at the
     *	moment).
     */
    /*
     *	XXX - this doesn't deal with routines that want
     *	to be exited if a signal happens... but at the
     *	moment there aren't any! (see syscall_sw.c)
     */
    retval = (*(trap_entry->mach_trap_function))(u.u_arg[0], u.u_arg[1],
						 u.u_arg[2], u.u_arg[3],
						 u.u_arg[4], u.u_arg[5],
						 u.u_arg[6]);
    
    return (retval);
}

/*
 * Indirect system call.
 * Used to be handled above, in syscall, but then everyone
 * was paying a performance penalty for this rarely-used
 * (and questionable) feature.
 */
indir()
{
    register int code, i;
    register struct sysent *callp;
    
    code = u.u_arg[0];
    /*
     *	CMU system calls have negative numbers
     */
    if (code < 0) {
	if (code < -ncmusysent)
	    callp = &sysent[63];	/* undefined */
	else
	    callp = &cmusysent[code + ncmusysent];
    } else
	callp = (code == 0 || code >= nsysent) ? &sysent[63]: &sysent[code];
    if (i = callp->sy_narg) {
	if (copyin((caddr_t)u.u_ar0[SP] + 3*NBPW,
		   (caddr_t)u.u_arg, i*sizeof(int))) {
	    u.u_error = EFAULT;
	    return;
	}
    }
    (*(callp->sy_call))(u.u_ap);
}

/*
 * nonexistent system call-- signal process (may want to handle it)
 * flag error if process won't see signal immediately
 * Q: should we do that all the time ??
 */
nosys()
{
    if (u.u_signal[SIGSYS] == SIG_IGN || u.u_signal[SIGSYS] == SIG_HOLD)
	u.u_error = EINVAL;
    thread_doexception(current_thread(),
		       EXC_SOFTWARE, EXC_UNIX_BAD_SYSCALL, 0);
}

/*
 *	Set up pointers to simulate system call trap and load initial
 *	program.
 */
mac2_load_init_program(frame)
register generic_exception_frame_t *frame;
{
    register struct pcb *pcb = current_thread_pcb();

    pcb->pcb_frame = frame;
    u.u_ar0 = &pcb->pcb_user.r_r0;
    load_init_program();
}

/*
 * Handle trace traps, both real and delayed.
 */
dotrace()
{
    register int r, s;
    register struct pcb *pcb = current_thread_pcb();
    register generic_exception_frame_t *frame;
#ifdef notyet
    struct proc *p = u.u_procp;
#endif

    frame = pcb->pcb_frame;
    s = spl6();
    r = pcb->pcb_ast;
    pcb->pcb_ast = 0;
    (void) splx(s);

    if (r & TRACE_AST) {
	while (thread_should_halt(current_thread()))
	    thread_halt_self();
#ifdef	notyet
	if ((p->p_flag&SOWEUPC) && u.u_prof.pr_scale) {
	    addupc(frame->f_special.f_pc, &u.u_prof, 1);
	    p->p_flag &= ~SOWEUPC;
	}
#endif
    }
    if (r & TRACE_USER) {
#if	MACOS_EMUL
	if ((pcb->pcb_flags&MAC_EMULATION) &&
	    macdebug == 0) {
	    if (frame->f_special.f_vector == EXC_MAC2_TRACE) {
		if (take_exception(frame, &pcb->pcb_user))
		    return;
	    }
	    else
		return;
	}
#endif
	thread_doexception(current_thread(),
			   EXC_BREAKPOINT, EXC_MAC2_TRACE, 0);
	frame->f_special.f_sr &= ~SR_TRACE;
    }
#if	MACOS_EMUL
    else if (pcb->pcb_flags&MAC_EMULATION)
	perform_mac_interrupts();
#endif

}

/*
 * Dump out a stack trace.  Assumes that
 * the bottom of stack is at the end of the
 * same page where the top of stack resides.
 */
dumpstack(fp, sp)
register struct call_frame *fp;
unsigned long sp;
{
    register vm_offset_t stack_page = trunc_page(sp);
    register i;
    static boolean_t	already_done = FALSE;

    if (already_done == TRUE)
	return;

    already_done = TRUE;

    printf("Begin traceback...fp = %x, sp = %x\n", fp, sp);
    while (trunc_page(fp) == stack_page) {
	if (fp == fp->f_fp) {
	    printf("FP loop at %x\n", fp);
	    break;
	}
	printf("Called from %x, fp %x, args", fp->f_pc, fp->f_fp);
	for (i = 0; i <= 5; i++) {
	    if (trunc_page(&fp->f_param[i]) == stack_page)
		printf(" %x", fp->f_param[i]);
	    else
		break;
	}
	printf("\n");

	fp = fp->f_fp;
    }
    printf("End traceback...\n");
}

/*
 * Dump out diagnostic info
 * following a trap.
 */
dumptrap(title, frame, regs)
char *title;
register generic_exception_frame_t *frame;
register regs_t *regs;
{
    register s, *r;
    
    s = spl7();
    printf("%s: %s\n", current_thread() ? u.u_comm : "no thread",
	   title ? title : "");
    printf("trap vector 0x%x, pid %d, pc = %x, sr = %x, frame fmt %x\n",
	   frame->f_normal.f_vector, current_thread() ? u.u_procp->p_pid : 0,
	   frame->f_normal.f_pc, frame->f_normal.f_sr, frame->f_normal.f_fmt);
    switch (frame->f_normal.f_fmt) {
      case STKFMT_SHORT_BUSERR:
#define short_buserr_frame (&frame->f_short_buserr)
	printf("dfault %d rw %d size %d fcode %d faultc %x faultb %x\n",
	       short_buserr_frame->f_dfault, short_buserr_frame->f_rw,
	       short_buserr_frame->f_size, short_buserr_frame->f_fcode,
	       short_buserr_frame->f_faultc, short_buserr_frame->f_faultb);
	if (short_buserr_frame->f_dfault)
	    printf("data fault address %x\n", short_buserr_frame->f_fault);

	if (short_buserr_frame->f_faultc)
	    printf("text fault address %x\n", short_buserr_frame->f_pc+2);

	if (short_buserr_frame->f_faultb)
	    printf("text fault address %x\n", short_buserr_frame->f_pc+4);

#undef short_buserr_frame
	break;
	
      case STKFMT_LONG_BUSERR:
#define long_buserr_frame (&frame->f_long_buserr)
	printf("dfault %d rw %d size %d fcode %d faultc %x faultb %x\n",
	       long_buserr_frame->f_dfault, long_buserr_frame->f_rw,
	       long_buserr_frame->f_size, long_buserr_frame->f_fcode,
	       long_buserr_frame->f_faultc, long_buserr_frame->f_faultb);
	if (long_buserr_frame->f_dfault)
	    printf("data fault address %x\n", long_buserr_frame->f_fault);

	if (long_buserr_frame->f_faultc)
	    printf("text fault address %x\n", long_buserr_frame->f_stageb-2);

	if (long_buserr_frame->f_faultb)
	    printf("text fault address %x\n", long_buserr_frame->f_stageb);

#undef long_buserr_frame
	break;
	
    }
    r = &regs->r_dreg[0];
    printf("D0-D7  %x %x %x %x %x %x %x %x\n",
	   r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
    r = &regs->r_areg[0];
    printf("A0-A7  %x %x %x %x %x %x %x %x\n",
	   r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]);
    (void) splx(s);
}
