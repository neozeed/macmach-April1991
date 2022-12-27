# /usr/src/games/hangman/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
SRCS=	endgame.c extern.c getguess.c getword.c main.c playgame.c \
	prdata.c prman.c prword.c setup.c
OBJS=	endgame.o extern.o getguess.o getword.o main.o playgame.o \
	prdata.o prman.o prword.o setup.o

all:	hangman .depend

hangman: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 744 hangman ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f hangman; ln -s dm hangman; chown games.bin hangman)

clean:	FRC
	rm -f .depend ${OBJS} core hangman

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
