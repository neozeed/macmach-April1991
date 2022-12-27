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
 * $Log:	ps_defs.h,v $
 * Revision 1.1  90/02/19  21:56:04  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.6  89/05/02  11:16:52  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 29-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Made psd_status field of ps_data_t an unsigned long.
 *	Added PS_DEBUG.
 *
 *  9-Feb-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * ps_defs.h
 *
 *
 * $ Header: ps_defs.h,v 1.6 89/05/02 11:16:52 dpj Exp $
 *
 */

/*
 * Internal definitions for the Port Search module.
 */


#ifndef	_PS_DEFS_
#define	_PS_DEFS_

#define PS_DEBUG		1

#include <mach/port.h>

#include "mem.h"
#include "disp_hdr.h"
#include "key_defs.h"
#include "nm_defs.h"
#include "port_defs.h"

/*
 * Structure used to send and reply to port search requests.
 */
typedef struct {
    disp_hdr_t		psd_disp_hdr;
    unsigned long	psd_status;
    np_uid_t		psd_puid;
    netaddr_t		psd_owner;
    netaddr_t		psd_receiver;
} ps_data_t, *ps_data_ptr_t;

/*
 * Status values.
 */
#define PS_OWNER_MOVED		1
#define PS_RECEIVER_MOVED	2
#define PS_PORT_DEAD		4
#define PS_PORT_UNKNOWN		8
#define PS_PORT_HERE		16


/*
 * Structure used to send and reply to authentication requests.
 */
typedef struct {
    disp_hdr_t		psa_disp_hdr;
    np_uid_t		psa_puid;
    secure_info_t	psa_token;
    long		psa_random;
} ps_auth_t, *ps_auth_ptr_t;


/*
 * Structure used to remember about port searches in progress.
 */
typedef struct {
    int		pse_state;
    netaddr_t	pse_destination;
    port_t	pse_lport;
    int		(*pse_retry)();
} ps_event_t, *ps_event_ptr_t;

/*
 * Possible event states.
 */
#define PS_OWNER_QUERIED	1
#define PS_RECEIVER_QUERIED	2
#define PS_DONE_BROADCAST	4
#define PS_AUTHENTICATION	8


/*
 * extern definitions internal to the Port Search module.
 */
extern ps_cleanup();
/*
ps_event_ptr_t	event_ptr;
int		reason;
*/


extern ps_handle_auth_reply();
/*
int		client_id;
sbuf_ptr_t	reply;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
*/

extern ps_handle_auth_request();
/*
sbuf_ptr_t	request;
netaddr_t	from;
boolean_t	broadcast;
int		crypt_level;
*/


extern ps_retry();
/*
ps_event_ptr_t	event_ptr;
*/

extern void ps_send_query();
/*
ps_event_ptr_t	event_ptr;
port_rec_ptr_t	port_rec_ptr;
*/

extern void ps_authenticate_port();
/*
ps_event_ptr_t	event_ptr;
port_rec_ptr_t	port_rec_ptr;
*/

/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_PSEVENT;


#endif	_PS_DEFS_
