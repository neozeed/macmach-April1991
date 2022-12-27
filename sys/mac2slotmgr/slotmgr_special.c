/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_special.c,v $
 * Revision 2.2  90/08/30  11:48:00  bohman
 * 	Created.
 * 	[90/08/29  13:05:06  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_special.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Special Slot Manager
 * routines.  See
 * "Inside Macintosh V".
 */

#include <mach/mach_types.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

SReadByte(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    unsigned char bl;
    struct rom_idoffset lstent;

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    spBlkPtr->spResult = (lstent.offset & 0xff);

    return (noErr);
}

SReadWord(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    unsigned char bl;
    struct rom_idoffset lstent;

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    spBlkPtr->spResult = (lstent.offset & 0xffff);

    return (noErr);
}

SReadLong(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    long data;
    unsigned char bl;
    struct rom_idoffset lstent;

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(lp, sizeof (long), &data, bl);

    spBlkPtr->spResult = data;

    return (noErr);
}

SGetCString(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    unsigned char bl;
    struct rom_idoffset lstent;
    char cstring[255];

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(lp, sizeof (cstring), cstring, bl);

    {
	register char *sp;
	register len;

	for (sp = cstring; sp < (cstring + sizeof (cstring) - 1); sp++) {
	    if (*sp == 0)
		break;
	}
	*sp = 0;

	len = sp - cstring;

	sp = (char *)NewPtr(len);

	bcopy(cstring, sp, len);

	spBlkPtr->spResult = (vm_offset_t)sp;
    }

    return (noErr);
}

SGetBlock(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    unsigned long size;
    unsigned char bl;
    struct rom_idoffset lstent;

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(lp, sizeof (long), &size, bl);

    lp = slot_pointer_offset(lp, sizeof (long), bl);

    {
	register vm_offset_t p;

	p = NewPtr(size - sizeof (long));
	slot_get_data(lp, size - sizeof (long), p, bl);
	spBlkPtr->spResult = p;
    }

    return (noErr);
}

SFindStruct(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    unsigned char bl;
    struct rom_idoffset lstent;

    lp = spBlkPtr->spsPointer;
    if (slot_get_bytelane(lp, &bl) == 0)
	return (smBLFieldBad);

    id = spBlkPtr->spID;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == id || lstent.id == endOfList)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    spBlkPtr->spsPointer = lp;

    return (noErr);
}

SReadStruct(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t p;
    unsigned char bl;

    p = spBlkPtr->spsPointer;
    if (slot_get_bytelane(p, &bl) == 0)
	return (smBLFieldBad);

    slot_get_data(p, spBlkPtr->spSize, (vm_offset_t)spBlkPtr->spResult, bl);

    return (noErr);
}

SReadInfo(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    bcopy((vm_offset_t)&slot_to_slotdata_ptr(slot)->SInfoRec,
	  (vm_offset_t)spBlkPtr->spResult,
	  sizeof (SInfoRecord));

    return (noErr);
}

SReadPRAMRec(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

#ifdef notdef
    if (slot_to_slotdata_ptr(slot)->SInfoRec.siStatusFlags & fCardIsChanged)
	return (smBadBoardId);
#endif

    bcopy((vm_offset_t)&slot_to_slotdata_ptr(slot)->SPRAMRec,
	  (vm_offset_t)spBlkPtr->spResult,
	  sizeof (SPRAMRecord));

    return (noErr);
}

SPutPRAMRec(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    bcopy(((vm_offset_t)spBlkPtr->spsPointer) + sizeof (short),
	  ((vm_offset_t)&slot_to_slotdata_ptr(slot)->SPRAMRec) +
	  sizeof (short),
	  sizeof (SPRAMRecord) - sizeof (short));

    return (noErr);
}

SReadFHeader(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register FHeaderRecPtr fh;
    register vm_offset_t p;
    register slot;
    unsigned char bl;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    p = slot_to_ptr(slot);

    fh = (FHeaderRecPtr)spBlkPtr->spResult;

    p = slot_get_bytelane(p, &bl);
    if (p == (vm_offset_t)0)
	return (smBLFieldBad);

    slot_get_fheader(p, fh, bl);

    return (noErr);
}

SCkCardStatus(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    return (noErr);
}

SFindDevBase(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp, p;
    register SRsrcLstEntPtr sr;
    register slot, id;
    register unsigned char bl;
    unsigned long offset;
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
	if (lstent.id == endOfList ||
	    lstent.id == minorBaseOS || lstent.id == majorBaseOS)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    lp = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(lp, sizeof (long), &offset, bl);

    if (lstent.id == minorBaseOS)
	p = slot_to_ptr(slot) + offset;
    else
	p = slot_to_super_ptr(slot) + offset;

    spBlkPtr->spResult = p;

    return (noErr);
}

SDeleteSRTRec(spBlkPtr)
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

	remqueue(q, qe);
    }

    DisposPtr((vm_offset_t)sr);

    return (noErr);
}

SPtrToSlot(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t p;
    register slot;

    p = spBlkPtr->spsPointer;
    if ((p & 0xf0000000) != 0xf0000000)
	return (smBadsPtrErr);

    slot = ((p >> 24) & 0xf);
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    spBlkPtr->spSlot = slot;

    return (noErr);
}
