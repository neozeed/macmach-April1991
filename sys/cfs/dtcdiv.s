/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * $Log:	dtcdiv.s,v $
 * Revision 2.2  90/07/05  11:27:34  mrt
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
Revision 1.1  90/05/31  17:04:24  dcs
Initial revision

 * 
 */

#include <dtc.h>
#if	NDTC > 0

 #	Divide of dtcsec//dtcusec by 2000000
 #		-- We have a 48 bit counter that holds the number of 
 #		   1/2 microsecond ticks that have elapsed since the clock
 #		   was started. This routine converts this number into
 #		   seconds and microseconds

 #				r2//mq = dividend
 #				r3 = divisor
 #				r15= link
 #			

.set mq      ,       r10                # multiplier quotient system control reg
 
	.globl	.oVncs
	.set	.oVncs,0

	.globl  _dtcsec
	.globl  _dtcusec
	.globl	_twomeg
	.data
L00_DATA:
	.set	_twomeg,L00_DATA+0x00000000
	.long	2000000

	.text
	.globl	_.dtcdiv
_.dtcdiv:
	mr	r0,r4
	load    r2,_dtcsec
	load	r4,_dtcusec
	mts	%mq,r4
	load	r3,_twomeg

here: 				# 32 divide steps:
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3		# 8

        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3		# 16

        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3		# 24

        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3
        d       r2,r3		# 32
 
	bc0x	1f		# if carry-0, no correction needed
	mfs	%mq,r4		# (retrieve the quotient)
	a	r2,r3		# correction: complete the final restore

1:	
	#the remainder is actually the number of 1/2 usecs, so 
	sri	r2,1
        #return to the calling routine
        store	r4,_dtcsec,r3
	store	r2,_dtcusec,r3
	brx	r15
	mr	r4,r0


	.globl	_dtcdiv
	.data
_dtcdiv:

#endif	/* NDTC */
