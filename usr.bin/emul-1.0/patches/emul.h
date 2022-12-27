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
 *	File: emul/patches/emul.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Registers are passed by value to OS emulation
 * routines.
 */
typedef struct {
    unsigned long	d_0;
    unsigned long	d_1;
    unsigned long	a_0;
    unsigned long	a_1;
} os_reg_t;
