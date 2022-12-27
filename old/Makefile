# /usr/src/old/Makefile for MacMach

DESTDIR=
SUBDIR=	compact

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done

clean:	FRC
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
