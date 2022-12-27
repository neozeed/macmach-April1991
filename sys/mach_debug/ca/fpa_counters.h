/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	fpa_counters.h,v $
 * Revision 2.5  89/03/09  20:25:37  rpd
 * 	More cleanup.
 * 
 * Revision 2.4  89/02/25  18:43:00  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.3  89/02/15  22:59:29  rpd
 * 	Moved to mach_debug/ca/.
 * 
 * Revision 2.2  89/01/12  07:39:58  rpd
 * 	Renamed fpa_counters_t as ibmrt_fpa_counters_t.
 * 	[89/01/12  06:41:01  rpd]
 * 	
 * 	Created.
 * 	[89/01/12  04:18:42  rpd]
 * 
 */ 

#ifndef	_MACH_DEBUG_CA_FPA_COUNTERS_H_
#define _MACH_DEBUG_CA_FPA_COUNTERS_H_

/*
 *	Remember to update the mig type definition
 *	in mach_debug_ca_types.defs when adding/removing fields.
 */

typedef struct ibmrt_fpa_counters {
	int pci_total;		/* total # of times fpa_pci called */
	int pci_kernel;		/* # of times trapped in kernel */
	int pci_from_save;	/* # of times trapped in save_context */
	int pci_info_ret;	/* # of times would return due to info */
	int pci_has_fpa;	/* # of times have hardware/regs */
	int pci_dma;		/* # of times page fault on dma op */
	int pci_dma_carry;	/* # of times need to add carry to addr */
	int pci_dma_ahead;	/* # of times addr was ahead by 4 */
	int pci_dma_except;	/* # of times fault results in exception */
	int pci_not_fpa;	/* # of times not FPA_TASK_EXCEPTION */
	int pci_no_reg;		/* # of times due to no regs */
	int pci_no_reg_ok;	/* # of times got reg set */
	int pci_sowefpa;	/* # of times SOWEFPA */
	int pci_resume;		/* # of times exception happened in resume */
	int pci_illegal;	/* # of times invalid ref to F or illegal */
	int pci_fixed_up;	/* # of times fixed up IEEE exception */
	int pci_not_fixed;	/* # of times exception not fixed */
	int pci_dir_check;	/* # of times check for direct mode */
	int pci_dir_user;	/* # of times in USER mode */
	int pci_dir_fault;	/* # of times faulted reading iar */
	int pci_dir_simul;	/* # of times simulate direct mode */
	int pci_dir_nosimul;	/* # of times didn't simulate direct mode */
	int pci_unrelated;	/* # of times wasn't FPA related */

	int bump;		/* # of times bumped someone */
	int grabreg;		/* # of times grabbed registers */
} ibmrt_fpa_counters_t;

#ifdef	KERNEL
#include <romp_fpa_count.h>

#if	ROMP_FPA_COUNT
extern ibmrt_fpa_counters_t fpa_counters;

#define fpa_counter(name)	fpa_counters.name++
#else	ROMP_FPA_COUNT
#define fpa_counter(name)
#endif	ROMP_FPA_COUNT

#endif	KERNEL
#endif	_MACH_DEBUG_CA_FPA_COUNTERS_H_
