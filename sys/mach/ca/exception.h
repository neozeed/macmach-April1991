/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	exception.h,v $
 * Revision 2.5  89/06/30  22:33:38  rpd
 * 	Work-around for APC FP hardware bug.
 * 	[89/06/30  16:18:08  rpd]
 * 
 * Revision 2.4  89/03/09  20:18:31  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  18:12:20  gm0w
 * 	Changes for cleanup.
 * 
 * 15-Feb-88  David Black (dlb) at Carnegie-Mellon University
 *	Move MCHECK to bad access, and change code.
 *
 * 29-Jan-88  Richard Sanzi (sanzi) at Carnegie-Mellon University
 *	Created.
 *
 */

#ifndef	_MACH_CA_EXCEPTION_H_
#define _MACH_CA_EXCEPTION_H_

#include <mach/kern_return.h>

/*
 *	EXC_BAD_ACCESS
 */
/*
 *	Romp has machine-dependent failure modes.  The codes
 *	are negative so as not to conflict with kern_return_t's.
 */
#define EXC_ROMP_MCHECK		((kern_return_t)-1) /* machine check */
#define EXC_ROMP_APC_BUG	((kern_return_t)-2) /* APC hardware bug */

/*
 * EXC_BAD_INSTRUCTION
 */
#define EXC_ROMP_PRIV_INST 	0x01
#define EXC_ROMP_ILLEGAL_INST	0x02

/*
 * EXC_BREAKPOINT
 */
#define EXC_ROMP_TRAP_INST 	0x01
#define EXC_ROMP_INST_STEP  	0x02

/*
 * EXC_ARITHMETIC
 *
 */

/*							   
 * Values for code when type == EXC_ARITHMETIC
 */							    

#define EXC_ROMP_FPA_EMUL	0x01
#define EXC_ROMP_68881		0x02
#define EXC_ROMP_68881_TIMEOUT	0x04
#define EXC_ROMP_FLOAT_SPEC	0x08

#endif	_MACH_CA_EXCEPTION_H_
