/* 
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *	File:	ps.c
 *	Author:	Karl Hauth
 *
 *	Our old friend, ps, with an intelligence upgrade and an
 *	understanding of threads and Mach, but still with the cruft that
 *	mucks around in /dev/kmem.  "It's not my fault!"
 *
 *	PS now needs to run as mach.kmem.  It needs to be in the kmem group
 *	so that it can read the /dev/kmem and use the task_by_unix_pid call.
 *
 * HISTORY
 * 30-Aug-89  David Black (dlb) at Carnegie-Mellon University
 *	Convert to new host_info call and new Mach priorities.
 *	This is not backwards compatible with previous versions.
 *
 *  2-Sep-88 Mary Thompson (mrt) at Carnegie Mellon
 *	Changed ps -U to do nothing (we are saving the U switch
 *	for future use.
 *
 * 30-Aug-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Print "." as the terminal when disconnected.
 *
 * 16-Aug-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Handle tasks with zero threads.  Allow argument size to be
 *	enlarged with command option "A".
 *
 * 16-May-88  David Golub (dbg) at Carnegie-Mellon University
 *	Added transition code:  if ps finds that table(TBL_PROCINFO)
 *	doesn't work, it reads the process structures from /dev/kmem.
 *	The kernel include version does not distinguish whether
 *	TBL_PROCINFO is implemented.
 *
 * 30-Apr-88  David Black (dlb) at Carnegie-Mellon University
 *	Fix m switch for multi-threaded tasks.
 *
 * 29-Apr-88  David Black (dlb) at Carnegie-Mellon University
 *	Added code to support reporting of cpu usage.
 *
 * 15-Mar-88  David Golub (dbg) at Carnegie-Mellon University
 *	Use new table option (TBL_PROCINFO) to eliminate use of
 *	/dev/kmem (yay!).  Restore sorting.
 *
 * 23-Feb-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	No longer prints kernel task when you ask for pid #.
 *
 * 18-Feb-88  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Print --- instead of 0.0 for cpu utilization (pending
 *	implementation).  Use "." instead of ":" in cpu time printouts.
 *
 * 12-Feb-88  Michael Young (mwyoung) at Carnegie-Mellon University
 *	Fix "t" switch handling, pid parsing.
 *
 * 12-Feb-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Implented the 'c' flag, as per facilities version.
 *
 * 26-Jan-88  David Golub (dbg) at Carnegie-Mellon University
 *      Try out new statistics interfaces.
 *
 * 21-Jan-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Fixed to handle printing the kernel task, but only under -xa.
 *
 * 19-Jan-88  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Fixed up some problems with argument parsing that was dumping
 *	core.
 *	Purged history.
 *
 * 14-Jul-87  Karl Hauth (hauth) at Carnegie-Mellon University
 *	Rewrote from near-scratch.  It still mucks around in /dev/kmem
 *	for the proc table, and I didn't want to rewrite that part of
 *	the code.  Someday (the sages tell) there will be a Unix Server
 *	which will enable us to clean up the mess at the end of this
 *	file.
 *
 */
#ifndef	KERNEL_FEATURES
#define	KERNEL_FEATURES
#endif	KERNEL_FEATURES

#ifndef mac2
#include <sys/features.h>	/* necessary? */
#endif
#include <sys/version.h>	/* this is what we really want */

#if	(INCLUDE_VERSION == 12)
#define	PROC_COMPAT	1	/* if TBL_PROCINFO missing, read proc structure
				   from /dev/kmem */
#endif

#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <sys/file.h>
#include <pwd.h>
#include <mach.h>
#include <sys/param.h>
#include <sys/table.h>
#include <sys/types.h>
#include <machine/vmparam.h>

#include <mach/host_info.h>

extern	uid_t	getuid();
extern	char	*calloc();
extern	char	*malloc();
extern	char	*realloc();
extern	char	*ttyname();
extern	char	*getenv();
extern	char	*tgetstr();	/* to find the width of the terminal */

#define sec_to_minutes(t)       ((t) / 60)
#define sec_to_seconds(t)       ((t) % 60)
#define usec_to_100ths(t)       ((t) / 10000)

#ifndef	TH_USAGE_SCALE
#define	TH_USAGE_SCALE	1000
#endif	TH_USAGE_SCALE
#define usage_to_percent(u)	((u*100)/TH_USAGE_SCALE)
#define usage_to_tenths(u)	(((u*1000)/TH_USAGE_SCALE) % 10)

int		width = 80;
int		cmdstart;
uid_t		our_uid;
int		pid_to_check=0;
int		aflg=0, cflg=0, eflg=0, gflg=0, lflg=0;
int		mflg=0, nflg=0, sflg=0, uflg=0, vflg=0;
int		xflg=0, Uflg=0, tflg=0;
char*		tptr = NULL;
char		*name_list_file = NULL;
int		arg_depth = 0x1000;	/* Stack depth to search for args */

typedef	struct proc_info	*proc_info_t;
struct	proc_info {
	uid_t			uid;
	short			pid;
	short			ppid;
	short			pgrp;
	int			status;
	int			flag;

	int			state;
	int			pri;
	int			base_pri;
	boolean_t		all_swapped;
	time_value_t		total_time;

	vm_size_t		virtual_size;
	vm_size_t		resident_size;
	float			percent_mem;
	int			cpu_usage;

	vm_size_t		text_size;
	char			*term;
	char			*command;

	int			num_threads;
	thread_basic_info_t	threads;	/* array */
};

proc_info_t			proc_table = (proc_info_t)0;
int				max_proc_table, nprocs;




/*
 *	ensure argspac is aligned on an integer boundary,
 *	since we scan it as integers
 */
char	*arguments;
int	arguments_size = 4097;

/*
 *	Translate thread state to a number in an ordered scale.
 *	When collapsing all the threads' states to one for the
 *	entire task, the lower-numbered state dominates.
 */
#define	STATE_MAX	7

int
mach_state_order(s, sleep_time)
        int s;
        long sleep_time;
 {
    switch (s) {
    case TH_STATE_RUNNING:      return(1);
    case TH_STATE_UNINTERRUPTIBLE:
                                return(2);
    case TH_STATE_WAITING:      return((sleep_time > 20) ? 4 : 3);
    case TH_STATE_STOPPED:      return(5);
    case TH_STATE_HALTED:       return(6);
    default:                    return(7);
    }
 }
			    /*01234567 */
char	mach_state_table[] = " RUSITH?";

char *
translate_terminal(dev, buf)
    dev_t dev;
    char buf[4];
{
    int num;

    buf[2] = 0; buf[3] = 0;

    switch (major(dev)) {
#ifdef	romp
    case  0:
	if (minor(dev) == 0) {
	    buf[0] = 'c';
	    buf[1] = 'o';
	} else {
	    buf[0] = '?';
	    buf[1] = ' ';
	}
	return(buf);
    case  1:	buf[0] = '0';	break;
    case  6:
    case  7:	buf[0] = 'p';	break;
    case 12:	buf[0] = 's';	break;
    case 17:
    case 18:	buf[0] = 'P';	break;
#endif	romp
#ifdef mac2
    case  0:
	buf[0] = 'c';
	buf[1] = 'o';
	return( buf );
    case 12:
	buf[0] = minor(dev) + 'a';
	buf[1] = ' ';
	return( buf );
    case 20:
    case 21:	buf[0] = 'p';	break;
    case 35:	buf[0] = 'P';	break;
#endif mac2
#ifdef	sun
    case  0:
	buf[0] = 'c';
	buf[1] = 'o';
	return( buf );
    case 12:
	buf[0] = minor(dev) + 'a';
	buf[1] = ' ';
	return( buf );
    case 20:
    case 21:	buf[0] = 'p';	break;
    case 35:	buf[0] = 'P';	break;
#endif	sun
#ifdef	vax
    case 0:	buf[0] = 'c';	buf[1] = 'o';	return(buf);
    /*   1:	dz */
    /*	 12:	dh */
    case 20:
    case 21:	buf[0] = 'p';	break;
    /*	 22:	dmf */
    case 28:
    case 29:	buf[0] = 'P';	break;
    /*	 37:	dmz */
    case 48:	buf[0] = 'q';	buf[1] = 'v';	return(buf);
    case 49:	buf[0] = 's';	buf[1] = 'l';	return(buf);
    case 50:	buf[0] = 'q';	buf[1] = 'v';	return(buf);
    /*	 55	dhu */
    case 59:	buf[0] = 'q';	buf[1] = 'd';	return(buf);
#endif	vax
#ifdef	multimax
    case 0:
	buf[0] = 'c';
	buf[1] = 'o';
	return( buf );
    case 1:	buf[0] = '0';	break;
    case 6:
    case 7:	buf[0] = 'p';	break;
    case 12:
    case 13:	buf[0] = 'P';	break;
#endif	multimax

    /* Disconnected terminals show up as using /dev/tty: */
    case 2:	buf[0] = '.';   buf[1] = '\0'; return(buf);

    default:	buf[0] = '?';	buf[1] = ' ';	return(buf);
    }

    num = minor(dev);
    buf[0] += num / 16;
    buf[1] = num % 16;
    if (buf[1] > 9)
	buf[1] += ('a' - 10);
    else
        buf[1] += '0';
    return( buf );
}


char *
digits(n)
    float	n;
{
    static char	tmp[10];	/* STATIC! */

    if ((n > 0) && (n < 10))
	sprintf(tmp, "%4.2f", n);
    else if ((n > 0) && (n < 100))
	sprintf(tmp, "%4.1f", n);
    else
	sprintf(tmp, "%4.0f", n);
    return(tmp);
}
char *
mem_to_string(n)
    vm_size_t	n;
{
    char	*s;

    /* convert to bytes */
    n /= 1024;

    s = malloc(11);
    if (n > 1024*1024)
	sprintf(s, "%sG", digits(((float)n)/(1024.0*1024.0)));
    else if (n > 1024)
	sprintf(s, "%sM", digits((float)n/(1024.0)));
    else
	sprintf(s, "%dK", n);

    return( s );
}




main(argc, argv)
int	argc;
char	*argv[];
{
    char	bp[1024];

    if (getenv("TERM") == NULL) {
	width = 80;
    } else {
	tgetent(bp, getenv("TERM"));
	width = tgetnum("co");
    }

    our_uid = getuid();

    while (argc > 1) {
	argc--;
	if (isdigit(*argv[argc]))	get_pid_to_check(argv[argc]);
	else if (*argv[argc] == '/')	get_file_name(argv[argc]);
	else				parse_switches(argv[argc]);
    }
    print_all_tasks();
    exit(0);
}

usage()
{
    fprintf(stderr, "usage: ps [aAceglmnstuUvwx] [#] [filename]\n");
    exit(-1);
}


get_pid_to_check(s)
char *s;
{
    if (isdigit(*s)) {
	sscanf(s, "%d", &pid_to_check);
	aflg++;
	xflg++;
    } else {
	usage();
    }
}

get_file_name(s)
char *s;
{
    name_list_file = s;
}


parse_switches(s)
char *s;
{
    do {
	switch (*s) {
	case '-':	break;
	case 'a':	aflg++;		break;
	case 'c':	cflg = !cflg;	break;
	case 'e':	eflg++;		break;
	case 'g':	gflg++;		break;
	case 'l':	lflg++;		break;
	case 'm':	mflg++;		break;
	case 'n':	nflg++;		break;
	case 's':	sflg++;		break;
	case 'u':	uflg++;		break;
	case 'x':	xflg++;		break;
	case 'U':   /* /etc/rc calls ps -U but for now we do nothing */
		Uflg++;
		exit(0);
		break;

	case 'A':
		arguments_size += arguments_size;
		break;

	case 'v':
	    vflg++;
	    cflg = 1;
	    break;

	case 'w':
	    if (width < 132)
		width = 132;
	    else if (width < BUFSIZ)
		width = BUFSIZ;
	    else
	    	width += width;
	    break;

	case 't':
	    tflg++;
	    if (*(tptr = s + 1) == '\0') {
		tptr = ttyname(0);
		if (strncmp(tptr, "/dev/", 5) == 0)
		    tptr += 5;
	    }
	    if (strncmp(tptr, "tty", 3) == 0)
			tptr += 3;
	    aflg++;
	    gflg++;
	    if (tptr && *tptr == '?')
		xflg++;
	    s = "\0\0";
	    break;

	default:
	    if (isdigit(*s)) {
		get_pid_to_check(s);
		return;
	    }

	    usage();
	}
    } while (*(++s) != NULL);
}

char *getname(uid)
	uid_t	uid;
{
	register struct passwd *pw;
	struct passwd *getpwuid();

	pw = getpwuid((int)uid);
	if (pw == NULL) {
		return( NULL );
	}
	return( pw->pw_name );
}


int pscomp(p1, p2)
	proc_info_t	p1, p2;
{
	register int i;

	if (uflg) {
	     return (p2->cpu_usage > p1->cpu_usage ? 1 : -1);
	}

	if (vflg) {
	    return (p2->resident_size - p1->resident_size);
	}

	i = strcmp(p1->term, p2->term);
	if (i == 0)
	    i = p1->pid - p2->pid;
	return (i);
}

print_all_tasks()
{
    int i;


    arguments = (char *) calloc((arguments_size / sizeof(int)), sizeof(int));

    max_proc_table = 200;
    proc_table = (proc_info_t)calloc((unsigned)max_proc_table,
    				     sizeof(struct proc_info));
    get_proc_table();
    print_header();

    qsort((char *)proc_table, nprocs, sizeof(struct proc_info),
    	  pscomp);

    for (i = 0; i < nprocs; i++)
	print_this_task(&proc_table[i]);
}


print_this_task(pi)
	proc_info_t	pi;
{

    if (mflg != 0) {
	printf("%-8.8s%5d %-2.2s",
		getname(pi->uid),
		pi->pid,
		pi->term);
	print_threads(pi->threads, pi->num_threads, pi->command);
	return;
    }

    /*
     *	Convert Mach priorities (0-31) to approximately corresponding
     *	Unix priorities.
     */
    if (pi->pri > 0) {
	    pi->pri *= 4;
	    pi->pri += 2;
    }
    if (pi->base_pri > 0) {
	pi->base_pri *= 4;
	pi->base_pri += 2;
    }

    if (lflg != 0) {

	/* I don't know why, I just know they can't all be in the
	       same printf.  Sigh.  -kh */
	printf("%7x %4d%6u%6u  0 %3d%3d    0%5.5s%5.5s     0",
		pi->flag,
		pi->uid,
		pi->pid,
		pi->ppid,
		/* cpu */
		pi->pri - 50,
		(pi->base_pri / 2) - 25,	/* 'nice' */
		/* addr */
		mem_to_string(pi->virtual_size),
		mem_to_string(pi->resident_size));
	printf(" %c%c%c  %-2.2s",
		mach_state_table[pi->state],
		(pi->all_swapped) ? 'W' : ' ',
		(pi->base_pri > 50) ? 'N' :
			(pi->base_pri < 40) ? '<' : ' ',
		pi->term);
	print_time(pi->total_time.seconds);

    } else if (vflg != 0) {

	printf("%5u %-2.2s %c%c%c ",
		pi->pid,
		pi->term,
		mach_state_table[pi->state],
		(pi->all_swapped) ? 'W' : ' ',
		(pi->base_pri > 50) ? 'N' :
			(pi->base_pri < 40) ? '<' : ' ');
	print_time(pi->total_time.seconds);
	printf("  0  0      0"
		/* sleep_time */
		/* number of seconds swapped in */
		/* major page faults */
		);
	printf("%5.5s%5.5s     0%5.5s",
		mem_to_string(pi->virtual_size - pi->text_size),
		mem_to_string(pi->resident_size),
		/* maxrss */
		mem_to_string(pi->text_size));
	printf("   0%4d.%1d%5.1f",
		/* text rss */
		usage_to_percent(pi->cpu_usage),
		usage_to_tenths(pi->cpu_usage),
		pi->percent_mem);

    } else if (uflg != 0) {

	if (nflg != 0) {
	    printf("%4d ", pi->uid);
	} else {
	    printf("%-8.8s ", getname(pi->uid));
	}
	printf("%5d%4d.%1d%5.1f%5.5s%5.5s ",
		pi->pid,
		usage_to_percent(pi->cpu_usage),
		usage_to_tenths(pi->cpu_usage),
		pi->percent_mem, 
		mem_to_string(pi->virtual_size),
		mem_to_string(pi->resident_size));
	printf("%-2.2s %c%c%c ",
		pi->term,
		mach_state_table[pi->state],
		(pi->all_swapped) ? 'W' : ' ',
		(pi->base_pri > 50) ? 'N' :
			(pi->base_pri < 40) ? '<' : ' ');
	print_time(pi->total_time.seconds);

    } else {

	if (sflg != 0)
	    printf("     "); /* kernel stack size */
	printf("%5u %-2.2s %c%c%c ",
		pi->pid,
		pi->term,
		mach_state_table[pi->state],
		(pi->all_swapped) ? 'W' : ' ',
		(pi->base_pri > 50) ? 'N' :
			(pi->base_pri < 40) ? '<' : ' ');
	print_time(pi->total_time.seconds);
    }
    printf(" %.*s\n", width - cmdstart - 2,  pi->command);
}

print_threads(thread_table, number, command)
    register thread_basic_info_t	thread_table;	/* array */
    int			number;
    char		*command;
{
    register int	i;
    register thread_basic_info_t	thi;

    if (number == 0) {
	printf("%9.9s%-28.28s%.*s\n", "", "<no threads>",
		width - cmdstart - 2, command);
	return;
    }
    for (i = 0, thi = thread_table; i < number; i++, thi++) {
	if (i != 0)
	    printf("%16.16s", "");
	printf("%4d.%1d  %c%c%c %3d %3d:%.2d.%.2d %3d:%.2d.%.2d  ",
	    usage_to_percent(thi->cpu_usage),
	    usage_to_tenths(thi->cpu_usage),
	    mach_state_table[mach_state_order(thi->run_state,
	    				      thi->sleep_time)],
	    (thi->flags&TH_FLAGS_SWAPPED) ? 'W' : ' ',
	    (thi->base_priority > 12) ? 'N' :
		(thi->base_priority < 10) ? '<' : ' ',
	    thi->cur_priority,
	    sec_to_minutes(thi->system_time.seconds),
	    sec_to_seconds(thi->system_time.seconds),
	    usec_to_100ths(thi->system_time.microseconds),
	    sec_to_minutes(thi->user_time.seconds),
	    sec_to_seconds(thi->user_time.seconds),
	    usec_to_100ths(thi->user_time.microseconds));
	if (i == 0)
	    printf("%.*s", width - cmdstart - 2, command);
	putchar('\n'); 
    }
}

print_time(seconds)
    long	seconds;
{
    if (seconds < 999*60) {
	printf("%3d:%02d", seconds/60, seconds % 60);
    }
    else if (seconds < 9999*60*60) {
	printf("%4dhr", seconds/(60*60));
    }
    else {
	printf("%3dday", seconds/(60*60*24));
    }
}

char *lhdr =
    "      F  UID   PID  PPID CP PRI NI ADDR   SZ  RSS WCHAN STAT TT  TIME";
char *vhdr =
    "  PID TT STAT  TIME SL RE PAGEIN SIZE  RSS   LIM TSIZ TRS  %CPU %MEM";
char *mhdr =
    "USER      PID TT  %CPU STAT PRI    SYSTEM      USER ";
char *uhdr =
    "%s   PID  %%CPU %%MEM   SZ  RSS TT STAT  TIME";
char *shdr =
    "%s  PID TT STAT  TIME";

print_header()
{
    char	header_buf[80];
    char	*hdr;

    if (lflg + mflg + sflg + uflg + vflg > 1) {
	fprintf(stderr, "ps: specify only one of l,m,s,u and v\n");
	exit(1);
    }
    if (lflg != 0) {
	hdr = lhdr;
    } else if (vflg != 0) {
	hdr = vhdr;
    } else if (uflg != 0) {
	sprintf(header_buf, uhdr, nflg ? " UID" : "USER    ");
	hdr = header_buf;
    } else if (mflg != 0) {
	hdr = mhdr;
    } else {
	sprintf(header_buf, shdr, sflg ? "SSIZ " : "");
	hdr = header_buf;
    }
    cmdstart = strlen(hdr);
    printf("%s COMMAND\n", hdr);
}


/* All of this should come out of the process manager... */

get_proc_table()
{
    register int i,j;
    long	nproc;
#define    NPROC    16        /* I don't know why, I just copy code -kh*/
    struct tbl_procinfo proc[NPROC];
    struct tbl_procinfo *mproc;

    nproc = table(TBL_PROCINFO, 0, (char *)0, 32767, 0);

#ifdef	PROC_COMPAT
    if (nproc < 0) {
	compat_get_proc_table();
	return;
    }
#endif	PROC_COMPAT

    for (i=0; i < nproc; i += NPROC) {
        j = nproc - i;
        if (j > NPROC)
            j = NPROC;
	j = table(TBL_PROCINFO, i, (char *)proc, NPROC, sizeof(proc[0]));
        for (j = j - 1; j >= 0; j--) {

            mproc = &proc[j];
	    if ((j==0) && (i==0) && (xflg != 0) && (aflg != 0)
		&& (pid_to_check == 0)) {
		save(mproc);
		continue;
	    }
	    if ((mproc->pi_status == PI_EMPTY) || (mproc->pi_pid == 0))
	        continue;
            if (xflg == 0 && mproc->pi_pgrp == 0)
                continue;
            if (tptr == 0 && xflg == 0 && gflg == 0 && mproc->pi_ppid == 1)
                continue;
            if (our_uid != mproc->pi_uid && aflg == 0)
                continue;
	    if (pid_to_check != 0) {
	        if (pid_to_check == mproc->pi_pid) {
		    save(mproc);
		    return;
		} else {
		    continue;
		}
	    }
            if (vflg && gflg == 0 && xflg == 0) {
                if (mproc->pi_status == PI_ZOMBIE ||
		    mproc->pi_status == PI_EXITING)
                    continue;
	    }
            save(mproc);
	}
    }
#undef	NPROC
}

char	str_defunct[] = "<defunct>";

save(mproc)
    struct tbl_procinfo *mproc;
{
    proc_info_t pi;
    char	terminal[4];
    task_t	task;

    if (mproc->pi_ttyd == -1)
	(void)strcpy(terminal, "? ");
    else {
	translate_terminal(mproc->pi_ttyd, terminal);
    }

    if (xflg == 0 && terminal[0] == '?')
	return;

    if (tflg) {
	if (strncmp(tptr,
		terminal,
		(strlen(tptr)>2) ? 2 : strlen(tptr))
	    != 0)
		return;
    }

    nprocs++;
    if (nprocs > max_proc_table) {
	max_proc_table *= 2;
	proc_table = (proc_info_t)realloc((char *)proc_table,
				(unsigned)max_proc_table*sizeof(*proc_table));
    }
    pi = &proc_table[nprocs-1];
    pi->uid	= mproc->pi_uid;
    pi->pid	= mproc->pi_pid;
    pi->ppid	= mproc->pi_ppid;
    pi->pgrp	= mproc->pi_pgrp;
    pi->status	= mproc->pi_status;
    pi->flag	= mproc->pi_flag;
/*    pi->text_size = u.u_tsize; */

    pi->term = malloc(4);
    (void)strcpy(pi->term, terminal);

    /*
     *	Find the other stuff
     */
    if (task_by_unix_pid(task_self(), pi->pid, &task) != KERN_SUCCESS) {
	pi->status = PI_ZOMBIE;
    }
    else {
	task_basic_info_data_t	ti;
	unsigned int		count;
	thread_array_t		thread_table;
	unsigned int		table_size;
	host_basic_info_data_t	hi;
	machine_info_data_t	mi;
	
	thread_basic_info_t	thi;
	thread_basic_info_data_t
				thi_data;
	int			i, t_state;

	count = TASK_BASIC_INFO_COUNT;
	if (task_info(task, TASK_BASIC_INFO, (task_info_t)&ti,
		      &count)
		!= KERN_SUCCESS) {
	    pi->status = PI_ZOMBIE;
	}
	else {
	    count = HOST_BASIC_INFO_COUNT;
	    (void) host_info(host_self(), HOST_BASIC_INFO, &hi, &count);

	    pi->virtual_size = ti.virtual_size;
	    pi->resident_size = ti.resident_size;

	    pi->percent_mem = ((double)ti.resident_size /
				(double)hi.memory_size) * 100.0;

	    (void)task_threads(task, &thread_table, &table_size);

	    pi->total_time = ti.user_time;
	    time_value_add(&pi->total_time, &ti.system_time);

	    pi->state = STATE_MAX;
	    pi->pri = 255;
	    pi->base_pri = 255;
	    pi->all_swapped = TRUE;
	    pi->cpu_usage = 0;
    
	    if (mflg) {
		pi->threads = (thread_basic_info_t)
				malloc(table_size *
					sizeof(thread_basic_info_data_t));
		pi->num_threads = 0;
		thi = pi->threads;
	    }
	    else {
		thi = &thi_data;
	    }

	    for (i = 0; i < table_size; i++) {
		count = THREAD_BASIC_INFO_COUNT;
		if (thread_info(thread_table[i], THREAD_BASIC_INFO,
				(thread_info_t)thi, &count) == KERN_SUCCESS) {
		    time_value_add(&pi->total_time, &thi->user_time);
		    time_value_add(&pi->total_time, &thi->system_time);
		    t_state = mach_state_order(thi->run_state,
					       thi->sleep_time);
		    if (t_state < pi->state)
			pi->state = t_state;
		    if (thi->cur_priority < pi->pri)
			pi->pri = thi->cur_priority;
		    if (thi->base_priority < pi->base_pri)
			pi->base_pri = thi->base_priority;
		    if ((thi->flags & TH_FLAGS_SWAPPED) == 0)
			pi->all_swapped = FALSE;
		    pi->cpu_usage += thi->cpu_usage;

		    if (mflg) {
			pi->num_threads++;
			thi++;
		    }
		}
	    }
	}
    }

    /*
     * now go find the command
     */

    if (pi->status == PI_ZOMBIE) {
	pi->command = str_defunct;
    }
    else if (cflg) {
	pi->command = malloc(sizeof(mproc->pi_comm));
	(void) strncpy(pi->command, mproc->pi_comm,
			sizeof(mproc->pi_comm)-1);
	pi->command[sizeof(mproc->pi_comm)-1] = '\0';
    }
    else {
	/*
	 *	Get command and arguments.
	 */
	int		command_length;
	register int 	*ip;
	register char	*cp;
	int		nbad;
	char		c;
	char		*end_argc;

	if (table(TBL_ARGUMENTS, pi->pid, arguments, 1,
	      arguments_size) != 1) {
	    /* table command failed, use the short command */
	    goto retucomm;
	}
	end_argc = &arguments[arguments_size];

	ip = (int *)end_argc;
	ip -= 2;		/* last arg word and .long 0 */
	while (*--ip)
	    if (ip == (int *)arguments)
		goto retucomm;

	*(char *)ip = ' ';
	ip++;
	nbad = 0;
	for (cp = (char *)ip; cp < end_argc; cp++) {
	    c = *cp & 0177;
	    if (c == 0)
		*cp = ' ';
	    else if (c < ' ' || c > 0176) {
		if (++nbad >= 5*(eflg+1)) {
		    *cp++ = ' ';
		    break;
		}
		*cp = '?';
	    }
	    else if (eflg == 0 && c == '=') {
		while (*--cp != ' ')
		    if (cp <= (char *)ip)
			break;
		break;
	    }
	}
	*cp = 0;
	while (*--cp == ' ')
	    *cp = 0;
	cp = (char *)ip;
	command_length = end_argc - cp;

	if (cp[0] == '-' || cp[0] == '?' || cp[0] <= ' ') {
	    /*
	     *	Not enough information - add short command name
	     */
	    pi->command = malloc((unsigned)command_length
				 + sizeof(mproc->pi_comm)
				 + 4);
	    (void) strncpy(pi->command, cp, command_length);
	    (void) strcat(pi->command, " (");
	    (void) strncat(pi->command, mproc->pi_comm,
			   sizeof(mproc->pi_comm));
	    (void) strcat(pi->command, ")");
	}
	else {
	    pi->command = malloc((unsigned)command_length + 1);
	    (void) strncpy(pi->command, cp, command_length);
	    pi->command[command_length] = '\0';
	}
	return;

    retucomm:
	pi->command = malloc(sizeof(mproc->pi_comm) + 4);
	(void) strcpy(pi->command, " (");
	(void) strncat(pi->command, mproc->pi_comm,
			sizeof(mproc->pi_comm));
	(void) strcat(pi->command, ")");
    }
}

#ifdef	PROC_COMPAT
#include <nlist.h>
#include <sys/proc.h>
#include <sys/user.h>

off_t get_kernel_long(kmem, loc)
	int	kmem;
	off_t	loc;
{
    off_t	word;

    lseek(kmem, (long)loc, 0);
    if (read(kmem, (char *)&word, sizeof(word)) != sizeof(word)) {
	fprintf(stderr, "error reading /dev/kmem at %x\n", loc);
	exit(1);
    }
    return (word);
}

compat_get_proc_table()
{
    struct proc *mproc;
    register int i,j;
    off_t	procp, nproc;
    int		kmem;

#define	NPROC	16
    struct proc	proc[NPROC];	/* read only 16 at a time */

    struct nlist nl[3];

    nl[0].n_name = "_proc";
    nl[1].n_name = "_nproc";
    nl[2].n_name = "";

    if (name_list_file == NULL)
	name_list_file = "/vmunix";

    nlist(name_list_file, nl);
    if (nl[0].n_type == 0) {
	fprintf(stderr, "'%s' No namelist.\n", name_list_file);
	exit(42);
    }

    if ((kmem = open("/dev/kmem", 0)) < 0) {
	fprintf(stderr, "Error opening /dev/kmem.\n");
	exit(42);
    }

    /* get start and size of proc table */

    procp = get_kernel_long(kmem, (off_t) nl[0].n_value);
    nproc = get_kernel_long(kmem, (off_t) nl[1].n_value);

    for (i = 0; i < nproc; i += NPROC) {
	lseek(kmem, (long)procp, 0);
	j = nproc - i;
	if (j > NPROC)
	    j = NPROC;
	j *= sizeof(struct proc);
	if (read(kmem, (char *)proc, j) != j) {
	    fprintf(stderr, "Can't read the proc table from /dev/kmem.\n");
	    exit(1);
	}
	procp += j;

	for (j = j / sizeof(struct proc) - 1; j >= 0; j --) {
	    mproc = &proc[j];

	    if (j == 0 && i == 0 && xflg != 0 && aflg != 0
		&& pid_to_check == 0) {
		compat_save(mproc);
		continue;
	    }
	    if (mproc->p_stat == SZOMB || mproc->p_pid == 0)
		continue;
	    if (xflg == 0 && mproc->p_pgrp == 0)
		continue;
	    if (tptr == 0 && xflg == 0 && gflg == 0 && mproc->p_ppid == 1)
		continue;
	    if (our_uid != mproc->p_uid && aflg == 0)
		continue;
	    if (pid_to_check != 0) {
		if (pid_to_check == mproc->p_pid) {
		    compat_save(mproc);
		    return;
		}
		else {
		    continue;
		}
	    }
	    if (vflg && gflg == 0 && xflg == 0) {
		if (mproc->p_pid == SZOMB || (mproc->p_flag & SWEXIT)) {
		    continue;
		}
	    }
	    compat_save(mproc);
	}
    }
}

compat_save(mproc)
    struct proc	*mproc;
{
    struct user	u;
    struct tbl_procinfo pi;	/* translate into this structure */

    pi.pi_uid	= mproc->p_uid;
    pi.pi_pid	= mproc->p_pid;
    pi.pi_ppid	= mproc->p_ppid;
    pi.pi_pgrp	= mproc->p_pgrp;
    pi.pi_flag	= mproc->p_flag;

    if (mproc->task == 0)
	pi.pi_status = PI_ZOMBIE;
    else {
	if (table(TBL_UAREA, mproc->p_pid,
		  (char *)&u, 1, sizeof(struct user)) != 1)
	    pi.pi_status = PI_ZOMBIE;
    }
    if (pi.pi_status == PI_ZOMBIE) {
	pi.pi_ttyd = NODEV;
	pi.pi_comm[0] = '\0';
    }
    else {
	if (mproc->p_flag & SWEXIT)
	    pi.pi_status = PI_EXITING;
	else
	    pi.pi_status = PI_ACTIVE;

	if (u.u_ttyp == 0)
	    pi.pi_ttyd = NODEV;
	else
	    pi.pi_ttyd = u.u_ttyd;

	(void) strncpy(pi.pi_comm, u.u_comm, sizeof(u.u_comm));
	pi.pi_comm[sizeof(pi.pi_comm)-1] = '\0';
    }

    save(&pi);		/* pretend that the table call worked */
}
#endif	PROC_COMPAT

