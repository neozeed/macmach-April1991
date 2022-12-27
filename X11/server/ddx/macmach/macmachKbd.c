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
 * macmachKbd.c --
 *	Functions for retrieving data from a keyboard.
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


#define NEED_EVENTS
#include "macmach.h"
#include <stdio.h>
#include "Xproto.h"
#include "inputstr.h"

extern CARD8 *macmachModMap[];
extern KeySymsRec macmachKeySyms[];

extern void	miPointerPosition();

static void 	  macmachBell();
static void 	  macmachKbdCtrl();
void 	  	  macmachKbdProcessEvent();
static void 	  macmachKbdDoneEvents();
int	  	  autoRepeatKeyDown = 0;
int	  	  autoRepeatDebug = 0;
int	  	  autoRepeatReady;
long		autoRepeatInitiate = 1000 * AUTOREPEAT_INITIATE;
long		autoRepeatDelay = 1000 * AUTOREPEAT_DELAY;
static int	  autoRepeatFirst;
long 		  autoRepeatLastKeyDownTv;
long 		  autoRepeatDeltaTv;
static KeybdCtrl  sysKbCtrl;

static KbPrivRec  	sysKbPriv = {
    -1,				/* Type of keyboard */
    macmachKbdProcessEvent,	/* Function to process an event */
    macmachKbdDoneEvents,		/* Function called when all events */
				/* have been handled. */
    (pointer) NULL,		/* Private to keyboard device */
    0,				/* offset for device keycodes */
    &sysKbCtrl,                 /* Initial full duration = .20 sec. */
};

extern int devkbdFd;

/*-
 *-----------------------------------------------------------------------
 * macmachKbdProc --
 *	Handle the initialization, etc. of a keyboard.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *
 * Note:
 *	When using macmach, all input comes off a single fd, stored in the
 *	global devkbdFd.  Therefore, only one device should be enabled and
 *	disabled, even though the application still sees both mouse and
 *	keyboard.  We have arbitrarily chosen to enable and disable devkbdFd
 *	in the keyboard routine macmachKbdProc rather than in macmachMouseProc.
 *
 *-----------------------------------------------------------------------
 */

int
macmachKbdProc (pKeyboard, what)
    DevicePtr	  pKeyboard;	/* Keyboard to manipulate */
    int	    	  what;	    	/* What to do to it */
{
    switch (what) {
	case DEVICE_INIT:
	    if (pKeyboard != LookupKeyboardDevice()) {
		ErrorF ("Cannot open non-system keyboard");
		return (!Success);
	    }
	    if (devkbdFd == 0) {
		if ((devkbdFd = open("/dev/kb0", O_RDWR|O_NDELAY)) < 0) {
			FatalError("Could not open /dev/kb0. \r\n");
			return (!Success);
		}
            }

	    /*
	     * Perform final initialization of the system private keyboard
	     * structure and fill in various slots in the device record
	     * itself which couldn't be filled in before.
	     */
	    pKeyboard->devicePrivate = (pointer)&sysKbPriv;
	    pKeyboard->on = FALSE;
	    sysKbCtrl = defaultKeyboardControl;
	    sysKbPriv.ctrl = &sysKbCtrl;

	    /*
	     * ensure that the keycodes on the wire are >= MIN_KEYCODE
	     */

	    sysKbPriv.type = KBTYPE_MACII;  

	    if (macmachKeySyms[sysKbPriv.type].minKeyCode < MIN_KEYCODE) {
		int offset = MIN_KEYCODE -macmachKeySyms[sysKbPriv.type].minKeyCode;

		macmachKeySyms[sysKbPriv.type].minKeyCode += offset;
		macmachKeySyms[sysKbPriv.type].maxKeyCode += offset;
		sysKbPriv.offset = offset;
	    }
	    InitKeyboardDeviceStruct(
		    pKeyboard,
		    &(macmachKeySyms[sysKbPriv.type]),
		    (macmachModMap[sysKbPriv.type]),
		    macmachBell,
		    macmachKbdCtrl);
	    break;

	case DEVICE_ON:
	    macmachKbdSetUp(devkbdFd, TRUE);

	    AddEnabledDevice(devkbdFd);

	    pKeyboard->on = TRUE;
	    break;

	case DEVICE_CLOSE:
	    macmachKbdSetUp(devkbdFd, FALSE);

	    RemoveEnabledDevice(devkbdFd);

	    pKeyboard->on = FALSE;

	    close(devkbdFd);
	    devkbdFd = 0;
	    break;

	case DEVICE_OFF:
	    macmachKbdSetUp(devkbdFd, FALSE);

	    RemoveEnabledDevice(devkbdFd);

	    pKeyboard->on = FALSE;
	    break;

    }
    return (Success);
}

#include <sys/ioctl.h>

int
macmachKbdSetUp(fd, openClose)
    int		fd;
    Bool	openClose;
{
	int iarg;

	if (openClose)
	{
	    iarg = 1;
	    if (ioctl(fd, FIOASYNC, &iarg) < 0)
	    {
		FatalError("macmachKbSetup: ioctl FIOASYNC failed!\n");
		return !Success;
	    }
	}
	else
	{
	    iarg = 0;
	    if (ioctl(fd, FIOASYNC, &iarg) < 0)
	    {
		FatalError("macmachKbSetup: ioctl FIOASYNC failed!\n");
		return !Success;
	    }
	}
}

/*-
 *-----------------------------------------------------------------------
 * macmachBell --
 *	Ring the terminal/keyboard bell
 *
 * Results:
 *    Ring the keyboard bell for an amount of time proportional to
 *    "loudness."
 *
 * Side Effects:
 *	None, really...
 *
 *-----------------------------------------------------------------------
 */
static void
macmachBell (loudness, pKeyboard)
    int	    	  loudness;	    /* Percentage of full volume */
    DevicePtr	  pKeyboard;	    /* Keyboard to ring */
{
    KbPrivPtr   pPriv = (KbPrivPtr) pKeyboard->devicePrivate;
    long countdown;

    if (loudness == 0) {
      return;
    }

}

/*-
 *-----------------------------------------------------------------------
 * macmachKbdCtrl --
 *	Alter some of the keyboard control parameters
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Some...
 *
 *-----------------------------------------------------------------------
 */
static void
macmachKbdCtrl (pKeyboard, ctrl)
    DevicePtr	  pKeyboard;	    /* Keyboard to alter */
    KeybdCtrl     *ctrl;
{
    KbPrivPtr   pPriv = (KbPrivPtr) pKeyboard->devicePrivate;
    *pPriv->ctrl = *ctrl;
}

/*-
 *-----------------------------------------------------------------------
 * macmachDoneEvents --
 *	Nothing to do, here...
 *
 * Results:
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */
static void
macmachKbdDoneEvents (pKeyboard)
    DevicePtr	  pKeyboard;
{
}

/*-
 *-----------------------------------------------------------------------
 * macmachKbdProcessEvent
 *	Process macmachevent destined for the keyboard.
 * 	
 * Results:
 *
 * Side Effects:
 *
 *-----------------------------------------------------------------------
 */

void
macmachKbdProcessEvent(pKeyboard,me)
    DeviceRec *pKeyboard;
    register unsigned char *me;
{   
    xEvent		xE;
    PtrPrivPtr	  	ptrPriv;
    KbPrivPtr           pPriv;
    int                 delta;
    static xEvent       autoRepeatEvent;
    BYTE		key;
    CARD16              keyModifiers;

    ptrPriv = (PtrPrivPtr) LookupPointerDevice()->devicePrivate;

    if (autoRepeatKeyDown && *me == AUTOREPEAT_EVENTID) {
       pPriv = (KbPrivPtr) pKeyboard->devicePrivate;
       if (pPriv->ctrl->autoRepeat != AutoRepeatModeOn) {
               autoRepeatKeyDown = 0;
               return;
       }

	/*
	 * Generate auto repeat event.	XXX one for now.
	 * Update time & pointer location of saved KeyPress event.
	 */
	if (autoRepeatDebug)
	    ErrorF("macmachKbdProcessEvent: autoRepeatKeyDown = %d\r\n",
			autoRepeatKeyDown);

	delta = autoRepeatDeltaTv;
	autoRepeatFirst = FALSE;

	/*
	 * Fake a key up event and a key down event
	 * for the last key pressed.
	 */
	autoRepeatEvent.u.keyButtonPointer.time += delta;
        miPointerPosition (screenInfo.screens[0],
                           &autoRepeatEvent.u.keyButtonPointer.rootX,
                           &autoRepeatEvent.u.keyButtonPointer.rootY);
	autoRepeatEvent.u.u.type = KeyRelease;
	(* pKeyboard->processInputProc) (&autoRepeatEvent, pKeyboard, 1);
	
	autoRepeatEvent.u.u.type = KeyPress;
	(* pKeyboard->processInputProc) (&autoRepeatEvent, pKeyboard, 1);
	
	/* Update time of last key down */
	autoRepeatLastKeyDownTv += autoRepeatDeltaTv;

	return;
    }

    key = KEY_DETAIL(*me) + sysKbPriv.offset;
    if ((keyModifiers = ((DeviceIntPtr)pKeyboard)->key->modifierMap[key]) == 0) {
        /*
         * Kill AutoRepeater on any real Kbd event.
         */
        autoRepeatKeyDown = 0;
        if (autoRepeatDebug)
            ErrorF("macmachKbdProcessEvent: autoRepeat off\r\n");
    }

    xE.u.keyButtonPointer.time = lastEventTime;
    miPointerPosition (screenInfo.screens[0],
                       &xE.u.keyButtonPointer.rootX,
                       &xE.u.keyButtonPointer.rootY);
    xE.u.u.type = (KEY_UP(*me) ? KeyRelease : KeyPress);
    xE.u.u.detail = key;

    if ((xE.u.u.type == KeyPress) && (keyModifiers == 0)) {
	  /* initialize new AutoRepeater event & mark AutoRepeater on */
        if (autoRepeatDebug)
            ErrorF("macmachKbdProcessEvent: KEY_DOWN\r\n");
        autoRepeatEvent = xE;
        autoRepeatFirst = TRUE;
        autoRepeatKeyDown++;
        autoRepeatLastKeyDownTv = lastEventTime;
    }

    (* pKeyboard->processInputProc) (&xE, pKeyboard, 1);
}


Bool
LegalModifier(key)
{
    return (TRUE);
}

static KeybdCtrl *pKbdCtrl = (KeybdCtrl *) 0;

#include <sys/time.h>
/*ARGSUSED*/
void
macmachBlockHandler(nscreen, pbdata, pptv, pReadmask)
    int nscreen;
    pointer pbdata;
    struct timeval **pptv;
    pointer pReadmask;
{
    static struct timeval artv = { 0, 0 };    /* autorepeat timeval */

    if (!autoRepeatKeyDown)
        return;

    if (pKbdCtrl == (KeybdCtrl *) 0)
      pKbdCtrl = ((KbPrivPtr) LookupKeyboardDevice()->devicePrivate)->ctrl;

    if (pKbdCtrl->autoRepeat != AutoRepeatModeOn)
      return;

    if (autoRepeatFirst == TRUE)
        artv.tv_usec = autoRepeatInitiate;
    else
        artv.tv_usec = autoRepeatDelay;
    *pptv = &artv;

    if (autoRepeatDebug)
        ErrorF("macmachBlockHandler(%d,%d): \r\n", artv.tv_sec, artv.tv_usec);

}

/*ARGSUSED*/
void
macmachWakeupHandler(nscreen, pbdata, err, pReadmask)
    int nscreen;
    pointer pbdata;
    unsigned long err;
    pointer pReadmask;
{
    long now;

    if (autoRepeatDebug)
        ErrorF("macmachWakeupHandler(ar=%d, err=%d):\r\n", autoRepeatKeyDown, err);

    if (pKbdCtrl == (KeybdCtrl *) 0)
      pKbdCtrl = ((KbPrivPtr) LookupKeyboardDevice()->devicePrivate)->ctrl;
    
    if (pKbdCtrl->autoRepeat != AutoRepeatModeOn)
      return;

    if (autoRepeatKeyDown) {
	struct timeval tv;

	gettimeofday (&tv, (struct timezone *)0);
	now = TVTOMILLI(tv);

        autoRepeatDeltaTv = now - autoRepeatLastKeyDownTv;
        if ((!autoRepeatFirst && autoRepeatDeltaTv > AUTOREPEAT_DELAY) ||
            (autoRepeatDeltaTv > AUTOREPEAT_INITIATE))
                autoRepeatReady++;
    }

    if (autoRepeatReady)
        ProcessInputEvents();
    autoRepeatReady = 0;

}

