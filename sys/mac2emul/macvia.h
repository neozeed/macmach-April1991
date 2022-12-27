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
 *	File: mac2emul/macvia.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * defines for macintosh
 * VIA emulator
 */

/*
 * Amount to shift device offset
 * to get register offset.
 */
#define VIA_REGISTER_SHIFT 9

/*
 * VIA Register offsets
 */
#define VIA_REG_DRB    0
#define VIA_REG_DDRB   2
#define VIA_REG_DDRA   3
#define VIA_REG_T1CL   4
#define VIA_REG_T1CH   5
#define VIA_REG_T1LL   6
#define VIA_REG_T1LH   7
#define VIA_REG_T2CL   8
#define VIA_REG_T2CH   9
#define VIA_REG_SR     10
#define VIA_REG_ACR    11
#define VIA_REG_PCR    12
#define VIA_REG_IFR    13
#define VIA_REG_IER    14
#define VIA_REG_DRA    15
