# /usr/src/etc/restore/Makefile for MacMach

VPATH=	../dump
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	main.c interactive.c restore.c dirs.c symtab.c tape.c utilities.c \
	../dump/dumprmt.c
OBJS=	main.o interactive.o restore.o dirs.o symtab.o utilities.o tape.o
ROBJS=	main.o interactive.o restore.o dirs.o symtab.o utilities.o rtape.o \
	dumprmt.o

all:	restore rrestore .depend

restore: ${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

rrestore: ${ROBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${ROBJS}

rtape.o: ${LIBC} tape.c
	${CC} ${CFLAGS} -c -DRRESTORE tape.c -o rtape.o

install: FRC
	rm -f ${DESTDIR}/etc/restore ${DESTDIR}/usr/etc/restore
	rm -f ${DESTDIR}/etc/rrestore ${DESTDIR}/usr/etc/rrestore
	install -c -s -o bin -g bin -m 755 restore ${DESTDIR}/usr/etc/restore
	install -c -s -o bin -g bin -m 755 rrestore ${DESTDIR}/usr/etc/rrestore
	ln -s ../usr/etc/restore ${DESTDIR}/etc/restore
	ln -s ../usr/etc/rrestore ${DESTDIR}/etc/rrestore

clean:	FRC
	rm -f .depend ${ROBJS} ${OBJS} core restore rrestore

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
