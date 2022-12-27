# /usr/src/ucb/vgrind/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
RSRCS=	retest.c regexp.c
ROBJS=	retest.o regexp.o
VSRCS=	regexp.c vfontedpr.c vgrindefs.c
VOBJS=	regexp.o vfontedpr.o vgrindefs.o

all:	retest vfontedpr .depend

vfontedpr: ${VOBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${VOBJS}

retest: ${ROBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${ROBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 vfontedpr ${DESTDIR}/usr/lib/vfontedpr
	install -c -o bin -g bin -m 755 vgrind.sh ${DESTDIR}/usr/ucb/vgrind
	install -c -o bin -g bin -m 755 vgrindefs.src ${DESTDIR}/usr/lib/vgrindefs
	install -c -o bin -g bin -m 755 tmac.vgrind ${DESTDIR}/usr/lib/tmac/tmac.vgrind

clean:	FRC
	rm -f .depend ${OBJS} ${ROBJS} ${VOBJS} core retest vfontedpr

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${RSRCS} ${VSRCS}

-include .depend

FRC:
