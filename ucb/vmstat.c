/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vm_stat.c,v $
 * Revision 1.2  89/05/05  18:26:25  mrt
 * 	Cleanup for Mach 2.5
 * 
 *
 *  6-Jun-86  Avadis Tevanian, Jr. (avie) at Carnegie-Mellon University
 *	Use official Mach interface.
 *
 */
/*
 *	File:	vm_stat.c
 *	Author:	Avadis Tevanian, Jr.
 *
 *	Copyright (C) 1986, Avadis Tevanian, Jr.
 *
 *	Display Mach VM statistics.
 */

#include <stdio.h>

#include <mach.h>

struct vm_statistics	vm_stat, last;
int	percent;


int	delay;
char	*pgmname;

main(argc, argv)
	int	argc;
	char	*argv[];
{
	pgmname = argv[0];
	delay = 0;
	if (argc == 2) {
		if (sscanf(argv[1], "%d", &delay) != 1)
			usage();
		if (delay < 0)
			usage();
	}

	if (delay == 0) {
		snapshot();
	}
	else {
		while (1) {
			print_stats();
			sleep(delay);
		}
	}
}

usage()
{
	fprintf(stderr, "usage: %s [ repeat-interval ]\n", pgmname);
	exit(1);
}

banner()
{
	get_stats(&vm_stat);
	printf("Mach Virtual Memory Statistics: ");
	printf("(page size of %d bytes, cache hits %d%%)\n",
				vm_stat.pagesize, percent);
	printf("%6s %6s %4s %4s %8s %8s %8s %8s %8s %8s\n",
		"free",
		"active",
		"inac",
		"wire",
		"faults",
		"copy",
		"zerofill",
		"reactive",
		"pageins",
		"pageout");
	bzero(&last, sizeof(last));
}
snapshot()
{

	get_stats(&vm_stat);
	printf("Mach Virtual Memory Statistics: (page size of %d bytes)\n",
				vm_stat.pagesize);
	pstat("Pages free:", vm_stat.free_count);
	pstat("Pages active:", vm_stat.active_count);
	pstat("Pages inactive:", vm_stat.inactive_count);
	pstat("Pages wired down:", vm_stat.wire_count);
	pstat("\"Translation faults\":", vm_stat.faults);
	pstat("Pages copy-on-write:", vm_stat.cow_faults);
	pstat("Pages zero filled:", vm_stat.zero_fill_count);
	pstat("Pages reactivated:", vm_stat.reactivations);
	pstat("Pageins:", vm_stat.pageins);
	pstat("Pageouts:", vm_stat.pageouts);
	printf("Object cache: %d hits of %d lookups (%d%% hit rate)\n",
			vm_stat.hits, vm_stat.lookups, percent);
}

pstat(str, n)
	char	*str;
	int	n;
{
	printf("%-25s %10d.\n", str, n);
}

print_stats()
{
	static count = 0;

	if (count++ == 0)
		banner();

	if (count > 20)
		count = 0;

	get_stats(&vm_stat);
	printf("%6d %6d %4d %4d %8d %8d %8d %8d %8d %8d\n",
		vm_stat.free_count,
		vm_stat.active_count,
		vm_stat.inactive_count,
		vm_stat.wire_count,
		vm_stat.faults - last.faults,
		vm_stat.cow_faults - last.cow_faults,
		vm_stat.zero_fill_count - last.zero_fill_count,
		vm_stat.reactivations - last.reactivations,
		vm_stat.pageins - last.pageins,
		vm_stat.pageouts - last.pageouts);
	last = vm_stat;
}

get_stats(stat)
	struct vm_statistics	*stat;
{
	if (vm_statistics(current_task(), stat) != KERN_SUCCESS) {
		fprintf(stderr, "%s: failed to get statistics.\n", pgmname);
		exit(2);
	}
	if (stat->lookups == 0)
		percent = 0;
	else
		percent = (stat->hits*100)/stat->lookups;
}
