# /usr/src/usr.lib/man/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	tmac.an tmac.an.new tmac.an6n tmac.an6t

all:	FRC

install: FRC
	-[ -d ${DESTDIR}/usr/lib/tmac ] || mkdir ${DESTDIR}/usr/lib/tmac
	-chmod 755 ${DESTDIR}/usr/lib/tmac
	-chown root.bin ${DESTDIR}/usr/lib/tmac
	for i in ${SRCS}; do \
	  (install -c -o bin -g bin -m 644 $$i ${DESTDIR}/usr/lib/tmac/$$i); \
	done

clean:	FRC

FRC:
