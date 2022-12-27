# /usr/src/lib/pcc/Makefile for MacMach

all:	FRC
	cd pcc.${MACHINE}; make ${MFLAGS}

install: FRC
	cd pcc.${MACHINE}; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd pcc.${MACHINE}; make ${MFLAGS} clean

FRC:
