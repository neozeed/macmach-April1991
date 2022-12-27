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
 * $Log:	font.h,v $
 * Revision 2.2  90/08/30  11:06:02  bohman
 * 	Created.
 * 	[90/08/29  12:45:20  bohman]
 * 
 */

/*
 *	Font data structures
 *
 *	Copyright 1986 Unisoft Corporation of Berkeley CA
 *
 *
 *	UniPlus Source Code. This program is proprietary
 *	with Unisoft Corporation and is not to be reproduced
 *	or used in any manner except as authorized in
 *	writing by Unisoft.
 *
 */

struct fontrec {
	unsigned short		fontType;
	unsigned short		firstChar;
	unsigned short		lastChar;
	unsigned short		widMax;
	short			kernMax;
	short			nDescent;
	unsigned short		fRectMax;
	unsigned short		chHeight;
	unsigned short		owTLoc;
	short			ascent;
	short			descent;
	short			leading;
	unsigned short		rowWords;
	unsigned short		bitImage[1];
};

#define	PROPFONT	0x9000	
#define	FIXEDFONT	0xB000	
#define	FONTWID		0xACB0	

struct font {
	struct fontrec *font_pnt;
	unsigned int font_height;
	unsigned int font_width;
	unsigned int font_leading;
	unsigned int font_rowwords;
	unsigned char *font_bitimage;
	unsigned short *font_loctable;
	unsigned short *font_owtable;
	unsigned int font_linewidth;
	unsigned int font_maxx;
	unsigned int font_maxy;
	unsigned int font_inverse;
	struct cons_data *font_screen;
	unsigned int font_theight;
	unsigned int font_offset;
};

/*
** N.B.
** These constants are used to "shave" off the top, left and right
** sides of the screen. We do this so that the vt100 "window" fits
** in a thing that looks like a mac screen.
**	These values are in pixels.
*/

# define	LEFT_SHAVE	6	/* The amount to shave off of the left edge */
# define	RIGHT_SHAVE	6	/* The amount to shave off of the right edge */

/*
** The number of characters to "shave" off of the top, and bottom of the screen.
*/

# define	TOP_SHAVE	1
# define	BOTTOM_SHAVE	1

