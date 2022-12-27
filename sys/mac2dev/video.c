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
 * $Log:	video.c,v $
 * Revision 2.2  90/08/30  11:07:05  bohman
 * 	Created.
 * 	[90/08/29  12:54:57  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2dev/video.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Mac2 Video Driver
 */
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

#include <mach/mach_types.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Errors.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/Video.h>
#include <mac2slotmgr/ROMDefs.h>
#include <mac2slotmgr/Devices.h>

#include <mac2slotmgr/slotmgr.h>

#include <mac2dev/video.h>

/*
 * global screen structures
 * (indexed by minor dev)
 */
struct video	video[NVIDEO];
int nvideo;

vm_offset_t	rbvideo_kern;

video_open(dev, flag)
dev_t dev;
{
    register struct video *vp;
    int unit = minor(dev);

    if (unit >= NVIDEO)
	return (ENXIO);

    vp = &video[unit];
    if ((vp->video_flags&VIDEO_F_INITED) == 0)
	return (ENXIO);

    if ((vp->video_flags&VIDEO_F_OPEN) == 0)
	vp->video_flags |= VIDEO_F_OPEN;
    
    return (0);
}

video_close(dev, flag)
dev_t dev;
{
    register struct video *vp;
    int unit = minor(dev);

    vp = &video[unit];

    video_setup(vp, FALSE);
    vp->video_flags &= ~VIDEO_F_OPEN;

    if (unit == 0)
	cons_reset();

    return (0);
}

/*
 * Video ioctls: Most of these call
 * the MacOS driver control and status
 * routines.
 */
video_ioctl(dev, cmd, addr, flag)
dev_t dev;
register vm_offset_t addr;
{
    register struct video *vp = &video[minor(dev)];
    register error = 0;
    CntrlParam params;

#define Ctrl_Call(code)	\
{									\
    register struct pcb *pcb = current_thread_pcb();			\
\
    params.csCode = code;						\
    *(Ptr *)params.csParam = addr;					\
    pcb->pcb_flags |= JMP_FAULT;						\
    if (!setjmp(&pcb->pcb_label)) {					\
	if (CallDriver(vp->video_driver, drvrCtl,			\
		       &params, vp->video_dce) == 0)			\
	    ;								\
	else								\
	    error = EIO;						\
    }									\
    else								\
	error = EFAULT;							\
\
    pcb->pcb_flags &= ~JMP_FAULT;					\
}

#define Stat_Call(code)	\
{									\
    register struct pcb *pcb = current_thread_pcb();			\
\
    params.csCode = code;						\
    *(Ptr *)params.csParam = addr;					\
    pcb->pcb_flags |= JMP_FAULT;						\
    if (!setjmp(&pcb->pcb_label)) {					\
	if (CallDriver(vp->video_driver, drvrStatus,			\
		       &params, vp->video_dce) == 0)			\
	    ;								\
	else								\
	    error = EIO;						\
    }									\
    else								\
	error = EFAULT;							\
\
    pcb->pcb_flags &= ~JMP_FAULT;					\
}
    
    switch (cmd) {
      case VIDEO_CTRL_Init:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)addr;

	    Ctrl_Call(cscReset);
	    v->csBaseAddr = slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_CTRL_KillIO:
	Ctrl_Call(cscKillIO);
	break;

      case VIDEO_CTRL_SetMode:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)addr;

	    Ctrl_Call(cscSetMode);
	    v->csBaseAddr = slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_CTRL_SetEntries:
	{
	    register VDEntRecPtr v = (VDEntRecPtr)addr;
	    register Ptr tbl;
	    register tbl_size;

	    if (v->csStart == -1) {
		error = EINVAL;
		break;
	    }

	    tbl_size = (v->csCount+1)*sizeof (ColorSpec);
	    tbl = NewPtr(tbl_size);

	    if (copyin(v->csTable, tbl, tbl_size)) {
		error = EFAULT;
		DisposPtr(tbl);
		break;
	    }
	    v->csTable = (ColorSpec *)tbl;
	    Ctrl_Call(cscSetEntries);
	    DisposPtr(tbl);
	}
	break;

      case VIDEO_CTRL_GrayScreen:
	Ctrl_Call(cscGrayPage);
	break;

      case VIDEO_CTRL_SetGray:
	Ctrl_Call(cscSetGray);
	break;
	
      case VIDEO_STAT_GetMode:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)addr;

	    Stat_Call(cscGetMode);
	    v->csBaseAddr = slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_STAT_GetEntries:
	{
	    register VDEntRecPtr v = (VDEntRecPtr)addr;
	    register ColorSpec *utbl;
	    register Ptr tbl;
	    register tbl_size;

	    if (v->csStart == -1) {
		error = EINVAL;
		break;
	    }

	    tbl_size = (v->csCount+1)*sizeof (ColorSpec);
	    tbl = NewPtr(tbl_size);

	    utbl = v->csTable;
	    v->csTable = (ColorSpec *)tbl;
	    Stat_Call(cscGetEntries);
	    v->csTable = utbl;
	    if (copyout(tbl, utbl, tbl_size)) {
		error = EFAULT;
		DisposPtr(tbl);
		break;
	    }
	    DisposPtr(tbl);
	}
	break;

      case VIDEO_STAT_GetPages:
	Stat_Call(cscGetPages);
	break;

      case VIDEO_STAT_GetBaseAddr:
	{
	    register VDPgInfoPtr v = (VDPgInfoPtr)addr;

	    Stat_Call(cscGetBaseAddr);
	    v->csBaseAddr = slot_ptr_to_offset(v->csBaseAddr);
	}
	break;

      case VIDEO_STAT_GetGray:
	Stat_Call(cscGetGray);
	break;

	/*
	 * Return the size of the video
	 * device.  This is probably not
	 * useful because the value stored
	 * on most cards is incorrect.
	 */
      case VIDEO_SIZE:
	bcopy(&vp->video_devsize, addr, sizeof (vm_size_t));
	break;

	/*
	 * Return the video parameter block
	 * for the mode that the card is
	 * currently in.
	 */
      case VIDEO_PARAMS:
	{
	    register struct pcb *pcb = current_thread_pcb();
	    CntrlParam params;
	    VDPageInfo vpi;

	    params.csCode = cscGetMode;
	    *(Ptr *)params.csParam = (Ptr)&vpi;
	    pcb->pcb_flags |= JMP_FAULT;
	    if (!setjmp(&pcb->pcb_label)) {
		if (CallDriver(vp->video_driver, drvrStatus,
			       &params, vp->video_dce) != 0)
		    error = EIO;
	    }
	    else
		error = EFAULT;

	    pcb->pcb_flags &= ~JMP_FAULT;

	    if (error == 0)
		if (video_get_params(vp->video_slot, vp->video_id,
				     vpi.csMode, addr) == FALSE)
		    error = EIO;
	}
	break;
	
      default:
	error = EINVAL;
	break;
    }
    return (error);

#undef	Ctrl_Call
#undef	Stat_Call
}

/*
 * mmap() system call interface.
 * The only offset restriction is that
 * only memory for the same slot can
 * be mapped.  I would have liked to
 * have restricted this to only the
 * particular device, but the size
 * information is incorrect for most
 * cards (see ioctl VIDEO_SIZE above).
 */
video_mmap(dev, offset, prot)
dev_t dev;
register vm_offset_t offset;
{
    register struct video *vp = &video[minor(dev)];
    
    if (vp->video_devsize == 0 ||
	slot_ptr_to_offset(vp->video_devbase+offset) >= (16*1024*1024))
	return (-1);
    
    return (mac2_btop(pmap_extract(kernel_pmap, vp->video_devbase + offset)));
}

/*
 * Retrieve a video parameter
 * block.
 */
video_get_params(slot, id, mode, addr)
vm_offset_t addr;
{
    SpBlock slot_params;

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;
    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    slot_params.spID = mode;
    if (SFindStruct(&slot_params) != noErr)
	return (FALSE);

    slot_params.spID = mVidParams;
    if (SGetBlock(&slot_params) != noErr)
	return (FALSE);

    bcopy(slot_params.spResult,
	  addr,
	  sizeof (VPBlock));

    DisposPtr(slot_params.spResult);

    return (TRUE);
}

static
eqstr(s1, s2)
register unsigned char *s1, *s2;
{
    register n;

    if (*s1 != *s2)
	return (FALSE);

    for (n = *s1; n > 0; n--)
	if (*++s1 != *++s2)
	    return (FALSE);

    return (TRUE);
}

/*
 * Retrieve a video driver.
 */
video_get_driver(slot, id, vp)
register struct video *vp;
{
    SpBlock slot_params;

    if (vp->video_driver == 0) {
	slot_params.spSlot = slot;
	slot_params.spID = id;
	slot_params.spExtDev = 0;
	if (SGetDriver(&slot_params) != noErr)
	    return (FALSE);

	/*
	 * Unfortuately, the ROM driver for the
	 * Apple old 8 bit graphics card (TFB) has serious
	 * bugs in it.  So I fetched the driver
	 * out of the System file and linked it into
	 * MACH and ....
	 */
	{
	    register DeviceDriverPtr dp, tfb_dp;
	    extern unsigned char drvr_120[];

	    tfb_dp = (DeviceDriverPtr)drvr_120;
	    dp = *(DeviceDriverHandle)slot_params.spResult;
	    if (eqstr(tfb_dp->drvrName, dp->drvrName) == TRUE) {
		vp->video_driver = tfb_dp;
		vp->video_flags |= VIDEO_F_STATIC_DRIVER;
		DisposHandle(slot_params.spResult);
		return (TRUE);
	    }
	}
	
	vp->video_driver = *(DeviceDriverHandle)slot_params.spResult;
    }

    return (TRUE);
}

/*
 * Retrieve the device base and size.
 */
video_get_devbase_size(slot, id, vp)
register struct video *vp;
{
    SpBlock slot_params;

    slot_params.spSlot = slot;
    slot_params.spID = id;
    slot_params.spExtDev = 0;
    if (SFindDevBase(&slot_params) != noErr)
	return (FALSE);

    vp->video_devbase = slot_params.spResult;

    if (SRsrcInfo(&slot_params) != noErr)
	return (FALSE);

    if ((vp->video_devbase&0xf0000000) == 0xf0000000)
	slot_params.spID = minorLength;
    else
	slot_params.spID = majorLength;

    if (SReadLong(&slot_params) != noErr)
	return (FALSE);

    vp->video_devsize = slot_params.spResult;

    return (TRUE);
}

/*
 * Called from autoconfig.  The
 * console device has already been
 * initialized.
 */
video_config(slot, id)
register slot, id;
{
    register unit;
    VPBlock v;

    if (video[0].video_slot == slot && video[0].video_id == id) {
	unit = 0;

	video_get_params(slot, id, oneBitMode, &v);
    }
    else {
	if (nvideo >= NVIDEO)
	    return (FALSE);

	unit = nvideo;

	if (video_get_params(slot, id, oneBitMode, &v) == FALSE)
	    return (FALSE);

	if (video_init(slot, id, &video[unit]) == FALSE)
	    return (FALSE);

	nvideo++;
    }
    printf("video sRsrc: slot %x id %d Bounds %d %d %d %d assigned to unit %d %s\n",
	   slot, id,
	   v.vpBounds.top, v.vpBounds.left,
	   v.vpBounds.bottom, v.vpBounds.right,
	   unit, unit? "": "(console)");

    return (TRUE);
}

/*
 * Initialize a video device.
 */
video_init(slot, id, vp)
register slot, id;
register struct video *vp;
{
    register AuxDCEPtr dp;

    if (video_get_devbase_size(slot, id, vp) == FALSE)
	return (FALSE);
    
    if (video_get_driver(slot, id, vp) == FALSE)
	return (FALSE);
    
    dp = (AuxDCEPtr)NewPtr(sizeof (AuxDCE));
    dp->dCtlSlot = vp->video_slot = slot;
    dp->dCtlSlotId = vp->video_id = id;
    dp->dCtlDevBase = vp->video_devbase;
    vp->video_dce = dp;
    
    if (video_setup(vp, TRUE) == FALSE) {
	video_remove(vp);
	return (FALSE);
    }
    vp->video_flags |= VIDEO_F_INITED;
    slot_to_slotdata_ptr(vp->video_slot)->SFlags |= SLOT_MAPPABLE;

    return (TRUE);
}

/*
 * Reverse a partially completed
 * initalization.
 */
video_remove(vp)
register struct video *vp;
{
    if ((vp->video_flags&VIDEO_F_STATIC_DRIVER) == 0)
	DisposHandle(RecoverHandle(vp->video_driver));
    vp->video_driver = 0;
    DisposPtr(vp->video_dce);
    vp->video_dce = 0;
    vp->video_flags = 0;
}

/*
 * Pick a video display device
 * to be used as the system console.
 */
video_cons_find(v)
VPBlockPtr *v;
{
    SpBlock slot_params;
    struct {
	unsigned char	sdSlot;
	unsigned char	sdSResource;
    } vd;

    if (nvideo > 0)
	return (FALSE);

    /*
     * First try to locate the display
     * used for the 'Happy Macintosh'
     * under MacOS.
     */
    rtc_readXpram(0x80, &vd, sizeof (vd));
    if (vd.sdSlot != 0) {
	slot_params.spSlot = vd.sdSlot;
	slot_params.spID = vd.sdSResource;
	slot_params.spExtDev = 0;
	if (SRsrcInfo(&slot_params) == noErr) {
	    if (slot_params.spCategory == catDisplay &&
		slot_params.spCType == typeVideo) {
		if (video_get_params(slot_params.spSlot,
				     slot_params.spID,
				     oneBitMode,
				     v) == TRUE &&
		    video_init(slot_params.spSlot,
			       slot_params.spID,
			       &video[0]) == TRUE) {
		    nvideo++;
		    return (TRUE);
		}
		else {
		    slot_params.spIOReserved |= SRsrcIOReserved_Ignore;
		    SUpdateSRT(&slot_params);
		}
	    }
	}
    }

    /*
     * Otherwise find the display with
     * lowest slot and id number.
     */
    if (SFirstsRsrc(&slot_params) == noErr)
	for (;;) {
	    if (slot_params.spCategory == catDisplay &&
		slot_params.spCType == typeVideo) {
		if (video_get_params(slot_params.spSlot,
				     slot_params.spID,
				     oneBitMode,
				     v) == TRUE &&
		    video_init(slot_params.spSlot,
			       slot_params.spID,
			       &video[0]) == TRUE) {
		    nvideo++;
		    return (TRUE);
		}
		else {
		    slot_params.spIOReserved |= SRsrcIOReserved_Ignore;
		    SUpdateSRT(&slot_params);
		}
	    }

	    if (SNextsRsrc(&slot_params) != noErr)
		return (FALSE);
	}
    else
	return (FALSE);
}

/*
 * Do MacOS driver calls necessary to
 * initialize a video device.  Called
 * with startup == TRUE when initializing
 * the system.  Called otherwise from
 * video_close().
 */
video_setup(vp, startup)
register struct video *vp;
{
    register failed = 0;
    label_t label;
    extern label_t *nofault;

    /*
     * Open the device.  Only
     * performed once due to
     * bugs in almost every
     * close routine.
     */
    if (startup) {
	SlotDevParam params;
	
	nofault = &label;
	if (!setjmp(&label) &&
	    CallDriver(vp->video_driver, drvrOpen,
		       &params, vp->video_dce) == 0)
	    ;
	else {
	    nofault = 0;
	    return (FALSE);
	}
	
	nofault = 0;
    }

    /*
     * Initialize the device.  Resets
     * the device to one-bit mode.
     */
    {
	CntrlParam params;
	VDPageInfo v;

	params.csCode = cscReset;
	*(Ptr *)params.csParam = (Ptr)&v;

	if (startup) {
	    nofault = &label;
	    if (!setjmp(&label) &&
		CallDriver(vp->video_driver, drvrCtl,
			   &params, vp->video_dce) == 0)
		;
	    else
		failed = 1;
	    
	    nofault = 0;
	}
	else {
	    register struct pcb *pcb = current_thread_pcb();

	    pcb->pcb_flags |= JMP_FAULT;
	    if (!setjmp(&pcb->pcb_label) &&
		CallDriver(vp->video_driver, drvrCtl,
			   &params, vp->video_dce) == 0)
		;
	    else
		failed = 1;

	    pcb->pcb_flags &= ~JMP_FAULT;
	}
    }

    /*
     * Load the CLUT with black & white
     * pixel values.
     * The invocation code is different
     * here; the SetEntries call is
     * allowed to return an error for
     * devices that have a read-only CLUT.
     */
    if (!failed) {
	CntrlParam params;
	VDEntryRecord v;
	ColorSpec color[2];

	color[0].value = 0;
	color[0].rgb.red = color[0].rgb.green = color[0].rgb.blue = 0xffff;

	color[1].value = 0;
	color[1].rgb.red = color[1].rgb.green = color[1].rgb.blue = 0x0000;

	v.csTable = color;
	v.csStart = 0;
	v.csCount = (2 - 1);

	params.csCode = cscSetEntries;
	*(Ptr *)params.csParam = (Ptr)&v;

	if (startup) {
	    nofault = &label;
	    if (!setjmp(&label)) {
		if (CallDriver(vp->video_driver, drvrCtl,
			       &params, vp->video_dce) == 0)
		;
	    }
	    else
		failed = 1 ;
	    
	    nofault = 0;
	}
	else {
	    register struct pcb *pcb = current_thread_pcb();

	    pcb->pcb_flags |= JMP_FAULT;
	    if (!setjmp(&pcb->pcb_label)) {
		if (CallDriver(vp->video_driver, drvrCtl,
			       &params, vp->video_dce) == 0)
		;
	    }
	    else
		failed = 1;

	    pcb->pcb_flags &= ~JMP_FAULT;
	}
    }

    /*
     * Close the device only
     * on failure during startup.
     */
    if (startup && failed) {
	SlotDevParam params;

	if (startup) {
	    nofault = &label;
	    if (!setjmp(&label) &&
		CallDriver(vp->video_driver, drvrClose,
			   &params, vp->video_dce) == 0)
		;
	    else
		failed = 1;
	    
	    nofault = 0;
	}
	else {
	    register struct pcb *pcb = current_thread_pcb();

	    pcb->pcb_flags |= JMP_FAULT;
	    if (!setjmp(&pcb->pcb_label) &&
		CallDriver(vp->video_driver, drvrClose,
			   &params, vp->video_dce) == 0)
		;
	    else
		failed = 1;

	    pcb->pcb_flags &= ~JMP_FAULT;
	}
    }

    return (!failed);
}
