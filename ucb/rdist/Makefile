# /usr/src/ucb/rdist/Makefile for MacMach

LIBC=	/lib/libc.a
RDIST=	/usr/ucb/rdist
CFLAGS=	-O -DRDIST=\"${RDIST}\"
SRCS=	docmd.c expand.c gram.y lookup.c main.c server.c
OBJS=	docmd.o expand.o gram.o lookup.o main.o server.o

all:	rdist .depend

rdist: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o root -g bin -m 4755 rdist ${DESTDIR}${RDIST}

clean:	FRC
	rm -f .depend ${OBJS} core rdist

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
