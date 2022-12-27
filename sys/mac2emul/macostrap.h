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
 *	File: mac2emul/macostrap.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * MacOS trap defines
 */

/*
 * Trap word bits
 */
#define OSTRAP_TBOX_TYPE 0x0800
#define OSTRAP_TBOX_APOP 0x0400

extern (*mac_osinst_vector[])();
