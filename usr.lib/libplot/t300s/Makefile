# /usr/src/usr.lib/libplot/t300s/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	arc.c box.c circle.c close.c dot.c erase.c label.c \
	line.c linmod.c move.c open.c point.c space.c subr.c
OBJS=	arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linmod.o move.o open.o point.o space.o subr.o

all:	../lib300s .depend

../lib300s: ${OBJS}
	ar cu $@ ${OBJS}

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
