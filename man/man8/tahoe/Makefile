# /usr/src/man/man8/tahoe/Makefile

MDIR=	/usr/man/man8/tahoe
SRCS=	crash.8 dlmpcc.8 enpload.8 reboot.8 vdformat.8

all:	FRC

clean:	FRC

install: FRC
	-mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}

FRC:
