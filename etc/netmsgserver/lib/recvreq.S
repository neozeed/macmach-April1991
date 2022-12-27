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
 * $Log:	recvreq.c,v $
 * Revision 1.4  89/05/02  11:05:21  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 */

#include "SYS.h"
#include <sys/syscall.h>

SYSCALL(recvreq)
	rts

