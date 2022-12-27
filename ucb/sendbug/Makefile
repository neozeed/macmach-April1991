# /usr/src/ucb/sendbug/Makefile for MacMach

CFLAGS=	-O -I.
LIBC=	/lib/libc.a
SRCS=	bugfiler.c error.c gethead.c process.c redist.c reply.c
OBJS=	bugfiler.o error.o gethead.o process.o redist.o reply.o

all:	bugfiler .depend

bugfiler: ${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

install: FRC
	install -c -s -o root -g bin -m 4755 bugfiler ${DESTDIR}/usr/lib/bugfiler
	install -c -o bin -g bin -m 755 sendbug.sh ${DESTDIR}/usr/ucb/sendbug
	install -c -o bin -g bin -m 644 bugformat ${DESTDIR}/usr/lib/bugformat

clean:	FRC
	rm -f .depend ${OBJS} core bugfiler

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
