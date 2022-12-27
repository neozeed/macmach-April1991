# /usr/src/man/man4/tahoe/Makefile

MDIR=	/usr/man/man4/tahoe
SRCS=	ace.4 autoconf.4 cons.4 cy.4 dr.4 enp.4 ik.4 intro.4 lp.4 mem.4 \
	mpcc.4 mpcctab.4 mtio.4 vd.4 vx.4
LINKS=	kmem.4

all:	FRC

clean: FRC

install: FRC
	-mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s mem.4 ${DESTDIR}${MDIR}/kmem.4

FRC:
