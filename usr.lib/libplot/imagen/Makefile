# /usr/src/usr.lib/libplot/imagen/Makefile for MacMach

# DEFRES == default printer resolution dpi
CFLAGS=	-O -DDEFRES=240
LIBC=	/lib/libc.a
SRCS=	arc.c box.c charset.c circle.c close.c cont.c dot.c \
	erase.c label.c line.c linemod.c move.c open.c point.c \
	scale.c space.c
OBJS=	arc.o box.o charset.o circle.o close.o cont.o dot.o \
	erase.o label.o line.o linemod.o move.o open.o point.o \
	scale.o space.o

all:	../libplotimagen .depend

../libplotimagen: ${OBJS}
	ar cr $@ ${OBJS}

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
