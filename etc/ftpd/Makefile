# /usr/src/etc/ftpd/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	ftpd.c ftpcmd.c ../../ucb/ftp/glob.c vers.c
OBJS=	ftpd.o ftpcmd.o glob.o vers.o

all:	ftpd .depend

ftpd:	${OBJS} ${LIBC}
	${CC} -o $@ ${OBJS}

glob.o:	../../ucb/ftp/glob.c
	${CC} ${CFLAGS} -c ../../ucb/ftp/glob.c

#vers.o: ftpd.c ftpcmd.y
#	-sh newvers.sh
#	${CC} ${CFLAGS} -c vers.c

install: FRC
	rm -f ${DESTDIR}/etc/ftpd ${DESTDIR}/usr/etc/ftpd
	install -c -s -o bin -g bin -m 755 ftpd ${DESTDIR}/usr/etc/ftpd
	ln -s ../usr/etc/ftpd ${DESTDIR}/etc/ftpd

clean:	FRC
	rm -f .depend ${OBJS} ftpd core ftpcmd.c

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
