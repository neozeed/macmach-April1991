# /usr/src/usr.bin/find/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	find.c bigram.c code.c
OBJS=	find.o bigram.o code.o

all:	find bigram code .depend

find:	find.c ${LIBC}
	${CC} ${CFLAGS} -o $@ find.c

bigram: bigram.c
	${CC} ${CFLAGS} -o $@ bigram.c

code:	code.c
	${CC} ${CFLAGS} -o $@ code.c

install: FRC
	install -c -s -o bin -g bin -m 755 find ${DESTDIR}/usr/bin/find
	-[ -d ${DESTDIR}/usr/lib/find ] || mkdir ${DESTDIR}/usr/lib/find
	install -c -s -o bin -g bin -m 755 bigram ${DESTDIR}/usr/lib/find/bigram
	install -c -s -o bin -g bin -m 755 code ${DESTDIR}/usr/lib/find/code
	install -c -o bin -g bin -m 755 updatedb.csh ${DESTDIR}/usr/lib/find/updatedb

clean:	FRC
	rm -f .depend ${OBJS} core find bigram code

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
