# /usr/src/games/backgammon/Makefile for MacMach

LIBC=	/lib/libc.a
LIBS=	-ltermlib
CFLAGS=	-O -DV7
SRCS=	allow.c board.c check.c data.c extra.c fancy.c init.c main.c move.c \
	odds.c one.c save.c subs.c table.c teach.c text.c ttext1.c ttext2.c \
	tutor.c version.c
BSRCS=	allow.c board.c check.c extra.c fancy.c init.c main.c move.c \
	odds.c one.c save.c subs.c table.c text.c version.c
BOBJS=	allow.o board.o check.o extra.o fancy.o init.o main.o move.o \
	odds.o one.o save.o subs.o table.o text.o version.o
TSRCS=	allow.c board.c check.c data.c fancy.c init.c odds.c one.c save.c \
	subs.c table.c teach.c ttext1.c ttext2.c tutor.c
TOBJS=	allow.o board.o check.o data.o fancy.o init.o odds.o one.o save.o \
	subs.o table.o teach.o ttext1.o ttext2.o tutor.o

all:	backgammon teachgammon .depend

# Backgammon program
backgammon: ${BOBJS} ${LIBC}
	${CC} -o $@ ${BOBJS} ${LIBS}

# Backgammon rules and tutorial
teachgammon: ${TOBJS} ${LIBC}
	${CC} -o $@ ${TOBJS} ${LIBS}

install: FRC
	install -c -s -o games -g bin -m 744 backgammon ${DESTDIR}/usr/games/hide
	install -c -s -o games -g bin -m 744 teachgammon ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f backgammon; ln -s dm backgammon; chown games.bin backgammon)
	(cd ${DESTDIR}/usr/games; rm -f teachgammon; ln -s dm teachgammon; chown games.bin teachgammon)

clean:	FRC
	rm -f .depend ${TOBJS} ${BOBJS} core teachgammon backgammon

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
