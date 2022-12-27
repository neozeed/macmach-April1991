# /usr/src/games/btlgammon/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	btlgammon.c
OBJS=

all:	btlgammon .depend

btlgammon: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${SRCS}

install: FRC
	install -c -s -o games -g bin -m 4744 btlgammon ${DESTDIR}/usr/games/hide
	install -c -o games -g bin -m 444 backrules ${DESTDIR}/usr/games/lib
	(cd ${DESTDIR}/usr/games; rm -f btlgammon; ln -s dm btlgammon; chown games.bin btlgammon)

clean:	FRC
	rm -f .depend ${OBJS} core btlgammon

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
