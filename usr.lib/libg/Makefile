# /usr/src/usr.lib/libg/Makefile for MacMach

all:	FRC
	cd libg.${MACHINE}; make ${MFLAGS}

install: FRC
	cd libg.${MACHINE}; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd libg.${MACHINE}; make ${MFLAGS} clean

FRC:
