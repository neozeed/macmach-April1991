/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_principal.c,v $
 * Revision 2.2  90/08/30  11:47:56  bohman
 * 	Created.
 * 	[90/08/29  13:04:49  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_principal.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Principal Slot Manager
 * routines.  See
 * "Inside Macintosh V".
 */

#include <mach/mach_types.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

SRsrcInfo(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register slot, id, extdev;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    id = spBlkPtr->spID;
    extdev = spBlkPtr->spExtDev;

    {
	register queue_t q;
	register queue_entry_t qe;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcId == id && sr->SRsrcExtDev == extdev)
		break;
	}

	if (queue_end(q, qe))
	    return (smRecNotFnd);
    }

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

SNextsRsrc(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register slot, id, extdev;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    id = spBlkPtr->spID;
    extdev = spBlkPtr->spExtDev;

    {
	register queue_t q;
	register queue_entry_t qe;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcExtDev == extdev &&
		(id == 0 || sr->SRsrcId == id))
		break;
	}

	if (queue_end(q, qe))
	    return (smRecNotFnd);

	qe = queue_next(qe);
	for (;;) {
	    if (!queue_end(q, qe))
		break;
	    else if (++slot <= SLOT_NUM_HIGH) {
		q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
		qe = queue_first(q);
		continue;
	    }
	    else
		return (smRecNotFnd);
	}
	sr = (SRsrcLstEntPtr)qe;
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

SNextTypesRsrc(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register slot, id, extdev, mask;
    register unsigned short category, ctype;
    unsigned short drvrsw, drvrhw;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    id = spBlkPtr->spID;
    extdev = spBlkPtr->spExtDev;

    {
	register queue_t q;
	register queue_entry_t qe;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcExtDev == extdev &&
		(id == 0 || sr->SRsrcId == id))
		break;
	}

	if (queue_end(q, qe))
	    return (smRecNotFnd);

	mask = spBlkPtr->spTBMask;
	category = spBlkPtr->spCategory;
	ctype = spBlkPtr->spCType;
	drvrsw = spBlkPtr->spDrvrSW;
	drvrhw = spBlkPtr->spDrvrHW;
	
	qe = queue_next(qe);
	for (;;) {
	    while (!queue_end(q, qe)) {
		sr = (SRsrcLstEntPtr)qe;
		if (((mask & 0x8) || sr->SRsrcType_Category == category) &&
		    ((mask & 0x4) || sr->SRsrcType_cType == ctype) &&
		    ((mask & 0x2) || sr->SRsrcType_DrvrSW == drvrsw) &&
		    ((mask & 0x1) || sr->SRsrcType_DrvrHW == drvrhw))
		    break;
		qe = queue_next(qe);
	    }
	    if (!queue_end(q, qe))
		break;
	    else if (++slot <= SLOT_NUM_HIGH) {
		q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
		qe = queue_first(q);
		continue;
	    }
	    else
		return (smRecNotFnd);
	}
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

SReadDrvrName(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register SRsrcLstEntPtr sr;
    register slot, id;
    register unsigned char bl;
    register StringPtr string;
    struct rom_idoffset lstent;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    id = spBlkPtr->spID;

    {
	register queue_t q;
	register queue_entry_t qe;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcId == id)
		break;
	}

	if (queue_end(q, qe))
	    return (smRecNotFnd);
    }

    bl = slot_to_slotdata_ptr(slot)->SInfoRec.siCPUByteLanes;

    lp = sr->SRsrcPtr;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == sRsrcName || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    string = (StringPtr)spBlkPtr->spResult;

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(lp, sizeof (string->text) - 1, &string->text[1], bl);

    {
	register i;

	for (i = 1; i < sizeof (string->text); i++)
	    if (string->text[i] == 0)
		break;

	string->text[0] = '.';
	string->length = i;
    }

    return (noErr);
}
