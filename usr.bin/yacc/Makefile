# /usr/src/usr.bin/yacc/Makefile for MacMach

CFLAGS=	-O -DWORD32
LIBC=	/lib/libc.a
SRCS=	y1.c y2.c y3.c y4.c
OBJS=	y1.o y2.o y3.o y4.o

all:	yacc .depend

yacc:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	install -c -s -o bin -g bin -m 755 yacc ${DESTDIR}/usr/bin/yacc
	install -c -o bin -g bin -m 755 yaccpar ${DESTDIR}/usr/lib/yaccpar

clean:	FRC
	rm -f .depend ${OBJS} core yacc

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
