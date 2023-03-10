#ifndef lint
static char *RCSid = "$Header: libs.c,v 5.0 89/03/01 01:39:31 bww Exp $";
#endif

/*
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 ************************************************************************
 * HISTORY
 * $Log:	libs.c,v $
 * Revision 5.0  89/03/01  01:39:31  bww
 * 	Version 5.
 * 	[89/03/01  01:30:54  bww]
 * 
 */

/*
 * look inside archives for notations a(b) and a((b)):
 *	a(b)	is file member b in archive a
 *	a((b))	is entry point _b in object archive a
 *
 * This stuff needs to be rewritten to handle different archive types
 * dynamically.  It should also know how to use "ranlib" info if present.
 */

#include "defs.h"

#ifndef COFF
#include <ar.h>
#include <a.out.h>
#include <fcntl.h>

FSTATIC struct ar_hdr arhead;
FSTATIC long strtab;
FSTATIC long arflen;
FSTATIC char arfname[sizeof(arhead.ar_name)];
FSTATIC FILE *arfd = NULL;
FSTATIC long arpos, arlen;
FSTATIC struct exec objhead;
FSTATIC struct nlist objentry;

#ifdef ASCARCH
extern long atol();
#endif

FSTATIC int openarch();
FSTATIC int getarch();
FSTATIC int getobj();
FSTATIC int eqstr();

time_t
lookarch(pname)
	register struct nameblock *pname;
{
	long nsym;
	FSTATIC char *getonam();

	if (dbgflag)
		printf("lookarch(%s)\n", pname->archp->namep);
	if (openarch(pname->archp->namep) == -1)
		return 0;

	while (getarch()) {
		if (pname->objarch == 0) {
			if (eqstr(arfname, pname->namep, sizeof(arfname)))
#ifdef ASCARCH
				return atol(arhead.ar_date);  /* XXX */
#else
				return arhead.ar_date;
#endif
			continue;
		}
		if (getobj() == 0)
			continue;
		for (nsym = objhead.a_syms / sizeof(objentry); nsym; --nsym) {
			(void) fread((char *) &objentry, sizeof(objentry), 1, arfd);
			objentry.n_un.n_name = getonam(objentry.n_un.n_strx);
			if (dbgflag)
				printf("foundsym(%s)\n", objentry.n_un.n_name);
			if ((objentry.n_type & N_EXT)
			&& ((objentry.n_type & ~N_EXT) || objentry.n_value)
			&& !unequal(objentry.n_un.n_name, pname->namep))
#ifdef ASCARCH
				return atol(arhead.ar_date);  /* XXX */
#else
				return arhead.ar_date;
#endif
		}
	}

	return 0;
}


FSTATIC int
openarch(f)
	register char *f;
{
#ifdef ASCARCH
	char magic[SARMAG];
#else
	int word;
#endif
	struct stat buf;
	static char lastf[MAXPATHLEN+1];
	static char arbuf[BUFSIZ];

	if (arfd == NULL || unequal(f, lastf)) {
		if (arfd != NULL)
			(void) fclose(arfd);
		if (dbgflag)
			printf("openarch(%s)\n", f);
		if ((arfd = fopen(f, "r")) == NULL)
			return -1;
		(void) fcntl(fileno(arfd), F_SETFD, 1);
		setbuf(arfd, arbuf);
		(void) strcpy(lastf, f);
		(void) fstat(fileno(arfd), &buf);
		arlen = buf.st_size;
#ifdef ASCARCH
		(void) fread(magic, SARMAG, 1, arfd);
		arpos = SARMAG;
		if (!eqstr(magic, ARMAG, SARMAG))
#else
		(void) fread((char *) &word, sizeof(word), 1, arfd);
		arpos = sizeof(word);
		if (word != ARMAG)
#endif
			fatal1("%s is not an archive", f);
	} else {
#ifdef ASCARCH
		(void) fseek(arfd, arpos = SARMAG, 0);
#else
		(void) fseek(arfd, arpos = sizeof(word), 0);
#endif
	}
	arflen = 0;
	return 0;
}


FSTATIC int
getarch()
{
	char *p;

	/*
	 * round archived file length up to even
	 */
	arpos += (arflen + 1) & ~1L;
	if (arpos >= arlen)
		return 0;
	(void) fseek(arfd, arpos, 0);
	(void) fread((char *) &arhead, sizeof(arhead), 1, arfd);
	arpos += sizeof(arhead);
#ifdef ASCARCH
	arflen = atol(arhead.ar_size);
#else
	arflen = arhead.ar_size;
#endif
	(void) strncpy(arfname, arhead.ar_name, sizeof(arfname));
	for (p = arfname + sizeof(arfname); *--p == ' '; *p = 0)
		;
	return 1;
}


FSTATIC int
getobj()
{
	long skip;

	(void) fread((char *) &objhead, sizeof(objhead), 1, arfd);
	if (N_BADMAG(objhead)) {
		if (dbgflag)
			printf("skipping(%.*s)\n", sizeof(arfname), arfname);
		return 0;
	}
	skip = objhead.a_text + objhead.a_data;
#ifndef pdp11
	skip += objhead.a_trsize + objhead.a_drsize;
#else
	if (!objhead.a_flag)
		skip *= 2;
#endif
	(void) fseek(arfd, skip, 1);
	strtab = ftell(arfd) + objhead.a_syms;
	return 1;
}


FSTATIC int
eqstr(a, b, n)
	register char *a, *b;
	int n;
{
	register int i;

	for (i = 0; i < n && (*a || *b); ++i)
		if (*a++ != *b++)
			return 0;
	return 1;
}


FSTATIC char *
getonam(p)
	long p;
{
	long at;
	register int c;
	register char *s;
	static char space[MAXNAMLEN+1];

	at = ftell(arfd);
	if (fseek(arfd, strtab + p, 0) == -1) {
		(void) sprintf(space, "%.*s", sizeof(arfname), arfname);
		fatal1("%s is a corrupt object module\n", space);
	}
	s = space;
	while ((c = fgetc(arfd)) != EOF && c && s < space + MAXNAMLEN)
		*s++ = c;
	*s = 0;
	(void) fseek(arfd, at, 0);
	return space;
}
#else /* COFF */
#include <pwd.h>
#include <ar.h>

/*
 * BSD flag seems to refer to use of BSD random library format. (ranlib)
 * UMAX never uses it, so turn it off.
 */

#ifdef  BSD
#undef  BSD
#endif  BSD
#define BSD     0

#if BSD
#include <ranlib.h>
#endif

/*
 * This routine was under the BSD symbol for some reason.  It is
 * needed to access fields in archive file structures.
 */
/*
 * static char ID[] = "@(#) sgetl.c: 1.1 1/8/82";
 *
 * The intent here is to provide a means to make the value of
 * bytes in an io-buffer correspond to the value of a long
 * in the memory while doing the io a `long' at a time.
 * Files written and read in this way are machine-independent.
 */
/*#include <values.h>  This file defines BITSPERBYTE, it's system 5 !@! */
#define BITSPERBYTE     8

long
sgetl(buffer)
register char *buffer;
{
        register long w = 0;
        register int i = BITSPERBYTE * sizeof(long);

        while ((i -= BITSPERBYTE) >= 0)
                w |= (long) ((unsigned char) *buffer++) << i;
        return (w);
}


#define equaln          !strncmp
#define equal           !strcmp

/*
 * For 6.0, create a make which can understand all three archive
 * formats.  This is kind of tricky, and <ar.h> isn't any help.
 * Note that there is no sgetl() and sputl() on the pdp11, so
 * make cannot handle anything but the one format there.
 */
char    archname[64];           /* archive file to be opened */

int     ar_type;        /* to distiguish which archive format we have */
#define ARpdp   1
#define AR5     2
#define ARport  3

long    first_ar_mem;   /* where first archive member header is at */
long    sym_begin;      /* where the symbol lookup starts */
long    num_symbols;    /* the number of symbols available */
long    sym_size;       /* length of symbol directory file */

/*
 * Defines for all the different archive formats.  See next comment
 * block for justification for not using <ar.h>'s versions.
 */
#define ARpdpMAG        0177545         /* old format (short) magic number */

#define AR5MAG          "<ar>"          /* 5.0 format magic string */
#define SAR5MAG         4               /* 5.0 format magic string length */

#define ARportMAG       "!<arch>\n"     /* Port. (6.0) magic string */
#define SARportMAG      8               /* Port. (6.0) magic string length */
#define ARFportMAG      "`\n"           /* Port. (6.0) end of header string */


/*
 * These are the archive file headers for the three formats.  Note
 * that it really doesn't matter if these structures are defined
 * here.  They are correct as of the respective archive format
 * releases.  If the archive format is changed, then since backwards
 * compatability is the desired behavior, a new structure is added
 * to the list.
 */
struct          /* pdp11 -- old archive format */
{
        char    ar_name[14];    /* null terminated */
        long    ar_date;        /* native machine bit representation */
        char    ar_uid;         /*      "       */
        char    ar_gid;         /*      "       */
        int     ar_mode;        /*      "       */
        long    ar_size;        /*      "       */
} ar_pdp;

struct          /* pdp11 a.out header */
{
        short           a_magic;
        unsigned        a_text;
        unsigned        a_data;
        unsigned        a_bss;
        unsigned        a_syms;         /* length of symbol table */
        unsigned        a_entry;
        char            a_unused;
        char            a_hitext;
        char            a_flag;
        char            a_stamp;
} arobj_pdp;

struct          /* pdp11 a.out symbol table entry */
{
        char            n_name[8];      /* null-terminated name */
        int             n_type;
        unsigned        n_value;
} ars_pdp;

struct          /* UNIX 5.0 archive header format: vax family; 3b family */
{
        char    ar_magic[SAR5MAG];      /* AR5MAG */
        char    ar_name[16];            /* ' ' terminated */
        char    ar_date[4];             /* sgetl() accessed */
        char    ar_syms[4];             /* sgetl() accessed */
} arh_5;

struct          /* UNIX 5.0 archive symbol format: vax family; 3b family */
{
        char    sym_name[8];    /* ' ' terminated */
        char    sym_ptr[4];     /* sgetl() accessed */
} ars_5;

struct          /* UNIX 5.0 archive member format: vax family; 3b family */
{
        char    arf_name[16];   /* ' ' terminated */
        char    arf_date[4];    /* sgetl() accessed */
        char    arf_uid[4];     /*      "       */
        char    arf_gid[4];     /*      "       */
        char    arf_mode[4];    /*      "       */
        char    arf_size[4];    /*      "       */
} arf_5;

struct          /* Portable (6.0) archive format: vax family; 3b family */
{
        char    ar_name[16];    /* '/' terminated */
        char    ar_date[12];    /* left-adjusted; decimal ascii; blank filled */
        char    ar_uid[6];      /*      "       */
        char    ar_gid[6];      /*      "       */
        char    ar_mode[8];     /* left-adjusted; octal ascii; blank filled */
        char    ar_size[10];    /* left-adjusted; decimal ascii; blank filled */
        char    ar_fmag[2];     /* special end-of-header string (ARFportMAG) */
} ar_port;


time_t          afilescan();
time_t          entryscan();
time_t          pdpentrys();
FILE            *arfd;
char            BADAR[] = "BAD ARCHIVE";

#define ARNLEN  15              /* max number of characters in name */


time_t
lookarch(pname)
	register struct nameblock *pname;
{
        time_t date = 0L;

	(void) strcpy(archname, pname->archp->namep); /* for error messages */

        if(openarch(archname) == -1)
                return(0L);
        if(pname->objarch)
                date = entryscan(pname->namep);   /* fatals if not found */
        else
                date = afilescan(pname->namep);
        clarch();
        return(date);
}


time_t
afilescan(name)         /* return date for named archive member file */
char *name;
{
        long    ptr;

        if (fseek(arfd, first_ar_mem, 0) != 0)
        {
        seek_error:;
                fatal1("Seek error on archive file %s", archname);
        }
        /*
        * Hunt for the named file in each different type of
        * archive format.
        */
        switch (ar_type)
        {
        case ARpdp:
                for (;;)
                {
                        if (fread((char *)&ar_pdp,
                                sizeof(ar_pdp), 1, arfd) != 1)
                        {
                                if (feof(arfd))
                                        return (0L);
                                break;
                        }
                        if (equaln(ar_pdp.ar_name, name, ARNLEN))       /* DAG -- bug fix (all must match) */
                                return (ar_pdp.ar_date);
                        ptr = ar_pdp.ar_size;
                        ptr += (ptr & 01);
                        if (fseek(arfd, ptr, 1) != 0)
                                goto seek_error;
                }
                break;
        case AR5:
                for (;;)
                {
                register char   *cp;

                        if (fread((char *)&arf_5, sizeof(arf_5), 1, arfd) != 1)
                        {
                                if (feof(arfd))
                                        return (0L);
                                break;
                        }
                        cp = &arf_5.arf_name[ARNLEN];   /* DAG -- bug fix (was fhead.ar_name); pad with NULs */
                        *cp = 0;
                        while (cp > arf_5.arf_name && *--cp == ' ')     /* DAG -- bug fix (was fhead.arname) */
                                *cp = 0;
                        if (equal(arf_5.arf_name, name))        /* DAG -- bug fix (all must match ) */
                                return (sgetl(arf_5.arf_date));
                        ptr = sgetl(arf_5.arf_size);
                        ptr += (ptr & 01);
                        if (fseek(arfd, ptr, 1) != 0)
                                goto seek_error;
                }
                break;
        case ARport:
                for (;;)
                {
                register char   *cp;

                        if (fread((char *)&ar_port, sizeof(ar_port), 1,
                                arfd) != 1 || !equaln(ar_port.ar_fmag,
                                ARFportMAG, sizeof(ar_port.ar_fmag)))
                        {
                                if (feof(arfd))
                                        return (0L);
                                break;
                        }
                        cp = &ar_port.ar_name[ARNLEN];  /* DAG -- bug fix (was fhead); pad with NULs */
                        *cp = 0;
                        while (cp > ar_port.ar_name && (*--cp == ' ' || *cp == '/'))    /* DAG -- bug fix (was fhead) */
                                *cp = 0;
                        if (equal(ar_port.ar_name, name))       /* DAG -- bug fix (all must match) */
                        {
                                long date;

                                if (sscanf(ar_port.ar_date, "%ld", &date) != 1)
                                {
                                        fatal1("Bad date field for %.14s in %s",
                                                name, archname);
                                }
                                return (date);
                        }
                        if (sscanf(ar_port.ar_size, "%ld", &ptr) != 1)
                        {
                                fatal1("Bad size field for %.14s in archive %s",
                                        name, archname);
                        }
                        ptr += (ptr & 01);
                        if (fseek(arfd, ptr, 1) != 0)
                                goto seek_error;
                }
                break;
        }
        /*
         * Only here if fread() [or equaln()] failed and not at EOF
         */
        fatal1("Read error on archive %s", archname);
        /*NOTREACHED*/
}


time_t
entryscan(name)         /* return date of member containing global var named */
char *name;
{
        /*
        * Hunt through each different archive format for the named
        * symbol.  Note that the old archive format does not support
        * this convention since there is no symbol directory to
        * scan through for all defined global variables.
        */
        if (ar_type == ARpdp)
                return (pdpentrys(name));
        if (sym_begin == 0L || num_symbols == 0L)
        {
        no_such_sym:;
                fatal1("Cannot find symbol %s in archive %s", name, archname);
        }
        if (fseek(arfd, sym_begin, 0) != 0)
        {
        seek_error:;
                fatal1("Seek error on archive file %s", archname);
        }
        if (ar_type == AR5)
        {
                register int i;
                int len = strlen(name) + 1;     /* DAG -- bug fix (added 1) */

                if (len > 8)
                        len = 8;
                for (i = 0; i < num_symbols; i++)
                {
                        if (fread((char *)&ars_5, sizeof(ars_5), 1, arfd) != 1)
                        {
                        read_error:;
                                fatal1("Read error on archive %s", archname);
                        }
                        if (equaln(ars_5.sym_name, name, len))
                        {
                                if (fseek(arfd, sgetl(ars_5.sym_ptr), 0) != 0)
                                        goto seek_error;
                                if (fread((char *)&arf_5,
                                        sizeof(arf_5), 1, arfd) != 1)
                                {
                                        goto read_error;
                                }
                                return (sgetl(arf_5.arf_date));
                        }
                }
        }
        else    /* ar_type == ARport */
        {
                int strtablen;
#if BSD
                register struct ranlib *offs;
#else
                register char *offs;    /* offsets table */
#endif
                register char *syms;    /* string table */
#if BSD
                register struct ranlib *offend; /* end of offsets table */
#else
                register char *strend;  /* end of string table */
#endif
                char *strbeg;

                /*
                * Format of the symbol directory for this format is
#if BSD
                * as follows:   [sputl()d number_of_symbols * sizeof(struct ranlib)]
#else
                * as follows:   [sputl()d number_of_symbols]
#endif
                *               [sputl()d first_symbol_offset]
#if BSD
                *               [sputl()d first_string_table_offset]
#endif
                *                       ...
                *               [sputl()d number_of_symbols'_offset]
#if BSD
                *               [sputl()d last_string_table_offset]
#endif
                *               [null_terminated_string_table_of_symbols]
                */
#if BSD
                /* have already read the num_symbols info */
                if ((offs = (struct ranlib *)malloc((unsigned) num_symbols * sizeof(struct ranlib))) == 0)
#else
                if ((offs = malloc((unsigned) num_symbols * sizeof(long))) == 0)
#endif
                {
                        fatal1("Cannot alloc offset table for archive %s",
                                archname);
                }
#if BSD
                if (fread(offs, sizeof(struct ranlib), (int) num_symbols, arfd)
                                                                != num_symbols)
#else
                if (fread(offs, sizeof(long), (int) num_symbols, arfd) != num_symbols)
#endif
                        goto read_error;
#if BSD
                strtablen = sym_size -
                        (sizeof(long) + num_symbols * sizeof(struct ranlib));
#else
                strtablen = sym_size - ((num_symbols + 1) * sizeof(long));
#endif
                if ((syms = malloc((unsigned) strtablen)) == 0)
                {
                        fatal1("Cannot alloc string table for archive %s",
                                archname);
                }
                if (fread(syms, sizeof(char), strtablen, arfd) != strtablen)
                        goto read_error;
#if BSD
                offend = &offs[num_symbols];
#else
                strend = &syms[strtablen];
#endif
                strbeg = syms;
#if BSD
                while (offs < offend )
#else
                while (syms < strend)
#endif
                {
#if BSD
                        if (strcmp(&syms[offs->ran_un.ran_strx], name) == 0)
#else
                        if (equal(syms, name))
#endif
                        {
                                long ptr, date;

#if BSD
                                ptr = offs->ran_off;
#else
                                ptr = sgetl(offs);
#endif
                                if (fseek(arfd, ptr, 0) != 0)
                                        goto seek_error;
                                if (fread((char *)&ar_port, sizeof(ar_port), 1,
                                        arfd) != 1 || !equaln(ar_port.ar_fmag,
                                        ARFportMAG, sizeof(ar_port.ar_fmag)))
                                {
                                        goto read_error;
                                }
                                if (sscanf(ar_port.ar_date, "%ld", &date) != 1)
                                {
                                        fatal1("Bad date for %.14s, archive %s",
                                                ar_port.ar_name, archname);
                                }
                                free(strbeg);
                                return (date);
                        }
#if BSD
                        offs += sizeof(struct ranlib);
#else
                        syms += strlen(syms) + 1;
                        offs += sizeof(long);
#endif
                }
                free(strbeg);
        }
        goto no_such_sym;
}


/*ARGSUSED*/
time_t
pdpentrys(name)
        char *name;
{
        fatal("Cannot do global variable search in pdp11 or old object file.");
#ifdef lint
	return ((time_t) 0);
#endif
}


openarch(f)
register char *f;
{
        unsigned short  mag_pdp;                /* old archive format */
        char            mag_5[SAR5MAG];         /* 5.0 archive format */
        char            mag_port[SARportMAG];   /* port (6.0) archive format*/

        arfd = fopen(f, "r");
        if (arfd == NULL)
                return(-1);
        /*
         * More code for three archive formats.  Read in just enough to
         * distinguish the three types and set ar_type.  Then if it is
         * one of the newer archive formats, gather more info.
         */
        if (fread((char *)&mag_pdp, sizeof(mag_pdp), 1, arfd) != 1)
                return (-1);
        if (mag_pdp == (unsigned short)ARpdpMAG)
        {
                ar_type = ARpdp;
                first_ar_mem = ftell(arfd);
                sym_begin = num_symbols = sym_size = 0L;
                return (0);
        }
        if (fseek(arfd, 0L, 0) != 0 || fread(mag_5, SAR5MAG, 1, arfd) != 1)
                return (-1);
        if (equaln(mag_5, AR5MAG, SAR5MAG))
        {
                ar_type = AR5;
                /*
                * Must read in header to set necessary info
                */
                if (fseek(arfd, 0L, 0) != 0 ||
                        fread((char *)&arh_5, sizeof(arh_5), 1, arfd) != 1)
                {
                        return (-1);
                }
                sym_begin = ftell(arfd);
                num_symbols = sgetl(arh_5.ar_syms);
                first_ar_mem = sym_begin + sizeof(ars_5) * num_symbols;
                sym_size = 0L;
                return (0);
        }
        if (fseek(arfd, 0L, 0) != 0 ||
                fread(mag_port, SARportMAG, 1, arfd) != 1)
        {
                return (-1);
        }
        if (equaln(mag_port, ARportMAG, SARportMAG))
        {
                ar_type = ARport;
                /*
                * Must read in first member header to find out
                * if there is a symbol directory
                */
                if (fread((char *)&ar_port, sizeof(ar_port), 1, arfd) != 1 ||
                        !equaln(ARFportMAG, ar_port.ar_fmag,
                        sizeof(ar_port.ar_fmag)))
                {
                        return (-1);
                }
#if BSD
                if (equaln(ar_port.ar_name,"__.SYMDEF       ",16))
#else
                if (ar_port.ar_name[0] == '/')
#endif
                {
                        char s[4];

                        if (sscanf(ar_port.ar_size, "%ld", &sym_size) != 1)
                                return (-1);
                        sym_size += (sym_size & 01);    /* round up */
                        if (fread(s, sizeof(s), 1, arfd) != 1)
                                return (-1);
#if BSD
                        num_symbols = sgetl(s) / sizeof(struct ranlib);
#else
                        num_symbols = sgetl(s);
#endif
                        sym_begin = ftell(arfd);
                        first_ar_mem = sym_begin + sym_size - sizeof(s);
                }
                else    /* there is no symbol directory */
                {
                        sym_size = num_symbols = sym_begin = 0L;
                        first_ar_mem = ftell(arfd) - sizeof(ar_port);
                }
                return (0);
        }
        fatal1("%s is not an archive", f);
        /*NOTREACHED*/
}


clarch()
{
        if (arfd != NULL)
                (void) fclose(arfd);
}
#endif /* COFF */
