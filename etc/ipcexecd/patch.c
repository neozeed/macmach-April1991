/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	patch.c,v $
 * Revision 1.1  90/02/19  21:47:48  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/07/04  17:37:20  mrt
 * 	Cleanup for Mach 2.5
 * 
 */

#include <stdio.h>

int STAT_WIDTH1, STAT_WIDTH2;
char *STAT_POS1, *STAT_POS2;

extern char *index();
extern long time();
extern long LAST_REQUEST_TIME;
extern int REQUESTS;
extern int NAME_SERVER_UP;

void init_patch(str)
char *str;
{
	char           *end_pos;

	STAT_POS1 = index(str, '/');
	STAT_POS1++;
	end_pos = index(STAT_POS1, '/');
	if (end_pos == 0) {
		STAT_WIDTH1 = 0;
		STAT_WIDTH2 = 0;
	} else {
		STAT_WIDTH1 = end_pos - STAT_POS1;
		end_pos++;
		STAT_POS2 = index(end_pos, '/');
		if (STAT_POS2 == 0)
			STAT_WIDTH2 = 0;
		else {
			STAT_POS2++;
			end_pos = index(STAT_POS2, '/');
			if (end_pos == 0)
				STAT_WIDTH2 = 0;
			else
				STAT_WIDTH2 = end_pos - STAT_POS2;
		}
	}
}

static int power(a, b)
int a, b;
{
	int             i, ans;

	ans = 1;
	for (i = 0; i < b; i++)
		ans *= a;
	return (ans);
}

static void patch_int(num, pos, width)
int num, width;
char *pos;
{
	int             i;

	if (width == 0)
		return;

	if (num < 0)
		for (i = 0; i < width; i++) {
			*pos = '-';
			pos++;
		}
	else if (num >= power(10, width)) {
		*pos = '>';
		pos++;
		for (i = 1; i < width; i++) {
			*pos = '0' + num % power(10, width - i) /
				power(10, width - i - 1);
			pos++;
		}
	} else {
		for (i = 0; i < width; i++) {
			*pos = '0' + num % power(10, width - i) /
				power(10, width - i - 1);
			pos++;
		}
	}
}

void patch_cmd_line()
{
	long            idle_time;

	idle_time = (time(0) - LAST_REQUEST_TIME) / 60;

	patch_int((NAME_SERVER_UP) ? idle_time : -1,
		  STAT_POS1, STAT_WIDTH1);
	patch_int(REQUESTS, STAT_POS2, STAT_WIDTH2);
}
