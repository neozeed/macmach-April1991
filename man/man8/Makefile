# /usr/src/man/man8/Makefile

MDIR=	/usr/man/man8
SRCS=	XNSrouted.8 ac.8 adduser.8 arp.8 badsect.8 bootpd.8 \
	bugfiler.8 chown.8 chroot.8 clri.8 comsat.8 config.8 \
	cron.8 dcheck.8 disklabel.8 diskpart.8 dm.8 dmesg.8 \
	dump.8 dumpfs.8 edquota.8 fastboot.8 fingerd.8 fsck.8 \
	fsdb.8 fstat.8 ftpd.8 gettable.8 getty.8 halt.8 \
	htable.8 icheck.8 ifconfig.8 inetd.8 init.8 intro.8 \
	kgmon.8 lpc.8 lpd.8 MACH.8 makedev.8 makekey.8 md.8 mkhosts.8 \
	mklost+found.8 mknod.8 mkpasswd.8 mkproto.8 mount.8 \
	named.8 ncheck.8 netmsgserver.8 newfs.8 pac.8 ping.8 \
	portmap.8 pstat.8 quot.8 quotacheck.8 quotaon.8 rc.8 \
	rdump.8 renice.8 repquota.8 restore.8 rexecd.8 rlogind.8 \
	rmt.8 route.8 routed.8 rpcinfo.8 rrestore.8 rshd.8 \
	rwhod.8 sa.8 savecore.8 sendmail.8 shutdown.8 slattach.8 \
	sticky.8 supservers.8 swapon.8 sync.8 syslogd.8 talkd.8 \
	telnetd.8 tftpd.8 timed.8 timedc.8 trpt.8 trsp.8 \
	tunefs.8 update.8 uucico.8 uuclean.8 uupoll.8 uusnap.8 \
	uuxqt.8 version.8 vipw.8
LINKS=	accton.8 umount.8 mkfs.8 quotaoff.8 uucpd.8 fasthalt.8 \
	supfilesrv.8

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s sa.8 ${DESTDIR}${MDIR}/accton.8
	ln -s mount.8 ${DESTDIR}${MDIR}/umount.8
	ln -s newfs.8 ${DESTDIR}${MDIR}/mkfs.8
	ln -s quotaon.8 ${DESTDIR}${MDIR}/quotaoff.8
	ln -s uucico.8 ${DESTDIR}${MDIR}/uucpd.8
	ln -s fastboot.8 ${DESTDIR}${MDIR}/fasthalt.8
	ln -s supservers.8 ${DESTDIR}${MDIR}/supfilesrv.8

FRC:
