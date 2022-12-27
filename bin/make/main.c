#ifndef lint
static char *RCSid = "$Header: main.c,v 5.0 89/03/01 01:39:55 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	main.c,v $
 * Revision 5.0  89/03/01  01:39:55  bww
 * 	Version 5.
 * 	[89/03/01  01:31:33  bww]
 * 
 */

static	char *sccsid = "@(#)main.c	4.10 (Berkeley) 87/11/15";

/*
 * command make to update programs.
 *
 * Flags:
 *	'd'  print out debugging comments
 *	'p'  print out a version of the input graph
 *	's'  silent mode--don't print out commands
 *	'f'  the next argument is the name of the description file;
 *	     "makefile" is the default
 *	'F'  a description file is compulsory
 *	'i'  ignore error codes from the shell
 *	'S'  stop after any command fails (normally do parallel work)
 *	'n'  don't issue, just print, commands
 *	't'  touch (update time of) files but don't issue command
 *	'q'  don't do anything, but check if object is up to date;
 *	     returns exit code 0 if up to date, -1 if not
 *	'c'  don't check out RCS files
 *	'C'  check out RCS files (default)
 *	'u'  don't unlink RCS working files
 *	'U'  unlink RCS working files automatically checked out (default)
 *	'm'  look in machine specific subdirectory first
 */

#include "defs.h"
#include <signal.h>
#ifdef lint
#undef SIG_IGN
SIG_IGN(){}
#endif

struct shblock *co_cmd		= 0;
struct shblock *rcstime_cmd	= 0;
struct chain *rmchain		= 0;
struct chain *deschain		= 0;
struct nameblock *mainname	= 0;
struct nameblock *firstname	= 0;
struct nameblock *inobjdir	= 0;
struct lineblock *sufflist	= 0;
struct varblock *firstvar	= 0;
struct dirhdr *firstod		= 0;

int rmflag	= YES;
int coflag	= YES;
int nocmds	= NO;
int rcsdep	= NO;
char *RCSdir	= RCS;
char *RCSsuf	= RCS_SUF;

int sighvalue	= 0;
int sigivalue	= 0;
int sigqvalue	= 0;
int sigtvalue	= 0;

int envover	= NO;
int dbgflag	= NO;
int prtrflag	= NO;
int silflag	= NO;
int noexflag	= NO;
int keepgoing	= NO;
int descflag	= NO;
int noruleflag	= NO;
int noconfig	= NO;
int touchflag	= NO;
int questflag	= NO;
int machdep	= NO;
int ignerr	= NO;    /* default is to stop on error */
int okdel	= YES;
int ndocoms	= 0;
int inarglist	= 0;
int ininternal	= 0;
#ifdef pwb
char *prompt	= ">";	/* other systems -- pick what you want */
#else
char *prompt	= "";	/* other systems -- pick what you want */
#endif
char *curfname	= "no description file";
char funny[128];
char options[26 + 1] = { '-' };
char Makeflags[] = "MAKEFLAGS";

FSTATIC int rddescf();
FSTATIC setcocmd();
FSTATIC printdesc();
FSTATIC printmac();
FSTATIC setflags();
FSTATIC optswitch();
FSTATIC getmflgs();
FSTATIC setmflgs();

void
main(argc,argv)
	int argc;
	char **argv;
{
	register struct nameblock *p;
	int i, descset, nfargs;
	time_t td;
	struct chain *ch;
	char *s;
	extern char *ctime();
#ifdef unix
	FSTATIC int intrupt();
#endif

	i = getdtablesize();
	while (i > 3)
		(void) close(--i);
#ifdef METERFILE
	meter(METERFILE);
#endif

	descset = 0;

	funny[0] = (META | TERMINAL);
	for (s = "=|^();&<>*?[]:$`'\"\\\n"; *s; ++s)
		funny[*s] |= META;
	for (s = "\n\t :;&>|()"; *s; ++s)
		funny[*s] |= TERMINAL;

	getmflgs();		/* Init $(MAKEFLAGS) variable */
	setflags(argc, argv);

	inarglist = 1;
	for (i = 1; i < argc; ++i)
		if (argv[i] && argv[i][0] != '-'
		&& eqsign(argv[i], (char *) 0, 0))
			argv[i] = 0;
	setvar("$", "$", 0);
	inarglist = 0;

	/*
	 * Summary of Assigning Macros and Variables
	 * from lower to higher priority.
	 *
	 *	-e not specified	-e specified
	 *	--------------------------------------------
	 *	internal definitions	internal definintions
	 *	environment		makefile(s)
	 *	makefile(s)		environment
	 *	command line		command line
	 */

	/*
	 * Read internal definitions and rules
	 */
	if (!noruleflag) {
		if (dbgflag)
			printf("Reading internal rules.\n");
		ininternal = 1;
		rdd1((FILE *) 0, "internal rules");
		ininternal = 0;
	}

	/*
	 * Read environment args.  Let file args which follow override
	 * unless "envover" variable is set.
	 */
	inarglist = envover;
	readenv();
	inarglist = 0;

	/*
	 * export MAKEFLAGS
	 */
	setenv(Makeflags, varptr(Makeflags)->varval, 1);

	/*
	 * MFLAGS=options to make
	 */
	if (strcmp(options, "-") == 0)
		*options = 0;
	setvar("MFLAGS", options, 0);

	if (!noconfig)
		makemove("Makeconf");

	setcocmd();  /* to possibly check-out description file */
	setvpath();
	mainname = 0;

	/*
	 * Read command line "-f" arguments.
	 */
	for (i = 1; i < argc; i++)
		if (argv[i] && strcmp(argv[i], "-f") == 0) {
			argv[i] = 0;
			if (i >= argc-1)
				fatal("No description argument after -f flag");
			if (rddescf(argv[++i]))
				fatal1("Cannot open %s", argv[i]);
			argv[i] = 0;
			++descset;
		}

	if (!descset)
#ifdef unix
		if (rddescf("makefile"))
			if (rddescf("Makefile"))
				if (descflag)
					fatal("No description file");
#endif
#ifdef gcos
		if (rddescf("makefile"))
			if (descflag)
				fatal("No description file");
#endif

	setvpath();
	setcocmd();  /* may have been redefined in description file */

	addincdirs();

	if (prtrflag)
		printdesc(NO);

	if (srchname(".IGNORE"))
		++ignerr;
	if (srchname(".SILENT"))
		silflag = 1;
	if (p = srchname(".SUFFIXES"))
		sufflist = p->linep;
	if (!sufflist)
		fprintf(stderr, "No suffix list.\n");
	if (inobjdir = srchname(".INOBJECTDIR"))
		doruntime(2, inobjdir);
	if (p = srchname(".EXPORT"))
		export(p);

#ifdef unix
	sighvalue = (signal(SIGHUP, SIG_IGN) == SIG_IGN);
	sigivalue = (signal(SIGINT, SIG_IGN) == SIG_IGN);
	sigqvalue = (signal(SIGQUIT, SIG_IGN) == SIG_IGN);
	sigtvalue = (signal(SIGTERM, SIG_IGN) == SIG_IGN);
	enbint(intrupt);
#endif

	if (p = srchname(".INIT")) {
		ch = 0;
		(void) doname(p, 0, &td, &ch);
	}

	nfargs = 0;

	for (i = 1; i < argc; ++i)
		if (s = argv[i]) {
			p = makename(s);
			++nfargs;
			ch = 0;
			(void) doname(p, 0, &td, &ch);
			if (dbgflag)
				printf("TIME(%s)=%s", p->namep, ctime(&td)+4);
			if (prtrflag)
				printdesc(YES);
		}

	/*
	 * If no file arguments have been encountered, make the first
	 * name encountered that doesn't start with a dot
	 */

	if (nfargs == 0) {
		if (mainname == 0)
			fatal("No arguments or description file");
		ch = 0;
		(void) doname(mainname, 0, &td, &ch);
		if (dbgflag)
			printf("TIME(%s)=%s", mainname->namep, ctime(&td)+4);
		if (prtrflag)
			printdesc(YES);
	}

	quit(0);
}


int
isdependent(p, np)
	register char *p;
	register struct nameblock *np;
{
	register struct lineblock *lp;
	register struct depblock *dp;

	if (np == 0)
		return NO;
	for (lp = np->linep; lp; lp = lp->nxtlineblock)
		for (dp = lp->depp; dp; dp = dp->nxtdepblock)
			if (dp->depname && !unequal(p, dp->depname->namep))
				return YES;
	return NO;
}


#ifdef unix
FSTATIC
int intrupt()
{
	char *p;
	struct stat sbuf;

	if (okdel && !noexflag && !touchflag
	&& (p = varptr("@")->varval)
	&& (stat(p, &sbuf) != -1 && (sbuf.st_mode&S_IFMT) == S_IFREG)
	&& !isprecious(p)) {
		printf("*** `%s' removed\n", p);
		(void) fflush(stdout);
		(void) unlink(p);
	}
	return quit(2);
}


int
isprecious(p)
	char *p;
{
	return isdependent(p, srchname(".PRECIOUS"));
}


enbint(k)
	int (*k)();
{
	if (sighvalue == 0)
		(void) signal(SIGHUP, k);
	if (sigivalue == 0)
		(void) signal(SIGINT, k);
	if (sigqvalue == 0)
		(void) signal(SIGQUIT, k);
	if (sigtvalue == 0)
		(void) signal(SIGTERM, k);
}
#endif


extern FILE *fin;

FSTATIC int
rddescf(descfile)
	char *descfile;
{
	register struct nameblock *np;
	FILE *k;

	/*
	 * read and parse description
	 */
	if (!unequal(descfile, "-")) {
		rdd1(stdin, "standard input");
		return 0;
	}

	np = rcsco(descfile);
	if ((k = fopen(np->alias ? np->alias : descfile, "r")) != NULL) {
		rdd1(k, descfile);
		(void) fclose(k);
		return 0;
	}

	return 1;
}


FSTATIC char MAKEFILE[] = "MAKEFILE";

rdd1(k, fn)
	FILE *k;
	char *fn;
{
	extern int yylineno;
	extern char *zznextc;

	setvar(MAKEFILE, copys(fn), 1);
	curfname = fn;
	fin = k;
	yylineno = 0;
	zznextc = 0;

	if (yyparse())
		fatal1("%s: Description file error", fn);
}


FSTATIC
setcocmd()
{
	struct nameblock *p;
	register struct lineblock *lp;

	if (p = srchname(".CO")) {
		for (lp = p->linep; lp; lp = lp->nxtlineblock)
			if (co_cmd = lp->shp)
				break;
		if (co_cmd == 0)
			coflag = NO;
		else if (p = srchname(".RCSTIME"))
			for (lp = p->linep; lp; lp = lp->nxtlineblock)
				if (rcstime_cmd = lp->shp)
					break;
	} else
		coflag = NO;
}


FSTATIC
printdesc(prntflag)
	int prntflag;
{
	struct nameblock *p;
	struct depblock *dp;
	struct dirhdr *od;
	struct shblock *sp;
	struct lineblock *lp;

#ifdef unix
	if (prntflag) {
		printf("Open directories:\n");
		for (od = firstod; od; od = od->nxtopendir)
			printf("\t%d: %s\n", dirfd(od->dirfc), od->dirn);
		printf("\n");
	}
#endif

	if (firstvar) {
		printf("Macros:\n");
		printmac(firstvar);
	}

	for (p = firstname; p; p = p->nxtnameblock) {
		printf("\n%s", p->namep);
		if (p->linep)
			printf(":");
		if (prntflag)
			printf("  done=%d", p->done);
		if (p == mainname)
			printf("  (MAIN NAME)");
		for (lp = p->linep; lp; lp = lp->nxtlineblock) {
			if (dp = lp->depp) {
				printf("\n depends on:");
				for (; dp; dp = dp->nxtdepblock)
					if (dp->depname)
						printf(" %s ", dp->depname->namep);
			}

			if (sp = lp->shp) {
				printf("\n commands:");
				for (; sp; sp = sp->nxtshblock)
					printf("\n\t%s", sp->shbp);
			}
		}
	}
	printf("\n");
}


FSTATIC
printmac(vp)
	register struct varblock *vp;
{
	if (vp->lftvarblock)
		printmac(vp->lftvarblock);
	printf(" %9s = %s\n" , vp->varname , vp->varval);
	if (vp->rgtvarblock)
		printmac(vp->rgtvarblock);
}


/*
 * Handle options and MAKEFLAGS and MFLAGS.
 */
FSTATIC
setflags(ac, av)
	int ac;
	char **av;
{
	register int i, j;
	register char c;
	int flflg = 0;		/* flag to note `-f' option. */

	for (i = 1; i < ac; ++i) {
		if (flflg) {
			flflg = 0;
			continue;
		}
		if (av[i] && av[i][0] == '-') {
			if (index(av[i], 'f'))
				flflg++;
			for (j = 1; c = av[i][j]; ++j)
				optswitch(c);
			av[i] = flflg ? "-f" : 0;
		}
	}
}


/*
 * Handle a single char option.
 */
FSTATIC
optswitch(c)
	register char c;
{
	char cb[2];

	switch (c) {
	case 'c':
		coflag = NO;
		setmflgs(c);
		break;
	case 'C':
		coflag = YES;
		setmflgs(c);
		break;
	case 'e':
		envover = YES;
		setmflgs(c);
		break;
	case 'd':
		dbgflag = YES;
		setmflgs(c);
		break;
	case 'p':
		prtrflag = YES;
		break;
	case 's':
		silflag = YES;
		setmflgs(c);
		break;
	case 'i':
		ignerr = YES;
		setmflgs(c);
		break;
	case 'S':
		keepgoing = NO;
		setmflgs(c);
		break;
	case 'k':
		keepgoing = YES;
		setmflgs(c);
		break;
	case 'F':
		descflag = YES;
		setmflgs(c);
		break;
	case 'n':
		noexflag = YES;
		setmflgs(c);
		break;
	case 'r':
		noruleflag = YES;
		break;
	case 'N':
		noconfig = YES;
		setmflgs(c);
		break;
	case 't':
		touchflag = YES;
		setmflgs(c);
		break;
	case 'q':
		questflag = YES;
		setmflgs(c);
		break;
	case 'm':
		machdep = YES;
		setmflgs(c);
		break;
	case 'u':
		rmflag = NO;
		setmflgs(c);
		break;
	case 'U':
		rmflag = YES;
		setmflgs(c);
		break;
	case 'x':
		nocmds = YES;
		setmflgs(c);
		break;
	case 'y':
		rcsdep = YES;
		setmflgs(c);
		break;
	case 'f':
		/*
		 * Named makefile; already handled by setflags().
		 */
		break;
	case 'b':
		/*
		 * compatibility
		 */
		break;
	default:
		cb[0] = c;
		cb[1] = 0;
		fatal1("Unknown flag argument %s", cb);
	}
}


/*
 * getmflgs() set the cmd line flags into an EXPORTED variable
 * for future invocations of make to read.
 */
FSTATIC
getmflgs()
{
	register struct varblock *vpr;
	register char **pe;
	register char *p;
	extern char **environ;

	vpr = varptr(Makeflags);
	setvar(Makeflags, "ZZZZZZZZZZZZZZZZZZZZZZZZZZ", 0);
	vpr->varval[0] = 0;
	vpr->noreset = 1;
	for (pe = environ; *pe; pe++) {
		if (strncmp(*pe, "MAKEFLAGS=", 10) == 0) {
			for (p = (*pe) + 10; *p; p++)
				optswitch(*p);
			return;
		}
	}
}


/*
 * setmflgs(c) sets up the cmd line input flags for EXPORT.
 * Also adds them to "MFLAGS" for compatibility with 4.3 make.
 */
FSTATIC
setmflgs(c)
	register char c;
{
	register struct varblock *vpr;
	register char *p;

	for (p = options; *p; p++)
		if (*p == c)
			break;
	if (*p == 0) {
		*p++ = c;
		*p = 0;
	}
	vpr = varptr(Makeflags);
	for (p = vpr->varval; *p; p++)
		if (*p == c)
			return;
	*p++ = c;
	*p = 0;
}
