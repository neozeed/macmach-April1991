# /usr/src/man/man5/Makefile

MDIR=	/usr/man/man5
SRCS=	L-devices.5 L-dialcodes.5 L.aliases.5 L.cmds.5 L.sys.5 \
	USERFILE.5 a.out.5 acct.5 aliases.5 ar.5 core.5 dbx.5 \
	dir.5 disklabel.5 disktab.5 dm.config.5 dump.5 error.5 \
	fs.5 fstab.5 gettytab.5 group.5 hosts.5 map3270.5 \
	mtab.5 networks.5 passwd.5 phones.5 plot.5 printcap.5 \
	protocols.5 remote.5 resolver.5 services.5 shells.5 \
	stab.5 tar.5 termcap.5 tp.5 ttys.5 types.5 tzfile.5 \
	utmp.5 uuencode.5 vfont.5 vgrindefs.5 rcsfile.5 \
	cpio_format.5
LINKS=	inode.5 dumpdates.5 wtmp.5

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s fs.5 ${DESTDIR}${MDIR}/inode.5
	ln -s dump.5 ${DESTDIR}${MDIR}/dumpdates.5
	ln -s utmp.5 ${DESTDIR}${MDIR}/wtmp.5

FRC:
