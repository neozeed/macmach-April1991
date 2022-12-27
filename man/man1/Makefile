# /usr/src/man/man1/Makefile

MDIR=	/usr/man/man1
SRCS=	adb.1 addbib.1 apply.1 apropos.1 ar.1 as.1 at.1 atq.1 \
	atrm.1 awk.1 basename.1 bc.1 biff.1 binmail.1 cal.1 \
	calendar.1 cat.1 cb.1 cc.1 cd.1 checknr.1 chgrp.1 \
	chmod.1 clear.1 cmp.1 col.1 colcrt.1 colrm.1 comm.1 \
	compress.1 cp.1 crypt.1 csh.1 ctags.1 date.1 dc.1 \
	dd.1 deroff.1 df.1 diction.1 diff.1 diff3.1 du.1 \
	echo.1 ed.1 efl.1 eqn.1 error.1 ex.1 expand.1 expr.1 \
	false.1 file.1 find.1 finger.1 fixit.1 fmt.1 fold.1 \
	fp.1 fpr.1 from.1 fsplit.1 ftp.1 gcore.1 gdb.1 gprof.1 graph.1 \
	grep.1 groups.1 head.1 hostid.1 hostname.1 indent.1 \
	install.1 intro.1 iostat.1 ipcexecd.1 join.1 kill.1 last.1 \
	lastcomm.1 ld.1 learn.1 leave.1 lex.1 lint.1 lisp.1 \
	liszt.1 ln.1 lock.1 logger.1 login.1 look.1 lookbib.1 \
	lorder.1 lpq.1 lpr.1 lprm.1 lptest.1 ls.1 lxref.1 m4.1 \
	mach.1 macherr.1 mail.1 make.1 man.1 mesg.1 mig.1 mkdep.1 mkdir.1 \
	mkstr.1 more.1 mset.1 msgs.1 mt.1 mv.1 netstat.1 \
	newaliases.1 nice.1 nm.1 nroff.1 od.1 pagesize.1 \
	passwd.1 pc.1 pdx.1 pi.1 pix.1 plot.1 pmerge.1 pr.1 \
	printenv.1 prof.1 ps.1 ptx.1 pwd.1 px.1 pxp.1 \
	pxref.1 quota.1 ranlib.1 ratfor.1 rcp.1 rdist.1 \
	refer.1 rev.1 rlogin.1 rm.1 rmail.1 rmdir.1 roffbib.1 \
	rsh.1 ruptime.1 rwho.1 sccs.1 script.1 sed.1 sendbug.1 \
	sh.1 size.1 sleep.1 soelim.1 sort.1 sortbib.1 spell.1 \
	spline.1 split.1 strings.1 strip.1 struct.1 stty.1 \
	style.1 su.1 sum.1 sup.1 symorder.1 sysline.1 systat.1 \
	tabs.1 tail.1 talk.1 tar.1 tbl.1 tc.1 tcopy.1 tee.1 \
	telnet.1 test.1 tftp.1 time.1 tip.1 tk.1 tn3270.1 \
	touch.1 tp.1 tr.1 troff.1 true.1 tset.1 tsort.1 \
	tty.1 ul.1 unifdef.1 uniq.1 units.1 uptime.1 users.1 \
	uucp.1 uuencode.1 uulog.1 uuname.1 uuq.1 uusend.1 \
	uux.1 vacation.1 vgrind.1 vi.1 vlp.1 vmstat.1 w.1 \
	wait.1 wall.1 wc.1 wh.1 what.1 whatis.1 whereis.1 \
	which.1 who.1 whoami.1 whois.1 window.1 write.1 \
	xsend.1 xstr.1 yacc.1 yes.1 cpio.1 dsh.1 courier.1 \
	ci.1 co.1 ident.1 rcs.1 rcsdiff.1 rcsfreeze.1 \
	rcsintro.1 rcsmerge.1 rlog.1 sccstorcs.1 merge.1 md.1
LINKS=	case.1 edit.1 for.1 if.1 while.1 checkeq.1 neqn.1 cu.1 \
	diffh.1 egrep.1 fgrep.1 nohup.1 page.1 reset.1 enroll.1 \
	spellin.1 spellout.1 xget.1 chfn.1 chsh.1 unexpand.1 \
	explain.1 indxbib.1 uncompress.1 zcat.1 uudecode.1 \
	Mail.1 limit.1

all:	FRC

eqn.1:	eqn.e
	eqn eqn.e > eqn.1

clean:	FRC
	rm -f eqn.1

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s mail.1 ${DESTDIR}${MDIR}/Mail.1
	ln -s sh.1 ${DESTDIR}${MDIR}/case.1
	ln -s ex.1 ${DESTDIR}${MDIR}/edit.1
	ln -s sh.1 ${DESTDIR}${MDIR}/for.1
	ln -s sh.1 ${DESTDIR}${MDIR}/if.1
	ln -s sh.1 ${DESTDIR}${MDIR}/while.1
	ln -s eqn.1 ${DESTDIR}${MDIR}/checkeq.1
	ln -s eqn.1 ${DESTDIR}${MDIR}/neqn.1
	ln -s tip.1 ${DESTDIR}${MDIR}/cu.1
	ln -s diff.1 ${DESTDIR}${MDIR}/diffh.1
	ln -s grep.1 ${DESTDIR}${MDIR}/egrep.1
	ln -s grep.1 ${DESTDIR}${MDIR}/fgrep.1
	ln -s nice.1 ${DESTDIR}${MDIR}/nohup.1
	ln -s more.1 ${DESTDIR}${MDIR}/page.1
	ln -s tset.1 ${DESTDIR}${MDIR}/reset.1
	ln -s xsend.1 ${DESTDIR}${MDIR}/enroll.1
	ln -s spell.1 ${DESTDIR}${MDIR}/spellin.1
	ln -s spell.1 ${DESTDIR}${MDIR}/spellout.1
	ln -s xsend.1 ${DESTDIR}${MDIR}/xget.1
	ln -s passwd.1 ${DESTDIR}${MDIR}/chfn.1
	ln -s passwd.1 ${DESTDIR}${MDIR}/chsh.1
	ln -s expand.1 ${DESTDIR}${MDIR}/unexpand.1
	ln -s diction.1 ${DESTDIR}${MDIR}/explain.1
	ln -s lookbib.1 ${DESTDIR}${MDIR}/indxbib.1
	ln -s compress.1 ${DESTDIR}${MDIR}/uncompress.1
	ln -s compress.1 ${DESTDIR}${MDIR}/zcat.1
	ln -s uuencode.1 ${DESTDIR}${MDIR}/uudecode.1
	ln -s csh.1 ${DESTDIR}${MDIR}/limit.1

FRC:
