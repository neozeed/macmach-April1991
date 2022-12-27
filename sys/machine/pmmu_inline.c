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
 * $Log:	pmmu_inline.c,v $
 * Revision 2.2  90/08/30  11:02:48  bohman
 * 	Created.
 * 	[90/08/29  11:44:06  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmmu_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_PMMU_INLINE_C_
#define	_MACHINE_PMMU_INLINE_C_

/*
 * Inline expansions for special
 * PMMU functions.
 */

/*
 * Set the Translation Control register.
 */
static inline
void
pmmu_set_tc(p)
PMMU_TC_reg	*p;
{
    asm("pmove	%0@,TC" : : "a" (p));
}

/*
 * Set the Supervisor Root Pointer register.
 */
static inline
void
pmmu_set_srp(p)
PMMU_RP_reg	*p;
{
    asm("pmove	%0@,SRP" : : "a" (p));
}

/*
 * Set the CPU Root Pointer register.
 */
static inline
void
pmmu_set_crp(p)
PMMU_RP_reg	*p;
{
    asm("pmove	%0@,CRP" : : "a" (p));
}

/*
 * Flush the ATC of entries corresponding
 * to a certain root pointer (68020).
 */
static inline
void
pmmu_flush_map(p)
PMMU_RP_reg	*p;
{
    asm("pflushr	%0@" : : "a" (p));
}

/*
 * Flush all user descriptors from the ATC.
 */
static inline
void
pmmu_flush_user_all()
{
    asm("pflush	#0,#4");
}

/*
 * Flush the user descriptor mapping
 * the specified address from the ATC.
 */
static inline
void
pmmu_flush_user(p)
vm_offset_t	p;
{
    asm("pflush	#0,#4,%0@" : : "a" (p));
}

/*
 * Flush all supervisor descriptors from the ATC.
 */
static inline
void
pmmu_flush_supr_all()
{
    asm("pflush #4,#4");
}

/*
 * Flush the supervisor descriptor mapping
 * the specified address from the ATC.
 */
static inline
void
pmmu_flush_supr(p)
vm_offset_t	p;
{
    asm("pflush	#4,#4,%0@" : : "a" (p));
}

/*
 * Flush all supervisor descriptors from the ATC
 * including shared entries (68020).
 */
static inline
void
pmmu_flush_supr_shared_all()
{
    asm("pflushs	#4,#4");
}

/*
 * Flush the supervisor descriptor mapping
 * the specified address from the ATC
 * including shared entries (68020).
 */
static inline
void
pmmu_flush_supr_shared(p)
vm_offset_t	p;
{
    asm("pflushs	#4,#4,%0@" : : "a" (p));
}

/*
 * Load the user descriptor mapping
 * the specified data address into the ATC.
 * Used for page faults on RMW cycles.
 */
static inline
void
pmmu_load_user(p)
vm_offset_t	p;
{
    asm("ploadw	#1,%0@" : : "a" (p));
}

/*
 * Return the PSR register obtained
 * by testing the specified user data address.
 */
static inline
unsigned short
pmmu_test_user_data(p)
vm_offset_t	p;
{
    volatile	/* 'volatile' cause the optimizer gets in the way */
	unsigned short psr;

    asm("ptestr	#1,%0@,#7" : : "a" (p));

    asm("pmove	PSR,%0" : "=m" (psr) : );

    return (psr);
}

/*
 * Return the PSR register obtained
 * by testing the specified user text address.
 */
static inline
unsigned short
pmmu_test_user_text(p)
vm_offset_t	p;
{
    volatile	/* 'volatile' cause the optimizer gets in the way */
	unsigned short psr;

    asm("ptestr	#2,%0@,#7" : : "a" (p));

    asm("pmove	PSR,%0" : "=m" (psr) : );

    return (psr);
}

#endif	_MACHINE_PMMU_INLINE_C_
