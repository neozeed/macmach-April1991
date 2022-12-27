/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	prs_fs.h,v $
 * Revision 2.6  89/06/03  15:31:57  jsb
 * 	Merged with newer ITC sources.
 * 	[89/05/26  19:08:24  jsb]
 * 
 * Revision 2.5  89/04/22  15:17:15  gm0w
 * 	Updated to RX version.
 * 	[89/04/14            gm0w]
 * 
 */

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
/*
	M.Satyanarayanan
	Information Technology Center
	Carnegie-Mellon University
*/


#ifndef _PRSFS_
#define _PRSFS_

/*
An access list is associated with each directory.  Possession of each of the following rights allows
the possessor the corresponding privileges on ALL files in that directory
*/
#define PRSFS_READ            1 /*Read files*/
#define PRSFS_WRITE           2 /*Write and write-lock existing files*/
#define PRSFS_INSERT          4 /*Insert and write-lock new files*/
#define PRSFS_LOOKUP          8 /*Enumerate files and examine access list */
#define PRSFS_DELETE          16 /*Remove files*/
#define PRSFS_LOCK            32 /*Read-lock files*/
#define PRSFS_ADMINISTER      64 /*Set access list of directory*/

#endif
