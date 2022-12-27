/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	fcrypt.h,v $
 * Revision 2.3  89/06/03  15:31:01  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:08:02  jsb]
 * 
 * Revision 2.2  89/04/22  15:16:35  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

/* $ Log:	fcrypt.h,v $
 * Revision 1.5  89/01/27  15:01:39  ota
 * Added #ifndef around defines for ENCRYPT and DECRYPT
 * 
 * Revision 1.4  88/11/20  15:15:32  kazar
 * improve splitting things up
 * 
 * Revision 1.3  88/11/18  16:31:06  ota
 * Surround whole include with #ifndef.
 * 
 * Revision 1.2  88/11/18  15:12:44  ota
 * remove spurious rcsid dcl
 * 
 * Revision 1.1  88/11/18  10:47:55  ota
 * Initial revision
 *  */

#ifndef ENCRYPTIONBLOCKSIZE

#define ENCRYPTIONBLOCKSIZE 8
/* this function round a length to the correct encryption block size */
#define round_up_to_ebs(v) (((v) + 7) & (~7))

typedef long fc_InitializationVector[ENCRYPTIONBLOCKSIZE/4];

#define MAXROUNDS 16
typedef long fc_KeySchedule[MAXROUNDS];

#ifndef ENCRYPT
#define ENCRYPT 1
#define DECRYPT 0
#endif

#endif
