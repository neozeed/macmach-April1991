# /usr/src/games/snake/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lm -ltermcap
SRCS1=	snake.c move.c
OBJS1=	snake.o move.o
SRCS2=	snscore.c
OBJS2=	snscore.o

all:	snake snscore .depend

snake:	${OBJS1} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS1} ${LIBS}

snscore: ${OBJS2} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS2}

install: FRC
	install -c -s -o games -g bin -m 4744 snake ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f snake; ln -s dm snake; chown games.bin snake)
	install -c -s -o games -g bin -m 744 snscore ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f snscore; ln -s dm snscore; chown games.bin snscore)
	-[ -s ${DESTDIR}/usr/games/lib/snakerawscores ] || install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/snakerawscores

clean:	FRC
	rm -f .depend ${OBJS1} ${OBJS2} core snake snscore busy

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS1} ${SRCS2}

-include .depend

FRC:
