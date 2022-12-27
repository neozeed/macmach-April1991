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
 * $Log:	call_frame.h,v $
 * Revision 2.2  90/08/30  11:00:50  bohman
 * 	Created.
 * 	[90/08/29  11:29:55  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/call_frame.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_CALL_FRAME_H_
#define _MACHINE_CALL_FRAME_H_

/*
 * C function call frame
 * definition (actually compiler dependent)
 */
struct call_frame {
    struct call_frame *f_fp;
    unsigned long f_pc;
    unsigned long f_param[1];
};

#endif	_MACHINE_CALL_FRAME_H_
