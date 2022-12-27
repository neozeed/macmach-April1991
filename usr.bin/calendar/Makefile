# /usr/src/usr.bin/calendar/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	calendar.c
OBJS=	calendar.o

all:	calendar .depend

calendar: calendar.c ${LIBC}
	${CC} ${CFLAGS} -o $@ calendar.c

install: FRC
	install -c -s -o bin -g bin -m 755 calendar ${DESTDIR}/usr/lib
	install -c -o bin -g bin -m 755 calendar.sh ${DESTDIR}/usr/bin/calendar

clean:	FRC
	rm -f .depend ${OBJS} core calendar

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
