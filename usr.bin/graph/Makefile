# /usr/src/usr.bin/graph/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	graph.c
OBJS=	graph.o

all:	graph .depend

graph: ${SRCS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${SRCS} -lplot -lm

install: FRC
	install -c -s -o bin -g bin -m 755 graph ${DESTDIR}/usr/bin/graph

clean:	FRC
	rm -f .depend ${OBJS} core graph

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
