# /usr/src/usr.lib/libplot/t4014/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	arc.c box.c circle.c close.c dot.c erase.c label.c \
	line.c linemod.c move.c open.c point.c space.c subr.c
OBJS=	arc.o box.o circle.o close.o dot.o erase.o label.o \
	line.o linemod.o move.o open.o point.o space.o subr.o

all:	../lib4014 .depend

../lib4014: ${OBJS}
	ar cu $@ `lorder ${OBJS} | tsort`

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
