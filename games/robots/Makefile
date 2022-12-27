# /usr/src/games/robots/Makefile for MacMach

DEFS=	-DMAX_PER_UID=5
CFLAGS=	-O ${DEFS}
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
SRCS=	extern.c init_field.c main.c make_level.c move.c move_robs.c \
	play_level.c query.c rnd_pos.c score.c flush_in.c
OBJS=	extern.o init_field.o main.o make_level.o move.o move_robs.o \
	play_level.o query.o rnd_pos.o score.o flush_in.o

all:	robots .depend

robots:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 4744 robots ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f robots; ln -s dm robots; chown games.bin robots)
	install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/robots_roll

clean:	FRC
	rm -f .depend ${OBJS} core robots

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
