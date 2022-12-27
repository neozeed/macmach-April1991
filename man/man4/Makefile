# /usr/src/man/man4/Makefile

MDIR=	/usr/man/man4
SRCS=	arp.4 bk.4 drum.4 icmp.4 idp.4 imp.4 inet.4 ip.4 \
	kdb.4 lo.4 netintro.4 ns.4 nsip.4 null.4 pty.4 spp.4 \
	tcp.4 tty.4 udp.4
LINKS=	networking.4

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s netintro.4 ${DESTDIR}${MDIR}/networking.4

FRC:
