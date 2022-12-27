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
 * $Log:	MacGlobals.h,v $
 * Revision 2.2  90/08/30  11:00:14  bohman
 * 	Created.
 * 	[90/08/29  10:55:34  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/MacGlobals.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Apple macOS Global variables
 * used by macmach
 */
#ifndef	_MAC2_MACGLOBALS_H_
#define	_MAC2_MACGLOBALS_H_

#define	G_MemTop	0x108	/* RAM highest addr+1 */
#define G_BufPtr	0x10c
#define G_JVBLTask	0xd28
#define G_HWCfgFlags	0xb22
#define G_hwCbFPU	12
#define G_hwCbAUX	9
#define G_JIODone	0x8fc
#define G_MMUInfoPtr	0xcb8
#define G_BootGlobPtr	0xddc

#endif	_MAC2_MACGLOBALS_H_
