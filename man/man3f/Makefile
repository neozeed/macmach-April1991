# /usr/src/man/man3f/Makefile

MDIR=	/usr/man/man3f
SRCS=	abort.3 access.3 alarm.3 bessel.3 bit.3 chdir.3 chmod.3 \
	etime.3 exit.3 fdate.3 flmin.3 flush.3 fork.3 fseek.3 \
	getarg.3 getc.3 getcwd.3 getenv.3 getlog.3 getpid.3 \
	getuid.3 hostnm.3 idate.3 index.3 intro.3 ioinit.3 \
	kill.3 link.3 loc.3 long.3 malloc.3 perror.3 plot.3 \
	putc.3 qsort.3 rand.3 random.3 rename.3 signal.3 sleep.3 \
	stat.3 system.3 time.3 topen.3 traper.3 trapov.3 trpfpe.3 \
	ttynam.3 unlink.3 wait.3
LINKS=	and.3 lshift.3 not.3 or.3 rshift.3 xor.3 box.3 ctime.3 \
	drand.3 fgetc.3 fputc.3 fstat.3 gmtime.3 irand.3 lstat.3 \
	ltime.3 short.3 symlnk.3 dffrac.3 dflmax.3 dflmin.3 dtime.3 \
	ffrac.3 flmax.3 fortran.3 ftell.3 inmax.3 itime.3 len.3 \
	lnblnk.3 rindex.3 tclose.3 tread.3 trewin.3 tskipf.3 \
	tstate.3 twrite.3 drandm.3 falloc.3 fpecnt.3 gerror.3 \
	getgid.3 iargc.3 ierrno.3 irandm.3 isatty.3 besj0.3 besj1.3 \
	besjn.3 besy0.3 besy1.3 besyn.3 dbesj0.3 dbesj1.3 dbesjn.3 \
	dbesy0.3 dbesy1.3 dbesyn.3

all:	FRC

clean:	FRC

install: FRC
	-[ -d ${DESTDIR}${MDIR} ] || mkdir ${DESTDIR}${MDIR}
	-chmod 755 ${DESTDIR}${MDIR}
	-chown root.bin ${DESTDIR}${MDIR}
	cd ${DESTDIR}${MDIR}; rm -f ${SRCS} ${LINKS}
	install -c -o bin -g bin -m 444 ${SRCS} ${DESTDIR}${MDIR}
	ln -s bit.3 ${DESTDIR}${MDIR}/and.3
	ln -s bit.3 ${DESTDIR}${MDIR}/lshift.3
	ln -s bit.3 ${DESTDIR}${MDIR}/not.3
	ln -s bit.3 ${DESTDIR}${MDIR}/or.3
	ln -s bit.3 ${DESTDIR}${MDIR}/rshift.3
	ln -s bit.3 ${DESTDIR}${MDIR}/xor.3
	ln -s plot.3 ${DESTDIR}${MDIR}/box.3
	ln -s time.3 ${DESTDIR}${MDIR}/ctime.3
	ln -s rand.3 ${DESTDIR}${MDIR}/drand.3
	ln -s getc.3 ${DESTDIR}${MDIR}/fgetc.3
	ln -s putc.3 ${DESTDIR}${MDIR}/fputc.3
	ln -s stat.3 ${DESTDIR}${MDIR}/fstat.3
	ln -s time.3 ${DESTDIR}${MDIR}/gmtime.3
	ln -s rand.3 ${DESTDIR}${MDIR}/irand.3
	ln -s stat.3 ${DESTDIR}${MDIR}/lstat.3
	ln -s time.3 ${DESTDIR}${MDIR}/ltime.3
	ln -s long.3 ${DESTDIR}${MDIR}/short.3
	ln -s link.3 ${DESTDIR}${MDIR}/symlnk.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/dffrac.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/dflmax.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/dflmin.3
	ln -s etime.3 ${DESTDIR}${MDIR}/dtime.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/ffrac.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/flmax.3
	ln -s intro.3 ${DESTDIR}${MDIR}/fortran.3
	ln -s fseek.3 ${DESTDIR}${MDIR}/ftell.3
	ln -s flmin.3 ${DESTDIR}${MDIR}/inmax.3
	ln -s idate.3 ${DESTDIR}${MDIR}/itime.3
	ln -s index.3 ${DESTDIR}${MDIR}/len.3
	ln -s index.3 ${DESTDIR}${MDIR}/lnblnk.3
	ln -s index.3 ${DESTDIR}${MDIR}/rindex.3
	ln -s topen.3 ${DESTDIR}${MDIR}/tclose.3
	ln -s topen.3 ${DESTDIR}${MDIR}/tread.3
	ln -s topen.3 ${DESTDIR}${MDIR}/trewin.3
	ln -s topen.3 ${DESTDIR}${MDIR}/tskipf.3
	ln -s topen.3 ${DESTDIR}${MDIR}/tstate.3
	ln -s topen.3 ${DESTDIR}${MDIR}/twrite.3
	ln -s random.3 ${DESTDIR}${MDIR}/drandm.3
	ln -s malloc.3 ${DESTDIR}${MDIR}/falloc.3
	ln -s trpfpe.3 ${DESTDIR}${MDIR}/fpecnt.3
	ln -s perror.3 ${DESTDIR}${MDIR}/gerror.3
	ln -s getuid.3 ${DESTDIR}${MDIR}/getgid.3
	ln -s getarg.3 ${DESTDIR}${MDIR}/iargc.3
	ln -s perror.3 ${DESTDIR}${MDIR}/ierrno.3
	ln -s random.3 ${DESTDIR}${MDIR}/irandm.3
	ln -s ttynam.3 ${DESTDIR}${MDIR}/isatty.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besj0.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besj1.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besjn.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besy0.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besy1.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/besyn.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesj0.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesj1.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesjn.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesy0.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesy1.3
	ln -s bessel.3 ${DESTDIR}${MDIR}/dbesyn.3

FRC:
