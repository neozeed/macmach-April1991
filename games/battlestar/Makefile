# /usr/src/games/battlestar/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
OBJS=	battlestar.o com1.o com2.o com3.o com4.o com5.o com6.o com7.o \
	init.o cypher.o getcom.o parse.o room.o save.o fly.o misc.o \
	globals.o dayfile.o nightfile.o dayobjs.o nightobjs.o words.o
SRCS=	battlestar.c com1.c com2.c com3.c com4.c com5.c com6.c com7.c \
	init.c cypher.c getcom.c parse.c room.c save.c fly.c misc.c \
	globals.c dayfile.c nightfile.c dayobjs.c nightobjs.c words.c

all:	battlestar .depend

battlestar: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 4744 battlestar ${DESTDIR}/usr/games/hide/battlestar
	install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/battlestar.log
	(cd ${DESTDIR}/usr/games; rm -f battlestar; ln -s dm battlestar; chown games.bin battlestar)

clean:	FRC
	rm -f .depend ${OBJS} core battlestar

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
