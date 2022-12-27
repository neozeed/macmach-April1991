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
 * $Log:	cpu_inline.c,v $
 * Revision 2.2  90/08/30  11:01:09  bohman
 * 	Created.
 * 	[90/08/29  11:32:19  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/cpu_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CPU_INLINE_C_
#define	_MACHINE_CPU_INLINE_C_

#include <machine/cpu.h>
#include <machine/psl.h>

/*
 * Inline functions dealing with
 * CPU details.
 */

/*
 * Return the address of the PCB for
 * the current thread.
 */
static inline
struct pcb *
current_thread_pcb()
{
    struct pcb *pcb;

    asm("movc	msp,%0" : "=r" (pcb) : );

    return (pcb);
}

/*
 * Flush both the instruction
 * and data caches.
 */
static inline
void
flush_cpu_caches()
{
    unsigned long p = (CPU_DCACHE_WRTALLOC |
		       CPU_DCACHE_CLEAR | CPU_DCACHE_ENABLE |
		       CPU_ICACHE_CLEAR | CPU_ICACHE_ENABLE);

    asm("movc	%0,cacr" : : "r" (p));
}

/*
 * Set the base of the interrupt
 * vector table
 */
static inline
void
set_vector_base(a)
unsigned long	a;
{
    asm("movc	%0,vbr" : : "r" (a));
}

/*
 * Functions for manipulating the
 * processor priority.
 */

/*
 * Canned  functions to set the
 * processor priority to a
 * compiled-in value.
 */
#define	define_spl(n, v)				\
static inline						\
int							\
spl##n()						\
{							\
    unsigned short r;					\
\
    asm("movw	sr,%0" : "=dm" (r) : );			\
\
    asm("movw	%0,sr" : :				\
	"i" ((unsigned short)(SR_SUPR | SR_IPL##v)));	\
\
    return (r & SR_IPL);				\
}

define_spl(7,		7)
define_spl(high,	7)
define_spl(hi,		7)
define_spl(vm,		7)
define_spl(imp,		7)
define_spl(sched,	7)

define_spl(6,		6)

define_spl(5,		5)

define_spl(4,		4)
define_spl(bio,		4)
define_spl(net,		4)
define_spl(tty,		4)

define_spl(3,		3)

define_spl(2,		2)

define_spl(1,		1)
define_spl(clock,	1)
define_spl(softclock,	1)

define_spl(0,		0)

/*
 * Set the processor priority
 * to the value passed in s.
 */
static inline
int
splx(s)
int s;
{
    unsigned short r;

    asm("movw	sr,%0" : "=dm" (r) : );

    asm("movw	%0,sr" : : "dm" ((unsigned short)(s|SR_SUPR)));

    return (r & SR_IPL);
}

#endif	_MACHINE_CPU_INLINE_C_
