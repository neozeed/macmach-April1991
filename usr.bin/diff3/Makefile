# /usr/src/usr.bin/diff3/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	diff3.c
OBJS=	diff3.o

all:	diff3 .depend

diff3: diff3.c ${LIBC}
	${CC} ${CFLAGS} -o $@ diff3.c

install: FRC
	install -c -s -o bin -g bin -m 755 diff3 $(DESTDIR)/usr/lib/diff3
	install -c -o bin -g bin -m 755 diff3.sh $(DESTDIR)/usr/bin/diff3

clean:	FRC
	rm -f .depend ${OBJS} core diff3

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
