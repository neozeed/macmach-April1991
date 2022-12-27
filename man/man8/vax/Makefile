# /usr/src/man/man8/vax/Makefile

MDIR=	/usr/man/man8/vax
SRCS=	arff.8 bad144.8 crash.8 drtest.8 format.8 installboot.8 rxformat.8 \
	reboot.8
LINKS=	flcopy.8

all:	FRC

clean:	FRC

install: FRC
	-mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s arff.8 ${DESTDIR}${MDIR}/flcopy.8

FRC:
