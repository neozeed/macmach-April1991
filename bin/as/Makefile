# /usr/src/bin/as/Makefile for MacMach

all:	FRC
	cd as.${MACHINE}; make ${MFLAGS}

install: FRC
	cd as.${MACHINE}; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd as.${MACHINE}; make ${MFLAGS} clean

FRC:
