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
 * $Log:	video.h,v $
 * Revision 2.2  90/08/30  11:07:17  bohman
 * 	Created.
 * 	[90/08/29  12:55:22  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/video.h
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Mac2 Video Driver
 */

#ifndef	_MAC2DEV_VIDEO_H_
#define	_MAC2DEV_VIDEO_H_	1

#ifdef KERNEL
struct video {
    unsigned short	video_slot;
    unsigned short	video_id;
    vm_offset_t		video_devbase;	/* device base address */
    vm_size_t		video_devsize;	/* device size */
    DeviceDriverPtr	video_driver;	/* ptr to MACOS driver */
    AuxDCEPtr		video_dce;
    unsigned long	video_flags;
#define VIDEO_F_INITED		0x00000001
#define VIDEO_F_STATIC_DRIVER	0x00000002
#define VIDEO_F_OPEN		0x00000004
};
#endif KERNEL

/*
 * Video Ioctl calls.
 * See "Designing Cards and
 * Drivers for Macintosh II
 * and Macintosh SE"
 * for info.
 */

/*
 * MAC driver control calls
 */
#define VIDEO_CTRL_Init		_IOR('V', 0, VDPageInfo)
#define VIDEO_CTRL_KillIO	_IO('V', 1)
#define VIDEO_CTRL_SetMode	_IOWR('V', 2, VDPageInfo)
#define VIDEO_CTRL_SetEntries	_IOW('V', 3, VDEntryRecord)
#define VIDEO_CTRL_SetGamma	_IOW('V', 4, Ptr)
#define VIDEO_CTRL_GrayScreen	_IOW('V', 5, VDPageInfo)
#define VIDEO_CTRL_SetGray	_IOW('V', 6, VDPageInfo)

/*
 * MAC driver status calls
 */
#define VIDEO_STAT_GetMode	_IOR('V', 7, VDPageInfo)
#define VIDEO_STAT_GetEntries	_IOWR('V', 8, VDEntryRecord)
#define VIDEO_STAT_GetPages	_IOWR('V', 9, VDPageInfo)
#define VIDEO_STAT_GetBaseAddr	_IOWR('V', 10, VDPageInfo)
#define VIDEO_STAT_GetGray	_IOR('V', 11, VDPageInfo)

/*
 * Return size of a video device.
 */
#define VIDEO_SIZE		_IOR('V', 20, vm_size_t)

/*
 * Return video parameters based on
 * current device mode.
 */
#define VIDEO_PARAMS		_IOR('V', 22, VPBlock)

#ifdef KERNEL
extern struct video video[];
int nvideo;

#define	NVIDEO	    6
#endif KERNEL

#endif	_MAC2DEV_VIDEO_H_
