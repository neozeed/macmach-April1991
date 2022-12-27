# /usr/src/lib/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
TAGSFILE=tags

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR=	libc gcc g++ libg++

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS} PWD=`pwd`

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done

tags:	FRC
	cd libc; make ${MFLAGS} TAGSFILE=../${TAGSFILE} tags

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
