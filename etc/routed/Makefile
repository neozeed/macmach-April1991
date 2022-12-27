# /usr/src/etc/routed/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a

SRCS=	af.c if.c input.c main.c output.c startup.c tables.c timer.c \
	trace.c inet.c
OBJS=	af.o if.o input.o main.o output.o startup.o tables.o timer.o \
	trace.o inet.o
HDRS=	af.h defs.h interface.h table.h trace.h

all:	routed .depend

routed:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

install: FRC
	rm -f ${DESTDIR}/etc/routed ${DESTDIR}/usr/etc/routed
	install -c -s -o bin -g bin -m 755 routed ${DESTDIR}/usr/etc/routed
	ln -s ../usr/etc/routed ${DESTDIR}/etc/routed

clean:	FRC
	rm -f .depend ${OBJS} core routed
	cd tools; make ${MFLAGS} clean

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
