# /usr/src/games/quiz/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	quiz.c
OBJS=

all:	quiz .depend

quiz:	${LIBC}
	${CC} -o $@ ${CFLAGS} ${SRCS}

install: FRC
	-[ -d ${DESTDIR}/usr/games/lib/quiz.k ] || mkdir ${DESTDIR}/usr/games/lib/quiz.k
	chmod 755 ${DESTDIR}/usr/games/lib/quiz.k
	chown games.bin ${DESTDIR}/usr/games/lib/quiz.k
	cd quiz.k; install -c -o games -g bin -m 444 * ${DESTDIR}/usr/games/lib/quiz.k
	install -c -s -o games -g bin -m 744 quiz ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f quiz; ln -s dm quiz; chown games.bin quiz)

clean:	FRC
	rm -f .depend ${OBJS} core quiz

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
