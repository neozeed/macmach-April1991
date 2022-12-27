# /usr/src/etc/fsck/Makefile for MacMach

# Note that ufs_subr.c and ufs_tables.c are in /usr/src/sys/bsd.

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	dir.c inode.c main.c pass1.c pass1b.c pass2.c pass3.c \
	pass4.c pass5.c setup.c utilities.c /usr/src/sys/bsd/ufs_subr.c \
	/usr/src/sys/bsd/ufs_tables.c

LOCOBJS=dir.o inode.o main.o pass1.o pass1b.o pass2.o \
	pass3.o pass4.o pass5.o setup.o utilities.o
SYSOBJS=ufs_subr.o ufs_tables.o
OBJS=	${LOCOBJS} ${SYSOBJS}

all:	fsck .depend

fsck:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

ufs_subr.o:
	${CC} ${CFLAGS} -c /usr/src/sys/bsd/ufs_subr.c

ufs_tables.o:
	${CC} ${CFLAGS} -I/usr/include/sys -c /usr/src/sys/bsd/ufs_tables.c

install: FRC
	install -c -s -o bin -g bin -m 755 fsck ${DESTDIR}/etc/fsck

clean:	FRC
	rm -f .depend ${OBJS} fsck core

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
