/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mach_traps.s,v $
 * Revision 1.2  90/02/20  00:46:48  bww
 * 	Make the RT symbol .oVncs global so that we can
 * 	"ld -x" this file and still link successfully.
 * 	[90/02/20  00:46:30  bww]
 * 
 * Revision 1.1  90/02/19  18:08:00  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:46:03  mrt
 * 	Cleanup for Mach 2.5
 * 
 *  1-Sep-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Created.  All the work gets done in "syscall_sw.h" now.
 *
 */

#if defined(ibmrt)
	.globl .oVncs
#endif
#include <mach/syscall_sw.h>
