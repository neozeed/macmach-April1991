/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	mig_strncpy.c,v $
 * Revision 1.1  90/02/19  18:08:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 2.1  89/05/09  22:06:06  mrt
 * Created.
 * 
 */
/*
 * mig_strncpy.c - by Joshua Block
 *
 * mig_strncp -- Bounded string copy.  Does what the library routine strncpy
 * OUGHT to do:  Copies the (null terminated) string in src into dest, a 
 * buffer of length len.  Assures that the copy is still null terminated
 * and doesn't overflow the buffer, truncating the copy if necessary.
 *
 * Parameters:
 * 
 *     dest - Pointer to destination buffer.
 * 
 *     src - Pointer to source string.
 * 
 *     len - Length of destination buffer.
 */
void mig_strncpy(dest, src, len)
char *dest, *src;
int len;
{
    int i;

    if (len <= 0)
	return;

    for (i=1; i<len; i++)
	if (! (*dest++ = *src++))
	    return;

    *dest = '\0';
    return;
}
