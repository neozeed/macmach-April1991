# /usr/src/etc/XNSrouted/Makefile for MacMach

#	Makefile for Xerox Routing Information Protocol
#	Daemon

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	af.c if.c input.c main.c output.c startup.c tables.c timer.c \
	trace.c
OBJS=	af.o if.o input.o main.o output.o startup.o tables.o timer.o \
	trace.o

all:	XNSrouted .depend

XNSrouted: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	rm -f ${DESTDIR}/etc/XNSrouted ${DESTDIR}/usr/etc/XNSrouted
	install -c -s -o bin -g bin -m 755 XNSrouted ${DESTDIR}/usr/etc/XNSrouted
	ln -s ../usr/etc/XNSrouted ${DESTDIR}/etc/XNSrouted

clean:	FRC
	rm -f .depend ${OBJS} core XNSrouted

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
