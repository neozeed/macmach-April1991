# /usr/src/etc/timed/Makefile for MacMach

# optional flags are: MEASURE TESTING DEBUG
#
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCDS=	acksend.c candidate.c correct.c master.c networkdelta.c readmsg.c \
	slave.c timed.c
OBJDS=	acksend.o candidate.o correct.o master.o networkdelta.o readmsg.o \
	slave.o timed.o
SRCCS=	cmds.c cmdtab.c timedc.c
OBJCS=	cmds.o cmdtab.o timedc.o
COMMS=	byteorder.c measure.c cksum.${MACHINE}.c ocsum.s
COMMO=	byteorder.o measure.o cksum.${MACHINE}.o ocsum.o

all:	timed timedc .depend

timed: 	${OBJDS} ${COMMO} ${LIBC}
	${CC} ${CFLAGS} ${OBJDS} ${COMMO} ${LIBS} -o $@

timedc: ${OBJCS} ${COMMO} ${LIBC}
	${CC} ${CFLAGS} ${OBJCS} ${COMMO} ${LIBS} -o $@

cksum.${MACHINE}.o:
	${CC} ${CFLAGS} -c $*.c

ocsum.o: ocsum.s
	${AS} ocsum.s -o ocsum.o

install: FRC
	rm -f ${DESTDIR}/etc/timed ${DESTDIR}/usr/etc/timed
	rm -f ${DESTDIR}/etc/timedc ${DESTDIR}/usr/etc/timedc
	install -c -s -o bin -g bin -m 755 timed ${DESTDIR}/usr/etc/timed
	install -c -s -o bin -g bin -m 755 timedc ${DESTDIR}/usr/etc/timedc
	ln -s ../usr/etc/timed ${DESTDIR}/etc/timed
	ln -s ../usr/etc/timedc ${DESTDIR}/etc/timedc

clean:	FRC
	rm -f .depend ${OBJDS} ${OBJCS} ${COMMO} core timed timedc

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCDS} ${SRCCS} ${COMMS}

-include .depend

FRC:
