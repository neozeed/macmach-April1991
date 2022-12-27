# /usr/src/usr.lib/liby/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	libmai.c libzer.c
OBJS=	libmai.o libzer.o

all:	liby.a .depend

liby.a: ${OBJS}
	ar cr $@ ${OBJS}

install: FRC
	install -c -o bin -g bin -m 644 liby.a ${DESTDIR}/usr/lib/liby.a
	ranlib ${DESTDIR}/usr/lib/liby.a

clean:	FRC
	rm -f .depend ${OBJS} core liby.a

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
