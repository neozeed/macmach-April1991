# /usr/src/games/dm/Makefile for MacMach

# -DLOG	log games in /usr/adm
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	dm.c
OBJS=

all:	dm .depend

dm:	${LIBC}
	${CC} -o $@ ${CFLAGS} dm.c

install: FRC
	install -c -s -o games -g kmem -m 6755 dm ${DESTDIR}/usr/games

clean:	FRC
	rm -f .depend core dm

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
