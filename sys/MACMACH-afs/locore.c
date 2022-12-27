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
 * $Log:	locore.s,v $
 * Revision 2.2  90/09/04  17:21:14  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/locore.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <macos_emul.h>

#include <sys/errno.h>

#include <mach/mac2/exception.h>

#include <mac2/asm.h>
#include <mac2/vmparam.h>
#include <mac2/cpu.h>
#include <mac2/pcb.h>
#include <mac2/psl.h>
#include <mac2/MacGlobals.h>
#include <mac2/trap.h>

#include <cputypes.h>

#include <assym.s>

/*
 * locore for MACMACH
 */

	.data

/*
 * The interrupt stack.
 */
	.globl	_intstack, eintstack
_intstack:
	.space	(INTSTACK_SIZE+MAC2_PGBYTES)

eintstack:

/*
 * Some globals.
 */
	.globl	_mem_size
	.globl	_is68030
	.globl	_macMMUMode
_mem_size:	.long	0
_is68030:	.long	0
_macMMUMode:	.long	0

	.globl	_bootdev
	.globl	_boothowto
_bootdev:	.long	0
_boothowto:	.long	0

poweroff_ok:	.long	0

/*
 * Macro used for entering
 * the trap handler.
 */
#define TRAP(type)	\
	.globl	__##type;	\
__##type: ;	\
	btst	\#SR_SUPR_BIT,sp@;	\
	bne	1f;	\
	movl	a0,sp@-;	\
	movc	msp,a0;	\
	moveml	d0-d7/a0-a6,a0@(PCB_REGS);	\
	movl	sp@+,a0@(PCB_A0);	\
	movl	usp,a1;	\
	movl	a1,a0@(PCB_SP);	\
	pea	sp@;	\
	movl	\#TRAP_##type,sp@-;	\
	jsr	_user_trap;	\
	addqw	\#8,sp;	\
	jmp	rte_user;	\
\
1:	moveml	d0-d7/a0-a7,sp@-;	\
	lea	sp@(R_SP+4),a0;	\
	pea	sp@;	\
	movl	a0,sp@-;	\
	movl	\#TRAP_##type,sp@-;	\
	jsr	_kernel_trap;	\
	addw	\#12,sp;	\
	jmp	rte_kernel

	.globl	_start
	.text
/*
 * Program entry point
 */
_start:	clrl	d0
	movl	d0,a0
	movl	#_start,a0@

/*
 * We check to see if
 * the kernel is running at
 * the correct location.  If
 * not, we use bcopy to copy
 * ourselves to the right place.
 */
	lea	pc@(2),a0
loc:	cmpl	#loc,a0
	beq	0f
	movl	a0,d0
	movl	#_edata,a0
	subl	#_start,a0
	movl	a0,sp@-
	movl	#_start,sp@-
	subl	#loc-_start,d0
	movl	d0,sp@-
	bsr	_bcopy
	lea	sp@(12),sp
	jmp	_start:l
0:

/*
 * OK.  Now we are running in
 * the right place.  Fetch bootdev
 * and boothowto from the stack and
 * store in variables.
 */
	lea	sp@(4),sp	    | pop return addr
	movl	sp@+,_bootdev:l
	movl	sp@+,_boothowto:l

/*
 * Reinitialize the application heap
 * to end before the kernel.
 */
	lea	_start,sp
	.word	0xa02c			| InitApplZone

/*
 * Switch to 32 bit mode
 */
	moveq	#1,d0
	.word	0xa05d			| SwapMMUMode
	movl	d0,_macMMUMode
	
	movw	#SR_HIGH,sr		| mask interrupts
/*
 * Switch to interrupt stack
 */
	movl	#eintstack,sp

/*
 * Turn off PMMU/Reset FPP
 */
	clrl	sp@-
 	pmove	sp@,TC
	addqw	#4,sp
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	clrl	sp@-
	frestore	sp@+
0:

/*
 * Clear CPU cache(s)/Test for 68030
 */
	movl	#CPU_CACHE_CLR_ENAB,d0
	movc	d0,cacr
	movc	cacr,d0
	andw	#CPU_DCACHE_ENABLE,d0
	beq	0f
	movl	#1,_is68030
0:

/*
 * Zero BSS segment
 */
	lea	_end,a0
	subl	#_edata,a0
	movl	a0,sp@-
	pea	_edata
	jsr	_bzero

/*
 * Calculate available physical
 * memory and store it.
 */
/*
 * avail_start is end of kernel bss
 * rounded to next page.
 */
	movl	#MAC2_PGBYTES-1,d1
	movl	#_end,d0
	addl	d1,d0
	notl	d1
	andl	d1,d0
	movl	d0,_avail_start

/*
 * avail_end is the addr of the last
 * available memory page.
 */
	movl	G_BufPtr,d0
	subql	#1,d0
	andl	d1,d0
	movl	d0,_avail_end

/*
 * mem_size is the total amount of
 * physical memory in the system.
 * memory is defined to start at 0.
 */
	movl	G_MemTop,_mem_size

/*
 * HACK HACK XXX
 *
 * Some MacOS globals must
 * be changed so that the
 * the macintosh video drivers
 * work correctly.  One good
 * hack deserves another.
 */
	movl	#ret_inst,G_JVBLTask
	bset	#G_hwCbAUX,G_HWCfgFlags
	movl	#ret_inst,G_JIODone

	jsr	_mac2_init
	jsr	_setup_main
/*
 * Switch to kernel stack
 */
	movl	d0,a0			| regain thread
	movl	a0@(THREAD_PCB),a1	| thread->pcb
	movl	a1@(PCB_K_SP),sp	| set up kernel stack
	movc	a1,msp			| save PCB address of thread
	jsr	_main			| simulate interrupt -> main
/*
 *	call to main never returns.  Instead, process 1 starts up here
 *	to load /etc/init over itself.
 */
	.globl	_mac2_load_init_program

ENTRY(start_init)
	movl	#USRSTACK,a0		| init user stack pointer
	movl	a0,usp
	movc	msp,a0			| get pointer to PCB of thread
	lea	a0@(KERNEL_STACK_SIZE), sp | the ksp base 
	clrw	sp@-			| dummy fmt & vor
	movl	#USRTEXT,sp@-		| pc (will be changed)
	movw	#SR_USER,sp@-		| sr
	lea	0,a6			| stack frame link 0
	pea	sp@
	jsr	_mac2_load_init_program	| load /etc/init
	addqw	#4,sp
	clrl	d0			| fake return value from trap
	jmp	rte_user

| device interrupt handlers
	.globl	_level1intr
_level1intr:
	jmp	_fdbhand

	.globl	_level2intr
_level2intr:
	moveml	d0-d1/a0-a1,sp@-
	movl	@(_VIA2_ADDR_PTR)@(0),a0
	btst	#5,a0@(VIA_IFR)		| check for softint
	beq	0f
	tstb	a0@(VIA_T2CL)		| and clear it
	btst	#7,a0@(VIA_IFR)		| test for other interrupts
	bne	0f
	jmp	rte_intr
0:
	link	a6,#0
	cmpl	#eintstack,sp
	ble	1f
	lea	eintstack,sp
1:
	pea	a6@(5*4)	    | push saved frame address
	jsr	_via2intr
	unlk	a6
	jmp	rte_intr

	.globl	_level4intr
_level4intr:
	moveml	d0-d1/a0-a1,sp@-
	link	a6,#0
	cmpl	#eintstack,sp
	ble	1f
	lea	eintstack,sp
1:
	pea	a6@(5*4)	    | push saved frame address
	jsr	_sccintr
	unlk	a6
	jmp	rte_intr

TRAP(BAD_ACCESS)

TRAP(TRACE)

#if	!MACOS_EMUL
TRAP(EMULATION_1010)
#endif

TRAP(EMULATION_1111)

TRAP(BAD_INSTRUCTION)

#if	!MACOS_EMUL
TRAP(PRIV_INSTRUCTION)
#endif

TRAP(BREAKPOINT)

TRAP(ARITHMETIC)

TRAP(SOFTWARE)

TRAP(ERROR)

/*
 * Reboot the processor
 */
	.globl	_boot_inst
_boot_inst:
	movw	#SR_HIGH,sr
	lea	eintstack,sp

/*
 * Make sure that the instruction
 * cache is enabled.  See below.
 */
	movl	#CPU_CACHE_CLR_ENAB,d0
	movc	d0,cacr

	clrl	sp@-
	movl	sp,a0
	pmove	a0@,TC
	addqw	#4,sp

/*
 * Jump to ROM entry point.
 *
 * This code depends on the
 * instruction cache being
 * enabled to work.
 */
	movl	0x40800000,sp
	lea	0f,a0
	bra	1f

0:
	movl	0x40800004,a0
	reset
1:
	jmp	a0@

/*
 * Halt the processor
 */
	.globl	_halt_inst
_halt_inst:
	movw	#SR_HIGH,sr
	movl	#1,poweroff_ok
	movw	#SR_SUPR+SR_IPL5,sr
	bra	.

/*
 * Power down the system
 */
	.globl	__poweroff
__poweroff:
	movw	#SR_HIGH,sr
	tstl	poweroff_ok
	bne	0f
	rte
0:
	movl	@(_VIA2_ADDR_PTR)@(0),a0
	bclr	#2,a0@(VIA_REGB)
	bset	#2,a0@(VIA_DDRB)
	bra	.

ret_inst:
	rts

/*
 * Routines to copy data to and from
 * user space.
 */

/*
 * Fetch user byte.
 */
ENTRY(fubyte)
ENTRY2(fuibyte)
	movl	#FC_UD,d0
        movc    d0,sfc		        | load source function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	moveq	#0,d0
	movsb	a0@,d0			| get the byte
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

/*
 * Fetch user short word.
 */
ENTRY(fusword)
	movl	#FC_UD,d0
        movc    d0,sfc		        | load source function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	moveq	#0,d0
	movsw	a0@,d0			| get the word
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

/*
 * Fetch user long word.
 */
ENTRY(fuword)
ENTRY2(fuiword)
	movl	#FC_UD,d0
        movc    d0,sfc		        | load source function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	movsl	a0@,d0			| get the long word
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

/*
 * Store user byte.
 */
ENTRY(subyte)
ENTRY2(suibyte)
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	movl	sp@(8),d0		| get the byte
	movsb	d0,a0@			| store it
	moveq	#0,d0			| return success
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

/*
 * Store user short word.
 */
ENTRY(susword)
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	movl	sp@(8),d0		| get the word
	movsw	d0,a0@			| store it
	moveq	#0,d0			| return success
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

/*
 * Store user long word.
 */
ENTRY(suword)
ENTRY2(suiword)
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a1	| get active thread
	movl	#fsuerr,a1@(THREAD_RECOVER)	| catch any faults
	movl	sp@(4),a0		| get address
	movl	sp@(8),d0		| get the word
	movsl	d0,a0@			| store it
	moveq	#0,d0			| return success
	clrl	a1@(THREAD_RECOVER)	| clear fault address
	rts

fsuerr:
	movl	#-1,d0			| return error
	clrl	a1@(THREAD_RECOVER)
	rts

/*
 * copyout(from, to, count)
 *
 * copy a block of data from kernel space
 * to user space.
 */
ENTRY(copyout)
	moveml	d2/a2,sp@-
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a2	| get active thread
	movl	#cperr,a2@(THREAD_RECOVER)	| catch any faults
	movl	sp@(12),a0		| get src address
	movl	sp@(16),a1		| get dst address
	movl	sp@(20),d1		| get count
	
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0
	
	/*
	 * check for count == 0
	 */
	beq	2f
	bra	1f
	
	/*
	 * loop for moving lower 16 bits
	 * worth of count
	 */
0:	movl	a0@+,d2			| fetch data
	movsl	d2,a1@+			| store it
1:	dbf	d0,0b
	
	/*
	 * check upper 16 bits
	 * of count for more to do
	 */
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b
	
	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movb	a0@+,d2			| fetch data
	movsb	d2,a1@+			| store it
1:	dbf	d1,0b

	moveq	#0,d0			| return success
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

/*
 * copyin(from, to, count)
 *
 * copy a block of data from user space
 * to kernel space.
 */
ENTRY(copyin)
	moveml	d2/a2,sp@-
	movl	#FC_UD,d0
	movc	d0,sfc			| load source function code (user xxx)
	movl	_active_threads,a2	| get active thread
	movl	#cperr,a2@(THREAD_RECOVER)	| catch any faults
	movl	sp@(12),a0		| get src address
	movl	sp@(16),a1		| get dst address
	movl	sp@(20),d1		| get count
	
	/*
	 * get count of long
	 * words into d0
	 */
	movl	d1,d0
	lsrl	#2,d0
	
	/*
	 * check for count == 0
	 */
	beq	2f
	bra	1f
	
	/*
	 * loop for moving lower 16 bits
	 * worth of count
	 */
0:	movsl	a0@+,d2			| fetch data
	movl	d2,a1@+			| store it
1:	dbf	d0,0b
	
	/*
	 * check upper 16 bits
	 * of count for more to do
	 */
	swap	d0
	tstw	d0
	beq	2f
	subqw	#1,d0
	swap	d0
	bra	0b
	
	/*
	 * check for residue bytes
	 */
2:	andl	#3,d1
	bra	1f

	/*
	 * move residue
	 */
0:	movsb	a0@+,d2			| fetch data
	movb	d2,a1@+			| store it
1:	dbf	d1,0b

	moveq	#0,d0			| return success
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

cperr:
	movl	#EFAULT,d0		| return error
	clrl	a2@(THREAD_RECOVER)	| clear fault address
	moveml	sp@+,d2/a2
	rts

/*
 * Copy a null terminated string from one point to another in kernel virtual
 * address space.  Returns a nonzero error code if the last character copied
 * was not the null character.
 *
 *	copystr(from, to, maxlength, &lencopied)
 */
ENTRY(copystr)
	moveml	a2/d2,sp@-		| save registers
	movl	sp@(8+4),a0		| from
	movl	sp@(8+8),a1		| to
	movl	sp@(8+12),d1		| maxlen
	movl	sp@(8+16),a2		| lencopied
	moveq	#0,d2			| bytescopied = 0
0:
	cmpl	d1,d2			| bytescopied >= maxlen?
	jge	2f			| yes, go return error.
	addql	#1,d2			| bytescopied++
	movb	a0@+,a1@+		| to++ = from++
	jne	0b			| keep copying?
1:
	moveq	#0,d0			| return 0
	tstl	a2			| null lencopied ptr.?
	jeq	3f			| yes, just return
	movl	d2,a2@			| *lencopied = bytescopied
	jra	3f
2:
	moveq	#ENOENT,d0		| return ENOENT
3:
	moveml	sp@+,a2/d2		| restore registers
	rts

/*
 * Copy a null terminated string from user to kernel address space.
 *
 *	copyinstr(from, to, maxlength, &lencopied)
 */
ENTRY(copyinstr)
	moveml	a2/a3/d2,sp@-		| save registers
	movl	#FC_UD,d0
	movc	d0,sfc			| load source function code (user xxx)
	movl	_active_threads,a3	| active thread on cpu
	movl	#cpstrerr,a3@(THREAD_RECOVER)	| catch a fault if and when

	movl	sp@(12+4),a0		| from
	movl	sp@(12+8),a1		| to
	movl	sp@(12+12),d1		| maxlen
	movl	sp@(12+16),a2		| lencopied
	moveq	#0,d2			| bytescopied = 0
0:
	cmpl	d1,d2			| bytescopied >= maxlen?
	jge	2f			| yes, go return error.
	addql	#1,d2			| bytescopied++
	movsb	a0@+,d0			| fetch byte
	movb	d0,a1@+			| store it
	jne	0b			| keep copying?
1:
	moveq	#0,d0			| return 0
	tstl	a2			| null lencopied ptr.?
	jeq	3f			| yes, just return
	movl	d2,a2@			| *lencopied = bytescopied
	jra	3f
2:
	moveq	#ENOENT,d0		| return ENOENT
3:
	clrl	a3@(THREAD_RECOVER)	| clear recovery address
	moveml	sp@+,a2/a3/d2		| restore registers
	rts


/*
 * Copy a null terminated string from kernel to user address space.
 *
 *	copyoutstr(from, to, maxlength, &lencopied)
 */
	ENTRY(copyoutstr)
	moveml	a2/a3/d2,sp@-		| save registers
	movl	#FC_UD,d0
	movc	d0,dfc			| load destination function code (user xxx)
	movl	_active_threads,a3	| active thread on cpu
	movl	#cpstrerr,a3@(THREAD_RECOVER)	| catch a fault if and when

	movl	sp@(12+4),a0		| from
	movl	sp@(12+8),a1		| to
	movl	sp@(12+12),d1		| maxlen
	movl	sp@(12+16),a2		| lencopied
	moveq	#0,d2			| bytescopied = 0
0:
	cmpl	d1,d2			| bytescopied >= maxlen?
	jge	2f			| yes, go return error.
	addql	#1,d2			| bytescopied++
	movb	a0@+,d0			| fetch byte
	movsb	d0,a1@+			| store it
	jne	0b			| keep copying?
1:
	moveq	#0,d0			| return 0
	tstl	a2			| null lencopied ptr.?
	jeq	3f			| yes, just return
	movl	d2,a2@			| *lencopied = bytescopied
	jra	3f
2:
	moveq	#ENOENT,d0		| return ENOENT
3:
	clrl	a3@(THREAD_RECOVER)	| clear recovery address
	moveml	sp@+,a2/a3/d2		| restore registers
	rts

cpstrerr:
	moveq	#EFAULT,d0		| return EFAULT
	clrl	a3@(THREAD_RECOVER)	| clear recovery address
	moveml	sp@+,a2/a3/d2		| restore registers
	rts

/*
 * switch_thread_context()
 * switch_task_context()
 *
 * Switch context to a different thread.
 */
ENTRY(switch_thread_context)
ENTRY2(switch_task_context)
	movl	sp@(4),a0

/*
 * MACH scheduler glue
 */
	cmpl	#TH_RUN,a0@(THREAD_STATE)
	bne	0f
	clrl	sp@-
	movl	a0,sp@-
	jsr	_thread_setrun
	addqw	#8,sp
	bra	1f

0:
	cmpl	#TH_RUN+TH_WAIT,a0@(THREAD_STATE)
	bne	0f
	andl	#~TH_RUN,a0@(THREAD_STATE)
	bra	1f

0:
	cmpl	#TH_RUN+TH_IDLE,a0@(THREAD_STATE)
	beq	1f

	movl	a0,sp@-
	jsr	_thread_continue
	addqw	#4,sp
1:

/*
 * Save context of old thread
 */
	movl	sp@(4),a0
	movl	a0@(THREAD_PCB),a0

/*
 * Save FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	fsave	a0@(PCB_FP_FRAME)
	tstw	a0@(PCB_FP_FRAME)
	beq	0f
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL)
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)
0:
/*
 * Save thread kernel context
 */
	movl	sp@,a0@(PCB_K_PC)
	movw	sr,a0@(PCB_K_SR)
	orw	#SR_IPL7,sr
	moveml	KDMOVEM/KAMOVEM,a0@(PCB_K_REGS)

/*
 * Restore context of new thread
 */
	movl	sp@(8),a0
	movl	a0@(THREAD_PCB),a1

/*
 * Forced restore of FP state
 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	

/*
 * Normal restore of FP state
 */
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
/*
 * Restore thread kernel context
 */
	moveml	a1@(PCB_K_REGS),KDMOVEM/KAMOVEM
	movl	a0,_active_threads
	movc	a1,msp
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@

/*
 * U**X !!!
 */
	jsr	_load_context_data

	rts

/*
 * These routines are used by the
 * multiprocessor code to start and
 * stop slave processors.  They are not called
 * in this system (but are referenced!),
 * and are probably not totally up to date.
 */

/*
 *	save_context()
 *
 *	Save the context of the active thread and return 0.
 */
ENTRY(save_context)
	movc	msp,a0			| pointer to PCB of active thread

	/*
	 * Save fpp state.
	 */
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	1f
	fsave	a0@(PCB_FP_FRAME)	| save internal state
	tstw	a0@(PCB_FP_FRAME)	| test for null state
	jeq	1f			| branch if so
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL) | save control registers
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)| save fp data registers
1:

	movl	sp@,a1			| save return pc
	movl	a1,a0@(PCB_K_PC)	| save return pc in pcb
	movw	sr,a0@(PCB_K_SR)	| save current psw
	orw	#SR_IPL,sr		| mask interrupts
	moveml	KDMOVEM/KAMOVEM,a0@(PCB_K_REGS)	| save cpu regs
	lea	eintstack,sp		| use the interrupt stack
	clrl	d0			| return 0
	jmp	a1@			| return to caller

/*
 *	Load context for specified thread and return 1.
 *
 *	load_context(thread)
 *	thread_t	thread;
 */
ENTRY(load_context)
	jbsr	_load_context_data	| load up per context variables
					| must be first, since clobbers a0,a1

	movl	sp@(4),a0		| save thread pointer
	movl	a0@(THREAD_PCB),a1	| get PCB
/*
 * Check to see if we already have context.
 * If so, then set up the next context.
 */
	movl	a0@(THREAD_TASK),a2	| thread->task
	tstl	a2@(TASK_KERNEL_VM_SPACE) | kernel only task?
	bne	2f			| yes, then dont load CRP

	movl	#CPU_CACHE_CLR_ENAB,d0	| flush old user context
	movc	d0,cacr			| and kernel (sigh)

	movl	a2@(TASK_MAP),a2	| thread->task->map
	movl	a2@(VM_MAP_PMAP),a2	| thread->task->map->pmap
	btst	#PMAP_24BIT_BIT,a2@(PMAP_FLAGS+3)
	beq	0f
	movl	a2@(PMAP_ALT_ROOT),a2
	bra	1f
0:
	movl	a2@(PMAP_ROOT),a2	| thread->task->map->pmap->root (phew!)
1:

	/*
	 * Form new CRP on stack.
	 */
	movl	a2,sp@-			| push tree root
	movw	#PMMU_VALID_RP,sp@-	| valid
	movw	#PMMU_RP_LIMIT,sp@-	| limit
	/*
	 * Load CRP. No Need to flush
	 * ATC with 68851.
	 */
	movl	sp,a0
	pmove	a0@,CRP
	addql	#8,sp
2:
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	2f
	bclr	#FP_RESTORE_BIT,a1@(PCB_FLAGS)
	beq	0f
	frestore a1@(PCB_FP_FRAME)
	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
	bra	2f	
0:
	tstw	a1@(PCB_FP_FRAME)
	beq	1f

	fmovem	a1@(PCB_FPS_REGS),fp0-fp7
	fmovem	a1@(PCB_FPS_CTRL),fpc/fps/fpi
1:
	frestore a1@(PCB_FP_FRAME)
2:
	moveml	a1@(PCB_K_REGS),KDMOVEM/KAMOVEM	| restore cpu regs
	movc	a1,msp			| save PCB address of thread
	movw	a1@(PCB_K_SR),sr
	movl	a1@(PCB_K_PC),sp@
	movl	#1,d0
	rts
	
/*
 * Called by trap #2 to do an instruction cache flush operation
 */
	.globl	__flush
__flush:
	movl	d0,sp@-
	movl	#CPU_CACHE_CLR_ENAB,d0
	movc	d0,cacr			| clear (and enable) the cache
	movl	sp@+,d0
	rte

	.data
nmi_lock:
	.long	0

	.text
	.globl	__nmi
__nmi:
	tas	nmi_lock
	bne	0f
#if	MACOS_EMUL
	moveml	d0-d1/a0-a1,sp@-
	jsr	_macintrsw_intr
	moveml	sp@+,d0-d1/a0-a1
#endif
	clrb	nmi_lock
0:
	rte

	.globl	__syscall
/*
 * Special case for syscall.
 * Everything in line because this is by far the most
 * common interrupt.
 */
__syscall:
	movl	a0,sp@-
	movc	msp,a0
	moveml	d0-d7/a0-a6,a0@(PCB_REGS)
	movl	sp@+,a0@(PCB_A0)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	movl	_active_threads,a1	| active thread on cpu
	movl	#syserr,a1@(THREAD_RECOVER)	| catch a fault if and when
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	usp,a0
	movsl	a0@,d0			| get the syscall code
syscont:
	clrl	a1@(THREAD_RECOVER)	| clear recovery address
	pea	sp@
	movl	d0,sp@-			| push syscall code
	jsr	_syscall		| go to C routine
	addqw	#8,sp			| pop args
	jmp	rte_user

syserr:
	movl	#63,d0			| set code to nosys
	jra	syscont			| back to mainline

/*
 *	Bootstrap code for new thread.
 */
ENTRY(thread_bootstrap)
	jmp	rte_user

/*
 * Return following a user trap.
 */
	.globl	rte_user
rte_user:
	orw	#SR_IPL,sr
	movc	msp,a1
	bclr	#RET_FRAME_BIT,a1@(PCB_FLAGS)
	jeq	2f
/*
 * Restore special return frame
 * from PCB
 */
	lea	eintstack,sp
	lea	a1@(KERNEL_STACK_SIZE),a0
	movl	a1@(PCB_RETURN_FRAME_SIZE),d0
	subl	d0,a0
	movl	a0,a3
	lsrl	#1,d0
	lea	a1@(PCB_RETURN_FRAME_DATA),a2
	bra	1f
0:	movw	a2@+,a0@+
1:	dbf	d0,0b
	clrl	a1@(PCB_RETURN_FRAME_SIZE)
	movl	a3,sp
2:
/*
 * AST/Trace bit handling
 */
	btst	#SR_TRACE_BIT,sp@(F_SR)
	beq	3f
	bset	#TRACE_USER_BIT,a1@(PCB_AST)
3:	bclr	#AST_SCHED_BIT,a1@(PCB_AST)
	bne	4f
	btst	#TRACE_AST_BIT,a1@(PCB_AST)
	beq	5f
4:	bset	#TRACE_AST_BIT,a1@(PCB_AST)
	bset	#SR_TRACE_BIT,sp@(F_SR)
5:
/*
 * Restore user registers
 */
	movc	msp,a1
	movl	a1@(PCB_SP),a0
	movl	a0,usp
	moveml	a1@,d0-d7/a0-a6
	btst	#SR_SUPR_BIT,sp@(F_SR)
	beq	9f
	trap	#15
9:
	rte

/*
 * Return following a kernel trap.
 */
	.globl	rte_kernel
rte_kernel:
	tstl	d0
	beq	0f

/*
 * Return with normal frame,
 * discarding existing frame
 */
	lea	sp@(R_SP+4),a0
	movl	a0,a1
	addw	d0,a1
	clrw	a1@(F_VOR)
	movl	a0@(F_PC),a1@(F_PC)
	movw	a0@(F_SR),a1@(F_SR)
	movl	a1,sp@(R_SP)
	moveml	sp@,d0-d7/a0-a7
	rte

/*
 * Return with existing frame
 */
0:
	moveml	sp@+,d0-d7/a0-a6
	addqw	#4,sp
	rte


/*
 * Return following a device interrupt.
 */
	.globl	rte_intr
rte_intr:
	orw	#SR_IPL,sr
	btst	#SR_SUPR_BIT,sp@(4*4)
	bne	1f
/*
 * AST handling
 */
	movc	msp,a0
	bclr	#AST_SCHED_BIT,a0@(PCB_AST)
	bne	0f
	btst	#TRACE_AST_BIT,a0@(PCB_AST)
	beq	1f
0:	bset	#TRACE_AST_BIT,a0@(PCB_AST)
	bset	#SR_TRACE_BIT,sp@(4*4)

/*
 * If returning to IPL 0 run
 * activities in 'software' list
 */
1:
	movw	sp@(4*4),d0
	andw	#SR_IPL,d0
	bne	2f
	jsr	_softint
/*
 * Restore saved registers
 */
2:	moveml	sp@+,d0-d1/a0-a1
	rte

/*
 * Save FPU registers into thread pcb.
 */
ENTRY(fp_sync)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0			| get thread PCB
	fsave	a0@(PCB_FP_FRAME)	| save internal state
	tstw	a0@(PCB_FP_FRAME)	| is there any?
	beq	0f			| bail out if not
	fmovem	fpc/fps/fpi,a0@(PCB_FPS_CTRL)	| save control regs
	fmovem	fp0-fp7,a0@(PCB_FPS_REGS)	| and data regs
	frestore a0@(PCB_FP_FRAME)	| restore internal state
0:
	rts

/*
 * Stop FPU and save internal state in thread pcb.
 */
ENTRY(fp_stop)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0
	fsave	a0@(PCB_FP_FRAME)
0:
	rts

/*
 * Restore FPU internal state from thread pcb.
 */
ENTRY(fp_continue)
	btst	#G_hwCbFPU,G_HWCfgFlags
	beq	0f
	movc	msp,a0
	frestore a0@(PCB_FP_FRAME)
0:
	rts
