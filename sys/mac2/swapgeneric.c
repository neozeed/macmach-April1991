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
 * $Log:	swapgeneric.c,v $
 * Revision 2.2  90/08/30  11:03:08  bohman
 * 	Created.
 * 	[90/08/29  11:49:53  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/swapgeneric.c
 */

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/buf.h>
#include <sys/vm.h>
#include <sys/systm.h>
#include <sys/reboot.h>
#include <sys/file.h>

/*
 * Generic configuration;  all in one
 */
dev_t	rootdev;

setconf()
{
    rootdev = (dev_t)bootdev;
}

/*
 * Called from
 * bsd/kern_exec.c: load_init_program()
 */
gets()
{
    return (0);
}
