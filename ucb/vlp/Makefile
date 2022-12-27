# /usr/src/ucb/vlp/Makefile for MacMach

DESTDIR= 

all:	FRC
	cd vlp.${MACHINE}; make ${MFLAGS}

install: FRC
	cd vlp.${MACHINE}; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd vlp.${MACHINE}; make ${MFLAGS} clean

FRC:
