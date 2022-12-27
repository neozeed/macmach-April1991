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
 * $Log:	Types.h,v $
 * Revision 2.2  90/08/30  11:46:57  bohman
 * 	Created.
 * 	[90/08/29  13:00:32  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/Types.h
 */

/*
	Types.h -- Common Defines and Types

	C Interface to the Macintosh Libraries
	Copyright Apple Computer,Inc. 1985-1987
	All rights reserved.
*/

#ifndef __TYPES__
#define __TYPES__

#define nil 0
#define NULL 0

enum {false,true};

typedef unsigned char Boolean;
typedef vm_offset_t Ptr;
typedef Ptr *Handle;
typedef void (*ProcPtr)();
typedef ProcPtr *ProcHandle;
typedef long Fixed;
typedef long Fract;
typedef unsigned long ResType;
typedef long OSType;
typedef short OSErr;
typedef unsigned char Style;

typedef struct Point {
    short v;
    short h;
} Point;

typedef struct Rect {
    short top;
    short left;
    short bottom;
    short right;
} Rect;

#define String(size)		\
struct {			\
    unsigned char length;	\
    unsigned char text[size];	\
}

typedef String(255) Str255,*StringPtr,**StringHandle;
#endif
