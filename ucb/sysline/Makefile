# /usr/src/ucb/sysline/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	sysline.c
OBJS=

all:	sysline .depend

sysline: ${LIBC}
	${CC} ${CFLAGS} -o $@ sysline.c -ltermlib -lcurses

install: FRC
	install -c -s -o bin -g kmem -m 2755 sysline ${DESTDIR}/usr/ucb/sysline

clean:	FRC
	rm -f .depend core sysline

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
