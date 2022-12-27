# /usr/src/old/compact/Makefile for MacMach

DESTDIR=
CFLAGS=-O
SRCS=	compact.c tree.c uncompact.c
OBJS=	compact.o tree.o uncompact.o

all:	compact uncompact .depend

compact: compact.o tree.o
	${CC} ${LDFLAGS} compact.o tree.o -o $@

uncompact: uncompact.o tree.o
	${CC} ${LDFLAGS} uncompact.o tree.o -o $@

install:	FRC
	install -c -s -o bin -g bin -m 755 compact ${DESTDIR}/usr/old/compact
	install -c -s -o bin -g bin -m 755 uncompact ${DESTDIR}/usr/old/uncompact
	install -c -o bin -g bin -m 755 ccat.sh ${DESTDIR}/usr/old/ccat

clean:	FRC
	rm -f .depend compact uncompact ${OBJS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
