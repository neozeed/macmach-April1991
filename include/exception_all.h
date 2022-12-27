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
 * $Log:	exception_all.h,v $
 * Revision 2.4  90/03/21  15:11:32  mrt
 * 	Added contents of mach/i386/exception.h
 * 	[90/03/21            mrt]
 * 
 * 	Added contents of mach/mmax/exception.h
 * 	[90/03/16            dlb]
 * 
 * Revision 2.3  90/03/07  16:00:31  mrt
 * 	Added contents of mach/mips/exception.h
 * 	[90/03/07            af]
 * 
 * 	Added contents mach/sun4/exception.h.
 * 	[90/02/21            jjc]
 * 
 * Revision 2.2  89/10/31  23:43:04  mrt
 * 	Combined mach/exception, mach/ca/exeption.h, 
 * 	mach/sun3/exception, mach/vax/exception.h
 * 	[89/10/29            mrt]
 * 
 *
 */

#ifndef	_EXCEPTION_ALL_H_
#define _EXCEPTION_ALL_H_

#ifndef _MACH_EXCEPTION_H_
#define _MACH_EXCEPTION_H_

/*
 *	Machine-independent exception definitions.
 */

#define EXC_BAD_ACCESS		1	/* Could not access memory */
		/* Code contains kern_return_t describing error. */
		/* Subcode contains bad memory address. */

#define EXC_BAD_INSTRUCTION	2	/* Instruction failed */
		/* Illegal or undefined instruction or operand */

#define EXC_ARITHMETIC		3	/* Arithmetic exception */
		/* Exact nature of exception is in code field */

#define EXC_EMULATION		4	/* Emulation instruction */
		/* Emulation support instruction encountered */
		/* Details in code and subcode fields	*/

#define EXC_SOFTWARE		5	/* Software generated exception */
		/* Exact exception is in code field. */
		/* Codes 0 - 0xFFFF reserved to hardware */
		/* Codes 0x10000 - 0x1FFFF reserved for OS emulation (Unix) */

#define EXC_BREAKPOINT		6	/* Trace, breakpoint, etc. */
		/* Details in code field. */

#endif /* _MACH_EXCEPTION_H_ */

/* 	Machine dependent exceptions for the ibm_rt
 * 	Revision 2.5  89/06/30  22:33:38  rpd
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

#endif	/* _MACH_CA_EXCEPTION_H_ */


/*
 *	Machine dependent exception definitions for the Sun3.
 * 	Revision 2.4  89/03/09  20:23:26  rpd
 */

#ifndef	_MACH_SUN3_EXCEPTION_H_
#define _MACH_SUN3_EXCEPTION_H_

/*
 *	EXC_BAD_INSTRUCTION
 */

#define EXC_SUN3_ILLEGAL_INSTRUCTION	0x10
#define EXC_SUN3_PRIVILEGE_VIOLATION	0x20
#define EXC_SUN3_COPROCESSOR		0x34
#define EXC_SUN3_TRAP1			0x84
#define EXC_SUN3_TRAP2			0x88
#define EXC_SUN3_TRAP3			0x8c
#define EXC_SUN3_TRAP4			0x90
#define EXC_SUN3_TRAP5			0x94
#define EXC_SUN3_TRAP6			0x98
#define EXC_SUN3_TRAP7			0x9c
#define EXC_SUN3_TRAP8			0xa0
#define EXC_SUN3_TRAP9			0xa4
#define EXC_SUN3_TRAP10			0xa8
#define EXC_SUN3_TRAP11			0xac
#define EXC_SUN3_TRAP12			0xb0
#define EXC_SUN3_TRAP13			0xb4
#define EXC_SUN3_TRAP14			0xb8
#define EXC_SUN3_FLT_BSUN		0xc0
#define EXC_SUN3_FLT_OPERAND_ERROR	0xd0

/*
 *	NOTE: TRAP0 is syscall, TRAP15 is breakpoint.
 */

/*
 *	EXC_ARITHMETIC
 */

#define EXC_SUN3_ZERO_DIVIDE		0x14
#define EXC_SUN3_FLT_INEXACT		0xc4
#define EXC_SUN3_FLT_ZERO_DIVIDE	0xc8
#define EXC_SUN3_FLT_UNDERFLOW		0xcc
#define EXC_SUN3_FLT_OVERFLOW		0xd4
#define EXC_SUN3_FLT_NOT_A_NUMBER	0xd8

/*
 *	EXC_EMULATION
 */

#define EXC_SUN3_LINE_1010		0x28
#define EXC_SUN3_LINE_1111		0x2c

/*
 *	EXC_SOFTWARE
 */

#define EXC_SUN3_CHK	0x18
#define EXC_SUN3_TRAPV	0x1c

/*
 *	EXC_BREAKPOINT
 */

#define EXC_SUN3_TRACE			0x24
#define EXC_SUN3_BREAKPOINT		0xbc

#endif	/* _MACH_SUN3_EXCEPTION_H_ */


/*
 *	Machine dependent exceptions for the vax
 * 	Revision 2.4  89/03/09  20:24:40  rpd
 */

/*
 *	EXC_BAD_INSTRUCTION
 */

#ifndef	_MACH_VAX_EXCEPTION_H_
#define _MACH_VAX_EXCEPTION_H_

#define EXC_VAX_PRIVINST		1
#define EXC_VAX_RESOPND			2
#define EXC_VAX_RESADDR			3
#define EXC_VAX_COMPAT			4

/*
 *	COMPAT subcodes
 */
#define EXC_VAX_COMPAT_RESINST		0
#define EXC_VAX_COMPAT_BPT		1
#define EXC_VAX_COMPAT_IOT		2
#define EXC_VAX_COMPAT_EMT		3
#define EXC_VAX_COMPAT_TRAP		4
#define EXC_VAX_COMPAT_RESOP		5
#define EXC_VAX_COMPAT_ODDADDR		6

/*
 *	EXC_ARITHMETIC
 */

#define EXC_VAX_INT_OVF			1
#define EXC_VAX_INT_DIV			2
#define EXC_VAX_FLT_OVF_T		3
#define EXC_VAX_FLT_DIV_T		4
#define EXC_VAX_FLT_UND_T		5
#define EXC_VAX_DEC_OVF			6

#define EXC_VAX_FLT_OVF_F		8
#define EXC_VAX_FLT_DIV_F		9
#define EXC_VAX_FLT_UND_F		10

/*
 *	EXC_SOFTWARE
 */

#define EXC_VAX_SUB_RNG			7

/*
 *	EXC_BREAKPOINT
 */

#define EXC_VAX_BPT			1
#define EXC_VAX_TRACE			2

#endif	/* _MACH_VAX_EXCEPTION_H_ */

/*
 *	Machine dependent exception definitions for Sun 4
 *	Revision 2.2.1.1  89/10/23  14:03:18  jjc
 */
#ifndef	_MACH_SUN4_EXCEPTION_H_
#define _MACH_SUN4_EXCEPTION_H_

/*
 *	EXC_BAD_INSTRUCTION
 */
#define	EXC_SUN4_UNIMPLEMENTED		0x2
#define	EXC_SUN4_PRIVILEGE_VIOLATION	0x3
#define	EXC_SUN4_WINDOW_OVERFLOW	0x5
#define	EXC_SUN4_ALIGNMENT		0x7

/*
 *	EXC_ARITHMETIC
 */
#define EXC_SUN4_FLT_EXCEPTION		0x8
#define EXC_SUN4_ZERO_DIVIDE		0x82

/*
 *	EXC_EMULATION
 */
#define	EXC_SUN4_TAG_OVERFLOW		0xa

/*
 *	EXC_BREAKPOINT
 */
#define	EXC_SUN4_BREAKPOINT	0x81	/* T_BREAKPOINT from sun4/trap.h */

#endif	/* _MACH_SUN4_EXCEPTION_H_ */


/*
 *	Machine dependent exceptions for mips
 * 	Revision 2.1  89/05/30  16:55:45  rvb
 */

#ifndef	_MACH_MIPS_EXCEPTION_H_
#define	_MACH_MIPS_EXCEPTION_H_

/*
 *	Codes and subcodes for Mips exceptions.
 */


/*
 *	Hardware level exceptions
 */

#define	EXC_MIPS_INT		0	/* interrupt */
#define	EXC_MIPS_MOD		1	/* TLB mod */
#define	EXC_MIPS_RMISS		2	/* Read TLB Miss */
#define	EXC_MIPS_WMISS		3	/* Write TLB Miss */
#define	EXC_MIPS_RADE		4	/* Read Address Error */
#define	EXC_MIPS_WADE		5	/* Write Address Error */
#define	EXC_MIPS_IBE		6	/* Instruction Bus Error */
#define	EXC_MIPS_DBE		7	/* Data Bus Error */
#define	EXC_MIPS_SYSCALL	8	/* SYSCALL */
#define	EXC_MIPS_BREAK		9	/* BREAKpoint */
#define	EXC_MIPS_II		10	/* Illegal Instruction */
#define	EXC_MIPS_CPU		11	/* CoProcessor Unusable */
#define	EXC_MIPS_OV		12	/* OVerflow */

/*
 *	Software exception codes
 */
#define	EXC_MIPS_SOFT_SEGV	16	/* Software detected seg viol */
#define	EXC_MIPS_SOFT_CPU	19	/* coprocessor unusable */


/*
 *	Bad instruction subcodes
 */

#define	EXC_MIPS_PRIVINST		1
#define	EXC_MIPS_RESOPND		2
#define	EXC_MIPS_RESADDR		3

/*
 *	EXC_ARITHMETIC subcodes
 */

#define	EXC_MIPS_FLT_UNIMP	1
#define	EXC_MIPS_FLT_INVALID	2
#define	EXC_MIPS_FLT_DIVIDE0	3
#define	EXC_MIPS_FLT_OVERFLOW	4
#define	EXC_MIPS_FLT_UNDERFLOW	5
#define	EXC_MIPS_FLT_INEXACT	6

/*
 *	EXC_BREAKPOINT subcodes
 */

#define	EXC_MIPS_BPT			1
#define EXC_MIPS_TRACE			2


#endif	/* _MACH_MIPS_EXCEPTION_H_ */

/*
 *	Codes and subcodes for ns32000 exceptions.
 *	Revision 2.5  89/03/09  20:21:39  rpd
 */

#ifndef	_MACH_MMAX_EXCEPTION_H_
#define _MACH_MMAX_EXCEPTION_H_

/*
 *	EXC_BAD_INSTRUCTION
 */

#define EXC_NS32K_FPU			3
#define EXC_NS32K_ILL			4
#define EXC_NS32K_UND			10

/*
 *	EXC_ARITHMETIC
 */

#define EXC_NS32K_FPU			3
#define EXC_NS32K_DVZ			6

/*
 *	FPU subcodes
 */

#define EXC_NS32K_FPU_UNDERFLOW 	1
#define EXC_NS32K_FPU_OVERFLOW		2
#define EXC_NS32K_FPU_DVZ		3
#define EXC_NS32K_FPU_ILLEGAL 		4
#define EXC_NS32K_FPU_INVALID		5
#define EXC_NS32K_FPU_INEXACT		6
#define EXC_NS32K_FPU_OPERAND		7

/*
 *	EXC_SOFTWARE
 */

#define EXC_NS32K_FLG			7

/*
 *	EXC_BREAKPOINT
 */

#define EXC_NS32K_BPT			8
#define EXC_NS32K_TRC			9

#endif	/* _MACH_MMAX_EXCEPTION_H_ */


#ifndef	_MACH_I386_EXCEPTION_H_
#define _MACH_I386_EXCEPTION_H_

#define EXC_I386_INVOP			1

/*
 *	EXC_ARITHMETIC
 */

#define EXC_I386_DIV			1
#define EXC_I386_INTO			2
#define EXC_I386_NOEXT			3
#define EXC_I386_EXTOVR			4
#define EXC_I386_EXTERR			5
#define EXC_I386_EMERR			6
#define EXC_I386_BOUND			7

/*
 *	EXC_SOFTWARE
 */

/*
 *	EXC_BAD_ACCESS
 */

/*
 *	EXC_BREAKPOINT
 */

#define EXC_I386_SGL			1
#define EXC_I386_BPT			2

#define EXC_I386_DIVERR		0	/* divide by 0 eprror		*/
#define EXC_I386_SGLSTP		1	/* single step			*/
#define EXC_I386_NMIFLT		2	/* NMI				*/
#define EXC_I386_BPTFLT		3	/* breakpoint fault		*/
#define EXC_I386_INTOFLT	4	/* INTO overflow fault		*/
#define EXC_I386_BOUNDFLT	5	/* BOUND instruction fault	*/
#define EXC_I386_INVOPFLT	6	/* invalid opcode fault		*/
#define EXC_I386_NOEXTFLT	7	/* extension not available fault*/
#define EXC_I386_DBLFLT		8	/* double fault			*/
#define EXC_I386_EXTOVRFLT	9	/* extension overrun fault	*/
#define EXC_I386_INVTSSFLT	10	/* invalid TSS fault		*/
#define EXC_I386_SEGNPFLT	11	/* segment not present fault	*/
#define EXC_I386_STKFLT		12	/* stack fault			*/
#define EXC_I386_GPFLT		13	/* general protection fault	*/
#define EXC_I386_PGFLT		14	/* page fault			*/
#define EXC_I386_EXTERRFLT	16	/* extension error fault	*/
#define EXC_I386_ENDPERR	33	/* emulated extension error flt	*/
#define EXC_I386_ENOEXTFLT	32	/* emulated ext not present	*/

#endif	/* _MACH_I386_EXCEPTION_H_ */

#ifndef _MACH_MAC2_EXCEPTION_H
#define _MACH_MAC2_EXCEPTION_H

/*
 * Machine dependent exception definitions for the Macintosh II.
 * This should agree with /usr/include/mac2/exception.h
 */

/*
 * EXC_BAD_INSTRUCTION
 */
#define	EXC_MAC2_ILLEGAL_INSTRUCTION	0x10
#define EXC_MAC2_PRIVILEGE_VIOLATION	0x20
#define EXC_MAC2_COPROCESSOR		0x34
#define EXC_MAC2_TRAP0			0x80
#define EXC_MAC2_TRAP1			0x84
#define EXC_MAC2_TRAP2			0x88
#define EXC_MAC2_TRAP3			0x8c
#define EXC_MAC2_TRAP4			0x90
#define EXC_MAC2_TRAP5			0x94
#define EXC_MAC2_TRAP6			0x98
#define EXC_MAC2_TRAP7			0x9c
#define EXC_MAC2_TRAP8			0xa0
#define EXC_MAC2_TRAP9			0xa4
#define EXC_MAC2_TRAP10			0xa8
#define EXC_MAC2_TRAP11			0xac
#define EXC_MAC2_TRAP12			0xb0
#define EXC_MAC2_TRAP13			0xb4
#define EXC_MAC2_TRAP14			0xb8
#define EXC_MAC2_TRAP15			0xbc
#define EXC_MAC2_FLT_BSUN		0xc0
#define EXC_MAC2_FLT_OPERAND_ERROR	0xd0

/*
 * EXC_ARITHMETIC
 */
#define EXC_MAC2_ZERO_DIVIDE		0x14
#define EXC_MAC2_FLT_INEXACT		0xc4
#define EXC_MAC2_FLT_ZERO_DIVIDE	0xc8
#define EXC_MAC2_FLT_UNDERFLOW		0xcc
#define EXC_MAC2_FLT_OVERFLOW		0xd4
#define EXC_MAC2_FLT_NOT_A_NUMBER	0xd8

/*
 * EXC_EMULATION
 */
#define EXC_MAC2_LINE_1010		0x28
#define EXC_MAC2_LINE_1111		0x2c

/*
 * EXC_SOFTWARE
 */
#define	EXC_MAC2_CHK			0x18
#define EXC_MAC2_TRAPV			0x1c

/*
 * EXC_BREAKPOINT
 */
#define EXC_MAC2_TRACE			0x24
#define EXC_MAC2_BREAKPOINT		0xbc

#endif	/* _MACH_MAC2_EXCEPTION_H */

#endif	/* _EXCEPTION_ALL_H_ */
