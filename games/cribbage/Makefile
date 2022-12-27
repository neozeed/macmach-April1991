# /usr/src/games/cribbage/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
HDRS=	cribbage.h deck.h cribcur.h
SRCS=	extern.c crib.c support.c cards.c score.c io.c
OBJS=	extern.o crib.o support.o cards.o score.o io.o
TOBJS=	test.o cards.o score.o io.o extern.o

all:	cribbage crib.instr .depend

cribbage: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

test:	${TOBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${TOBJS} ${LIBS}

crib.instr: cribbage.n macro
	nroff cribbage.n > crib.instr

install: FRC
	install -c -o games -g bin -m 444 crib.instr ${DESTDIR}/usr/games/lib/crib.instr
	install -c -s -o games -g bin -m 4744 cribbage ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f cribbage; ln -s dm cribbage; chown games.bin cribbage)

clean:	FRC
	rm -f .depend ${OBJS} core crib.instr cribbage test

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
