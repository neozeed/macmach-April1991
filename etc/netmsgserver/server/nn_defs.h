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
 * $Log:	nn_defs.h,v $
 * Revision 1.1  90/02/19  21:54:27  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.6  89/05/02  11:14:43  dpj
 * 	Fixed all files to conform to standard copyright/log format
 * 
 * 26-Mar-88  Daniel Julin (dpj) at Carnegie-Mellon University
 *	Converted to use the new memory management module.
 *
 * 27-Apr-87  Robert Sansom (rds) at Carnegie Mellon University
 *	Placed lock and condition inline in request record.
 *	Changed nn_table to consist of inline lock_queue records.
 *
 * 23-Dec-86  Robert Sansom (rds) at Carnegie Mellon University
 *	Started.
 *
 */
/*
 * nn_defs.h
 *
 *
 * $ Header: nn_defs.h,v 1.6 89/05/02 11:14:43 dpj Exp $
 *
 */

/*
 * Internal definitions for the network name service module.
 */


#ifndef	_NN_DEFS_
#define	_NN_DEFS_

#include <ctype.h>

#include "mem.h"
#include "disp_hdr.h"
#include "lock_queue.h"
#include "netname_defs.h"
#include "port_defs.h"

typedef struct nn_entry {
    struct nn_entry	*next;
    netname_name_t	nne_name;
    port_t		nne_port;
    port_t		nne_signature;
} nn_entry_t, *nn_entry_ptr_t;

#define NN_TABLE_SIZE	32
extern struct lock_queue	nn_table[NN_TABLE_SIZE];

#define NN_NAME_HASH(index, name) { 			\
    register char *cp = (char *)(name);			\
    (index) = 0;					\
    while ((*cp) != '\0') { (index) += *cp; cp++;}	\
    (index) = (index) % NN_TABLE_SIZE;			\
}

#define NN_CONVERT_TO_UPPER(name) {			\
    register char *cp = (char *)(name);			\
    while ((*cp) != '\0') {				\
	if (islower(*cp)) *cp = toupper(*cp);		\
	cp++;						\
    }							\
}


/*
 * Structures used to make and remember about network name requests.
 */
typedef struct {
    disp_hdr_t		nnr_disp_hdr;
    netname_name_t	nnr_name;
    network_port_t	nnr_nport;
} nn_req_t, *nn_req_ptr_t;

typedef struct {
    struct condition	nnrr_condition;
    struct mutex	nnrr_lock;
    port_t		nnrr_lport;
    int			nnrr_result;
} nn_req_rec_t, *nn_req_rec_ptr_t;


/*
 * Functions public to the network name service module.
 */
extern nn_handle_request();
extern nn_handle_reply();
extern nn_name_test();
extern nn_network_look_up();

/*
 * Memory management definitions.
 */
extern mem_objrec_t		MEM_NNREC;


#endif	_NN_DEFS_
