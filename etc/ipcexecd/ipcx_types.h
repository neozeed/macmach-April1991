/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	ipcx_types.h,v $
 * Revision 1.1  90/02/19  21:47:39  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/07/04  17:37:03  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*
 * Abstract: ipcx_types.h
 *	declarations for ipcexecd 
 */
#ifndef _ipcx_types
#define _ipcx_types

typedef char a_string[256];

#define DEFAULT_PREFIX	"Mach-"

#define IPC_ERROR_BASE  	27600
#define IPCX_FILENOTFOUND 	(IPC_ERROR_BASE + 0)
#define IPCX_USERWRONG	 	(IPC_ERROR_BASE + 1)
#define IPCX_FORKFAILED 	(IPC_ERROR_BASE + 2)

#endif /* _ipcx_types */

