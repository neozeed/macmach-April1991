# /usr/src/ucb/tftp/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	main.c tftp.c tftpsubs.c
OBJS=	main.o tftp.o tftpsubs.o

all:	tftp .depend

tftp:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 tftp ${DESTDIR}/usr/ucb/tftp

clean:	FRC
	rm -f .depend ${OBJS} core tftp

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
