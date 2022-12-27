# /usr/src/games/atc/Makefile for MacMach

CFLAGS=	-O -DBSD -DDEST=\"${DESTDIR}/usr/games/lib/atc/\"
YFLAGS=	-d
LIBC=	/lib/libc.a
LIBS=	-ll -lm -lcurses -ltermcap
SRCS=	extern.c grammar.c graphics.c input.c lex.c list.c log.c \
	main.c tunable.c update.c
OBJS=	extern.o grammar.o input.o lex.o list.o log.o main.o tunable.o \
	graphics.o update.o

all:	atc .depend

atc:	${OBJS} ${LIBC}
	${CC} ${CFLAGS} ${OBJS} ${LIBS} -o $@

install: FRC
	-[ -d ${DESTDIR}/usr/games/lib/atc ] || mkdir ${DESTDIR}/usr/games/lib/atc
	chmod 755 ${DESTDIR}/usr/games/lib/atc
	chown games.bin ${DESTDIR}/usr/games/lib/atc
	(cd games; install -c -o games -g bin -m 644 * ${DESTDIR}/usr/games/lib/atc)
	install -c -s -o games -g bin -m 4744 atc ${DESTDIR}/usr/games/hide/atc
	(cd ${DESTDIR}/usr/games; rm -f atc; ln -s dm atc; chown games.bin atc)

clean:	FRC
	rm -f .depend ${OBJS} core atc grammar.c y.tab.h y.tab.c lex.yy.c lex.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} lex.c grammar.c ${SRCS}

-include .depend

FRC:
