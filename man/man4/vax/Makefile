# /usr/src/man/man4/vax/Makefile

MDIR=	/usr/man/man4/vax
SRCS=	acc.4 ad.4 autoconf.4 cons.4 crl.4 css.4 ct.4 ddn.4 de.4 dh.4 dhu.4 \
	dmc.4 dmf.4 dmz.4 dn.4 dz.4 ec.4 en.4 ex.4 fl.4 hdh.4 hk.4 hp.4 \
	ht.4 hy.4 ik.4 il.4 intro.4 ix.4 kg.4 lp.4 mem.4 mt.4 mtio.4 np.4 \
	pcl.4 ps.4 qe.4 rx.4 tb.4 tm.4 tmscp.4 ts.4 tu.4 uda.4 up.4 ut.4 \
	uu.4 va.4 vp.4 vv.4
LINKS=	kmem.4

all:	FRC

clean:	FRC

install: FRC
	-mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s mem.4 ${DESTDIR}${MDIR}/kmem.4

FRC:
