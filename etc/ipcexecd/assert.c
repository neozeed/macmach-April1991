/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	assert.c,v $
 * Revision 1.1  90/02/19  21:47:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  21:47:02  bww]
 * 
 * Revision 1.4  89/07/04  17:36:45  mrt
 * 	Cleanup for Mach 2.5
 * 
 * 1-Jan-88 Mary Thompson @ Carnegie Mellon
 *	Fixed assert_names to deal with failure of gethostbyname
 *	moved "Checked in message to after call to netname_check_in
 */
#include <stdio.h>
#include <mach.h>
#include <netdb.h>

struct hostent *HOST_ENT;
char            myname[100];


init_assert()
{

	gethostname(myname, 100);
	HOST_ENT = gethostbyname(myname);
	/* this is now a net-work call and can fail */
}

static int assert_one_name(pattern, onename, oneport)
char *pattern;
char *onename;
port_t oneport;
{
	int             gr;
	char            name[256];

	sprintf(name, pattern, onename);
	gr = netname_check_in(NameServerPort, name, task_self(), oneport);
	fprintf(stderr, "Checked in '%s'.  gr=%d\n", name, gr);
	fflush(stderr);
	return (gr);
}

int assert_names(pattern, theport)
char *pattern;
port_t theport;
{
	char          **namep;
	int             gr;

	if ((int)HOST_ENT != 0)
	{   if ((gr = assert_one_name(pattern, HOST_ENT->h_name, theport))
	    	!= KERN_SUCCESS)
	    return (gr);

	    namep = HOST_ENT->h_aliases;
	    while (*namep != 0) {
		if ((gr = assert_one_name(pattern, *namep, theport))
		    != KERN_SUCCESS)
			return (gr);
		namep++;
		}
	}
	else
	{   if ((gr = assert_one_name(pattern, myname, theport))
	    	!= KERN_SUCCESS)
	    return (gr);
	}
	fprintf(stderr, "All names checked in, Service ready.\n");
	fflush(stderr);

	return (KERN_SUCCESS);
}
