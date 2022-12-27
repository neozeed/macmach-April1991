/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	error_codes.c,v $
 * Revision 1.1  90/02/19  18:07:51  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:41:18  mrt
 * 	09-Mar-88	Douglas Orr (dorr) at Carnegie-Mellon University
 * 
 *	created.
 */
/*
 * File:	errr_codes.c
 * Purpose:
 *	Generic error code interface
 */

#include <mach/error.h>
#include <servers/errorlib.h>
#include "err_kern.sub"
#include "err_us.sub"
#include "err_server.sub"
#include "err_ipc.sub"


struct error_system errors[err_max_system+1] = {
	/* 0; err_kern */
	{
		errlib_count(err_os_sub),
		"(operating system/?) unknown subsystem error",
		err_os_sub,
	},
	/* 1; err_us */
	{
		errlib_count(err_us_sub),
		"(user space/?) unknown subsystem error",
		err_us_sub,
	},
	/* 2; err_server */
	{
		errlib_count(err_server_sub),
		"(server/?) unknown subsystem error",
		err_server_sub,
	},
	/* 3 (& 3f); err_ipc */
	{
		errlib_count(err_ipc_sub),
		"(ipc/?) unknown subsystem error",
		err_ipc_sub,
	},
};


int error_system_count = errlib_count(errors);


