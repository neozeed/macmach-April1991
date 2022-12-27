#ifndef lint
static char *RCSid = "$Header: doname.c,v 5.0 89/03/01 01:37:26 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	doname.c,v $
 * Revision 5.0  89/03/01  01:37:26  bww
 * 	Version 5.
 * 	[89/03/01  01:28:42  bww]
 * 
 */

static	char *sccsid = "@(#)doname.c	4.9 (Berkeley) 87/06/18";

#include "defs.h"

FSTATIC int docom1();
FSTATIC tsetvars();
FSTATIC int cmds();
FSTATIC expand();

/*
 * BASIC PROCEDURE.  RECURSIVE.
 *
 * p->done = 0   don't know what to do yet
 * p->done = 1   file in process of being updated
 * p->done = 2   file already exists in current state
 * p->done = 3   file make failed
 */
int
doname(p, reclev, tval, ochain)
	register struct nameblock *p;
	int reclev;
	time_t *tval;
	struct chain **ochain;
{
	register struct depblock *q;
	register struct lineblock *lp;
	int errstat, okdel1, didwork;
	time_t td, td1, tdep, ptime, ptime1;
	struct depblock *qtemp, *suffp, *suffp1;
	struct nameblock *p1, *p2;
	struct shblock *implcom, *explcom;
	struct lineblock *lp1, *lp2;
	char srcname[BUFSIZ], prefix[BUFSIZ], temp[BUFSIZ];  /* XXX */
	char concsuff[20], pct[BUFSIZ];  /* XXX */
	char *pnamep, *p1namep, *cp, *setimpl, *inobj;
	struct chain *achain, *qchain, *cochain;
	int found, onetime;
	static char abuf[QBUFMAX];
	extern time_t prestime();
	extern struct depblock *srchdir();
	extern struct chain *appendq();
	extern char *mkqlist(), *ctime(), *strcpy();

	if (p == 0) {
		*tval = 0;
		return 0;
	}

	if (dbgflag)
		printf("doname(%s, %d)\n", p->namep, reclev);

	if (p->done) {
		/*
		 * if we want to check-out RCS files, and we have previously
		 * determined that we can, then append it to the previous
		 * level's cochain.
		 */
		if (p->RCSnamep)
			*ochain = appendq(*ochain, p->namep);
		*tval = p->modtime;
		return p->done == 3;
	}

	cochain = 0;
	errstat = 0;
	tdep = 0;
	implcom = 0;
	explcom = 0;
	ptime = exists(p, ochain, 1);
	ptime1 = 0;
	didwork = NO;
	p->done = 1;	/* avoid infinite loops */
	achain = 0;
	qchain = 0;

	/*
	 * Perform runtime dependency translations.
	 */
	if (p->rundep == 0) {
		tsetvars(p->tmacp);
		okdel1 = okdel;
		okdel = NO;
		setvar("@", p->namep, 1);
		dynamicdep(p, pct);
		setvar("@", (char *) 0, 1);
		okdel = okdel1;
	}

	/*
	 * Expand any names that have embedded metacharaters. Must be
	 * done after dynamic dependencies because the dyndep symbols
	 * ($(*D)) may contain shell meta characters.
	 */
	for (lp = p->linep; lp; lp = lp->nxtlineblock)
		for (q = lp->depp; q; q = qtemp) {
			qtemp = q->nxtdepblock;
			expand(q);
		}

	/*
	 * make sure all dependents are up to date
	 */
	for (lp = p->linep; lp; lp = lp->nxtlineblock) {
		td = 0;
		for (q = lp->depp; q; q = q->nxtdepblock) {
			if (q->depname == 0)
				continue;
			errstat += doname(q->depname, reclev+1, &td1, &cochain);
			if (dbgflag)
				printf("TIME(%s)=%s", q->depname->namep, ctime(&td1)+4);
			if (td1 > td)
				td = td1;
			achain = appendq(achain, q->depname->namep);
			if (ptime < td1)
				qchain = appendq(qchain, q->depname->namep);
		}
		if (p->septype != SOMEDEPS) {
			if (lp->shp) {
				if (explcom)
					fprintf(stderr, "Too many command lines for `%s'\n", p->namep);
				else
					explcom = lp->shp;
			}
			if (td > tdep)
				tdep = td;
			continue;
		}
		if (lp->shp && (ptime < td || (ptime == 0 && td == 0) || lp->depp == 0)) {
			okdel1 = okdel;
			okdel = NO;
			if (!questflag) {
				tsetvars(p->tmacp);
				if (cochain)
					co(cochain);
				setvar("@", p->namep, 1);
				setvar("%", pct, 1);
				setvar(">", strcpy(abuf, mkqlist(achain)), 1);
				setvar("?", mkqlist(qchain), 1);
				errstat += docom(lp->shp);
				setvar("@", (char *) 0, 1);
			}
			achain = 0;
			qchain = 0;
			cochain = 0;
			okdel = okdel1;
			ptime1 = prestime();
			didwork = YES;
		}
	}

	found = 0;
	onetime = 0;

	/*
	 * Look for implicit dependents, using suffix rules
	 */
	setimpl = 0;
	for (lp = sufflist; lp; lp = lp->nxtlineblock)
	for (suffp = lp->depp; suffp; suffp = suffp->nxtdepblock) {
		if (suffp->depname == 0)
			continue;
		pnamep = suffp->depname->namep;
		if (!suffix(p->namep, pnamep, prefix))
			continue;
searchdir:
		(void) srchdir(concat(prefix, "*", temp), NO, (struct depblock *) 0);
		if (coflag)
			srchRCS(temp);
		srchmachine(temp);
		for (lp1 = sufflist; lp1; lp1 = lp1->nxtlineblock)
		for (suffp1 = lp1->depp; suffp1; suffp1 = suffp1->nxtdepblock) {
			if (suffp1->depname == 0)
				continue;
			/*
			 * only use a single suffix if it really has rules
			 */
			if (onetime == 1) {
				for (lp2 = suffp1->depname->linep; lp2; lp2 = lp2->nxtlineblock)
					if (lp2->depp || lp2->shp)
						break;
				if (lp2 == 0)
					continue;
			}
			p1namep = suffp1->depname->namep;
			p1 = srchname(concat(p1namep, pnamep, concsuff));
			if (p1 == 0)
				continue;
			p2 = srchname(concat(prefix, p1namep, srcname));
			if (p2 == 0)
				continue;
			found = 1;
			errstat += doname(p2, reclev+1, &td, &cochain);
			if (dbgflag)
				printf("TIME(%s)=%s", p2->namep, ctime(&td)+4);
			if (td > tdep)
				tdep = td;
			achain = appendq(achain, p2->namep);
			if (ptime < td)
				qchain = appendq(qchain, p2->namep);
			for (lp2 = p1->linep; lp2; lp2 = lp2->nxtlineblock)
				if (implcom = lp2->shp) {
					setimpl = p2->namep;
					break;
				}
			goto endloop;
		}
		if (onetime == 1)
			/*
			 * quit search for single suffix rule
			 */
			goto endloop;
		setvar("*", (cp = rindex(prefix, '/')) ? cp+1 : prefix, 1);
	}
endloop:

	/*
	 * look for a single suffix type rule.
	 * only possible if no shell rules are found, and nothing
	 * has been done so far (previously, `make' would exit
	 * with 'Don't know how to make ...' message).
	 */
	if (found == 0 && onetime == 0) {
		for (lp = p->linep; lp; lp = lp->nxtlineblock)
			if (lp->shp)
				break;
		if (lp == 0) {
			onetime = 1;
			if (dbgflag)
				printf("Looking for Single suffix rule.\n");
			(void) concat(p->namep, "", prefix);
			pnamep = "";
			goto searchdir;
		}
	}

	inobj = 0;

	/*
	 * if we would normally say 'is up to date' and the target
	 * should have a version in the object directory, then we
	 * set things up so that an out of date object version will
	 * fire the "inobjdir" rule.
	 */
	if (!noconfig && tdep == 0 && errstat == 0
	&& explcom == 0 && implcom == 0 && ptime != 0
	&& didwork == NO && isdependent(p->namep, inobjdir)) {
		if (p->alias == 0) {
			if (dbgflag)
				printf("Looking for source version.\n");
			tdep = exists(p, ochain, 0);
		} else {
			tdep = ptime;
			ptime = 0;
		}
		for (lp = inobjdir->linep; lp; lp = lp->nxtlineblock)
			if (implcom = lp->shp) {
				inobj = p->namep;
				break;
			}
	}

	if (dbgflag && cochain)
		printf("CO(%s): %s\n", p->namep, mkqlist(cochain));
	if (p->RCSnamep && (explcom || (implcom && inobj == 0))) {
		if (!keepgoing)
			fatal1("%s has both an RCS file and rules", p->namep);
		errstat++;
		printf("%s has both an RCS file and rules\n", p->namep);
	}

	if (p->alias && (explcom || (implcom && inobj == 0))) {
		fprintf(stderr, "Warning: target `%s' found as `%s' ignored\n",
							p->namep, p->alias);
		p->alias = 0;
		ptime = 0;
	}
	if (errstat == 0 && (ptime < tdep || (ptime == 0 && tdep == 0))) {
		ptime = (tdep > 0 ? tdep : prestime());
		tsetvars(p->tmacp);
		if (cochain)
			co(cochain);
		if (setimpl) {
			setvar("*", prefix, 1);
			setvar("<", setimpl, 1);
		} else if (inobj)
			setvar("<", inobj, 1);
		setvar("@", p->namep, 1);
		setvar("%", pct, 1);
		setvar(">", strcpy(abuf, mkqlist(achain)), 1);
		setvar("?", mkqlist(qchain), 1);
		if (explcom)
			errstat += docom(explcom);
		else if (implcom)
			errstat += docom(implcom);
		else if (p->septype == 0) {
			if (p1 = srchname(".DEFAULT")) {
				setvar("<", p->namep, 1);
				for (lp2 = p1->linep; lp2; lp2 = lp2->nxtlineblock)
					if (implcom = lp2->shp) {
						errstat += docom(implcom);
						break;
					}
			} else if (keepgoing) {
				printf("Don't know how to make %s\n", p->namep);
				++errstat;
			} else
				fatal1(" Don't know how to make %s", p->namep);
		}
		setvar("@", (char *) 0, 1);
		if (cmds(explcom) || cmds(implcom))
			if (noexflag || (ptime = exists(p, (struct chain **) 0, 1)) == 0)
				ptime = prestime();
	}
	
	else if (errstat && reclev == 0)
		printf("`%s' not remade because of errors\n", p->namep);

	else if (!questflag && reclev == 0 && didwork == NO)
		printf("`%s' is up to date.\n", p->namep);

	if (inobj)
		p->alias = 0;
	if (questflag && reclev == 0)
		quit(ndocoms > 0 ? -1 : 0);
	p->done = (errstat ? 3 : 2);
	if (ptime1 > ptime)
		ptime = ptime1;
	p->modtime = ptime;
	*tval = ptime;
	return errstat;
}


int
docom(q)
	struct shblock *q;
{
	char *s;
	int exok, ign, nopr;
	char string[OUTMAX];
	char string2[OUTMAX];
	extern char *sindex();

	if (questflag) {
		if (!nocmds)
			++ndocoms;
		return 0;
	}

	if (touchflag) {
		if (nocmds)
			return 0;
		s = varptr("@")->varval;
		if (!silflag)
			printf("%stouch %s\n", (noexflag ? "" : prompt), s);
		if (!noexflag)
			touch(NO, s);
		return 0;
	}

	for (; q; q = q->nxtshblock) {
		exok = (sindex(q->shbp, "$(MAKE)") ||
			sindex(q->shbp, "${MAKE}"));
		if (nocmds && !exok)
			continue;
		(void) subst(q->shbp, string2);
		fixname(string2, string);
		ign = ignerr;
		nopr = NO;
		for (s = string; *s == '-' || *s == '@'; ++s)
			if (*s == '-')
				ign = YES;
			else
				nopr = YES;
		if (docom1(s, ign, nopr, exok) && !ign)
			return 1;
	}
	return 0;
}


FSTATIC int
docom1(comstring, nohalt, noprint, exok)
	register char *comstring;
	int nohalt, noprint, exok;
{
	if (!silflag && (!noprint || noexflag))
		printf("%s%s\n", (noexflag ? "" : prompt),
			(*comstring ? comstring : "(null command)"));
	if (*comstring == 0 || (noexflag && !exok))
		return 0;
	return dosys(comstring, nohalt);
}


FSTATIC
tsetvars(t)
	struct tmacblock *t;
{
	register struct tmacblock *p;

	for (p = t; p; p = p->nxttmacblock) {
		if (dbgflag)
			printf("%s\n", p->tmacbp);
		(void) eqsign(p->tmacbp, (char *) 0, 1);
	}
}


FSTATIC int
cmds(p)
	struct shblock *p;
{
	register struct shblock *q;

	for (q = p; q; q = q->nxtshblock)
		if (*q->shbp)
			return 1;
	return 0;
}


/*
 * If there are any Shell meta characters in the name,
 * expand into a list, after searching directory
 */
FSTATIC
expand(q)
	register struct depblock *q;
{
	register char *s;
	char *s1;
	struct depblock *p;
	extern struct depblock *srchdir();

	if (q->depname == 0)
		return;
	s1 = s = q->depname->namep;
	for (;;) {
		switch (*s++) {
		case 0:
			return;
		case '*':
		case '?':
		case '[':
			if (p = srchdir(s1, YES, q->nxtdepblock)) {
				q->nxtdepblock = p;
				q->depname = 0;
			}
			return;
		}
	}
}
