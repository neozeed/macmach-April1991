# /usr/src/man/man7/Makefile

MDIR=	/usr/man/man7
SRCS=	ascii.7 environ.7 eqnchar.7 hier.7 hostname.7 intro.7 \
	mailaddr.7 man.7 me.7 ms.7 term.7
LINKS=	miscellaneous.7

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s intro.7 ${DESTDIR}${MDIR}/miscellaneous.7

FRC:
