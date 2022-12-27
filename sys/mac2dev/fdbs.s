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
 * $Log:	fdbs.s,v $
 * Revision 2.2  90/08/30  11:05:59  bohman
 * 	Created.
 * 	[90/08/29  12:44:40  bohman]
 * 
 */

/*
 * (C) 1986 UniSoft Corp. of Berkeley CA
 *
 * UniPlus Source Code. This program is proprietary
 * with Unisoft Corporation and is not to be reproduced
 * or used in any manner except as authorized in
 * writing by Unisoft.
 */

#include "assym.s"
#include <mac2/psl.h>

	.globl _fdbhand
	.globl _fdb_command
	.globl _fdb_timeout
	.globl _fdb_select
	.globl _fdb_pollflg
	.globl _fdb_exb
	.globl _fdb_state
	.globl _fdb_error
	.globl _fdb_cnt
	.globl _fdb_datap
	.globl _via1intr

_fdbhand:
	moveml	d0-d1/a0-a1,sp@-
	movl	@(_VIA1_ADDR_PTR)@(0),a0
	btst	#2,a0@(VIA_IFR)		| Check to see if it was a FDB (sr)
	bne	do_fdb			|	interrupt

	btst	#7,a0@(VIA_IFR)		| any other intr?
	beq	rte_intr			| no, do rei

do_intr:
	movw	#SR_SUPR+SR_IPL1,sr
	link	a6,#0
	cmpl	#eintstack,sp
	ble	1f
	lea	eintstack,sp
1:
	pea	a6@(5*4)	    | push saved frame address
	jsr	_via1intr
	unlk	a6
	jmp	rte_intr

do_fdb:
	btst	#2,a0@(VIA_IER)		| Check to see if it was a FDB (sr)
	beq	do_intr
	link	a6,#0
	cmpl	#eintstack,sp
	ble	1f
	lea	eintstack,sp
1:
	bsr	_fdb_inthand
	unlk	a6
	movl	@(_VIA1_ADDR_PTR)@(0),a0
	btst	#7,a0@(VIA_IFR)			| any further interrupt
	bne	do_intr				| processing?
	btst	#2,@(_VIA1_SOFT_PTR)@(0)
	bne	do_intr
	jra	rte_intr

	.globl	_fdb_inthand
_fdb_inthand:
	movl	@(_VIA1_ADDR_PTR)@(0),a1
	movb	#0x04,a1@(VIA_IFR)	| clear interrupt
	tstb	_fdb_state		| Did we just get an attention
					|	interrupt?
	bne	incc
	bset	#2,@(_VIA1_SOFT_PTR)@(0)| say we have a real interrupt to be
					| serviced
	rts
incc:	movl	_fdb_datap,a0		| Get the data register pointer
	btst	#1,_fdb_command		| Are we doing a Talk?
	bne	TalkInt
	btst	#0,_fdb_command		| a listen?
	bne	ListInt
	btst	#2,_fdb_command		| Checking for existance?
	bne	TalkInt
	btst	#3,_fdb_command		| flushing?
	bne	FlushInt
ResetInt:
	btst	#0,_fdb_state		| Do we now have to do a dummy S2?
	beq	S2Reset
	bclr	#0x04,a1@(VIA_ACR)	| change to shift in
	movb	a1@(VIA_SR),d0	| start input
	bra	SetS2
S2Reset:
	movb	a1@(VIA_SR),d0	| Dummy read
	clrl	d0			| Flag OK
	bra	SetS3			| Goto idle state

FlushInt:				
	btst	#0,_fdb_state		| Do we now have to do a dummy S2?
	beq	S2Flush
	bclr	#0x04,a1@(VIA_ACR)	| change to shift in
	movb	a1@(VIA_SR),d0	| start input
	btst	#3,a1@(VIA_REGB)	| Did a poll succeed?
	seq	_fdb_pollflg
	bne	SetS2
	movb	#2,_fdb_command		| Turn it into a talk
	movl	#_fdb_exb,a0		| Get last poll pointer .....
	bra	SetS1			| Switch to state 1

S2Flush:
	btst	#3,a1@(VIA_REGB)	| Is there a service request?
	seq	_fdb_select		
	movb	a1@(VIA_SR),d0	| Dummy read
	clrl	d0			| Flag OK
	bra	SetS3			| Goto idle state
	
	
TalkInt:
	btst	#0,_fdb_state		| We are doing a talk, what state are
	beq	S1End			|	we in?

S0End:
	btst	#3,a1@(VIA_REGB)	| Did a poll succeed?
	seq	_fdb_pollflg
	bne	S0x
	movl	#_fdb_exb,a0		| Get last poll pointer ....
S0x:	bclr	#0x04,a1@(VIA_ACR)	| change to shift in
	movb	a1@(VIA_SR),d0	| start input
	bra	SetS1			| Switch to state 1

S1End:
	btst	#1,_fdb_state		| Are we in state 1
	beq	S2End
	btst	#3,a1@(VIA_REGB)	| Did a timeout occur??
	seq	_fdb_timeout
	movb	a1@(VIA_SR),a0@+	| Save the read data
	bra	SetS2			| Switch to state 2

StateErr:
	movb	a1@(VIA_SR),d0	| An error occured, reset the SR
	movl	#0xff,d0		| Flag the error
	bra	SetS3			| Goto idle state

S2End:
	btst	#2,_fdb_state		| Are we in state 2?
	beq	StateErr		| If not we are in trouble
	btst	#3,a1@(VIA_REGB)	| Is there a service request?
	seq	_fdb_select		
	movb	a1@(VIA_SR),a0@+	| Save the data
	clrl	d0			| Flag OK
	bra	SetS3			| Goto idle state

ListInt:
	btst	#0,_fdb_state		| Are we in state 0?
	beq	List1
List0:
	btst	#3,a1@(VIA_REGB)	| Did a poll succeed?
	seq	_fdb_pollflg
	bne	List0Cont
	bclr	#0x04,a1@(VIA_ACR)	| change to shift in
	movb	a1@(VIA_SR),d0	| start input
	movl	#_fdb_exb,a0		| Get last poll pointer ....
	movb	#2,_fdb_command		| Turn it into a talk
	bra	SetS1			| Switch to state 1
List0Cont:
	tstb	_fdb_cnt			| Are there any characters
	beq	List3			| 	left to transfer?
	movb	a0@+,a1@(VIA_SR)	| Send the next one
	subb	#1,_fdb_cnt		| Decrement the count
	bra	SetS1			| Switch to state 1

List1:
	btst	#1,_fdb_state		| Are we in state 1?
	beq	List2
	btst	#3,a1@(VIA_REGB)	| Did a timeout occur??
	seq	_fdb_timeout
	tstb	_fdb_cnt			| Are there any characters left 
	beq	List3			|	to transfer?
	movb	a0@+,a1@(VIA_SR)	| Send the next one
	subb	#1,_fdb_cnt		| Decrement the count
	bra	SetS2			| Switch to state 2

List2:
	btst	#2,_fdb_state		| Are we in state 2?
	beq	StateErr		| If not bugout
	btst	#3,a1@(VIA_REGB)	| Is there a service request?
	seq	_fdb_select		
	tstb	_fdb_cnt			| Are there any characters
	beq	List3			|	left to transfer?
	movb	a0@+,a1@(VIA_SR)	| Send the next one
	subb	#1,_fdb_cnt		| Decrement the count
	bra	SetS1			| Switch to state 1

List3:
	clrl	d0			| Transfer completed OK
	movb	a1@(VIA_SR),d0	| clear interrupt
	bra	SetS3			| switch to state 3

SetS1:
	movl	#1,d0			| Switching to state 1
	bra	SetS

SetS2:
	movl	#2,d0			| Switching to state 2
SetS:
	movl	a0,_fdb_datap		| Save the data pointer back again
	clrb	_fdb_state		| Move to the next state
	bset	d0,_fdb_state
	lslb	#4,d0			| Set the state in the VIA
	movb	a1@(VIA_REGB),d1
	andb	#0xcf,d1
	orb	d0,d1
	movb	d1,a1@(VIA_REGB)
	rts				

SetS3:
	movb	d0,_fdb_error		| Save the error status
	bset	#2,@(_VIA1_SOFT_PTR)@(0)| say we have a real interrupt to be
					| serviced
	bclr	#0x04,a1@(VIA_ACR)	| change to shift in
	orb	#0x30,a1@(VIA_REGB)	| Set the via to state 3
	rts
