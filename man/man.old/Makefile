# /usr/src/man/man.old/Makefile

MDIR=	/usr/man/man.old
SRCS=	analyze.8 arcv.8 cxref.1 net.1 netcp.1 netlog.1 \
	netlogin.1 netlpr.1 netmail.1 netq.1 netrm.1 nettroff.1 \
	print.1 prmail.1 pti.1 sdb.1 trman.1 ttytype.5 compact.1
LINKS=	ccat.1 uncompact.1

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s compact.1 ${DESTDIR}${MDIR}/ccat.1
	ln -s compact.1 ${DESTDIR}${MDIR}/uncompact.1

FRC:
