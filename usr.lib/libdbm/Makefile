# /usr/src/usr.lib/libdbm/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	dbm.c
OBJS=	dbm.o

all:	libdbm.a libdbm_p.a .depend

libdbm.a: dbm.c
	${CC} -c ${CFLAGS} dbm.c
	ld -r -o libdbm.a -x dbm.o
	rm -f dbm.o

libdbm_p.a: dbm.c
	${CC} -p -c ${CFLAGS} dbm.c
	ld -r -o libdbm_p.a -X dbm.o
	rm -f dbm.o

install: FRC
	install -c -o bin -g bin -m 644 libdbm.a ${DESTDIR}/usr/lib/libdbm.a
	install -c -o bin -g bin -m 644 libdbm_p.a ${DESTDIR}/usr/lib/libdbm_p.a
	install -c -o bin -g bin -m 444 dbm.h ${DESTDIR}/usr/include/dbm.h

clean:	FRC
	rm -f .depend ${OBJS} core libdbm.a libdbm_p.a

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
