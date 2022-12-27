# /usr/src/ucb/man/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
ASRCS=	apropos.c
MSRCS=	man.c

all:	man apropos .depend

apropos: ${LIBC}
	${CC} -o $@ ${CFLAGS} ${ASRCS}

man: ${LIBC}
	${CC} -o $@ ${CFLAGS} ${MSRCS}

install: FRC
	rm -f ${DESTDIR}/usr/ucb/whatis ${DESTDIR}/usr/ucb/apropos
	install -c -s -o bin -g bin -m 755 apropos ${DESTDIR}/usr/ucb/apropos
	install -c -s -o bin -g bin -m 755 man ${DESTDIR}/usr/ucb/man
	ln -s apropos ${DESTDIR}/usr/ucb/whatis

clean:	FRC
	rm -f .depend core apropos man

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${ASRCS} ${MSRCS}

-include .depend

FRC:
