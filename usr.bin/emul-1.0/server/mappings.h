/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: emul/server/mappings.h
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/slotmgr.h>

#define ROM_ADDR 	0x40800000
#define ROM_COMPAT_ADDR	(ROM_ADDR&0x00ffffff)
#define ROM_SIZE 	(512*1024)

#define SND_ADDR 	0x50f14000
#define SND_COMPAT_ADDR	(SND_ADDR&0x00ffffff)
#define SND_SIZE 	(8*1024)

#define IWM_ADDR	0x50f16000
#define IWM_COMPAT_ADDR	(IWM_ADDR&0x00ffffff)
#define IWM_SIZE	(8*1024)

#define SCC_ADDR	0x50f04000
#define SCC_COMPAT_ADDR	(SCC_ADDR&0x00ffffff)
#define SCC_SIZE	(8*1024)

#define SLOT_ADDR(slot)		slot_to_ptr(slot)
#define SLOT_SIZE		(16*1024*1024)
#define SLOT_COMPAT_ADDR(slot)	((slot)<<20)
#define SLOT_COMPAT_SIZE	(1024*1024)

#define SLOT_SPACE_BASE		SLOT_ADDR(SLOT_NUM_LOW)
#define SLOT_SPACE_EXTENT	((SLOT_NUM_HIGH-SLOT_NUM_LOW+1)*SLOT_SIZE)

#define SLOT_MINOR_SPACE_BASE	SLOT_COMPAT_ADDR(SLOT_NUM_LOW)
#define SLOT_MINOR_SPACE_EXTENT \
	((SLOT_NUM_HIGH-SLOT_NUM_LOW+1)*SLOT_COMPAT_SIZE)
