#ifndef lint
static char *RCSid = "$Header: dyndep.c,v 5.0 89/03/01 01:38:13 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	dyndep.c,v $
 * Revision 5.0  89/03/01  01:38:13  bww
 * 	Version 5.
 * 	[89/03/01  01:29:26  bww]
 * 
 */

static	char *sccsid = "@(#)dyndep.c	3.4";

/*
 * Dynamicdep() checks each dependency by calling runtime().
 * Runtime() determines if a dependent line contains "$@"
 * or "$(@F)" or "$(@D)". If so, it makes a new dependent line
 * and insert it into the dependency chain of the input name, p.
 * Here, "$@" gets translated to p->namep. That is
 * the current name on the left of the colon in the
 * makefile.  Thus,
 *	xyz:	s.$@.c
 * translates into
 *	xyz:	s.xyz.c
 *
 * Also, "$(@F)" translates to the same thing without a prededing
 * directory path (if one exists).
 * Note, to enter "$@" on a dependency line in a makefile
 * "$$@" must be typed. This is because `make' expands
 * macros upon reading them.
 *
 * Now, also does '%' and "` ... `" style dynamic dependencies.
 */

#include "defs.h"

FSTATIC char var[11][BUFSIZ];  /* XXX */
FSTATIC int used[11];
FSTATIC int unify();
FSTATIC instant();

static struct lineblock *copyline();
static struct depblock *wordsub(), *execsub();


dynamicdep(p, pct)
	register struct nameblock *p;
	char *pct;
{
	register struct lineblock *lp;
	register struct nameblock *q;
	int i;

	p->rundep = 1;

	for (i = 0; i < 11; i++)
		used[i] = 0;

	if (p->RCSnamep)
		goto pass1;
	for (lp = p->linep; lp; lp = lp->nxtlineblock)
		if (lp->shp)
			goto pass1;

	for (q = firstname; q; q = q->nxtnameblock) {
		if (q->linep == 0 || q->percent == 0
		|| unify(p->namep, "", q->namep) == 0)
			continue;
		if (q->septype != ALLDEPS)
			fatal1("%s rule can only use a single ':'", q->namep);
		if (dbgflag) {
			printf("unify(%s, %s):\n", p->namep, q->namep);
			for (i = 0; i < 11; i++) {
				if (used[i] == 0)
					continue;
				if (i == 0)
					printf("\t%% : %s\n", var[i]);
				else
					printf("\t%%%d: %s\n", i-1, var[i]);
			}
		}
		p->septype = ALLDEPS;
		if (p->linep == 0)
			p->linep = copyline(q->linep);
		else {
			lp = p->linep;
			while (lp->nxtlineblock)
				lp = lp->nxtlineblock;
			lp->nxtlineblock = copyline(q->linep);
		}
		doruntime(0, p);
		break;
	}

pass1:
	setvar("%", strcpy(pct, used[0] ? var[0] : ""), 1);
	doruntime(1, p);
	doruntime(2, p);
}


doruntime(pass, p)
	int pass;
	struct nameblock *p;
{
	register struct lineblock *lp, *nlp;
	struct lineblock *backlp = 0;
	FSTATIC struct lineblock *runtime();

	for (lp = p->linep; lp; lp = lp->nxtlineblock) {
		if (nlp = runtime(lp, pass))
			if (backlp)
				backlp->nxtlineblock = nlp;
			else
				p->linep = nlp;
		backlp = (nlp == 0) ? lp : nlp;
	}
}


FSTATIC struct lineblock *
runtime(lp, pass)
	register struct lineblock *lp;
	int pass;
{
	register struct depblock *q, *nq;
	register struct shblock *s, *ns;
	register char *pc;
	struct nameblock *ap, *lap;
	struct lineblock *nlp;
	char buf[INMAX];  /* XXX */

	switch (pass) {
	case 0:
		for (q = lp->depp; q; q = q->nxtdepblock)
			if (q->depname && (q->depname->percent
			|| (q->depname->archp && q->depname->archp->percent)))
				break;
		if (q)
			break;
		for (s = lp->shp; s; s = s->nxtshblock)
			if (index(s->shbp, '%'))
				break;
		if (s == 0)
			return 0;
		break;
	case 1:
		for (q = lp->depp; q; q = q->nxtdepblock)
			if (q->depname && (q->depname->dollar
			|| (q->depname->archp && q->depname->archp->dollar)))
				break;
		if (q == 0)
			return 0;
		break;
	case 2:
		for (q = lp->depp; q; q = q->nxtdepblock)
			if (q->depname && (q->depname->bquotes
			|| (q->depname->archp && q->depname->archp->bquotes)))
				break;
		if (q == 0)
			return 0;
		break;
	default:
		fatal("Unknown pass in dyndep");
	}

	nlp = ALLOC(lineblock);
	nlp->nxtlineblock = lp->nxtlineblock;
	nq = nlp->depp = (lp->depp ? ALLOC(depblock) : 0);

	switch (pass) {
	case 0:
		for (q = lp->depp; q; q = q->nxtdepblock) {
			if (q->depname && index(pc = q->depname->namep, '%')) {
				instant(pc, buf);
				nq->depname = makename(buf);
				nq->depname->archp = q->depname->archp;
				nq->depname->objarch = q->depname->objarch;
			} else
				nq->depname = q->depname;
			nq = nq->nxtdepblock = (q->nxtdepblock ?
					ALLOC(depblock) : 0);
		}
		break;
	case 1:
		for (q = lp->depp; q; q = q->nxtdepblock) {
			if (q->depname && index(pc = q->depname->namep, '$')) {
				(void) subst(pc, buf);
				nq = wordsub(nq, q->depname, buf);
			} else
				nq->depname = q->depname;
			nq = nq->nxtdepblock = (q->nxtdepblock ?
					ALLOC(depblock) : 0);
		}
		break;
	case 2:
		for (q = lp->depp; q; q = q->nxtdepblock) {
			if (q->depname
			&& (pc = index(q->depname->namep, '`'))
			&& index(pc+1, '`'))
				nq = execsub(nq, q->depname);
			else
				nq->depname = q->depname;
			nq = nq->nxtdepblock = (q->nxtdepblock ?
					ALLOC(depblock) : 0);
		}
		break;
	}

	lap = 0;

	switch (pass) {
	case 0:
		for (q = nlp->depp; q; q = q->nxtdepblock)
			if (q->depname && q->depname->archp
			&& q->depname->archp->percent) {
				if (q->depname->archp != lap) {
					lap = q->depname->archp;
					instant(lap->namep, buf);
					ap = makename(buf);
				}
				q->depname->archp = ap;
			}
		break;
	case 1:
		for (q = nlp->depp; q; q = q->nxtdepblock)
			if (q->depname && q->depname->archp
			&& q->depname->archp->dollar) {
				if (q->depname->archp != lap) {
					lap = q->depname->archp;
					(void) subst(lap->namep, buf);
					ap = makename(buf);
				}
				q->depname->archp = ap;
			}
		break;
	case 2:
		for (q = nlp->depp; q; q = q->nxtdepblock)
			if (q->depname && q->depname->archp
			&& q->depname->archp->bquotes)
				fatal1("%s: illegal dynamic library name",
						q->depname->archp->namep);
		break;
	}

	switch (pass) {
	case 0:
		ns = nlp->shp = (lp->shp ? ALLOC(shblock) : 0);
		for (s = lp->shp; s; s = s->nxtshblock) {
			instant(s->shbp, buf);
			ns->shbp = copys(buf);
			ns = ns->nxtshblock = (s->nxtshblock ?
					ALLOC(shblock) : 0);
		}
		break;
	case 1:
	case 2:
		nlp->shp = lp->shp;
		break;
	}

	return nlp;
}


/*
 * find a substitution of h|p that matches s.
 */
FSTATIC int
unify(s, h, p)
	char *s;
	char *h;
	char *p;
{
	char *q, *r;
	int vn;

	if (*h)
		return *s == *h && unify(s + 1, h + 1, p);
	if (*p == 0)
		return *s == 0;
	if (*p != '%')
		return *s == *p && unify(s + 1, h, p + 1);
	if (used[vn = isdigit(*++p) ? *p++ - '0' + 1 : 0])
		return unify(s, var[vn], p);
	for (q = s, r = var[vn]; *r = *q++; r++)
		;
	for (used[vn] = 1; q > s; *--r = 0)
		if (unify(--q, h, p))
			return 1;
	used[vn] = 0;
	return 0;
}


/*
 * copy string a into b, substituting for % variables
 */
FSTATIC
instant(a, b)
	register char *a, *b;
{
	register char *c;
	register int vn;

	if (a == 0) {
		*b = 0;
		return;
	}
	while (*a) {
		if (*a != '%') {
			if (*a == '\\' && *(a+1) == '%')
				a++;
			*b++ = *a++;
			continue;
		}
		if (used[vn = isdigit(*++a) ? *a++ - '0' + 1 : 0] == 0)
			fatal("Reference to uninstantiated variable");
		for (c = var[vn]; *c; *b++ = *c++)
			;
	}
	*b = 0;
	return;
}


FSTATIC struct depblock *
wordsub(q, np, s)
	register struct depblock *q;
	register struct nameblock *np;
	register char *s;
{
	register char *p;
	char temp[OUTMAX];

	q->depname = 0;
	for (;;) {
		while (*s && isspace(*s))
			s++;
		if (*s == 0)
			break;
		p = temp;
		do {
			if ((*p++ = *s++) != '`')
				continue;
			while (*s && *s != '`')
				*p++ = *s++;
			if (*s)
				*p++ = *s++;
		} while (*s && !isspace(*s));
		*p = 0;
		if (q->depname)
			q = q->nxtdepblock = ALLOC(depblock);
		q->depname = makename(temp);
		q->depname->archp = np->archp;
		q->depname->objarch = np->objarch;
		if (*s == 0)
			break;
	}
	q->nxtdepblock = 0;
	return q;
}


FSTATIC struct depblock *
execsub(q, np)
	register struct depblock *q;
	register struct nameblock *np;
{
	register int c;
	register FILE *f;
	register char *p;
	char *comm;
	int ign, nopr;
	char cmd[OUTMAX];
	char temp[OUTMAX];
	char buf[BUFSIZ];
	extern FILE *pfopen();

	q->depname = 0;
	comm = p = np->namep;
	while (*p)
		p++;
	if (*comm++ != '`' || *--p != '`')
		fatal("`...` dependencies must be whole names");
	*p = 0;
#ifdef notdef
	/*
	 * Attempt to be smart about finding command
	 * arguments.  Probably not worth it.
	 */
	for (p = comm; *p; p++) {
		char *w;

		if (isspace(*p))
			continue;
		for (w = p; *p && !isspace(*p); p++) {
			if (*p != '\'' && *p != '"')
				continue;
			c = *p;
			while (*++p && *p != c)
					;
		}
		c = *p;
		*p = 0;
		if (metas(w) == 0 && *w != '-')
			(void) rcsco(w);
		if ((*p = c) == 0)
			break;
	}
#endif
	fixname(comm, cmd);
	ign = ignerr;
	nopr = NO;
	for (p = cmd; *p == '-' || *p == '@'; ++p)
		if (*p == '-')
			ign = YES;
		else
			nopr = YES;
	if (*p == 0)
		return q;
	if (!silflag && (!nopr || noexflag))
		printf("%s%s\n", (noexflag ? "" : prompt), p);
	if ((f = pfopen(p, ign)) == NULL)
		return q;
	setbuf(f, buf);

	if (dbgflag)
		printf("execsub:");
	for (;;) {
		do
			c = getc(f);
		while (c != EOF && isspace(c));
		if (c == EOF)
			break;
		p = temp;
		do
			*p++ = c;
		while ((c = getc(f)) != EOF && !isspace(c));
		*p = 0;
		if (dbgflag)
			printf(" '%s'", temp);
		if (q->depname)
			q = q->nxtdepblock = ALLOC(depblock);
		q->depname = makename(temp);
		q->depname->archp = np->archp;
		q->depname->objarch = np->objarch;
		if (c == EOF)
			break;
	}
	if (dbgflag)
		printf("\n");

	(void) pfclose(f, ign);
	q->nxtdepblock = 0;
	return q;
}


FSTATIC struct shblock *
copysh(sp)
	struct shblock *sp;
{
	register struct shblock *nsp;

	if (sp == 0)
		return 0;
	nsp = ALLOC(shblock);
	nsp->shbp = copys(sp->shbp);
	nsp->nxtshblock = copysh(sp->nxtshblock);
	return nsp;
}


FSTATIC struct depblock *
copydep(dp)
	struct depblock *dp;
{
	register struct depblock *ndp;

	if (dp == 0)
		return 0;
	ndp = ALLOC(depblock);
	ndp->depname = dp->depname;
	ndp->nxtdepblock = copydep(dp->nxtdepblock);
	return ndp;
}


FSTATIC struct lineblock *
copyline(lp)
	struct lineblock *lp;
{
	register struct lineblock *nlp;

	if (lp == 0)
		return 0;
	nlp = ALLOC(lineblock);
	nlp->depp = copydep(lp->depp);
	nlp->shp = copysh(lp->shp);
	nlp->nxtlineblock = copyline(lp->nxtlineblock);
	return nlp;
}
