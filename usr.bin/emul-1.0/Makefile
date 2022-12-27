# /usr/src/usr.bin/emul/Makefile for MacMach

DESTDIR=

SYS=../../../sys

SUBDIR=	server patches

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make SYS=${SYS} ${MFLAGS}

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	install -c -o bin -g bin -m 555 emul.csh ${DESTDIR}/usr/bin/emul

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
