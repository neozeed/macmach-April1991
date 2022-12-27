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
 * $Log:	ptrace.h,v $
 * Revision 2.2  90/08/30  11:02:54  bohman
 * 	Created.
 * 	[90/08/29  11:44:40  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ptrace.h
 */

#ifndef _MAC2_PTRACE_H_
#define _MAC2_PTRACE_H_

/*
 * Request values for the ptrace system call
 */
enum ptracereq {
	PTRACE_TRACEME = 0,		/* 0, by tracee to begin tracing */
	PTRACE_CHILDDONE = 0,		/* 0, tracee is done with his half */
	PTRACE_PEEKTEXT,		/* 1, read word from text segment */
	PTRACE_PEEKDATA,		/* 2, read word from data segment */
	PTRACE_PEEKUSER,		/* 3, read word from user struct */
	PTRACE_POKETEXT,		/* 4, write word into text segment */
	PTRACE_POKEDATA,		/* 5, write word into data segment */
	PTRACE_POKEUSER,		/* 6, write word into user struct */
	PTRACE_CONT,			/* 7, continue process */
	PTRACE_KILL,			/* 8, terminate process */
	PTRACE_SINGLESTEP,		/* 9, single step process */
	PTRACE_GETSREGS,                /* 10, get sr & pc */
	PTRACE_SETSREGS,                /* 11, set sr & pc */
	PTRACE_GETREGS,			/* 12, get all registers */
	PTRACE_SETREGS,			/* 13, set all registers */
	PTRACE_GETFPREGS,		/* 14, get all floating point regs */
	PTRACE_SETFPREGS,		/* 15, set all floating point regs */
};

#endif _MAC2_PTRACE_H_
