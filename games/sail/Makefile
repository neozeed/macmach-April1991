# /usr/src/games/sail/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
SRCS=	main.c pl_main.c pl_1.c pl_2.c pl_3.c pl_4.c pl_5.c pl_6.c pl_7.c \
	dr_main.c dr_1.c dr_2.c dr_3.c dr_4.c dr_5.c lo_main.c \
	assorted.c game.c globals.c misc.c parties.c sync.c version.c
OBJS=	main.o pl_main.o pl_1.o pl_2.o pl_3.o pl_4.o pl_5.o pl_6.o pl_7.o \
	dr_main.o dr_1.o dr_2.o dr_3.o dr_4.o dr_5.o lo_main.o \
	assorted.o game.o globals.o misc.o parties.o sync.o version.o

all:	sail .depend

sail:	${OBJS} ${LIBC}
	${CC} -o $@ ${OBJS} ${LIBS}

install: FRC
	install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/saillog
	install -c -s -o games -g bin -m 4744 sail ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f sail; ln -s dm sail; chown games.bin sail)

clean:	FRC
	rm -f .depend ${OBJS} core sail

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
