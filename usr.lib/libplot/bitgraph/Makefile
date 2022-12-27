# /usr/src/usr.lib/libplot/bitgraph/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	box.c circle.c arc.c cont.c dot.c erase.c line.c linemod.c point.c \
	label.c move.c open.c close.c space.c
OBJS=	box.o circle.o arc.o cont.o dot.o erase.o line.o linemod.o point.o \
	label.o move.o open.o close.o space.o

all:	../libplotbg .depend

../libplotbg: ${OBJS}
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
