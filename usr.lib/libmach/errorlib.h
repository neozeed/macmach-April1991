/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	errorlib.h,v $
 * Revision 1.1  90/02/18  17:47:53  bww
 * 	Mach Release 2.5
 * 	[90/02/18  17:47:49  bww]
 * 
 * Revision 2.1  89/06/13  16:51:13  mrt
 * Created.
 * 
 * Revision 1.2  89/05/05  18:41:26  mrt
 * 	Added definition of SERV_NETMEMORY_MOD
 * 
 * 	16-May-88	Mary R. Thompson (mrt) at Carnegie Mellon
 * 	[89/04/29            mrt]
 * 
 *	Corrected the definitions of IPC_RCV_MOD and IPC_SEND_MOD
 *
 * 09-Mar-88	Douglas Orr (dorr) at Carnegie-Mellon University
 *	created.
 */
/*
 * File:	errrolib.h
 * Purpose:
 *	Generic error code interface
 */

#ifndef _ERRORLIB_H_
#define _ERRORLIB_H_

#include <mach/error.h>

#define	IPC_SEND_MOD		(err_ipc|err_sub(0))
#define	IPC_RCV_MOD		(err_ipc|err_sub(1))
#define	IPC_MIG_MOD		(err_ipc|err_sub(2))

#define	SERV_NETNAME_MOD	(err_server|err_sub(0))
#define	SERV_ENV_MOD		(err_server|err_sub(1))
#define	SERV_EXECD_MOD		(err_server|err_sub(2))
#define	SERV_NETMEMORY_MOD	(err_server|err_sub(3))


#define	NO_SUCH_ERROR		"unknown error code"

struct error_subsystem {
	char			* subsys_name;
	int			max_code;
	char			* * codes;
};

struct error_system {
	int			max_sub;
	char			* bad_sub;
	struct error_subsystem	* subsystem;
};

extern	struct error_system 	errors[err_max_system+1];

#define	errlib_count(s)		(sizeof(s)/sizeof(s[0]))

#endif /* _ERRORLIB_H_ */
