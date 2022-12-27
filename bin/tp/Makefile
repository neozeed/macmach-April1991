# /usr/src/bin/tp/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	tp0.c tp1.c tp2.c tp3.c
OBJS=	tp0.o tp1.o tp2.o tp3.o

all:	tp .depend

tp:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 tp ${DESTDIR}/bin/tp

clean:	FRC
	rm -f .depend ${OBJS} core tp

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
