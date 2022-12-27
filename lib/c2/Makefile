# /usr/src/lib/c2/Makefile for MacMach

DESTDIR= 

all:	FRC
	cd c2.${MACHINE}; make ${MFLAGS}

install: FRC
	cd c2.${MACHINE}; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd c2.${MACHINE}; make ${MFLAGS} clean

FRC:
