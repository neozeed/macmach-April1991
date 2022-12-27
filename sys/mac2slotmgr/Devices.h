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
 * $Log:	Devices.h,v $
 * Revision 2.2  90/08/30  11:09:09  bohman
 * 	Created.
 * 	[90/08/29  12:59:12  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/Devices.h
 */

/*
	Devices.h - Device Manager

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1985,1986,1987
	All rights reserved.
*/

#define	ParamBlockHeader	\
    struct QElem *qLink;	\
    short qType;		\
    short ioTrap;		\
    Ptr ioCmdAddr;		\
    ProcPtr ioCompletion;	\
    OSErr ioResult;		\
    StringPtr ioNamePtr;	\
    short ioVRefNum

typedef struct CntrlParam {
    ParamBlockHeader;
    short ioCRefNum;
    short csCode;
    short csParam[11];
} CntrlParam;

typedef struct SlotDevParam {
    ParamBlockHeader;
    short ioRefNum;
    char ioVersNum;
    char ioPermssn;
    Ptr ioMix;
    short ioFlags;
    char ioSlot;
    char ioID;
} SlotDevParam;

typedef struct MultiDevParam {
    ParamBlockHeader;
    short ioRefNum;
    char ioVersNum;
    char ioPermssn;
    Ptr ioMix;
    short ioFlags;
    Ptr ioSEBlkPtr;
} MultiDevParam;

typedef union ParamBlockRec {
    struct SlotDevParam slotDevParam;
    struct MultiDevParam multiDevParam;
} ParamBlockRec, *ParmBlkPtr;

#define CallDriver(driver, rout, param, dce)	\
    ((unsigned short)				\
     _CallDriver((vm_offset_t)(driver)+(driver)->rout, param, dce))

typedef struct DeviceDriver {
    unsigned short drvrFlags;
    unsigned short drvrDelay;
    unsigned short drvrEMask;
    unsigned short drvrMenu;
    unsigned short drvrOpen;
    unsigned short drvrPrime;
    unsigned short drvrCtl;
    unsigned short drvrStatus;
    unsigned short drvrClose;
    unsigned char drvrName[1];
} DeviceDriver, *DeviceDriverPtr, **DeviceDriverHandle;

typedef struct QElem {
    struct QElem *qLink;
    short qType;
    short qData[1];
} QElem, *QElemPtr;

typedef struct QHdr {
    short qFlags;
    QElemPtr qHead;
    QElemPtr qTail;
} QHdr,*QHdrPtr;

typedef struct DCtlEntry {
    Ptr dCtlDriver;
    short dCtlFlags;
    QHdr dCtlQHdr;
    long dCtlPosition;
    Handle dCtlStorage;
    short dCtlRefNum;
    long dCtlCurTicks;
    struct GrafPort *dCtlWindow;
    short dCtlDelay;
    short dCtlEMask;
    short dCtlMenu;
} DCtlEntry,*DCtlPtr,**DCtlHandle;

typedef struct AuxDCE {
    Ptr dCtlDriver;
    short dCtlFlags;
    QHdr dCtlQHdr;
    long dCtlPosition;
    Handle dCtlStorage;
    short dCtlRefNum;
    long dCtlCurTicks;
    struct GrafPort *dCtlWindow;
    short dCtlDelay;
    short dCtlEMask;
    short dCtlMenu;
    char dCtlSlot;
    char dCtlSlotId;
    long dCtlDevBase;
    Ptr dCtlOwner;
    char dCtlExtDev;
    char fillByte;
} AuxDCE,*AuxDCEPtr,**AuxDCEHandle;
