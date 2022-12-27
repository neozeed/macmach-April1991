/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_dispatch.c,v $
 * Revision 2.2.1.1  90/09/07  00:58:34  bohman
 * 	Final cleanup.
 * 	[90/09/07            bohman]
 * 
 * Revision 2.2  90/09/04  17:36:27  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_dispatch.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Slot Manager
 * trap dispatch routine.
 */

#include <mach/mach_types.h>

#include <mach/mac2/frame.h>
#include <mach/mac2/reg.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>

#include <mac2slotmgr/slotmgr.h>

#define SBadRoutineSelector	((int (*)())0)

extern    SReadByte(),		SReadWord(),		SReadLong();
extern    SGetCString(),	SGetBlock();
extern    SFindStruct(),	SReadStruct();
extern    SReadInfo(),		SReadPRAMRec();
extern    SPutPRAMRec(),	SReadFHeader(),		SNextsRsrc();
extern    SNextTypesRsrc(),	SRsrcInfo();
extern    SCkCardStatus(),	SReadDrvrName();
extern    SFindDevBase();
extern    InitSDeclMgr();
#define	InitSDeclMgr	SBadRoutineSelector
extern    SPrimaryInit(),	SCardChanged(),		SExec();
extern    SOffsetData(),	InitPRAMRecs(),		SReadPBSize();
#define	InitPRAMRecs	SBadRoutineSelector
extern    SCalcStep(),		InitsRsrcTable();
#define	InitsRsrcTable	SBadRoutineSelector
extern    SSearchSRT(),		SUpdateSRT(),		SCalcSPtr();
#define	SSearchSRT	SBadRoutineSelector
#define	SUpdateSRT	SBadRoutineSelector
extern    SGetDriver(),		SPtrToSlot(),		SFindsInfoRecPtr();
extern    SFindsRsrcPtr(),	SDeleteSRTRec();

int	(*slotmgr_routines[])() =
{
    SReadByte,		SReadWord,		SReadLong,
    SGetCString,	SBadRoutineSelector,	SGetBlock,
    SFindStruct,	SReadStruct,		SBadRoutineSelector,
    SBadRoutineSelector,SBadRoutineSelector,	SBadRoutineSelector,
    SBadRoutineSelector,SBadRoutineSelector,	SBadRoutineSelector,
    SBadRoutineSelector,SReadInfo,		SReadPRAMRec,
    SPutPRAMRec,	SReadFHeader,		SNextsRsrc,
    SNextTypesRsrc,	SRsrcInfo,		SBadRoutineSelector,
    SCkCardStatus,	SReadDrvrName,		SBadRoutineSelector,
    SFindDevBase,	SBadRoutineSelector,	SBadRoutineSelector,
    SBadRoutineSelector,SBadRoutineSelector,	InitSDeclMgr,
    SPrimaryInit,	SCardChanged,		SExec,
    SOffsetData,	InitPRAMRecs,		SReadPBSize,
    SBadRoutineSelector,SCalcStep,		InitsRsrcTable,
    SSearchSRT,		SUpdateSRT,		SCalcSPtr,
    SGetDriver,		SPtrToSlot,		SFindsInfoRecPtr,
    SFindsRsrcPtr,	SDeleteSRTRec,
};    

slotmgr_dispatch(frame, regs)
register generic_exception_frame_t *frame;
register regs_t *regs;
{
    register selector;
    register (*function)();

    selector = regs->r_r0;
    if (selector < 0 || selector > 49) {
	regs->r_r0 = smSelOOBErr;
	return;
    }

    function = slotmgr_routines[selector];
    if (function == SBadRoutineSelector) {
	regs->r_r0 = smSelOOBErr;
	return;
    }

    regs->r_r0 = (*function)(regs->r_areg[0]);
}
