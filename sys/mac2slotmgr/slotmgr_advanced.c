/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_advanced.c,v $
 * Revision 2.2  90/09/04  17:35:42  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_advanced.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Advanced Slot Manager
 * routines.  See
 * "Inside Macintosh V".
 */

#include <mach/mach_types.h>

#include <mach/mac2/vm_param.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/ROMDefs.h>

#include <mac2slotmgr/slotmgr.h>

struct slotdata slots[SLOT_NUM_HIGH-SLOT_NUM_LOW+1];

InitSDeclMgr(spBlkPtr)
SpBlockPtr spBlkPtr;
{
    register vm_offset_t p, ptr_bytelanes;
    register SInfoRecPtr si;
    register struct rom_idoffset *r;
    register slot, crc;
    FHeaderRec fheader;
    unsigned char bl;

    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
#define slot_fatal_error(err)		\
	{				\
	    si->siInitStatusA = (err);	\
	    continue;			\
	}

	si = &slot_to_slotdata_ptr(slot)->SInfoRec;

	p = slot_to_ptr(slot);

	if (!slot_exists(p))
	    slot_fatal_error(smEmptySlot);

	ptr_bytelanes = p = slot_get_bytelane(p, &bl);
	if (p == (vm_offset_t)0)
	    slot_fatal_error(smBLFieldBad);

	p = slot_get_fheader(p, &fheader, bl);
	if (fheader.fhTstPat != testPattern)
	    slot_fatal_error(smBLFieldBad);

	if (fheader.fhReserved)
	    slot_fatal_error(smResrvErr);

	if (fheader.fhFormat != appleFormat)
	    slot_fatal_error(smFormatErr);

	if (fheader.fhROMRev < 1 || fheader.fhROMRev > 9)
	    slot_fatal_error(smRevisionErr);

	crc = slot_calc_crc(ptr_bytelanes, fheader.fhLength, bl);
	if (crc != fheader.fhCRC)
	    slot_fatal_error(smCRCFail);

	r = (struct rom_idoffset *)&fheader.fhDirOffset;
	if (r->id != 0 || r->offset == 0)
	    slot_fatal_error(smNoDir);

	si->siDirPtr = slot_pointer_offset(p, r->offset, bl);
	si->siState = stateSDMInit;
	si->siCPUByteLanes = bl;
	si->siTopOfROM = (ptr_bytelanes & 0xf);
	si->siTOConst = defaultTO;
    }
#undef slot_fatal_error

    return (noErr);
}

SPrimaryInit(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register SInfoRecPtr si;
    register slot, result;
    SEBlock sExecPBlk;

    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	register queue_t q;
	register queue_entry_t qe;

	slot_check_status(slot, continue);

	si = &slot_to_slotdata_ptr(slot)->SInfoRec;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcType_Category == catBoard &&
		sr->SRsrcType_cType == typeBoard)
		break;
	}

	if (queue_end(q, qe))
	    continue;

	sExecPBlk.seSlot = slot;
	sExecPBlk.sesRsrcId = sr->SRsrcId;
	sExecPBlk.seFlags = spBlkPtr->spFlags;
	sExecPBlk.seStatus = 0;

	spBlkPtr->spsPointer = sr->SRsrcPtr;
	spBlkPtr->spID = primaryInit;
	spBlkPtr->spsExecPBlk = (vm_offset_t)&sExecPBlk;

	if ((result = SExec(spBlkPtr)) == 0) {
	    if ((si->siInitStatusV = sExecPBlk.seStatus) < 0)
		si->siInitStatusA = smIntStatVErr;
	}
	else if (result >= smEmptySlot && result <= smBusErrTO)
	    si->siInitStatusA = result;
    }

    return (noErr);
}

SCardChanged(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    if (slot_to_slotdata_ptr(slot)->SInfoRec.siStatusFlags & fCardIsChanged)
	spBlkPtr->spResult = TRUE;
    else
	spBlkPtr->spResult = FALSE;

    return (noErr);
}

SExec(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp;
    register id;
    register SExecHdrPtr eh;
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

    eh = (SExecHdrPtr)NewPtr(size - sizeof (long));

    lp = slot_pointer_offset(lp, sizeof (long), bl);

    slot_get_data(lp, size - sizeof (long), eh, bl);

    if (eh->rev != sCodeRev) {
	DisposPtr(eh);
	return (smCodeRevErr);
    }

    if (eh->CPU != sCPU68020) {
	DisposPtr(eh);
	return (smCPUErr);
    }

    SExecCode(((vm_offset_t)&eh->offset) + eh->offset,
	      spBlkPtr->spsExecPBlk);

    DisposPtr(eh);

    return (noErr);
}

SOffsetData(spBlkPtr)
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

    spBlkPtr->spOffsetData = lstent.offset;
    spBlkPtr->spByteLanes = bl;

    return (noErr);
}

SReadPBSize(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp, p;
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

    p = slot_pointer_offset(lp, lstent.offset, bl);

    slot_get_data(p, sizeof (long), &size, bl);

    if ((spBlkPtr->spFlags & fCkReserved) && (size & 0xff000000))
	return (smReservedErr);

    p = slot_pointer_offset(p, sizeof (long), bl);

    spBlkPtr->spsPointer = p;
    spBlkPtr->spSize = size;
    spBlkPtr->spByteLanes = bl;

    return (noErr);
}

SCalcStep(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register vm_offset_t p;
    register unsigned long offset;
    register unsigned char bl;

    p = spBlkPtr->spsPointer;
    bl = spBlkPtr->spByteLanes;

    if (((1 << (p & 0x3)) & bl) == 0)
	return (smBadsPtrErr);

    if (spBlkPtr->spFlags & fConsecBytes)
	offset = sizeof (char);
    else
	offset = sizeof (long);

    offset = slot_pointer_offset(p, offset, bl) - p;

    spBlkPtr->spResult = offset;

    return (noErr);
}

InitsRsrcTable(spBlkPtr)
SpBlockPtr spBlkPtr;
{
    register struct slotdata *sd;
    register SInfoRecPtr si;
    register SRsrcLstEntPtr sr;
    register vm_offset_t dp, lp;
    register slot;
    register unsigned char bl;
    struct rom_idoffset dirent, lstent;

    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	sd = slot_to_slotdata_ptr(slot);
	queue_init(&sd->SRsrcLst);

	slot_check_status(slot, continue);

	si = &sd->SInfoRec;
	bl = si->siCPUByteLanes;

	dp = si->siDirPtr;
	for (;;) {
	    slot_get_data(dp, sizeof (dirent), &dirent, bl);
	    if (dirent.id == endOfList)
		break;

	    sr = (SRsrcLstEntPtr)NewPtr(sizeof (SRsrcLstEnt));
	    bzero(sr, sizeof (*sr));
	    enqueue(&sd->SRsrcLst, &sr->SRsrcLst);

	    sr->SRsrcId = dirent.id;

	    lp = slot_pointer_offset(dp, dirent.offset, bl);
	    sr->SRsrcPtr = lp;

	    for (;;) {
		register vm_offset_t p;

		slot_get_data(lp, sizeof (lstent), &lstent, bl);
		if (lstent.id == endOfList)
		    break;

		switch (lstent.id) {
		  case sRsrcType:
		    p = slot_pointer_offset(lp, lstent.offset, bl);
		    slot_get_data(p,
				  sizeof (sr->SRsrcType),
				  &sr->SRsrcType,
				  bl);
		    break;

		  case sRsrcHWDevId:
		    sr->SRsrcHWDevID = (lstent.offset & 0xff);
		    break;
		}

		lp = slot_pointer_offset(lp, sizeof (lstent), bl);
	    }
	    dp = slot_pointer_offset(dp, sizeof (dirent), bl);
	}
    }

    return (noErr);
}

InitPRAMRecs(spBlkPtr)
SpBlockPtr spBlkPtr;
{
    register vm_offset_t lp, p;
    register SRsrcLstEntPtr sr;
    register SInfoRecPtr si;
    register SPRAMRecPtr sp;
    register slot, board_id;
    register unsigned char bl;
    struct rom_idoffset lstent;


    for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	register queue_t q;
	register queue_entry_t qe;

	slot_check_status(slot, continue);

	si = &slot_to_slotdata_ptr(slot)->SInfoRec;

	sp = &slot_to_slotdata_ptr(slot)->SPRAMRec;

	bl = si->siCPUByteLanes;

	q = &slot_to_slotdata_ptr(slot)->SRsrcLst;
	for (qe = queue_first(q); !queue_end(q, qe); qe = queue_next(qe)) {
	    sr = (SRsrcLstEntPtr)qe;
	    if (sr->SRsrcType_Category == catBoard &&
		sr->SRsrcType_cType == typeBoard)
		break;
	}

	if (queue_end(q, qe)) {
	    si->siInitStatusA = smNoBoardsRsrc;
	    continue;
	}

	board_id = 0;
	p = 0;

	lp = sr->SRsrcPtr;

	for (;;) {
	    slot_get_data(lp, sizeof (lstent), &lstent, bl);
	    if (lstent.id == endOfList)
		break;

	    switch (lstent.id) {
	      case boardId:
		board_id = (lstent.offset & 0xffff);
		break;

	      case pRAMInitData:
		p = slot_pointer_offset(lp, lstent.offset, bl);
		break;
	    }
	    if (p && board_id)
		break;

	    lp = slot_pointer_offset(lp, sizeof (lstent), bl);
	}

	if (board_id == 0) {
	    si->siInitStatusA = smNoBoardId;
	    continue;
	}

	if (board_id != sp->boardID) {
	    si->siStatusFlags |= fCardIsChanged;
	    bzero(sp, sizeof (SPRAMRecord));
	    sp->boardID = board_id;
	    if (p)
		slot_get_data(p,
			      sizeof (SPRAMRecord) - sizeof (short),
			      &sp->vendorUse1,
			      bl);
	}
    }

    return (noErr);
}

SSearchSRT(spBlkPtr)
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

	if (spBlkPtr->spFlags & fCkForNext) {
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
    }

    spBlkPtr->spSlot = slot;
    spBlkPtr->spsPointer = (vm_offset_t)sr;

    return (noErr);
}

SUpdateSRT(spBlkPtr)
SpBlockPtr spBlkPtr;
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

    sr->SRsrcIOReserved = spBlkPtr->spIOReserved;

    return (noErr);
}

SCalcSPtr(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    spBlkPtr->spsPointer = slot_pointer_offset(spBlkPtr->spsPointer,
					       spBlkPtr->spOffsetData,
					       spBlkPtr->spByteLanes);

    return (noErr);
}

SFindsInfoRecPtr(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register slot;

    slot = spBlkPtr->spSlot;
    if (!slot_in_range(slot))
	return (smSlotOOBErr);

    slot_check_status(slot, return status);

    spBlkPtr->spResult = (Ptr)&(slot_to_slotdata_ptr(slot)->SInfoRec);

    return (noErr);
}

SGetDriver(spBlkPtr)
register SpBlockPtr spBlkPtr;
{
    register SRsrcLstEntPtr sr;
    register vm_offset_t lp, dp, h;
    register slot, id, extdev;
    register unsigned char bl;
    unsigned long size;
    struct rom_idoffset lstent, dirent;

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

    bl = slot_to_slotdata_ptr(slot)->SInfoRec.siCPUByteLanes;

    lp = sr->SRsrcPtr;
    for (;;) {
	slot_get_data(lp, sizeof (lstent), &lstent, bl);
	if (lstent.id == endOfList || lstent.id == sRsrcDrvrDir)
	    break;

	lp = slot_pointer_offset(lp, sizeof (lstent), bl);
    }

    if (lstent.id == endOfList)
	return (smBadRefId);

    dp = slot_pointer_offset(lp, lstent.offset, bl);
    for (;;) {
	slot_get_data(dp, sizeof (dirent), &dirent, bl);
	if (dirent.id == endOfList || dirent.id == sMacOS68020)
	    break;

	dp = slot_pointer_offset(dp, sizeof (dirent), bl);
    }

    if (dirent.id == endOfList)
	return (smsGetDrvrErr);

    dp = slot_pointer_offset(dp, dirent.offset, bl);
    
    slot_get_data(dp, sizeof (long), &size, bl);

    h = NewHandle(size - sizeof (long));

    dp = slot_pointer_offset(dp, sizeof (long), bl);

    slot_get_data(dp,
		  size - sizeof (long),
		  *(vm_offset_t *)h,
		  bl);

    spBlkPtr->spResult = h;

    return (noErr);
}

SFindsRsrcPtr(spBlkPtr)
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

    return (noErr);
}
