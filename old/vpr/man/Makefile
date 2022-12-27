# /usr/src/old/vpr/man/Makefile for MacMach

SRCS=
OBJS=	fed.0 vfontinfo.0 vpr.0 vtroff.0 vwidth.0 vpac.0
LINKS=

.SUFFIXES: .8 .1 .0

.8.0:
	${DESTDIR}/usr/man/manroff $*.8 > $*.0
.1.0:
	${DESTDIR}/usr/man/manroff $*.1 > $*.0

all:	${OBJS}

clean:	FRC
	rm -f ${OBJS}

install:	FRC
	install -c -o bin -g bin -m 444 ${OBJS1} ${OBJS8} ${DESTDIR}/usr/man/cat.old

FRC:
