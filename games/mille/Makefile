# /usr/src/games/mille/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib
SRCS=	comp.c end.c extern.c init.c mille.c misc.c move.c print.c \
	roll.c save.c table.c types.c varpush.c
MSRCS=	comp.c end.c extern.c init.c mille.c misc.c move.c print.c \
	roll.c save.c types.c varpush.c
MOBJS=	comp.o end.o extern.o init.o mille.o misc.o move.o print.o \
	roll.o save.o types.o varpush.o
TSRCS=	table.c extern.c
TOBJS=	table.o extern.o

all:	mille .depend

mille:	${MOBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${MOBJS} ${LIBS}

table:	${TOBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${TOBJS}

install: FRC
	install -c -s -o games -g bin -m 744 mille ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f mille; ln -s dm mille; chown games.bin mille)

clean:	FRC
	rm -f .depend ${OBJS} ${TOBJS} ${MOBJS} core mille table

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
