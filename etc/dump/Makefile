# /usr/src/etc/dump/Makefile for MacMach

#	dump.h		header file
#	dumpitime.c	reads /etc/dumpdates
#	dumpmain.c	driver
#	dumpoptr.c	operator interface
#	dumptape.c	handles the mag tape and opening/closing
#	dumptraverse.c	traverses the file system
#	unctime.c	undo ctime
#
#	DEBUG		use local directory to find ddate and dumpdates
#	TDEBUG		trace out the process forking

DFLAGS=
CFLAGS=	-O ${DFLAGS}
LIBC=	/lib/libc.a
PSRCS=	/usr/include/protocols/dumprestore.h dump.h dumpmain.c \
	dumptraverse.c dumptape.c dumpoptr.c dumpitime.c unctime.c
SRCS=	dumpitime.c dumpmain.c dumpoptr.c dumprmt.c dumptape.c \
	dumptraverse.c unctime.c
OBJS=	dumpitime.o dumpmain.o dumpoptr.o dumptape.o dumptraverse.o \
	unctime.o
ROBJS=	dumpitime.o dumprmain.o dumpoptr.o dumprmt.o dumprtape.o \
	dumptraverse.o unctime.o

all:	dump rdump .depend

dump:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

rdump:	${ROBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${ROBJS}

dumprmain.o: dumpmain.c
	${CC} -c -DRDUMP ${CFLAGS} dumpmain.c -o dumprmain.o

dumprtape.o: dumptape.c
	${CC} -c -DRDUMP ${CFLAGS} dumptape.c -o dumprtape.o

install: FRC
	rm -f ${DESTDIR}/etc/rdump ${DESTDIR}/usr/etc/rdump
	rm -f ${DESTDIR}/etc/dump ${DESTDIR}/usr/etc/dump
	install -c -s -o bin -g bin -m 755 rdump ${DESTDIR}/usr/etc/rdump
	install -c -s -o bin -g bin -m 755 dump ${DESTDIR}/usr/etc/dump
	ln -s ../usr/etc/rdump ${DESTDIR}/etc/rdump
	ln -s ../usr/etc/dump ${DESTDIR}/etc/dump

clean:	FRC
	rm -f .depend ${ROBJS} ${OBJS} rdump dump core

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
