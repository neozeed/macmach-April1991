# /usr/src/usr.lib/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a
TAGSFILE=tags

# Subdirectories whose routines are included in the making of the
# master tags file (/usr/lib/tags); the Fortran libraries should
# be on this list, but we don't control them....
#
TAGSDIR=libcurses libdbm libln libm libmp libtermlib

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	man learn lib2648 libcurses libdbm libg libln libm libmach libmp \
	libplot libtermlib libthreads liby lpr me migcom sendmail

# C source that lives in the current directory
SRCS=	getNAME.c makekey.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	getNAME makekey

all:	${SUBDIR} ${STD} .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

${STD}:	${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${STD}; do \
	  (install -c -s -o bin -g bin -m 755 $$i ${DESTDIR}/usr/lib/$$i); \
	done
	install -c -o bin -g bin -m 755 lib.b ${DESTDIR}/usr/lib/lib.b

clean:	FRC
	rm -f .depend a.out core *.s *.o tags ${STD} ${NSTD}
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

tags:	FRC
	-for i in ${TAGSDIR}; do \
	  (cd $$i; make ${MFLAGS} TAGSFILE=../${TAGSFILE} tags); \
	done

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
