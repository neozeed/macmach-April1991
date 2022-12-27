# /usr/src/ucb/tset/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	tset.c

all:	tset .depend

tset: ${SRCS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${SRCS} -ltermlib

install: FRC
	install -c -s -o bin -g bin -m 755 tset ${DESTDIR}/usr/ucb/tset
	rm -f ${DESTDIR}/usr/ucb/reset
	ln -s tset ${DESTDIR}/usr/ucb/reset

clean:	FRC
	rm -f .depend ${OBJS} core tset

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
