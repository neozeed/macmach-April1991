/************************************************************ 
Copyright 1988 by Apple Computer, Inc, Cupertino, California
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

************************************************************/
/*-
 * macmachInit.c --
 *	Initialization functions for screen/keyboard/mouse, etc.
 *
 * Copyright (c) 1987 by the Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 *
 */

/************************************************************
Copyright 1987 by Sun Microsystems, Inc. Mountain View, CA.

                    All Rights Reserved

Permission  to  use,  copy,  modify,  and  distribute   this
software  and  its documentation for any purpose and without
fee is hereby granted, provided that the above copyright no-
tice  appear  in all copies and that both that copyright no-
tice and this permission notice appear in  supporting  docu-
mentation,  and  that the names of Sun or MIT not be used in
advertising or publicity pertaining to distribution  of  the
software  without specific prior written permission. Sun and
M.I.T. make no representations about the suitability of this
software for any purpose. It is provided "as is" without any
express or implied warranty.

SUN DISCLAIMS ALL WARRANTIES WITH REGARD TO  THIS  SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
NESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUN BE  LI-
ABLE  FOR  ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,  DATA  OR
PROFITS,  WHETHER  IN  AN  ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#include    "macmach.h"
#include    <servermd.h>
#include    "dixstruct.h"
#include    "dix.h"
#include    "opaque.h"
#include    "mipointer.h"

#include <sys/ioctl.h>
#include <sys/types.h>

extern int macmachMouseProc();
extern void macmachKbdProc();
extern int macmachKbdSetUp();
extern Bool macmachMonoProbe();
extern Bool macmachColorProbe();
extern void ProcessInputEvents();

extern void SetInputCheck();
extern char *strncpy();

int macmachSigIO = 0;	 /* For use with SetInputCheck */
static int autoRepeatHandlersInstalled; /* FALSE each time InitOutput called */


static int macmachOpenAllScreens = TRUE;
static int macmachScreensWanted = 0;


/*-
 *-----------------------------------------------------------------------
 * SigIOHandler --
 *	Signal handler for SIGIO - input is available.
 *
 * Results:
 *	macmachSigIO is set - ProcessInputEvents() will be called soon.
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
static int
SigIOHandler(sig, code, scp)
    int		code;
    int		sig;
    struct sigcontext *scp;
{
    macmachSigIO=1;
    return(1);
}

macmachFbDataRec macmachFbData[] = {
#ifdef MM_KLUDGE
    macmachColorProbe,	"/dev/scn0",  FALSE,  1,  neverProbed,
    macmachColorProbe,	"/dev/scn1",  FALSE,  1,  neverProbed,
    macmachColorProbe,	"/dev/scn2",  FALSE,  1,  neverProbed,
    macmachColorProbe,	"/dev/scn3",  FALSE,  1,  neverProbed,
    macmachColorProbe,	"/dev/scn4",  FALSE,  1,  neverProbed,
#else MM_KLUDGE
    macmachColorProbe,	"/dev/scn0",  FALSE,  DEFAULTDEPTH,  neverProbed,
    macmachColorProbe,	"/dev/scn1",  FALSE,  DEFAULTDEPTH,  neverProbed,
    macmachColorProbe,	"/dev/scn2",  FALSE,  DEFAULTDEPTH,  neverProbed,
    macmachColorProbe,	"/dev/scn3",  FALSE,  DEFAULTDEPTH,  neverProbed,
    macmachColorProbe,	"/dev/scn4",  FALSE,  DEFAULTDEPTH,  neverProbed,
#endif MM_KLUDGE
};

/*
 * NUMSCREENS is the number of supported frame buffers (i.e. the number of
 * structures in macmachFbData which have an actual probeProc).
 */
#define NUMSCREENS (sizeof(macmachFbData)/sizeof(macmachFbData[0]))
#define NUMDEVICES 2

fbFd	macmachFbs[NUMSCREENS];  /* Space for descriptors of open frame buffers */

static PixmapFormatRec	formats[] = {
    1, 1, BITMAP_SCANLINE_PAD,	/* 1-bit deep */
    8, 8, BITMAP_SCANLINE_PAD,	/* 8-bit deep */
};
#define NUMFORMATS	(sizeof formats)/(sizeof formats[0])

/*-
 *-----------------------------------------------------------------------
 * InitOutput --
 *	Initialize screenInfo for all actually accessible framebuffers.
 *
 * Results:
 *	screenInfo init proc field set
 *
 * Side Effects:
 *	None
 *
 *-----------------------------------------------------------------------
 */

InitOutput(pScreenInfo, argc, argv)
    ScreenInfo 	  *pScreenInfo;
    int     	  argc;
    char    	  **argv;
{
    int     	  i, index, ac = argc;
    char	  **av = argv;

    pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    pScreenInfo->numPixmapFormats = NUMFORMATS;
    for (i=0; i< NUMFORMATS; i++)
    {
        pScreenInfo->formats[i] = formats[i];
    }

    autoRepeatHandlersInstalled = FALSE;

    for (i = 0, index = 0; i < NUMSCREENS; i++) {
        if ( macmachOpenAllScreens || macmachFbData[i].Wanted ) {
	    if ((* macmachFbData[i].probeProc) (pScreenInfo, index, i, argc, argv)) {
	        /* This display exists OK */
	        index++;
	    } else {
	        /* This display can't be opened */
	        if ( macmachFbData[i].Wanted )
		    FatalError("Could not open display %s\n", macmachFbData[i].devName);
	    }
	}
    }
    if (index == 0)
	FatalError("Can't find any displays\n");

    pScreenInfo->numScreens = index;
}

/*-
 *-----------------------------------------------------------------------
 * InitInput --
 *	Initialize all supported input devices...what else is there
 *	besides pointer and keyboard?
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Two DeviceRec's are allocated and registered as the system pointer
 *	and keyboard devices.
 *
 *-----------------------------------------------------------------------
 */
/*ARGSUSED*/
InitInput(argc, argv)
    int     	  argc;
    char    	  **argv;
{
    DevicePtr p, k;
    static int  zero = 0;
    
    p = AddInputDevice(macmachMouseProc, TRUE);
    k = AddInputDevice(macmachKbdProc, TRUE);
    if (!p || !k)
        FatalError("failed to create input devices in InitInput");

    RegisterPointerDevice(p);
    RegisterKeyboardDevice(k);
    miRegisterPointerDevice(screenInfo.screens[0], p);
    signal(SIGIO, SigIOHandler);

    SetInputCheck (&zero, &macmachSigIO);
}


/*-
 *-----------------------------------------------------------------------
 * macmachScreenInit --
 *	Things which must be done for all types of frame buffers...
 *	Should be called last of all.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	The graphics context for the screen is created. The CreateGC,
 *	CreateWindow and ChangeWindowAttributes vectors are changed in
 *	the screen structure.
 *
 *	Both a BlockHandler and a WakeupHandler are installed for the
 *	first screen.  Together, these handlers implement autorepeat
 *	keystrokes.
 *
 *-----------------------------------------------------------------------
 */
Bool
macmachScreenInit (pScreen)
    ScreenPtr	  pScreen;
{
    extern void   macmachBlockHandler();
    extern void   macmachWakeupHandler();
    static ScreenPtr autoRepeatScreen;
    extern miPointerCursorFuncRec macmachPointerCursorFuncs;

    /*
     *	Block/Unblock handlers
     */
    if (autoRepeatHandlersInstalled == FALSE) {
	autoRepeatScreen = pScreen;
	autoRepeatHandlersInstalled = TRUE;
    }

    if (pScreen == autoRepeatScreen) {
        pScreen->BlockHandler = macmachBlockHandler;
        pScreen->WakeupHandler = macmachWakeupHandler;
    }

    miDCInitialize (pScreen, &macmachPointerCursorFuncs);

    return TRUE;
}

/*-
 *-----------------------------------------------------------------------
 * macmachOpenFrameBuffer --
 *	Open a frame buffer through the /dev/scn* interface.
 *
 * Results:
 *	The fd of the framebuffer.
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
int
macmachOpenFrameBuffer(index, fbNum, argc, argv)
int	    	  fbNum;    	/* Index into the macmachFbData array */
int	    	  index;    	/* Screen index */
int	    	  argc;	    	/* Command-line arguments... */
char	  **argv;   	/* ... */
{
    int           fd = -1;	    	/* Descriptor to device */
    VPBlockPtr v;
    int mode;
  

    if ((fd = open(macmachFbData[fbNum].devName, O_RDWR, 0)) < 0)
	return (-1);
    macmachFbs[index].fd = fd;

    (void) macmachVideoSetInit(index);
    /* copy the driver data, base address and page fields onto all possible modes */
    for ( mode = 1; mode < VIDEO_MAXMODES; mode++)
    {
	macmachFbs[index].vpinfo[mode].csData = macmachFbs[index].vpinfo[0].csData;
	macmachFbs[index].vpinfo[mode].csPage = macmachFbs[index].vpinfo[0].csPage;
	macmachFbs[index].vpinfo[mode].csBaseAddr = macmachFbs[index].vpinfo[0].csBaseAddr;
    }
    mode = 0; macmachFbs[index].default_depth = 1;
    v = &(macmachFbs[index].info[mode]);
    if (ioctl(fd, VIDEO_PARAMS, v) < 0 )
    {
	FatalError("could not obtain characteristics (ioctl VIDEO_PARAMS    failed!)\n");
	(void) close (fd);
	return(!Success);
    }
    else
    {
	macmachFbs[index].default_depth = v->vpPixelSize;
    }

  return (fd);
}

void
AbortDDX()
{
    extern int devkbdFd;

    if (devkbdFd > 0) {
	macmachKbdSetUp(devkbdFd, FALSE); /* Must NOT FatalError() anywhere! */
	close(devkbdFd);
	devkbdFd = 0;
    }
}

/* Called by GiveUp(). */
void
ddxGiveUp()
{
}


int
ddxProcessArgument(argc,argv,i)
int	argc;
char	*argv[];
int	i;
{
    int skip = 0;

    if (!strcmp( argv[i], "-dev" ))
    {
	register macmachFbDataRec *ppScr = macmachFbData;
	register macmachFbDataRec *pScr;
	int found= FALSE;
	int count;

	macmachOpenAllScreens = FALSE;
	count = 0;
	while ( ( pScr = ppScr++ ) && !found && (count < NUMSCREENS) ) {
	    if (!strcmp( argv[i+1], pScr->devName)) {
		pScr->Wanted = TRUE;
		macmachScreensWanted++;
		found= TRUE;
		skip= 2;
		if (!strcmp( argv[i+2], "-mono")) {
		    pScr->depthRequested = 1;
		    skip++;
		} else if (!strcmp( argv[i+2], "-color")) {
		    pScr->depthRequested = 8;
		    skip++;
		} else if (!strcmp( argv[i+2], "-depth")) {
		    pScr->depthRequested = atoi(argv[i+3]);
		    skip = skip+2;
		} else { /* use default depth supported by hardware */
		    pScr->depthRequested = DEFAULTDEPTH;
		}
	    }
	    count++;
	}
	if (!found) FatalError("%s not supported\n", &argv[i][1]);
    }
    return(skip);
}


void
ddxUseMsg()
{
}

/*-
 *-----------------------------------------------------------------------
 * macmachWhiteScreen --
 *	Fill a frame buffer with pixel 0.
 *
 * Results:
 *	None
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
int
macmachWhiteScreen(index)
	int index;
{
    fbFd *pf;
    register unsigned char* fb;
    register int fbinc, line, lw;
    register unsigned int *fbt;

    pf = &macmachFbs[index];
    fb = pf->fb; /* Assumed longword aligned! */

    switch (pf->info[macmachFbs[index].mode].vpPixelSize) {
    case 1:
      {
	    fbinc = pf->info[macmachFbs[index].mode].vpRowBytes;
	    for (line = pf->info[macmachFbs[index].mode].vpBounds.top; line < pf->info[macmachFbs[index].mode].vpBounds.bottom; line++) {
		lw = ((pf->info[macmachFbs[index].mode].vpBounds.right - pf->info[macmachFbs[index].mode].vpBounds.left) + 31) >> 5;
	    fbt = (unsigned int *)fb;
	    do {
		*fbt++ = 0x00000000;
	    } while (--lw);
	    fb += fbinc;
	}
	break;
      }
    case 8:
      {
	    fbinc = pf->info[macmachFbs[index].mode].vpRowBytes;
	    for (line = pf->info[macmachFbs[index].mode].vpBounds.top; line < pf->info[macmachFbs[index].mode].vpBounds.bottom; line++) {
		lw = ((pf->info[macmachFbs[index].mode].vpBounds.right - pf->info[macmachFbs[index].mode].vpBounds.left) + 31) >> 5;
	    fbt = (unsigned int *)fb;
	    do {
		*fbt++ = 0x0;
	    } while (--lw);
	    fb += fbinc;
	}
	break;
      }
    default:
	ErrorF("Bad depth in macmachWhiteScreen.");
	break;
    }
}
