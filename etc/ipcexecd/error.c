/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	error.c,v $
 * Revision 1.1  90/02/19  21:47:33  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.2  89/07/04  17:36:54  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
#include <stdio.h>
#include <mach.h>

void PrintGR(gr, str)
int gr;
char *str;
{
	fprintf(stderr, "%s: gr=%d\n", str, gr);
	fflush(stderr);
}

void HaltPrintingError(gr, str)
int gr;
char *str;
{
	PrintGR(gr, str);
	exit(2);
}
