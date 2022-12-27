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
 * $Log:	Video.h,v $
 * Revision 2.2  90/08/30  11:47:05  bohman
 * 	Created.
 * 	[90/08/29  13:00:50  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/Video.h
 */

/*
	Video.h -- Video Interface 

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1986-1987
	All rights reserved.
*/

#ifndef __VIDEO__
#define __VIDEO__

#define mVidParams		1          /* Video parameter block id. */
#define mTable			2          /* Offset to the table. */
#define mPageCnt		3          /* Number of pages */
#define mDevType		4          /* Device Type */

				/* Video sResource List Id's */
#define oneBitMode		128	/* Id of OneBitMode Parameter list */
#define twoBitMode		129	/* Id of TwoBitMode Parameter list */
#define fourBitMode		130	/* Id of FourBitMode Parameter list */
#define eightBitMode		131	/* Id of EightBitMode Parameter list */

/* mVidParams block */
typedef struct VPBlock  {  	/* Video Parameters block. */
    long	vpBaseOffset;	/* Offset to page zero of video RAM */
    short	vpRowBytes;	/* Width of each row of video memory */
    Rect	vpBounds;	/* BoundsRect for the video display  */
    short	vpVersion; 	/* PixelMap version number */
    short	vpPackType;                   
    long	vpPackSize;                   
    long	vpHRes;   	/* Horizontal resolution of the device
				 * (pixels per inch) */
    long	vpVRes;   	/* Vertical resolution of the device
				 * (pixels per inch) */
    short	vpPixelType;	/* Defines the pixel type */
    short	vpPixelSize; 	/* Number of bits in pixel */
    short	vpCmpCount; 	/* Number of components in pixel */
    short	vpCmpSize;	/* Number of bits per component */
    long	vpPlaneBytes;	/* Offset from one plane to the next */
} VPBlock, *VPBlockPtr;

typedef struct RGBColor {
    unsigned short	red;
    unsigned short	green;
    unsigned short	blue;
} RGBColor;

typedef struct ColorSpec {
    unsigned short	value;
    RGBColor	rgb;
} ColorSpec;

				/* Control Codes */
#define cscReset		0
#define cscKillIO		1
#define cscSetMode		2
#define cscSetEntries		3
#define cscSetGamma		4
#define cscGrayPage		5
#define cscSetGray		6

				/* Status Codes */
#define cscGetMode		2
#define cscGetEntries		3
#define cscGetPages		4
#define cscGetBaseAddr		5
#define cscGetGray		6

typedef struct VDEntryRecord{
    ColorSpec	*csTable;	/* Pointer to an array of color specs */
    short	csStart;	/* Which spec in array to start with, or -1 */
    short	csCount;	/* Number of color spec entries to set */
} VDEntryRecord, *VDEntRecPtr;
		
typedef struct VDPageInfo{
    short	csMode;		/* (word) mode within device */
    long	csData;		/* (long) data supplied by driver */
    short	csPage;		/* (word) page to switch in */
    Ptr		csBaseAddr;	/* (long) base address of page */
} VDPageInfo, *VDPgInfoPtr;

#endif

