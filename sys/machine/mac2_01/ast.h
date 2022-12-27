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
 * $Log:	ast.h,v $
 * Revision 2.2  90/08/30  11:00:33  bohman
 * 	Created.
 * 	[90/08/29  10:57:27  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ast.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2_AST_H_
#define _MAC2_AST_H_

#include <kern/macro_help.h>
#include <kern/thread.h>

#include <mac2/pcb.h>

/*
 * ast_context sets up ast context for this thread on
 * the specified cpu (always the current cpu).  On the
 * mac2 this goes in the pcb.
 */
#define	ast_context(thread, cpu)		\
MACRO_BEGIN					\
    if ((thread)->ast)				\
	(thread)->pcb->pcb_ast |= AST_SCHED;	\
    else					\
	(thread)->pcb->pcb_ast &= ~AST_SCHED; 	\
MACRO_END

/*
 * ast_propagate: cause an ast if needed.
 */
#define ast_propagate(thread, cpu)		\
MACRO_BEGIN					\
    if ((thread)->ast)				\
	(thread)->pcb->pcb_ast |= AST_SCHED;	\
MACRO_END

/*
 * ast_needed: Is an ast already pending?
 */
#define ast_needed(cpu)	\
    ((current_thread_pcb()->pcb_ast&AST_SCHED) == AST_SCHED)

#endif _MAC2_AST_H_
