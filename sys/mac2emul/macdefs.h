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
 *	File: mac2emul/macdefs.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2EMUL_MACDEFS_H_
#define _MAC2EMUL_MACDEFS_H_

/*
 * Software interrupts
 */
#define MAC_INTR_INTR	0
#define MAC_INTR_ETHER	1
#define MAC_INTR_SLOT	2
#define MAC_INTR_ASC	3
#define MAC_INTR_TIMER2	4
#define MAC_INTR_TIMER1	5
#define MAC_INTR_CLK	6
#define MAC_INTR_INP	7
#define MAC_INTR_SR	8

#define MAC_INTR_NUMB	9

#define MAC_DEBUG_BERR	0x01
#define MAC_DEBUG_INST	0x02
#define MAC_DEBUG_EXCEP	0x04

#endif _MAC2EMUL_MACDEFS_H_
