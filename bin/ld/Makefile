# /usr/src/bin/ld/Makefile for MacMach

CFLAGS=	-O
#SRCS=	cplus-dem.c getopt.c getopt1.c ld.c
#OBJS=	cplus-dem.o getopt.o getopt1.o ld.o
SRCS=	ld.c
OBJS=	ld.o

all:	ld .depend

ld:	${OBJS}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 ld ${DESTDIR}/bin/ld

clean:	FRC
	rm -f .depend ${OBJS} core ld

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
