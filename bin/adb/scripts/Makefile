# /usr/src/bin/adb/scripts/Makefile for MacMach

LIST=	buf callout callout.next clist clist.nxt dino dir dir.nxt \
	dmap file filsys findinode findinode.nxt findproc host.nxt hosts \
	hosts.nxt hosttable idp ifaddr ifnet ifrw imp in_ifaddr inode inpcb \
	iovec ipreass ipreass.nxt mact mact.nxt mbstat mbuf mbuf.nxt \
	mbufchain mbufs mbufs.nxt mount nameidata nspcb nsrtentry packetchain \
	protosw quota rawcb route rtentry rusage setproc setproc.done socket \
	spp sppcb stat tcpcb tcpip tcpreass tcpreass.nxt text traceall \
	traceall.nxt u u.all ubadev uio un unpcb urppcb vtimes

all:	FRC

install: FRC
	install -c -o bin -g bin -m 444 ${LIST} ${DESTDIR}/usr/lib/adb

clean:	FRC

FRC:
