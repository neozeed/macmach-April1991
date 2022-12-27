# /usr/src/lib/pcc/pcc.mac2/Makefile for MacMach

all:	FRC
	cd ../../gcc; make ${MFLAGS}

install: FRC
	cd ../../gcc; make DESTDIR=${DESTDIR} install

clean:	FRC
	cd ../../gcc; make ${MFLAGS} clean

FRC:
