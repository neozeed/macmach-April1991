/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	voldefs.h,v $
 * Revision 2.6  89/06/03  15:32:53  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:09:22  jsb]
 * 
 * Revision 2.5  89/04/22  15:17:43  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
#endif

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/*

	System:		VICE-TWO
	Module:		voldefs.h
	Institution:	The Information Technology Center, Carnegie-Mellon University

 */

/* If you add volume types here, be sure to check the definition of
   volumeWriteable in volume.h */

#define readwriteVolume		RWVOL
#define readonlyVolume		ROVOL
#define backupVolume		BACKVOL

#define RWVOL			0
#define ROVOL			1
#define BACKVOL			2

/* All volumes will have a volume header name in this format */
#define VFORMAT "V%010lu.vol"
#define VMAXPATHLEN 64		/* Maximum length (including null) of a volume
				   external path name */

/* Values for connect parameter to VInitVolumePackage */
#define CONNECT_FS	1
#define DONT_CONNECT_FS	0
