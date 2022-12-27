# /usr/src/bin/adb/Makefile for MacMach

SCRIPTDIR= ${DESTDIR}/usr/lib/adb
SUBDIR=	scripts scripts.${MACHINE} adb.${MACHINE}

all:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS}); done

install: FRC
	rm -rf ${SCRIPTDIR}
	mkdir ${SCRIPTDIR}
	chown bin.bin ${SCRIPTDIR}
	chmod 755 ${SCRIPTDIR}
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} install); done

clean:	FRC
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

FRC:
