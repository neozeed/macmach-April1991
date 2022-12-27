#ifndef lint
static char *RCSid = "$Header: misc.c,v 5.0 89/03/01 01:40:38 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	misc.c,v $
 * Revision 5.0  89/03/01  01:40:38  bww
 * 	Version 5.
 * 	[89/03/01  01:32:42  bww]
 * 
 */

static	char *sccsid = "@(#)misc.c	4.5 (Berkeley) 87/10/22";

#include "defs.h"

FSTATIC struct nameblock *hashtab[HASHSIZE];
FSTATIC int lasthash	= 0;
FSTATIC int nhashed	= 0;
FSTATIC int inenvir	= 0;
FSTATIC char snam[]	= "@*<?>%";
FSTATIC struct varblock svar[sizeof(snam)-1];
FSTATIC int hasslash();

/*
 * simple linear hash.
 */
FSTATIC int
hashloc(s)
	char *s;
{
	register unsigned int hashval;
	register char *t;
	register int i;

	hashval = 0;

	for (t = s; *t; t++)
		hashval += (*t << 8) | *(t + 1);

	lasthash = hashval %= HASHSIZE;

	for (i = hashval; hashtab[i]
		&& (hashval != hashtab[i]->hashval
		|| *s != *(hashtab[i]->namep)
		|| unequal(s, hashtab[i]->namep));
			i = (i+1) % HASHSIZE)
		;

	return i;
}


struct nameblock *
srchname(s)
	char *s;
{
	return hashtab[hashloc(s)];
}


struct nameblock *
makename(s)
	char *s;
{
	register struct nameblock *p;
	register int i;
	char *pc;
	extern char *copys();

	if ((p = hashtab[i = hashloc(s)]) == 0) {
		if (nhashed++ > HASHSIZE-8)
			fatal("Hash table overflow");
		p = ALLOC(nameblock);
		p->namep = copys(s);
		p->dollar = (index(s, '$') != 0);
		p->percent = (index(s, '%') != 0);
		p->bquotes = ((pc = index(s, '`')) != 0
				&& index(pc+1, '`') != 0);
		p->alias = 0;
		p->linep = 0;
		p->tmacp = 0;
		p->RCSnamep = 0;
		p->archp = 0;
		p->hashval = lasthash;
		p->nxtnameblock = firstname;
		firstname = p;
		hashtab[i] = p;
	}
	if (mainname == 0)
		if (*s != '.' || hasslash(s))
			mainname = p;
	return p;
}


FSTATIC int
hasslash(s)
	register char *s;
{
	for (; *s; ++s)
		if (*s == '/')
			return 1;
	return 0;
}


char *
copys(s)
	register char *s;
{
	register char *t, *t0;

	t = t0 = ckalloc(strlen(s)+1);
	while (*t++ = *s++)
		;
	return t0;
}


/*
 * c = concatenation of a and b
 */
char *
concat(a, b, c)
	register char *a, *b;
	char *c;
{
	register char *t;

	t = c;
	while (*t = *a++)
		t++;
	while (*t++ = *b++)
		;
	return c;
}


/*
 * is b the suffix of a?  if so, set p = prefix
 */
int
suffix(a, b, p)
	register char *a, *b, *p;
{
	register char *a0 = a, *b0 = b;

	while (*a)
		a++;
	while (*b)
		b++;
	if ((a-a0) < (b-b0))
		return 0;
	while (b > b0)
		if (*--a != *--b)
			return 0;
	while (a0 < a)
		*p++ = *a0++;
	*p = 0;
	return 1;
}


char *
ckalloc(n)
	register int n;
{
	register char *p;
	extern char *calloc();

	if (p = calloc(1, (unsigned) n))
		return p;
	fatal("out of memory");
#ifdef lint
	return ckalloc(n);  /* use and return */
#endif
}


/*
 * copy t into s, return the location of the next free character in s
 */
FSTATIC char *
copstr(s, t)
	register char *s, *t;
{
	if (t == 0)
		return s;
	while (*t)
		*s++= *t++;
	*s = 0;
	return s;
}


struct varblock *
srchvar(v)
	register char *v;
{
	register struct varblock *vp;
	register int len, n;
	register char *p;

	if ((len = strlen(v)) == 1 && (p = index(snam, *v)))
		return (vp = svar) + (p - snam);  /* vp = hccom bug */
	vp = firstvar;
	while (vp) {
		if ((n = (len - vp->varlen)) == 0
		&& (n = (*v - *vp->varname)) == 0
		&& (n = strcmp(v, vp->varname)) == 0)
			return vp;
		vp = (n < 0) ? vp->lftvarblock : vp->rgtvarblock;
	}
	return 0;
}


int
gobblename(s, b)
	register char *s, *b;
{
	register int closer;
	register int i, n;

	if ((*b = *s) != '(' && *s != '{')
		return 1;
	n = 0;
	closer = (n++, b++, *s++ == '(') ? ')' : '}';
	while (*s && *s != closer)
		if (n++, (*b++ = *s++) == '$') {
			i = gobblename(s, b);
			n += i;
			b += i;
			s += i;
		}
	if (*s)
		n++, *b = *s;
	return n;
}

FSTATIC char *condtrans();
FSTATIC char *trans();
FSTATIC char *condtrans();
FSTATIC char *sufftrans();
FSTATIC char *substrans();

FSTATIC struct {
	char	sm_key;  /* unique */
	char	sm_sep;
	char	*(*sm_func)();
} strmod[] = {
	{ '?', ':', condtrans },
	{ ':', '=', sufftrans },
	{ '/', '/', substrans },
	{ ';', ';', substrans }
};

#define	NMODS	(sizeof(strmod)/sizeof(strmod[0]))

struct xlt {
	char	*x_sepp;
	char	*x_endp;
	char	*(*x_func)();
};


FSTATIC char *
findname(s, v, b, xp)
	register char *s, *v, *b;
	register struct xlt *xp;
{
	register int closer, n;
	register char sep, *iv;

	xp->x_sepp = 0;
	xp->x_endp = 0;
	xp->x_func = 0;
	if (*s != '(' && *s != '{')
		*v++ = *s++;
	else {
		closer = (*s++ == '(') ? ')' : '}';
		while (isspace(*s))
			s++;
		iv = v;
		while (*s && *s != closer) {
			for (n = 0; n != NMODS; n++)
				if (*s == strmod[n].sm_key)
					break;
			if (n != NMODS || *s == '\\') {
				if (v != iv && *(v-1) == '\\')
					--v;
				else if (n != NMODS && (v !=iv
				|| index(snam, *s) == 0))
					break;
			}
			if ((*v++ = *s++) == '$') {
				n = gobblename(s, v);
				s += n;
				v += n;
			}
		}
		while (v != iv && isspace(*(v-1)))
			--v;
		if (*s && *s != closer) {
			xp->x_func = strmod[n].sm_func;
			sep = strmod[n].sm_sep;
			*b++ = *s++;
		}
		while (*s && *s != closer) {
			if ((*b = *s++) == '$') {
				n = gobblename(s, ++b);
				s += n;
				b += n;
			} else if (*b != sep)
				b++;
			else if (*(b-1) == '\\')
				*(b-1) = *b;
			else if (xp->x_sepp == 0)
				xp->x_sepp = b++;
			else if (xp->x_endp == 0)
				xp->x_endp = b++;
			else
				b++;
		}
		if (*s)
			s++;
	}
	*v = 0;  *b = 0;
	return s;
}


/*
 * copy string a into b, substituting for arguments
 */
char *
subst(a, b)
	register char *a, *b;
{
	char vname[BUFSIZ], vtemp[BUFSIZ], vbuf[BUFSIZ];  /* XXX */
	struct xlt xlt;
	static depth = 0;

	if (a == 0) {
		*b = 0;
		return b;
	}
	if (depth++ >= 100)
		fatal("infinitely recursive macro?");
	while (*a) {
		if (*a != '$' || *++a == 0 || *a == '$') {
			*b++ = *a++;
			continue;
		}
		a = findname(a, vtemp, vbuf, &xlt);
		(void) subst(vtemp, vname);
		b = trans(b, vname, vbuf, &xlt);
	}
	--depth;
	*b = 0;
	return b;
}


/*
 * copy string a into b, only substituting for 'name'
 */
FSTATIC char *
onlysub(name, a, b)
	char *name;
	register char *a, *b;
{
	char sep, vname[BUFSIZ], vbuf[BUFSIZ];  /* XXX */
	struct xlt xlt;
	struct varblock *vbp;

	if (a == 0) {
		*b = 0;
		return b;
	}
	while (*a) {
		if (*a != '$') {
			*b++ = *a++;
			continue;
		}
		if (*++a == 0 || *a == '$') {
			*b++ = '$';
			*b++ = *a++;
			continue;
		}
		sep = *a;
		a = findname(a, vname, vbuf, &xlt);
		if (unequal(vname, name)) {
			*b++ = '$';
			if (vname[0] && vname[1] == 0 && vbuf[0] == 0) {
				*b++ = vname[0];
				continue;
			}
			if (sep != '(' && sep != '{')
				sep = '(';
			*b++ = sep;
			b = copstr(b, vname);
			b = copstr(b, vbuf);
			*b++ = (sep == '(') ? ')' : '}';
			continue;
		}
		b = trans(b, vname, vbuf, &xlt);
		vbp = srchvar(vname);
		if (vbp && vbp->varval)
			vbp->used = 0;
	}
	*b = 0;
	return b;
}


/*
 * Do the $(@D) type translations.
 */
FSTATIC char *
dftrans(b, vname)
	register char *b;
	char *vname;
{
	char c, c1;
	char *p1, *p2;
	struct varblock *vbp;

	c1 = vname[1];
	vname[1] = 0;
	vbp = srchvar(vname);
	if (vbp && vbp->varval) {
		for (p1 = p2 = vbp->varval; *p1; p1++)
			if (*p1 == '/')
				p2 = p1;
		if (*p2 == '/') {
			if (c1 == 'D') {
				if (p2 == vbp->varval)
					p2++;
				c = *p2;
				*p2 = 0;
				b = copstr(b,vbp->varval);
				*p2 = c;
			} else
				b = copstr(b, p2+1);
		} else {
			if (c1 == 'D')
				b = copstr(b, ".");
			else
				b = copstr(b, p2);
		}
	}
	vname[1] = c1;
	return b;
}


/*
 * Standard translation, nothing fancy.
 */
FSTATIC char *
straightrans(b, vname)
	register char *b;
	char *vname;
{
	register struct varblock *vbp;

	if (amatch(vname, "[@*<][DF]"))
		return dftrans(b, vname);
	vbp = srchvar(vname);
	if (vbp && vbp->varval) {
		b = subst(vbp->varval, b);
		vbp->used = 1;
	}
	return b;
}


/*
 * give only the unique elements of a list
 */
FSTATIC char *
uniqtrans(b, vname)
	register char *b;
	char *vname;
{
	register char *p, *q, *r;
	char temp[OUTMAX], ttemp[OUTMAX];  /* XXX */
	char tmp, *psave, *ob;

	*(p = ttemp) = 0;
	(void) straightrans(p, vname);
	r = temp;
	for (;;) {
		ob = b;
		while (isspace(*p))
			*b++ = *p++;
		if (*p == 0)
			break;
		psave = p;
		while (*p && !isspace(*p))
			p++;
		tmp = *p;
		*p = 0;
		for (q = temp; q != r; q++)
			if (strcmp(q, psave) == 0)
				break;
		if (q != r)
			b = ob;
		else {
			b = copstr(b, psave);
			r = copstr(r, psave) + 1;
		}
		*p = tmp;
	}
	return b;
}


/*
 * $(name?*:*) type translations.
 */
FSTATIC char *
condtrans(b, vname, alt, cp)
	register char *b;
	char *vname, *alt, *cp;
{
	register struct varblock *vbp;

	*cp = 0;
	if ((vbp = srchvar(vname)) == 0 || vbp->varval == 0 || vbp->builtin)
		b = subst(cp+1, b);
	else {
		b = subst(alt, b);
		vbp->used = 1;
	}
	*cp = ':';
	return b;
}


/*
 * Translate the $(name:*=*) type things.
 */
FSTATIC char *
sufftrans(b, vname, mod, ep)
	char *b, *vname, *mod, *ep;
{
	register char *p;
	register int fromlen;
	char from[BUFSIZ], to[BUFSIZ];  /* XXX */
	char ttemp[OUTMAX];  /* XXX */
	char tmp, *psave;

	*ep = 0;
	(void) subst(mod, from);
	*ep++ = '=';
	(void) subst(ep, to);
	fromlen = 0;
	for (p = from; *p; p++)
		fromlen++;

	*(p = ttemp) = 0;
	(void) straightrans(p, vname);
	for (;;) {
		while (isspace(*p))
			*b++ = *p++;
		if (*p == 0)
			break;
		psave = p;
		while (*p && !isspace(*p))
			p++;
		if (fromlen <= p - psave
		&& strncmp(p - fromlen, from, fromlen) == 0) {
			tmp = *(p - fromlen);
			*(p - fromlen) = 0;
			b = copstr(b, psave);
			*(p - fromlen) = tmp;
			b = copstr(b, to);
		} else {
			tmp = *p;
			*p = 0;
			b = copstr(b, psave);
			*p = tmp;
		}
	}
	return b;
}


#define	QUOTE	(0200)  /* XXX ASCII */

/*
 * Do $(name/lhs/rhs/) translations.
 */
FSTATIC char *
substrans(b, vname, mod, sp, ep)
	char *b, *vname, *mod, *sp, *ep;
{
	register char *p, *q, *r;
	char *rep, pat[BUFSIZ];  /* XXX */
	char temp[OUTMAX], ttemp[OUTMAX];  /* XXX */
	char tmp, *psave;

	tmp = *sp;
	*sp = 0;
	(void) subst(mod, pat);
	*sp++ = tmp;
	rep = sp;
	if (ep) {
		if (*(ep+1))
			fatal1("'%s': extra chars", mod);
		tmp = *ep;
		*ep = 0;
	}

	*(p = ttemp) = 0;
	(void) straightrans(p, vname);
	r = temp;
	for (;;) {
		while (isspace(*p))
			*r++ = *p++;
		if (*p == 0)
			break;
		psave = p;
		while (*p && !isspace(*p)) {
			if (*p == '$')
				*p |= QUOTE;
			p++;
		}
		tmp = *p;
		*p = 0;
		/*
		 * should be replaced with "real" substitution
		 */
		if (strcmp(pat, "^") == 0) {
			r = copstr(r, rep);
			r = copstr(r, psave);
		} else if (strcmp(pat, "*") == 0) {
			for (q = rep; *q; q++)
				if (*q == '&')
					r = copstr(r, psave);
				else
					*r++ = *q;
		} else if (strcmp(pat, "$") == 0) {
			r = copstr(r, psave);
			r = copstr(r, rep);
		} else
			fatal1("sorry, no can do '%s'", pat);
		*p = tmp;
	}
	*r = 0;

	if (ep)
		*ep = tmp;
	(void) subst(temp, b);
	for (p = b; *p; p++)
		if (*p & QUOTE)
			*p &= ~QUOTE;
	return p;
}


/*
 * Do csh-style $(name:[ehrt]) translations.
 */
FSTATIC char *
cshtrans(b, vname, c)
	register char *b;
	char *vname;
	register int c;
{
	register char *p1, *p2;
	char sep, *psave;
	char tmp, ttemp[BUFSIZ];  /* XXX */

	if (isupper(c))
		c = tolower(c);
	if (c != 'h' && c != 't') {
		sep = '.';
		c = (c == 'r') ? 'h' : 't';
	} else
		sep = '/';

	*(p1 = ttemp) = 0;
	(void) straightrans(p1, vname);

	for (;;) {
		while (isspace(*p1))
			*b++ = *p1++;
		if (*p1 == 0)
			break;
		psave = p1;
		p2 = 0;
		while (*p1 && !isspace(*p1)) {
			if (*p1 == sep)
				p2 = p1;
			p1++;
		}
		tmp = *p1;
		*p1 = 0;
		if (p2 == 0) {
			if (c != 't' || sep != '.')
				b = copstr(b, psave);
		} else {
			if (c == 'h') {
				*p2 = 0;
				b = copstr(b, psave);
				*p2 = sep;
			} else
				b = copstr(b, p2 + 1);
		}
		*p1 = tmp;
	}

	return b;
}


FSTATIC char *
trans(b, vn, vb, xp)
	char *b, *vn;
	register char *vb;
	register struct xlt *xp;
{
	register char *p;

	for (p = vn; *p; p++)
		if (*p & QUOTE)
			*p &= ~QUOTE;
	if (*vb == 0)
		return straightrans(b, vn);
	if (amatch(vb, ":[uU]"))
		return uniqtrans(b, vn);
	if (amatch(vb, ":[ehrtEHRT]"))
		return cshtrans(b, vn, *(vb+1));
	if (xp->x_sepp == 0 || xp->x_func == 0)
		fatal1("Unrecognized modifier '%s'", vb);
	return (*xp->x_func)(b, vn, vb+1, xp->x_sepp, xp->x_endp);
}


setvar(v, s, rst)
	char *v, *s;
	int rst;
{
	register struct varblock *p;

	p = varptr(v);
	if (p->noreset)
		return;
	p->varval = s;
	p->noreset = inarglist;
	p->builtin = ininternal;
	if (p->used == 0 || rst)
		return;
	fprintf(stderr, "Warning: %s changed after being used\n", v);
}


/*
 * look for arguments with equal signs but not colons
 */
int
eqsign(a, ep, rst)
	register char *a, *ep;
	int rst;
{
	register char *s, *t;
	register char c;
	char sbuf[INMAX], buf[INMAX];  /* XXX */

	while (isspace(*a))
		a++;
	if (ep == 0)
		for (ep = a; *ep && *ep != ':'; ep++)
			if (*ep == '=')
				break;
	if (*ep != '=')
		return 0;
	for (t = ep; t != a && isspace(*(t-1)); --t)
		;
	c = *t;
	*t = 0;
	for (s = ep+1; isspace(*s); s++)
		;
	(void) subst(a, sbuf);
	(void) onlysub(sbuf, s, buf);
	if (inarglist && inenvir == 0)
		setenv(sbuf, buf, 1);
	setvar(sbuf, copys(buf), rst);
	*t = c;
	return 1;
}


/*
 * target macro definition
 */
int
teqsign(a, cp)
	register char *a;
	char *cp;
{
	register struct nameblock *p;
	register struct tmacblock *q;
	register char *s, *t;
	register char c;

	if (*cp != ':' || *(cp+1) != '=')
		return 0;
	for (s = cp+2; isspace(*s); s++)
		;
	s = copys(s);
	*cp = 0;
	for (;;) {
		while (isspace(*a))
			a++;
		if (*a == 0)
			break;
		for (t = a; *t && !isspace(*t); t++)
			;
		c = *t;
		*t = 0;
		p = makename(a);
		q = ALLOC(tmacblock);
		q->nxttmacblock = p->tmacp;
		q->tmacbp = s;
		p->tmacp = q;
		*(a = t) = c;
	}
	*cp = ':';
	return 1;
}


struct varblock *
varptr(v)
	register char *v;
{
	register struct varblock *vp, *vlp;
	register int len, n;
	register char *p;

	if ((len = strlen(v)) == 1 && (p = index(snam, *v)))
		return (vp = svar) + (p - snam);  /* vp = hccom bug */
	vlp = 0;
	vp = firstvar;
	while (vp) {
		if ((n = (len - vp->varlen)) == 0
		&& (n = (*v - *vp->varname)) == 0
		&& (n = strcmp(v, vp->varname)) == 0)
			return vp;
		vlp = vp;
		vp = (n < 0) ? vp->lftvarblock : vp->rgtvarblock;
	}

	vp = ALLOC(varblock);
	vp->lftvarblock = 0;
	vp->rgtvarblock = 0;
	vp->varname = copys(v);
	vp->varlen = len;
	vp->varval = 0;

	if (vlp == 0)
		firstvar = vp;
	else if (n < 0)
		vlp->lftvarblock = vp;
	else
		vlp->rgtvarblock = vp;

	return vp;
}


fatal1(s, t)
	char *s;
	char *t;
{
	char buf[BUFSIZ];

	(void) sprintf(buf, s, t);
	fatal(buf);
}


fatal(s)
	char *s;
{
	(void) fflush(stdout);
	if (s)
		fprintf(stderr, "Make: %s.  Stop.\n", s);
	else
		fprintf(stderr, "\nStop.\n");
#ifdef unix
	quit(1);
#endif
#ifdef gcos
	quit(0);
#endif
}


yyerror(s)
	char *s;
{
	char buf[256];
	extern int yylineno;

	(void) sprintf(buf, "%%s, line %d: %s", yylineno, s);
	fatal1(buf, curfname);
}


struct chain *
appendq(head, tail)
	struct chain *head;
	char *tail;
{
	register struct chain *p, *q;

	p = ALLOC(chain);
	p->datap = tail;
	p->nextp = 0;

	if (head) {
		for (q = head; q->nextp; q = q->nextp)
			;
		q->nextp = p;
		return head;
	}
	return p;
}


char *
mkqlist(p)
	struct chain *p;
{
	register char *qbufp, *s;
	static char qbuf[QBUFMAX];

	if (p == 0) {
		*qbuf = 0;
		return qbuf;
	}
	for (qbufp = qbuf; p; p = p->nextp) {
		s = p->datap;
		if (qbufp+strlen(s) > &qbuf[QBUFMAX-3]) {
			fprintf(stderr, "$? list too long\n");
			break;
		}
		while (*s)
			*qbufp++ = *s++;
		*qbufp++ = ' ';
	}
	*--qbufp = 0;
	return qbuf;
}


/*
 * Called in main
 * If a string like "CC=" occurs then CC is not put in environment.
 * This is because there is no good way to remove a variable
 * from the environment within the shell.
 */
readenv()
{
	register char **ea, *p;
	extern char **environ;

	inenvir	= 1;
	for (ea = environ; *ea; ea++) {
		for (p = *ea; *p && *p != '='; p++)
			if (!isalnum(*p) && *p != '_' && *p != '-')
				break;
		if (*p == '=' && *(p+1))
			(void) eqsign(*ea, p, 0);
	}
	inenvir	= 0;
}


/*
 * add the .EXPORT dependencies to the environment
 */
export(p)
	struct nameblock *p;
{
	register struct lineblock *lp;
	register struct depblock *q;
	register char *v;
	char buf[INMAX];  /* XXX */

	for (lp = p->linep; lp; lp = lp->nxtlineblock)
		for (q = lp->depp; q; q = q->nxtdepblock) {
			if (q->depname == 0)
				continue;
			if (v = varptr(q->depname->namep)->varval) {
				(void) subst(v, buf);
				setenv(q->depname->namep, buf, 1);
			}
		}
}


/*
 * hook for catching make as it exits
 */
FSTATIC
onexit()
{
	struct nameblock *p;
	time_t td;
	struct chain *ch;
	static int once = 0;

	if (once)
		return;
	once = 1;
	if ((p = srchname(".EXIT")) == 0)
		return;
	ch = 0;
	(void) doname(p, 0, &td, &ch);
}


quit(val)
	int val;
{
	onexit();
	touchflag = 0;		/* don't try to touch anything */
	rm();			/* clean up co'ed files */
	exit(val);
}


/*
 * ncat copies n chars of string b at a, returning a pointer to the
 * null terminating a.  This way, it can be used to daisy-chain.
 * If n < 0, all chars of b will be copied
 */
char *
ncat(a, b, n)
	register char *a, *b;
	register int n;
{
	if (n < 0) {
		while (*a++ = *b++)
			;
		return --a;
	}
	while (n--)
		if ((*a++ = *b++) == 0)
			return --a;
	*a = 0;
	return a;
}


char *
sindex(big, small)
	char *big, *small;
{
	register char *bp, *bp1, *sp;
	register char c = *small++;

	if (c == 0)
		return 0;
	for (bp = big; *bp; bp++)
		if (*bp == c) {
			for (sp = small, bp1 = bp+1; *sp && *sp == *bp1++; sp++)
				;
			if (*sp == 0)
				return bp;
		}
	return 0;
}
