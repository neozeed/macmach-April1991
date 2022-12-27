# /usr/src/etc/newfs/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	newfs.c mkfs.c
OBJS=	newfs.o mkfs.o

all:	newfs .depend

newfs: ${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 newfs ${DESTDIR}/etc/newfs

clean:	FRC
	rm -f .depend ${OBJS} core newfs

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
