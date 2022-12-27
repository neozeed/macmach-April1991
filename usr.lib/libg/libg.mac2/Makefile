# /usr/src/usr.lib/libg/libg.mac2/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=
OBJS=

all:	libg.a

libg.a: dbxxx.s
	${AS} dbxxx.s -o $@

install: FRC
	install -c -o bin -g bin -m 644 libg.a ${DESTDIR}/usr/lib/libg.a

clean:	FRC
	rm -f .depend ${OBJS} core libg.a

FRC:
