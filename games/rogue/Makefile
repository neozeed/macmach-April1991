# /usr/src/games/rogue/Makefile for MacMach

CFLAGS=	-O -DUNIX -DUNIX_BSD4_2
LIBC=	/lib/libc.a
SRCS=	curses.c hit.c init.c inventory.c level.c machdep.c main.c \
	message.c monster.c move.c object.c pack.c play.c random.c ring.c \
	room.c save.c score.c spec_hit.c throw.c trap.c use.c zap.c
OBJS=	curses.o hit.o init.o inventory.o level.o machdep.o main.o \
	message.o monster.o move.o object.o pack.o play.o random.o ring.o \
	room.o save.o score.o spec_hit.o throw.o trap.o use.o zap.o

all:	rogue .depend

rogue:	${OBJS} ${LIBC}
	${CC} ${OBJS} -o $@ -lcurses -ltermlib

clean:	FRC
	rm -f .depend ${OBJS} core rogue

install: FRC
	install -c -s -o games -g bin -m 4744 rogue ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f rogue; ln -s dm rogue; chown games.bin rogue)

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
