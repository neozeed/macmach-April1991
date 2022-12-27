# /usr/src/ucb/netstat/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	host.c inet.c if.c main.c mbuf.c route.c unix.c ns.c
OBJS=	host.o inet.o if.o main.o mbuf.o route.o unix.o ns.o

all:	netstat .depend

netstat: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g kmem -m 2755 netstat ${DESTDIR}/usr/ucb/netstat

clean:	FRC
	rm -f .depend ${OBJS} core netstat

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
