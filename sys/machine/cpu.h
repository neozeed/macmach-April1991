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
 * $Log:	cpu.h,v $
 * Revision 2.2  90/08/30  11:01:06  bohman
 * 	Created.
 * 	[90/08/29  11:31:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/cpu.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CPU_H_
#define	_MACHINE_CPU_H_

#ifdef	KERNEL
#include <sys/types.h>
#endif	KERNEL

/*
 * cpu architecture defines
 */

#ifndef	ASSEMBLER
/*
 * An interrupt vector
 */
struct ivect {
    void	(*vector)();
};
#endif	ASSEMBLER

/*
 * Cache Control
 */
#define	CPU_ICACHE_ENABLE   0x0001
#define	CPU_ICACHE_FREEZE   0x0002
#define	CPU_ICACHE_CLRENTRY 0x0004
#define	CPU_ICACHE_CLEAR    0x0008
#define CPU_ICACHE_BURSTEN  0x0010
#define CPU_DCACHE_ENABLE   0x0100
#define CPU_DCACHE_FREEZE   0x0200
#define CPU_DCACHE_CLRENTRY 0x0400
#define CPU_DCACHE_CLEAR    0x0800
#define CPU_DCACHE_BURSTEN  0x1000
#define CPU_DCACHE_WRTALLOC 0x2000

#ifdef	ASSEMBLER
/*
 * Value for CACR we use
 */
#define CPU_CACHE_CLR_ENAB CPU_DCACHE_WRTALLOC+CPU_DCACHE_CLEAR+CPU_DCACHE_ENABLE+CPU_ICACHE_CLEAR+CPU_ICACHE_ENABLE
#endif	ASSEMBLER

/*
 * Function code register values.
 */
#define	FC_UD	1		/* user data */
#define	FC_UP	2		/* user program */
#define	FC_SD	5		/* supervisor data */
#define	FC_SP	6		/* supervisor program */
#define	FC_CPU	7		/* cpu space */

#ifndef	ASSEMBLER
#define	set_cpu_number()

#define	cpu_number()	(0)

int	master_cpu;

extern struct ivect	*ivect_tbl, ivect_tbl_prototype[];
#endif	ASSEMBLER
#endif	_MACHINE_CPU_H_
