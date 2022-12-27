# /usr/src/games/monop/Makefile for MacMach

# Nifty foo monopoly maker
#	Program:  Ken Arnold
#	Makefile:  K.S.
#
CFLAGS=	-O
LIBC=	/lib/libc.a
MSRCS=	monop.c cards.c execute.c getinp.c houses.c jail.c misc.c morg.c \
	print.c prop.c rent.c roll.c spec.c trade.c
MOBJS=	monop.o cards.o execute.o getinp.o houses.o jail.o misc.o morg.o \
	print.o prop.o rent.o roll.o spec.o trade.o
ISRCS=	initdeck.c
IOBJS=	initdeck.o

all:	monop initdeck cards.pck .depend

monop:	${MOBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${MOBJS}

initdeck: ${IOBJS}
	${CC} ${CFLAGS} -o $@ ${IOBJS}

cards.pck:
	./initdeck

install: FRC
	install -c -o games -g bin -m 444 cards.pck ${DESTDIR}/usr/games/lib
	install -c -s -o games -g bin -m 744 monop ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f monop; ln -s dm monop; chown games.bin monop)

clean:	FRC
	rm -f .depend ${IOBJS} ${MOBJS} core monop initdeck cards.pck

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${MSRCS} ${ISRCS}

-include .depend

FRC:
