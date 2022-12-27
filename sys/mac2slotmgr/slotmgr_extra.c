/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_extra.c,v $
 * Revision 2.2  90/08/30  11:47:46  bohman
 * 	Created.
 * 	[90/08/29  13:04:18  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_extra.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Slot Manager
 * MACH kernel dependent routines.
 */

#include <mach/mach_types.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

/*
 * Check for the
 * existence of a
 * slot.
 */
slot_exists(p)
register vm_offset_t p;
{
    register i;

    p |= 0x00ffffff;
    for (i = 0; i < 4; i++, p--)
	if (probeb(p) == 0)
	    return (TRUE);

    return (FALSE);
}

/*
 * Return the first sRsrc
 * in the Slot Resource Table.
 */
SFirstsRsrc(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register slot;

    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	slot_check_status(slot, continue);

	{
	    register queue_t q;

	    q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	    if (queue_empty(q))
		continue;
	    sr = (SRsrcLstEntPtr)queue_first(q);
	}

	spBlkPtr->spSlot = slot;
	spBlkPtr->spID = sr->SRsrcId;
	spBlkPtr->spExtDev = sr->SRsrcExtDev;
	spBlkPtr->spsPointer = sr->SRsrcPtr;
	spBlkPtr->spIOReserved = sr->SRsrcIOReserved;
	spBlkPtr->spRefNum = 0;
	spBlkPtr->spCategory = sr->SRsrcType_Category;
	spBlkPtr->spCType = sr->SRsrcType_cType;
	spBlkPtr->spDrvrSW = sr->SRsrcType_DrvrSW;
	spBlkPtr->spDrvrHW = sr->SRsrcType_DrvrHW;
	spBlkPtr->spHwDev = sr->SRsrcHWDevID;

	return (noErr);
    }

    return (smRecNotFnd);
}
