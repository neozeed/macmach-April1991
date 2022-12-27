# /usr/src/ucb/compress/Makefile for MacMach

CFLAGS=	-O -DBSD4_2 -DSACREDMEM=256000 -DUSERMEM=`cat USERMEM`
LIBC=	/lib/libc.a
SRCS=	compress.c
OBJS=	compress.o

all:	compress .depend

compress: USERMEM ${OBJS} ${LIBC}
	${CC} ${CFLAGS} -o $@ ${OBJS}

# USERMEM may have to be set by hand.  It should contain the amount of
# available user memory in bytes.  Set it to zero, for physical memory
# less than 1 Meg.
USERMEM:
	sh usermem.sh > USERMEM

install: FRC
	install -c -s -o bin -g bin -m 755 compress ${DESTDIR}/usr/ucb/compress
	rm -f ${DESTDIR}/usr/ucb/uncompress ${DESTDIR}/usr/ucb/zcat
	ln -s compress ${DESTDIR}/usr/ucb/uncompress
	ln -s compress ${DESTDIR}/usr/ucb/zcat

clean:	FRC
	rm -f .depend ${OBJS} core compress USERMEM

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
