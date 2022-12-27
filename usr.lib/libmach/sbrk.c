/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * 10-Apr-90  Mary Thompson (mrt) at Carnegie-Mellon University
 *	Added asm(".data") for all assembly cases
 *
 * $Log:	sbrk.c,v $
 * Revision 1.4  90/01/25  15:34:43  mrt
 * 	Delinted call to vm_allocate
 * 	[90/01/25            mrt]
 * 
 * Revision 1.3  89/08/01  18:08:41  mrt
 * 	Mips support.
 * 	[89/07/12            af]
 * 
 * Revision 1.2  89/05/05  18:46:50  mrt
 * 	Changes to use new include files and general cleanup.
 * 	[89/04/25            mrt]
 * 
 * 20-Oct-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Changed sbrk to vm_allocate at curbrk if possible
 *	to be compatible with the Unix sbrk on rts.
 *
 *  6-Apr-87  Mary Thompson (mrt) at Carnegie Mellon
 *	Added conditional defintions of curbrk and minbrk for
 *	lint.
 *
 *
 *  1-Dec-86  David L. Black (dlb) at Carnegie-Mellon University
 *	Multimax bug fix; put minbrk and curbrk in .data section.
 *
 *
 * 16-Oct-86  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Fixed case where sbrk(0) was being mishandled; put in
 *	initialization to get break value right before first call.
 *
 *  2-Jun-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Created.
 *
 */
/*
 *	File:	sbrk.c
 *
 *	Unix compatibility for sbrk system call.
 */

#define  EXPORT_BOOLEAN
#include <mach.h>		/* for vm_allocate, vm_offset_t */
#include <stdio.h>		/* for stderr */
#include <sys/types.h>		/* for caddr_t */
#include <mach_init.h>		/* for vm_page_size */

#if	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined(i386)) || defined(mac2)
#if	multimax
DEF_FUNC()
{
#endif	multimax
asm(".data");
asm(".globl	curbrk");
asm(".globl	minbrk");
asm(".globl	_curbrk");
asm(".globl	_minbrk");
asm(".globl	_end");
#if	multimax
asm("_minbrk:");
asm("minbrk:	.double	_end");
asm("_curbrk:");
asm("curbrk:	.double	_end");
asm(".text");
}
#else	multimax
asm("_minbrk:");
asm("minbrk:	.long	_end");
asm("_curbrk:");
asm("curbrk:	.long	_end");
asm(".text");
#endif	multimax
#else	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined (i386))

/* Will not find get "assembler" forms of cubrk, minbrk. */

#ifdef	mips
extern char end;
#define curbrk _curbrk
#define minbrk _minbrk
caddr_t curbrk = &end;
caddr_t minbrk = &end;
#endif	mips

#endif	(defined(vax) || defined(ibmrt) || defined(ns32000) || defined(sun) || defined(i386))

#ifdef lint
   /* lint doesn't see asm stuff */
caddr_t	curbrk;
caddr_t	minbrk;
#else lint
extern caddr_t curbrk;
extern caddr_t minbrk;
#endif lint

#define	roundup(a,b)	((((a) + (b) - 1) / (b)) * (b))

static int sbrk_needs_init = FALSE;

caddr_t sbrk(size)
	int	size;
{
	vm_offset_t	addr;
	kern_return_t	ret;
	caddr_t		ocurbrk;

	if (sbrk_needs_init) {
		sbrk_needs_init = FALSE;
		/*
		 *	Get "curbrk"
		 */

	}
	
	if (size <= 0)
		return(curbrk);
	addr = (vm_offset_t) roundup((int)curbrk,vm_page_size);
	ocurbrk = curbrk;
	if (((int)curbrk+size) > addr)
	{	ret = vm_allocate(task_self(), &addr, 
			    (vm_size_t) size -((int)addr-(int)curbrk), FALSE);
		if (ret == KERN_NO_SPACE) {
			ret = vm_allocate(task_self(), &addr, (vm_size_t) size, TRUE);
			ocurbrk = (caddr_t)addr;
		}
		if (ret != KERN_SUCCESS) 
			return((caddr_t) -1);
	}

	curbrk = (caddr_t)ocurbrk + size;
	return(ocurbrk);

}

void brk(x)
	caddr_t x;
{
	fprintf(stderr, "brk: not implemented\n");
}
