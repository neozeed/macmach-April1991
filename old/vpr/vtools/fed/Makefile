# /usr/src/old/vpr/vtools/fed/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	fed.c subr.c io.c
OBJS=	fed.o subr.o io.o
LIBS=	-lm -l2648

all:	fed .depend

fed:	${OBJS} ${LIBC} /usr/lib/lib2648.a
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

install:	FRC
	install -c -s -o bin -g bin -m 755 fed ${DESTDIR}/usr/old/fed

clean:	FRC
	rm -f .depend ${OBJS} core fed

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
