# /usr/src/ucb/msgs/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	msgs.c
OBJS=	msgs.o

all:	msgs .depend

msgs:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -ltermlib

install: FRC
	install -c -s -o bin -g bin -m 755 msgs ${DESTDIR}/usr/ucb/msgs

clean:	FRC
	rm -f .depend ${OBJS} core msgs

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
