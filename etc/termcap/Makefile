# /usr/src/etc/termcap/Makefile for MacMach

all:	termcap

termcap: reorder termcap.src
	ex - termcap.src < reorder

install: FRC
	install -c -o bin -g bin -m 444 map3270 ${DESTDIR}/etc/map3270
	install -c -o bin -g staff -m 444 termcap ${DESTDIR}/etc/termcap
	rm -rf ${DESTDIR}/usr/lib/tabset
	mkdir ${DESTDIR}/usr/lib/tabset
	chown bin.bin ${DESTDIR}/usr/lib/tabset
	chmod 755 ${DESTDIR}/usr/lib/tabset
	install -c -o bin -g bin -m 444 tabset/* ${DESTDIR}/usr/lib/tabset

clean:	FRC
	rm -f termcap

FRC:
