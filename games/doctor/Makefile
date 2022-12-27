# /usr/src/games/doctor/Makefile for MacMach

all:	doctor

doctor:	FRC
	liszt -mrq -o doctor doctor.l

install: FRC
	install -c -o games -g bin -m 700 doctor ${DESTDIR}/usr/games/hide
	rm -f ${DESTDIR}/usr/games/doctor
	ln -s ${DESTDIR}/usr/games/dm ${DESTDIR}/usr/games/doctor
	chown games.bin ${DESTDIR}/usr/games/doctor

clean:	FRC
	rm -f doctor core

FRC:
