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
 * $Log:	autoconf.c,v $
 * Revision 2.2  90/08/30  11:00:36  bohman
 * 	Created.
 * 	[90/08/29  10:59:15  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/autoconf.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <sys/types.h>

#include <mach/vm_param.h>

#include <mac2/autoconf.h>
#include <mac2/cpu.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/ROMDefs.h>
#include <mac2slotmgr/Slots.h>

#include <mac2slotmgr/slotmgr.h>

extern label_t *nofault;

/*
 * Configure all non-slot
 * devices from configuration-
 * dependent table in ioconf.c.
 */ 
config_devs()
{
    register struct devconf *dp;

    for (dp = devconf; dp->dc_name; dp++) {
	if (dp->dc_csr == 0) {
	    if ((*dp->dc_init)(dp) == 0)
	    	continue;
	} else if (probeb(dp->dc_csr) == 0) {
	    if ((*dp->dc_init)(dp) == 0)
	    	continue;
	} else
	    continue;

	if (dp->dc_vec) {
	    if (dp->dc_intr)
		ivect_tbl[dp->dc_vec/sizeof (*ivect_tbl)].vector = dp->dc_intr;

	    printf("%s%d at csr %x vector %x\n",
		   dp->dc_name,
		   dp->dc_unit,
		   dp->dc_csr,
		   dp->dc_vec);
	} else
	    printf("%s%d at csr %x\n",
		   dp->dc_name,
		   dp->dc_unit,
		   dp->dc_csr);
    }
}

/*
 * Initalize the slot manager.
 */
config_slotmgr()
{
    SpBlock slot_params;

    InitSDeclMgr(&slot_params);

    InitsRsrcTable(&slot_params);

    InitPRAMRecs(&slot_params);

    slot_params.spFlags = 0;
    SPrimaryInit(&slot_params);
}

/*
 * Configure all slot devices.
 */
config_slots()
{
    register slot, id;
    register unsigned short category, ctype;
    register SInfoRecPtr si;
    SpBlock slot_params;

    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	si = &slot_to_slotdata_ptr(slot)->SInfoRec;
	if (si->siInitStatusA < 0 && si->siInitStatusA != smEmptySlot)
	    printf("defective board: slot %x error %d\n",
		   slot, si->siInitStatusA);
    }

    if (SFirstsRsrc(&slot_params) != noErr)
	return;

    for (;;) {
	slot = slot_params.spSlot;
	id = slot_params.spID;
	category = slot_params.spCategory;
	ctype = slot_params.spCType;

	if (slot_params.spIOReserved&SRsrcIOReserved_Ignore)
	    config_ignored_sRsrc(&slot_params);
	else
	if (category == catBoard || category == catTest)
	    ;
	else
	if (category == catDisplay && ctype == typeVideo) {
	    if (video_config(slot, id) == FALSE)
		config_ignored_sRsrc(&slot_params);
	}
	else
	if (category == catNetwork && ctype == typeEtherNet) {
	    if (enattach(slot, id) == FALSE)
		config_ignored_sRsrc(&slot_params);
	}
	else
	    config_ignored_sRsrc(&slot_params);

	if (SNextsRsrc(&slot_params) != noErr)
	    break;
    }
}

config_ignored_sRsrc(p)
SpBlockPtr p;
{
    printf("ignored sRsrc: slot %x id %x category %x ctype %x\n",
	   p->spSlot, p->spID, p->spCategory, p->spCType);
}

/*
 * Try to read a byte from the
 * location addr.  Returns 1 on error
 * 0 otherwise.  Uses nofault mechanism
 * in trap.c.
 */
label_t	    probe_vec;

probeb(addr)
volatile char *addr;
{
    register tmp;

    nofault = &probe_vec;
    if (setjmp(nofault))
	return (1);

    tmp = *addr;
    nofault = 0;
    return (0);
}
