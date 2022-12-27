# /usr/src/usr.bin/roff/troff_font/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	mkfont.c
OBJS=	mkfont.o

all:	mkfont .depend

mkfont: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC

clean:	FRC
	rm -f .depend ${OBJS} core mkfont

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
