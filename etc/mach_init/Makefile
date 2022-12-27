# /usr/src/etc/mach_init/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	main.c service.c serviceServer.c
OBJS=	main.o service.o serviceServer.o

all:	mach_init .depend

mach_init: ${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} -lmach

serviceServer.c: service.defs
	mig $? -header service.h -server $@ -user /dev/null

install: FRC
	install -c -s -o bin -g bin -m 755 mach_init ${DESTDIR}/etc/mach_init

clean:	FRC
	rm -f .depend ${OBJS} mach_init service.h serviceServer.c core

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
