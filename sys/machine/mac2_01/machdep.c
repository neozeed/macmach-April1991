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
 * $Log:	machdep.c,v $
 * Revision 2.2.1.1  90/09/07  00:56:05  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/08/30  11:01:55  bohman
 * 	Created.
 * 	[90/08/29  11:39:37  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/machdep.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <cpus.h>
#include <confdep.h>
#include <cputypes.h>

#include <mach_rfs.h>
#include <mach_load.h>
#include <quota.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)machdep.c	7.1 (Berkeley) 6/5/86
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/kernel.h>
#include <sys/map.h>
#include <sys/vm.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/reboot.h>
#include <sys/conf.h>
#include <sys/inode.h>
#include <sys/file.h>
#include <sys/clist.h>
#include <sys/callout.h>
#include <sys/cmap.h>
#include <sys/mbuf.h>
#include <sys/msgbuf.h>
#include <sys/quota.h>
#include <sys/mount.h>
#include <sys/fs.h>
#include <sys/socket.h>

#include <kern/thread.h>
#include <kern/lock.h>
#include <kern/zalloc.h>

#include <vm/vm_kern.h>

#include <mach/machine.h>
#include <mach/vm_param.h>

#include <net/if.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <machine/psl.h>
#include <machine/reg.h>
#include <machine/vmparam.h>

#if	MACH_RFS
#include <rfs/rfs.h>
#endif	MACH_RFS

/*
 * Declare these as initialized data so we can patch them.
 */
#ifdef	NBUF
int	nbuf = NBUF;
#else
int	nbuf = 0;
#endif
#ifdef	BUFPAGES
int	bufpages = BUFPAGES;
#else
int	bufpages = 0;
#endif

int	msgbuf_mapped = FALSE;
vm_map_t	buffer_map;
/*
 * Machine-dependent startup code
 */
startup(firstaddr)
caddr_t	firstaddr;
{
    register unsigned int i;
    register caddr_t v;
    vm_size_t	size;
    kern_return_t	ret;
    vm_offset_t	trash_offset;
    int		base, residual;
    vm_offset_t	tmp_addr;
    extern is68030;

    /*
     * Configure the CPU
     */
    machine_slot[0].is_cpu = TRUE;
    if (is68030) {
	machine_slot[0].cpu_type = CPU_TYPE_MC68030;
	machine_slot[0].cpu_subtype = 0;
    }
    else {
	machine_slot[0].cpu_type = CPU_TYPE_MC68020;
	machine_slot[0].cpu_subtype = 0;
    }
    machine_slot[0].clock_freq = hz;

    /*
     * Configure all I/O
     * (non-slot) devices.
     */
    config_devs();

    /*
     * Initialize the slot manager.
     */
    config_slotmgr();

    /*
     * Find the console screen and
     * initialize it enough so that
     * kernel printfs work.
     */
    cons_printf_init();

    /*
     * Set rootdev.
     */
    setconf();

    /*
     * Good {morning,afternoon,evening,night}.
     */
    panic_init();
    printf(version);
#define MEG	(1024*1024)
    printf("physical memory = %d.%d%d megabytes.\n", mem_size/MEG,
	   ((mem_size%MEG)*10)/MEG,
	   ((mem_size%(MEG/10))*100)/MEG);
    
    /*
     * Allocate space for system data structures.
     * The first available real memory address is in "firstaddr".
     * The first available kernel virtual address is in "v".
     * As pages of kernel virtual memory are allocated, "v" is incremented.
     * As pages of memory are allocated and cleared,
     * "firstaddr" is incremented.
     * An index into the kernel page table corresponding to the
     * virtual memory address maintained in "v" is kept in "mapaddr".
     */
    /*
     *	Since the virtual memory system has already been set up,
     *	we cannot bypass it to allocate memory as the old code
     *	does.  We therefore make two passes over the table
     *	allocation code.  The first pass merely calculates the
     *	size needed for the various data structures.  The
     *	second pass allocates the memory and then sets the
     *	actual addresses.  The code must not change any of
     *	the allocated sizes between the two passes.
     */
    firstaddr = 0;
    for (;;) {
	v = firstaddr;	    
#define	valloc(name, type, num) \
	(name) = (type *)v; v = (caddr_t)((name)+(num))
#define	valloclim(name, type, num, lim) \
	    (name) = (type *)v; v = (caddr_t)((lim) = ((name)+(num)))
#if	MACH_RFS
		valloc(rfsProcessTable, struct rfsProcessEntry, nproc);
#endif	MACH_RFS
	valloclim(inode, struct inode, ninode, inodeNINODE);
	valloclim(file, struct file, nfile, fileNFILE);
	valloclim(proc, struct proc, nproc, procNPROC);
	valloc(cfree, struct cblock, nclist);
	valloc(callout, struct callout, ncallout);
#if	MACH_DIRECTORY
#else	MACH_DIRECTORY
	valloc(namecache, struct namecache, nchsize);
#endif	MACH_DIRECTORY
#if	QUOTA
	valloclim(quota, struct quota, nquota, quotaNQUOTA);
	valloclim(dquot, struct dquot, ndquot, dquotNDQUOT);
#endif	QUOTA
	{
	    extern struct arptab *arptab;
	    extern int arptab_size, arptab_bsiz, arptab_nb;

	    arptab_size = arptab_bsiz * arptab_nb;
	    valloc(arptab, struct arptab, arptab_size);
	}
	
	/* 
	 *	Use 5% of memory for buffers, regardless.
	 *	Since these pages are virtual-size pages (larger
	 *	than physical page size), use only one page
	 *	per buffer.
	 */
 	if (bufpages == 0)
	    bufpages = atop(mem_size / 20);
	if (nbuf == 0) {
	    if ((nbuf = bufpages) < 16)
		nbuf = 16;
	}
	if (bufpages > nbuf * (MAXBSIZE / page_size))
	    bufpages = nbuf * (MAXBSIZE / page_size);
	valloc(buf, struct buf, nbuf);
	
	/*
	 * Clear space allocated thus far, and make r/w entries
	 * for the space in the kernel map.
	 */
	if (firstaddr == 0) {
	    /*
	     *	Size has been calculated; allocate memory.
	     */
	    size = (vm_size_t)(v - firstaddr);
	    tmp_addr = kmem_alloc(kernel_map, round_page(size));
	    if (tmp_addr == 0)
		panic("startup: no room for tables");
	    firstaddr = (caddr_t)tmp_addr;
	}
	else {
	    /*
	     *	Memory has been allocated.  Make sure that
	     *	table size has not changed.
	     */
	    if ((vm_size_t)(v - firstaddr) != size)
		panic("startup: table size inconsistent");
	    break;
	}
    }
    
    /*
     * Now allocate buffers proper.  They are different than the above
     * in that they usually occupy more virtual memory than physical.
     */
    v = 0;	/* use the same trick */
    valloc(buffers, char, MAXBSIZE * nbuf);
    base = bufpages / nbuf;
    residual = bufpages % nbuf;
    
    /*
     *	Allocate virtual memory for buffer pool.
     */
    size = round_page((vm_size_t)v);
    buffer_map = kmem_suballoc(kernel_map,
			       &tmp_addr,
			       &trash_offset /* max */,
			       size,
			       TRUE);
    buffers = (char *)tmp_addr;
    ret = vm_map_find(buffer_map, vm_object_allocate(size), (vm_offset_t) 0,
		      &tmp_addr, size, FALSE);
    
    if (ret != KERN_SUCCESS)
	panic("startup: unable to allocate buffer pool");
    
    for (i = 0; i < nbuf; i++) {
	vm_size_t	thisbsize;
	vm_offset_t	curbuf;
	
	/*
	 * First <residual> buffers get (base+1) physical pages
	 * allocated for them.  The rest get (base) physical pages.
	 *
	 * The rest of each buffer occupies virtual space,
	 * but has no physical memory allocated for it.
	 */
	
	thisbsize = page_size*(i < residual ? base+1 : base);
	curbuf = (vm_offset_t)buffers + i * MAXBSIZE;
	vm_map_pageable(buffer_map, curbuf, curbuf+thisbsize,
			VM_PROT_READ|VM_PROT_WRITE);
    }
    
    
    /*
     * Initialize callouts
     */
    callfree = callout;
    for (i = 1; i < ncallout; i++)
	callout[i-1].c_next = &callout[i];
    
    /*
     * Initialize memory allocator and swap
     * and user page table maps.
     *
     * THE USER PAGE TABLE MAP IS CALLED ``kernelmap''
     * WHICH IS A VERY UNDESCRIPTIVE AND INCONSISTENT NAME.
     */
    {
	register int	nbytes;
	extern int	vm_page_free_count;
	
	nbytes = ptoa(vm_page_free_count);
	printf("available memory = %d.%d%d megabytes.\n", nbytes/MEG,
	       ((nbytes%MEG)*10)/MEG,
	       ((nbytes%(MEG/10))*100)/MEG);
	nbytes = ptoa(bufpages);
	printf("using %d buffers containing %d.%d%d megabytes of memory\n",
	       nbuf,
	       nbytes/MEG,
	       ((nbytes%MEG)*10)/MEG,
	       ((nbytes%(MEG/10))*100)/MEG);
    }
    mb_map = kmem_suballoc(kernel_map,
			   (vm_offset_t *) &mbutl,
			   (vm_offset_t *) &embutl,
			   (vm_size_t) (nmbclusters * CLBYTES),
			   FALSE);

    /*
     * Configure all slot
     * devices.
     */
    config_slots();
    
    /*
     * Now that configure is done, write protect
     * the interrrupt vector page in the kernel pmap.
     */
    pmap_protect(kernel_pmap,
		 (vm_offset_t)ivect_tbl,
		 (vm_offset_t)ivect_tbl+NBPG,
		 VM_PROT_READ);
}

/*
 * Clear registers on exec
 */
setregs(entry)
unsigned long entry;
{
    register struct pcb *pcb = current_thread_pcb();
    register struct regs *regs;
    register generic_exception_frame_t *frame;
    register int i;
    
    regs = &pcb->pcb_user;
    for (i = 0; i < 8; i++) {
	regs->r_dreg[i] = 0;
	if (&regs->r_areg[i] != &regs->r_sp)
	    regs->r_areg[i] = 0;
    }
    
    frame = pcb->pcb_frame;
    frame->f_normal.f_sr = PSL_USERSET;
    frame->f_normal.f_pc = entry;

    frame->f_normal.f_fmt = STKFMT_NORMAL;
    frame->f_normal.f_vector = 0;
    
    /*
     *	don't pop syscall number off stack-
     *	it's not there!
     */
    u.u_eosys = JUSTRETURN;	/* ucb's name for this */
    
    /*
     * Clear fpu state.
     */
    pcb->pcb_fp_frame.fpf_format = 0;
    fp_continue();
}

int sendsigdebug = 0;

/*
 * Send an interrupt to process.
 *
 * When using new signals user code must do a
 * sys #139 to return from the signal, which
 * calls sigcleanup below, which resets the
 * signal mask and the notion of onsigstack,
 * and returns from the signal handler.
 */
sendsig(p, sig, sigmsk)
int (*p)(), sig, sigmsk;
{
    register struct pcb *pcb = current_thread_pcb();
    register struct regs *regs;
    register generic_exception_frame_t *frame;
    register int usp, scp;
    struct {
	int	sig;
	int	code;
	int	scp;
    } sigframe;
    struct sigcontext sc;
    int scflags, frame_size;
    
    regs = &pcb->pcb_user;
    frame = pcb->pcb_frame;
    scflags = u.u_onstack;

    if (sendsigdebug)
	printf("sendsig: sig %d sigmsk %x fmt %x\n",
	       sig, sigmsk, frame->f_normal.f_fmt);
    
    if (!u.u_onstack && (u.u_sigonstack & sigmask(sig))) {
	usp = (int)u.u_sigsp;
	u.u_onstack = 1;
    } else
	usp = regs->r_sp;
    
    if (frame->f_normal.f_fmt != STKFMT_NORMAL) {
	frame_size = save_frame(usp);
	if (frame_size > 0) {
	    usp -= frame_size;
	    scflags |= SC_FRAMESAVE;
	}
    }
    
    usp -= sizeof (struct sigcontext);
    scp = usp;
    usp -= sizeof (sigframe);
    
    if (useracc((caddr_t)usp, sizeof (sigframe) + sizeof (sc), B_WRITE) == 0) {
	/*
	 * Process has trashed its stack; give it an illegal
	 * instruction to halt it in its tracks.
	 */
	printf("sendsig: bad user stack pid=%d, sig=%d\n",
	       u.u_procp->p_pid, sig);
	printf("usp is %x, action is %x, upc is %x\n", usp, p, frame->f_normal.f_pc);
	u.u_signal[SIGILL] = SIG_DFL;
	sig = sigmask(SIGILL);
	u.u_procp->p_sigignore &= ~sig;
	u.u_procp->p_sigcatch &= ~sig;
	u.u_procp->p_sigmask &= ~sig;
	psignal(u.u_procp, SIGILL);
	return;
    }
    
    /*
     * push sigcontext structure.
     */
    sc.sc_flags = scflags;
    sc.sc_mask = sigmsk;
    sc.sc_sp = regs->r_sp;
    sc.sc_pc = frame->f_normal.f_pc;
    sc.sc_ps = frame->f_normal.f_sr;
    (void) copyout((caddr_t)&sc, (caddr_t)scp, sizeof (sc));
    
    /*
     * push call frame.
     */
    sigframe.sig = sig;
    if (sig == SIGILL || sig == SIGFPE || sig == SIGEMT) {
	sigframe.code = u.u_code;
	u.u_code = 0;
    } else
	sigframe.code = 0;
    sigframe.scp = scp;
    (void) copyout((caddr_t)&sigframe, (caddr_t)usp, sizeof (sigframe));
    
    regs->r_sp = usp;
    if (frame->f_normal.f_fmt != STKFMT_NORMAL) {
	frame =&pcb->pcb_return_frame_data;
	pcb->pcb_return_frame_size = NORMAL_EXCEPTION_FRAME_SIZE;
	frame->f_normal.f_fmt = STKFMT_NORMAL;
	frame->f_normal.f_vector = 0;
	pcb->pcb_frame = frame;
	pcb->pcb_flags |= RET_FRAME;
    }
    pcb->pcb_ast &= ~TRACE_USER;
    frame->f_normal.f_sr = 0;
    frame->f_normal.f_pc = (unsigned long)p;
}

/*
 * Routine to cleanup state after a signal
 * has been taken.  Reset signal mask and
 * notion of on signal stack from context
 * left there by sendsig (above).  Pop these
 * values and return.
 */
sigreturn()
{
    osigcleanup();
}

osigcleanup()
{
    register struct pcb *pcb = current_thread_pcb();
    register struct regs *regs = &pcb->pcb_user;
    register generic_exception_frame_t *frame;
    struct sigcontext *scp, sc;
    
    scp = (struct sigcontext *)fuword((caddr_t)regs->r_sp + sizeof(long));
    if ((int)scp == -1)
	return;
    if (copyin((caddr_t)scp, (caddr_t)&sc, sizeof (sc)))
	return;
    
    if (sc.sc_flags & SC_FRAMESAVE)
	restore_frame((int)(scp+1));

    frame = pcb->pcb_frame;

    if ((pcb->pcb_flags&RET_SPECIAL) == 0) {
	frame->f_normal.f_pc = sc.sc_pc;
	frame->f_normal.f_sr = sc.sc_ps;
    }
    frame->f_normal.f_sr &= ~(SR_T0|SR_SUPR|SR_MASTER|SR_IPL);
    pcb->pcb_ast &= ~TRACE_USER;
    
    regs->r_sp = sc.sc_sp;
    
    u.u_onstack = sc.sc_flags & SC_ONSTACK;
    u.u_procp->p_sigmask =
	sc.sc_mask &~ (sigmask(SIGKILL)|sigmask(SIGCONT)|sigmask(SIGSTOP));
    
    u.u_eosys = JUSTRETURN;
}

/*
 * Save current stack frame
 * onto user stack.
 */
save_frame(usp)
register usp;
{
    register struct pcb *pcb = current_thread_pcb();
    register generic_exception_frame_t *frame;
    register frame_size;
    
    frame = pcb->pcb_frame;
    switch (frame->f_normal.f_fmt) {
      case STKFMT_SHORT_BUSERR:
	frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
	break;
	
      case STKFMT_LONG_BUSERR:
	frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
	break;
	
      default:
	frame_size = 0;
	break;
    }
    if (frame_size > 0) {
	usp -= frame_size;
	/*
	 * copy frame onto user stack
	 */
	if (copyout(frame, usp, frame_size))
	    frame_size = 0;
    }
    return (frame_size);
}

/*
 * Restore stack frame from user stack
 * for resumption.
 */
restore_frame(usp)
register generic_exception_frame_t *usp;
{
    register struct pcb *pcb = current_thread_pcb();
    register generic_exception_frame_t *frame;
    
    frame = &pcb->pcb_return_frame_data;
    if (copyin(usp, frame, NORMAL_EXCEPTION_FRAME_SIZE))
	return;
    
    switch (frame->f_normal.f_fmt) {
      case STKFMT_SHORT_BUSERR:
	if (copyin(&usp->f_short_buserr.f_internal,
		   &frame->f_short_buserr.f_internal,
		   (SHORT_BUSERR_EXCEPTION_FRAME_SIZE -
		    NORMAL_EXCEPTION_FRAME_SIZE)))
	    return;
	pcb->pcb_return_frame_size = SHORT_BUSERR_EXCEPTION_FRAME_SIZE;
	pcb->pcb_frame = frame;
	pcb->pcb_flags |= (RET_SPECIAL|RET_FRAME);
	break;
	
      case STKFMT_LONG_BUSERR:
	if (copyin(&usp->f_long_buserr.f_internal,
		   &frame->f_long_buserr.f_internal,
		   (LONG_BUSERR_EXCEPTION_FRAME_SIZE -
		    NORMAL_EXCEPTION_FRAME_SIZE)))
	    return;
	pcb->pcb_return_frame_size = LONG_BUSERR_EXCEPTION_FRAME_SIZE;
	pcb->pcb_frame = frame;
	pcb->pcb_flags |= (RET_SPECIAL|RET_FRAME);
	break;
	
      default:
	break;
    }
}

int	waittime = -1;

boot(paniced, arghowto)
int paniced, arghowto;
{
    register int howto;
    register int devtype;
#if	MACH_RFS
    extern void	rfs_boot();
#endif	MACH_RFS
    
#if	MACH_RFS
    rfs_boot(paniced, arghowto);
#endif	MACH_RFS
    howto = arghowto;
    if (paniced != RB_PANIC &&
	(howto&RB_NOSYNC) == 0 && waittime < 0 && bfreelist[0].b_forw) {
	/*
	 *  Force an accurate time into the root file system super-
	 *  block.
	 */
	mount[0].m_bufp->b_un.b_fs->fs_fmod = 1;
	waittime = 0;
	(void) splnet();
	printf("syncing disks... ");
	/*
	 * Release inodes held by texts before update.
	 */
	update();
	{ register struct buf *bp;
	  int iter, nbusy;
	  int obusy = 0;
	  
	  for (iter = 0; iter < 20; iter++) {
	      nbusy = 0;
	      for (bp = &buf[nbuf]; --bp >= buf; )
		  if ((bp->b_flags & (B_BUSY|B_INVAL)) == B_BUSY)
		      nbusy++;
	      if (nbusy == 0)
		  break;
	      printf("%d ", nbusy);
	      if (nbusy != obusy)
		  iter = 0;
	      obusy = nbusy;
	      DELAY(40000 * iter);
	  }
	}
	printf("done\n");
	/*
	 * If we've been adjusting the clock, the todr
	 * will be out of synch; adjust it now.
	 */
	resettodr();
    }
    spl7();			/* extreme priority */
    devtype = major(rootdev);
    if (howto&RB_HALT) {
	halt_cpu();
    } else {
	if (paniced == RB_PANIC) {
	    asm("movl   sp,sp@-");
	    asm("movl   a6,sp@-");
	    asm("jsr    _dumpstack");
	    halt_cpu();
	}
	restart_cpu();
    }
}

halt_cpu()
{
    machine_slot[cpu_number()].running = FALSE;

    printf("cpu halted\n");
    halt_inst();
}

restart_cpu()
{
    boot_inst();
}

/*
 * Here to satisfy external
 * reference (never called).
 */
slave_config()
{
}

/*
 * Return the best possible estimate of the time in the timeval
 * to which tvp points.  We do this by reading the interval count
 * register to determine the time remaining to the next clock tick.
 * We must compensate for wraparound which is not yet reflected in the time
 * (which happens when the ICR hits 0 and wraps after the splhigh(),
 * but before the mfpr(ICR)).  Also check that this time is no less than
 * any previously-reported time, which could happen around the time
 * of a clock adjustment.  Just for fun, we guarantee that the time
 * will be greater than the value obtained by a previous call.
 * NOTE:  Sun doesn't have interval count register.
 */
microtime(tvp)
register struct timeval *tvp;
{
    int s = splhigh();
    static struct timeval lasttime;
    register long t;
    
    *tvp = time;
    if (tvp->tv_sec == lasttime.tv_sec &&
	tvp->tv_usec <= lasttime.tv_usec &&
	(tvp->tv_usec = lasttime.tv_usec + 1) > 1000000) {
	tvp->tv_sec++;
	tvp->tv_usec -= 1000000;
	}
    lasttime = *tvp;
    splx(s);
}

/*
 * Perform raw disk I/O
 */
physstrat(bp, strat, prio)
register struct buf *bp;
int (*strat)(), prio;
{
    register vm_offset_t buffer, page, iobuffer;
    register vm_size_t size;
    register kern_return_t result;
    vm_offset_t iopage;
    int s;

    /*
     * Save get user buffer address and
     * page address; calculate rounded
     * size.
     */
    buffer = (vm_offset_t)bp->b_un.b_addr;
    page = trunc_page(buffer);
    size = round_page(buffer+bp->b_bcount) - page;

    /*
     * Allocate some virtual address
     * space in the kernel_map.
     */
    result = vm_allocate(kernel_map, &iopage, size, TRUE);

    if (result != KERN_SUCCESS) {
	bp->b_flags |= B_ERROR;
	bp->b_error = 0;
	return;
    }

    /*
     * Make iobuffer point at
     * the buffer, not the base
     * page.
     */
    iobuffer = iopage + (buffer - page);

    /*
     * Map the page(s) in the
     * task map into the space
     * allocated in the kernel_map.
     */
#define map_wired_memory_to_kernel_map(map, va, ka, size)	\
    {								\
	register pmap_t _pmap = vm_map_pmap(map);		\
	register vm_offset_t _va, _ka;				\
	register _npg;						\
\
	_va = (va);						\
	_ka = (ka);						\
	_npg = mac2_btop(size);					\
	while (_npg-- > 0) {					\
	    pmap_enter(kernel_pmap,				\
		       _ka,					\
		       pmap_extract(_pmap, _va),		\
		       VM_PROT_READ|VM_PROT_WRITE,		\
		       TRUE);					\
	    _va += MAC2_PGBYTES;				\
	    _ka += MAC2_PGBYTES;				\
	}							\
    }
    map_wired_memory_to_kernel_map(bp->b_proc->task->map, page, iopage, size);

    bp->b_un.b_addr = (caddr_t)iobuffer;

    (*strat)(bp);

    s = splbio();
    while ((bp->b_flags & B_DONE) == 0)
	sleep((caddr_t)bp, prio);
    splx(s);

    bp->b_un.b_addr = (caddr_t)buffer;

    /*
     * Clean up by removing
     * the virtual address
     * space from the kernel_map.
     */
    vm_deallocate(kernel_map, iopage, size);
}
