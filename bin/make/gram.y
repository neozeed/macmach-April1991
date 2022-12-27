%{
#ifndef lint
static char *RCSid = "$Header: gram.y,v 5.0 89/03/01 01:39:13 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	gram.y,v $
 * Revision 5.0  89/03/01  01:39:13  bww
 * 	Version 5.
 * 	[89/03/01  01:30:23  bww]
 * 
 */

static	char *sccsid = "@(#)gram.y	4.2 (Berkeley) 87/10/22";

#include "defs.h"

struct depchain {
	struct depblock *head;
	struct depblock *tail;
};

FSTATIC struct depblock *pp, *prevdep;
FSTATIC struct shblock *prevshp;
FSTATIC int sepc;
FSTATIC int retsh();
FSTATIC int nextlin();
FSTATIC int GETC();
FSTATIC fstack();
%}

%token NAME SHELLINE START MACRODEF COLON DOUBLECOLON LPAREN RPAREN AMPERSAND

%union {
	struct shblock *yshblock;
	struct depblock *ydepblock;
	struct nameblock *ynameblock;
	struct depchain ydepchain;
}

%type <yshblock> SHELLINE, shlist, shellist
%type <ydepblock> deplist, dlist
%type <ydepchain> name, namelist, optnamelist
%type <ynameblock> NAME


%%

file	: /* empty */
	| file comline
	;

comline	: START
	| MACRODEF
	| START dlist shellist
		/* NULL target - rule ignored */
	| START namelist deplist shellist
		{ rule($2.head, $3, $4); }
	| error
	;

namelist: name
	| namelist name
		{
			$1.tail->nxtdepblock = $2.head;
			$$.head = $1.head;
			$$.tail = $2.tail;
		}
	;

deplist	: /* empty */
		{ nosepchar(); }
	| dlist
	;

dlist	: sepchar
		{ $$ = 0; }
	| dlist AMPERSAND
		/* DYNIX style parallelism - currently just ignored */
	| dlist name
		{
			if ($1 == 0)
				$$ = $2.head;
			else
				prevdep->nxtdepblock = $2.head;
			prevdep = $2.tail;
		}
	;

name	: NAME
		{
			pp = ALLOC(depblock);
			pp->nxtdepblock = 0;
			pp->depname = $1;
			$$.head = $$.tail = pp;
		}
	| NAME LPAREN optnamelist RPAREN
		{
			for (pp = $3.head; pp; pp = pp->nxtdepblock)
				if (pp->depname)
					pp->depname->archp = $1;
			$$ = $3;
		}
	| NAME LPAREN LPAREN optnamelist RPAREN RPAREN
		{
			for (pp = $4.head; pp; pp = pp->nxtdepblock)
				if (pp->depname) {
					pp->depname->archp = $1;
					pp->depname->objarch = 1;
				}
			$$ = $4;
		}
	;

optnamelist: /* empty */
		{
			pp = ALLOC(depblock);
			pp->nxtdepblock = 0;
			pp->depname = 0;
			$$.head = $$.tail = pp;
		}
	| namelist
	;

sepchar	: COLON
		{ sepc = ALLDEPS; }
	| DOUBLECOLON
		{ sepc = SOMEDEPS; }
	;

shellist: /* empty */
		{ $$ = 0; }
	| shlist
	;

shlist	: SHELLINE
		{ prevshp = $1; }
	| shlist SHELLINE
		{ prevshp = prevshp->nxtshblock = $2; }
	;

%%
extern char *builtin[];
FSTATIC char **linesptr	= builtin;

FILE *fin	= NULL;
int yylineno	= 0;
char *zznextc	= 0;


FSTATIC
rule(np, dp, sp)
	register struct depblock *np;
	struct depblock *dp;
	struct shblock *sp;
{
	register struct nameblock *leftp;
	register struct lineblock *lp, *lpp;

	for (; np; np = np->nxtdepblock) {
		leftp = np->depname;
		if (leftp->septype == 0)
			leftp->septype = sepc;
		else if (leftp->septype != sepc)
			fprintf(stderr, "Inconsistent rules lines for `%s'\n", leftp->namep);
		else if (sepc == ALLDEPS && *(leftp->namep) != '.' && sp) {
			for (lp = leftp->linep; lp->nxtlineblock; lp = lp->nxtlineblock)
				if (lp->shp)
					fprintf(stderr, "Multiple rules lines for `%s'\n", leftp->namep);
		}

		lp = ALLOC(lineblock);
		lp->nxtlineblock = 0;
		lp->depp = dp;
		lp->shp = sp;

		if (!unequal(leftp->namep, ".SUFFIXES") && dp == 0)
			leftp->linep = 0;
		else if (leftp->linep == 0)
			leftp->linep = lp;
		else {
			lpp = leftp->linep;
			while (lpp->nxtlineblock)
				lpp = lpp->nxtlineblock;
			if (sepc == ALLDEPS && leftp->namep[0] == '.')
				lpp->shp = 0;
			lpp->nxtlineblock = lp;
		}
	}
}


FSTATIC
nosepchar()
{
	char junk[64];

	(void) sprintf(junk, "%%s: Must be a separator on line %d", yylineno);
	fatal1(junk, curfname);
}


FSTATIC int
yylex()
{
	register char *p;
	register char *q;
	register int n;
	char word[INMAX];

	if (zznextc == 0)
		return nextlin();

	while (isspace(*zznextc))
		++zznextc;
	if (*zznextc == 0)
		return nextlin();

	if (*zznextc == ':') {
		if (*++zznextc == ':') {
			++zznextc;
			return DOUBLECOLON;
		}
		return COLON;
	}

	if (*zznextc == '&') {
		++zznextc;
		return AMPERSAND;
	}

	if (*zznextc == '(' || *zznextc == ')')
		return (*zznextc++ == '(') ? LPAREN : RPAREN;

	if (*zznextc == ';')
		return retsh(zznextc);

	p = zznextc;
	q = word;

	for (;;) {
		if (p != zznextc && *(p-1) == '$') {
			/*
			 * we must treat macros specially
			 * since (, ), and : are terminals.
			 */
			n = gobblename(p, q);
			p += n;
			q += n;
			continue;
		}
		if (*p == '`') {
			/*
			 * wordify `...` for dynamic dependencies
			 */
			do
				*q++ = *p++;
			while (*p && *p != '`');
			if (*p == 0)
				break;
			*q++ = *p++;
			continue;
		}
		if (funny[*p] & TERMINAL)
			break;
		*q++ = *p++;
	}

	if (p != zznextc) {
		*q = 0;
		yylval.ynameblock = makename(word);
		zznextc = p;
		return NAME;
	}

	fprintf(stderr,"Bad character %c (octal %o), file %s line %d",
			*zznextc, *zznextc, curfname, yylineno);
	fatal((char *) 0);
	/*NOTREACHED*/
}


FSTATIC int
retsh(q)
	char *q;
{
	register char *p;
	struct shblock *sp;
	extern char *copys();

	for (p = q+1; isspace(*p); ++p)
		;
	sp = ALLOC(shblock);
	sp->nxtshblock = 0;
	sp->shbp = (fin == NULL ? p : copys(p));
	yylval.yshblock = sp;
	zznextc = 0;
	return SHELLINE;
}


FSTATIC int
nextlin()
{
	register int c, kc;
	register char *p, *t;
	int incom, indep;
	char *incmd, *text, templin[INMAX];
	static char yytext[INMAX];
	static char *yytextl = yytext+INMAX;

again:
	incom = 0;
	indep = 0;
	incmd = 0;
	zznextc = 0;

	if (fin == NULL) {
		if ((text = *linesptr++) == 0)
			return 0;
		++yylineno;
	} else {
		for (p = text = yytext; p < yytextl; *p++ = kc)
			switch (kc = GETC()) {
			case 0:
				goto again;
			case '\t':
				if (p != yytext)
					break;
			case ';':
				if (!incom && !indep && incmd == 0)
					incmd = p;
				break;
			case '#':
				if (incmd)
					break;
				if (p == yytext || p[-1] != '\\')
					incom = 1, kc = 0;
				else
					--p;
				break;
			case '`':
				indep = !indep;
				break;
			case '\n':
				++yylineno;
				if (p == yytext || p[-1] != '\\') {
					*p = 0;
					goto endloop;
				}
				p[-1] = ' ';
				while ((kc = GETC()) != EOF && isspace(kc))
					if (kc == '\n')
						++yylineno;
				if (kc != EOF)
					break;
			case EOF:
				*p = 0;
				return 0;
			}

		fatal("line too long");
	}

endloop:
	if ((c = text[0]) == '\t')
		return retsh(text);

	/*
	 * DO include FILES HERE.
	 */
	kc = 0;
	if (c == '-' && strncmp(text, "-include", 8) == 0) {
		kc = 1;
		text++;
	}
	if (text[0] == 'i' && strncmp(text, "include", 7) == 0
	&& (text[7] == ' ' || text[7] == '\t')) {
		char *pfile;

		for (p = text + 8; *p; p++)
			if (*p != ' ' && *p != '\t')
				break;
		pfile = p;
		while (*p && !isspace(*p))
			p++;
		*p = 0;
		/*
		 * Start using new file.
		 */
		fstack(pfile, &fin, &yylineno, kc == 0);
		goto again;
	}
	if (kc)
		text--;

	if (isalpha(c) || isdigit(c) || c == '_'
	|| c == ' ' || c == '.' || c == '$')
		for (p = text+1; *p; p++) {
			if (*(p-1) == '$')
				p += gobblename(p, templin);
			if (*p == ':') {
				if (*(p+1) != '=')
					break;
				(void) teqsign(text, p);
				return MACRODEF;
			}
			if (*p == '=') {
				(void) eqsign(text, p, 0);
				return MACRODEF;
			}
		}

	/*
	 * substitute for macros on dependency
	 * line up to the semicolon if any
	 */
	if (incmd)
		*incmd = 0;
	t = subst(yytext, templin);
	if (incmd) {
		*t = ';';
		while (*++t = *++incmd)
			;
	}

	p = templin;
	t = yytext;
	while (*t++ = *p++)
		;
	for (p = zznextc = text; *p; ++p)
		if (*p != ' ' && *p != '\t')
			return START;
	goto again;
}


/*
 * GETC automatically unravels stacked include files. That is,
 * during include file processing, when a new file is encountered
 * fstack will stack the FILE pointer argument. Subsequent
 * calls to GETC with the new FILE pointer will get characters
 * from the new file. When an EOF is encountered, GETC will
 * check to see if the file pointer has been stacked. If so,
 * a character from the previous file will be returned.
 * The external references are "GETC()" and "fstack(fname,stream,lno)".
 * "Fstack(stfname,ream,lno)" is used to stack an old file pointer before
 * the new file is assigned to the same variable. Also stacked are the
 * file name and the old current lineno, generally, yylineno.
 */

FSTATIC int morefiles = 0;
FSTATIC struct sfiles {
	char sfname[MAXPATHLEN];
	FILE *sfilep;
	char *sfilen;
	int syylno;
} sfiles[NOFILE];


FSTATIC int
GETC()
{
	register int c;

	if (fin == NULL)
		fatal("NULL fin in GETC");
	c = getc(fin);
	while (c == EOF && morefiles) {
		(void) fclose(fin);
		yylineno = sfiles[--morefiles].syylno;
		curfname = sfiles[morefiles].sfilen;
		if ((fin = sfiles[morefiles].sfilep) == NULL)
			return 0;
		c = getc(fin);
	}
	return c;
}


FSTATIC
fstack(fname, oldfp, oldlno, ferr)
	register char *fname;
	register FILE **oldfp;
	register int *oldlno;
	int ferr;
{
	register char *p, *q, *r;
	register struct nameblock *np;
	FILE *savfp;
	char incname[MAXPATHLEN+1];
	char newname[MAXPATHLEN+1];

	(void) subst(fname, incname);
	if (*incname == 0)
		fatal("No include file name");
	p = newname;
	if (*incname != '/') {
		r = 0;
		for (q = curfname; *q; *p++ = *q++)
			if (*q == '/')
				r = p + 1;
		p = r ? r : newname;
	}
	for (q = incname; *q; *p++ = *q++)
		;
	*p = 0;
	if (dbgflag)
		printf("Include file: \"%s\"\n", newname);
	savfp = *oldfp;
	setvpath();
	np = rcsco(newname);
	if ((*oldfp = fopen(np->alias ? np->alias : newname, "r")) == NULL) {
		if (ferr)
			fatal1("Cannot open %s", newname);
		*oldfp = savfp;
		return;
	}
	/*
	 * Stack the new file name, the old file pointer and the
	 * old yylineno;
	 */
	(void) strncpy(sfiles[morefiles].sfname, newname, MAXPATHLEN);
	sfiles[morefiles].sfilep = savfp;
	sfiles[morefiles].sfilen = curfname;
	sfiles[morefiles].syylno = *oldlno;
	curfname = sfiles[morefiles++].sfname;
	yylineno = 0;
}
