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
 *	File: mac2emul/mactrap.s
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2/cpu.h>
#include <mac2/trap.h>

#include <assym.s>

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
	btst	\#MAC_EMULATION_BIT,a0@(PCB_FLAGS);	\
	bne	2f;	\
\
0:	moveml	d0-d7/a0-a6,a0@(PCB_REGS);	\
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
	jmp	rte_kernel;	\
2:

	.data
	.globl	_Atrap_count
_Atrap_count:
	.long	0
	.text

TRAP(EMULATION_1010)
	addql	#1,_Atrap_count
	movl	d0,a0@(PCB_D0)
	movl	sp@+,a0@(PCB_A0)
	movl	#FC_UD,d0
	movc	d0,sfc
	movl	sp@(F_PC),a0
	movsw	a0@,d0

	andw	#0x0fff,d0
	btst	#11,d0
	beq	0f
	btst	#10,d0
	beq	0f
	bclr	#10,d0
0:	tstl	@(_mac_osinst_vector:l,d0:W:4)
	bne	1f

	btst	#11,d0
	beq	2f

/*
 * An emulated ToolBox trap
 */
	bfclr	d0{#16:#6}
	movc	msp,a0
	movl	a0@(PCB_EMUL),a0
	bftst	a0@(MAC_TBOX_MAP){d0:#1}
	beq	3f
	movl	#FC_UD,d0
	movc	d0,dfc

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	movl	#Afault,a0@(THREAD_RECOVER)

	movl	usp,a0
	movl	sp@(F_PC),d0
	movsl	d0,a0@-
	movl	a0,usp

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	clrl	a0@(THREAD_RECOVER)

	movc	msp,a0
	movl	a0@(PCB_EMUL),a0
	bfclr	a0@(MAC_SR){#0:#2}
	movl	a0@(MAC_TBOX_VECTOR),a0
	movsl	a0@,d0
	movl	d0,sp@(F_PC)
/*
 * Clear the trace bit in the return
 * frame if TRACE_AST is not set.
 */
	movc	msp,a0
	btst	#TRACE_AST_BIT,a0@(PCB_AST)
	bne	0f
	bclr	#SR_TRACE_BIT,sp@(F_SR)
0:	movl	a0@(PCB_D0),d0
	movl	a0@(PCB_A0),a0
	rte

/*
 * An emulated OS trap
 */
2:
	bfclr	d0{#16:#8}
	movc	msp,a0
	movl	a0@(PCB_EMUL),a0
	bftst	a0@(MAC_OS_MAP){d0:#1}
	beq	3f
	movl	#FC_UD,d0
	movc	d0,dfc

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	movl	#Afault,a0@(THREAD_RECOVER)

	movl	usp,a0
	movl	sp@(F_PC),d0
	movsl	d0,a0@-
	movl	a0,usp

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	clrl	a0@(THREAD_RECOVER)

	movc	msp,a0
	movl	a0@(PCB_EMUL),a0
	bfclr	a0@(MAC_SR){#0:#2}
	movl	a0@(MAC_OS_VECTOR),a0
	movsl	a0@,d0
	movl	d0,sp@(F_PC)
/*
 * Clear the trace bit in the return
 * frame if TRACE_AST is not set.
 */
	movc	msp,a0
	btst	#TRACE_AST_BIT,a0@(PCB_AST)
	bne	0f
	bclr	#SR_TRACE_BIT,sp@(F_SR)
0:	movl	a0@(PCB_D0),d0
	movl	a0@(PCB_A0),a0
	rte

/*
 * Inline version of take_exception()
 *
 * Push the exception frame on the
 * user stack.
 */
3:
	movl	#FC_UD,d0
	movc	d0,dfc

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	movl	#Afault,a0@(THREAD_RECOVER)

	movl	usp,a0
	movw	sp@(F_VOR),d0
	movsw	d0,a0@-
	movl	sp@(F_PC),d0
	movsl	d0,a0@-
	movw	sp@(F_SR),d0
	movsw	d0,a0@-
	movl	a0,usp

	movc	msp,a0
	movl	a0@(PCB_THREAD),a0
	clrl	a0@(THREAD_RECOVER)

/*
 * Clear the trace bits in
 * the software status register,
 * and change the PC in the return
 * frame to return to the trap handler.
 */
	movc	msp,a0
	movl	a0@(PCB_EMUL),a0
	bfclr	a0@(MAC_SR){#0:#2}
	movl	a0@(MAC_VBR),a0
	movsl	a0@(0x28),d0
	movl	d0,sp@(F_PC)
/*
 * Clear the trace bit in the return
 * frame if TRACE_AST is not set.
 */
	movc	msp,a0
	btst	#TRACE_AST_BIT,a0@(PCB_AST)
	bne	0f
	bclr	#SR_TRACE_BIT,sp@(F_SR)
0:	movl	a0@(PCB_D0),d0
	movl	a0@(PCB_A0),a0
	rte

/*
 * A fault occurred while pushing
 * the exception frame onto the user
 * stack.  Restore the user registers,
 * set the low order bit on the user PC
 * and return.
 */
Afault:
	bclr	#SR_TRACE_BIT,sp@(F_SR)
	movl	sp@(F_PC),d0
	bset	#0,d0
	movl	d0,sp@(F_PC)
	movc	msp,a0
	movl	a0@(PCB_D0),d0
	movl	a0@(PCB_A0),a0
	rte

/*
 * Handle a kernel emulated MacOS trap
 */
1:	movc	msp,a0
	movl	d1,a0@(PCB_D1)
	movl	a1,a0@(PCB_A1)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	movl	a0,sp@-
	pea	sp@(4)
	jsr	_mac_osinst
	addqw	#8,sp
	movc	msp,a0
	movl	a0@(PCB_SP),a1
	movl	a1,usp
	movl	a0@(PCB_A1),a1
	movl	a0@(PCB_D1),d1
	movl	a0@(PCB_D0),d0
	movl	a0@(PCB_A0),a0
	rte

TRAP(PRIV_INSTRUCTION)
	moveml	d0-d7/a0-a6,a0@(PCB_REGS)
	movl	sp@+,a0@(PCB_A0)
	movl	usp,a1
	movl	a1,a0@(PCB_SP)
	btst	#TRACE_USER_BIT,a0@(PCB_AST)
	bne	0f
	bclr	#SR_TRACE_BIT,sp@(F_SR)
0:	movl	a0,sp@-
	pea	sp@(4)
	movl	_macdebug,d0
	andb	#2,d0
	beq	0f
	movl	#TRAP_PRIV_INSTRUCTION,sp@-
	jsr	_user_trap
	addqw	#8,sp
	movc	msp,a0
	movl	a0@(PCB_FRAME),sp@-
0:	jsr	_macpriv_inst
	tstl	d0
	bne	0f
	jsr	_take_exception
	addqw	#8,sp
	jmp	rte_user

0:	addqw	#8,sp
	movc	msp,a0
	bclr	#TRACE_USER_BIT,a0@(PCB_AST)
	movl	a0,sp@-
	jsr	_check_mac_interrupts
	addqw	#4,sp
	jmp	rte_user

	.data
	.globl	_trap1_count
_trap1_count:
	.long	0
	.text

#define	TRAP_trap1	TRAP_ERROR
TRAP(trap1)
	addql	#1,_trap1_count
	movl	#0xcb2,a0		| must be atomic
	movsb	d0,a0@
	extbl	d0
	movl	d0,sp@-
	jsr	_mac_set_addr_mode
	addqw	#8,sp			| the trap code saved A0 on stack
	rte
