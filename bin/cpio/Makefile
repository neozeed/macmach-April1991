# /usr/src/bin/cpio/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	cpio.c
OBJS=

all:	cpio .depend

cpio:	${LIBC}
	${CC} -o $@ ${CFLAGS} cpio.c

install: FRC
	install -c -s -o bin -g bin -m 755 cpio ${DESTDIR}/bin/cpio

clean:	FRC
	rm -f .depend ${OBJS} core cpio

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
