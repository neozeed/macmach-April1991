# /usr/src/ucb/systat/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
LIBS=	-lcurses -ltermlib -lm
SRCS=	cmds.c cmdtab.c disks.c fetch.c iostat.c keyboard.c vmstat.c main.c \
	mbufs.c netcmds.c netstat.c pigs.c swap.c
OBJS=	cmds.o cmdtab.o disks.o fetch.o iostat.o keyboard.o vmstat.o main.o \
	mbufs.o netcmds.o netstat.o pigs.o swap.o

all:	FRC
	@echo "*** need systat for mac2 ***"

#all:	systat .depend

systat:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS} ${LIBS}

clean:	FRC
	rm -f .depend ${OBJS} core systat

install: FRC
	@echo "*** need systat for mac2 ***"
#	install -c -s -o bin -g kmem -m 2755 systat ${DESTDIR}/usr/ucb/systat

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
