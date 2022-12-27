/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	emdefs.h,v $
 * Revision 1.1  90/02/19  21:47:12  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/07/30  20:28:47  mrt
 * 	Made error code values start ate SERV_ENV_MOD+0 instead of +1 to
 * 	correspond to error_code routine.
 * 	[89/07/29            mrt]
 * 
 * Revision 1.3  89/05/09  22:00:38  mrt
 * 	Added ENV_PORT_NULL error code.
 * 	[89/05/06            mrt]
 * 
 * Revision 1.2  89/05/05  18:28:59  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 *  File: emdefs
 *	Exported definitions for Environment Maganger
 *  Author: Mary Thompson
 */

#ifndef _env_mgr_defs
#define _env_mgr_defs

#include <servers/errorlib.h>

#define env_name_size		(80)
#define env_val_size		(256)

#define ENV_SUCCESS			(KERN_SUCCESS)

#define ENV_VAR_NOT_FOUND	(SERV_ENV_MOD | 0)
#define ENV_WRONG_VAR_TYPE	(SERV_ENV_MOD | 1)
#define ENV_UNKNOWN_PORT	(SERV_ENV_MOD | 2)
#define ENV_READ_ONLY		(SERV_ENV_MOD | 3)
#define ENV_NO_MORE_CONN	(SERV_ENV_MOD | 4)
#define ENV_PORT_TABLE_FULL	(SERV_ENV_MOD | 5)
#define ENV_PORT_NULL		(SERV_ENV_MOD | 6)

typedef char	env_name_t[env_name_size];

typedef char	env_str_val_t[env_val_size];

typedef env_name_t *env_name_list; 	/* Variable sized array */

typedef env_str_val_t *env_str_list; 	/* Variable sized array */

#endif /* _env_mgr_defs */
