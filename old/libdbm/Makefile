# /usr/src/old/libdbm/Makefile for MacMach

CFLAGS=-O
TAGSFILE=tags

all:	libdbm.a

libdbm.a: dbm.c
	${CC} -c ${CFLAGS} dbm.c
	mv dbm.o libdbm.a

install:	FRC
	install -c -m 644 libdbm.a $(DESTDIR)/usr/lib/libodbm.a

clean:	FRC
	rm -f libdbm.a errs a.out core

FRC:
