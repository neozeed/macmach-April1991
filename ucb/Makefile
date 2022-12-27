# /usr/src/ucb/Makefile for MacMach

DESTDIR=
CFLAGS=	-O
LIBC=	/lib/libc.a

# Programs that live in subdirectories, and have makefiles of their own.
# Note: lisp needs to be fixed for mac2
SUBDIR=	Mail compress ctags dsh error ex ftp gprof grep indent \
	man more msgs netstat rdist sendbug systat talk telnet tftp \
	tn3270 tset vgrind vlp sysline window

# Shell scripts that need only be installed and are never removed.
SCRIPT= which

# C source that lives in the current directory
SRCS=	apply.c biff.c checknr.c clear.c colcrt.c colrm.c expand.c finger.c \
	fold.c from.c gcore.c groups.c head.c last.c lastcomm.c \
	leave.c lock.c logger.c mkstr.c printenv.c quota.c rlogin.c \
	rsh.c ruptime.c rwho.c sccs.c script.c soelim.c strings.c symorder.c \
	tail.c tcopy.c ul.c unexpand.c unifdef.c users.c \
	vacation.c vmstat.c w.c wc.c what.c whereis.c whoami.c whois.c \
	xstr.c yes.c

# C programs that live in the current directory and do not need
# explicit make lines.
STD=	apply biff checknr colcrt colrm expand finger fold from \
	groups head last lastcomm leave lock logger mkstr printenv \
	ruptime rwho sccs script soelim strings symorder tail tcopy \
	unexpand unifdef users whereis whoami whois what wc xstr yes

# C programs that live in the current directory and need explicit make lines.
NSTD=	clear ul vacation

# Programs that get installed with special permissions
PERM=	gcore quota rlogin rsh w

all:	${SUBDIR} ${STD} ${NSTD} ${PERM} vmstat .depend

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

${STD} ${PERM}: ${LIBC}
	${CC} ${CFLAGS} -o $@ $@.c

vmstat:
	$(CC) $(CFLAGS) -o vmstat vmstat.c -ltermlib -lmach

install: FRC
	-for i in ${SUBDIR}; do \
	  (cd $$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	-for i in ${SCRIPT}; do \
	  (install -c -o bin -g bin -m 755 $$i.csh ${DESTDIR}/usr/ucb/$$i); \
	done
	install -c -s -o bin -g bin -m 755 ${STD} ${NSTD} ${DESTDIR}/usr/ucb
	install -c -s -o root -g bin -m 4755 quota ${DESTDIR}/usr/ucb/quota
	install -c -s -o root -g bin -m 4755 rlogin ${DESTDIR}/usr/ucb/rlogin
	install -c -s -o root -g bin -m 4755 rsh ${DESTDIR}/usr/ucb/rsh
	install -c -s -o bin -g kmem -m 2755 gcore ${DESTDIR}/usr/ucb/gcore
	install -c -s -o bin -g kmem -m 2755 vmstat ${DESTDIR}/usr/ucb/vmstat
	install -c -s -o bin -g kmem -m 2755 w ${DESTDIR}/usr/ucb/w
	rm -f ${DESTDIR}/usr/ucb/uptime ${DESTDIR}/usr/ucb/f
	ln -s w ${DESTDIR}/usr/ucb/uptime
	ln -s finger ${DESTDIR}/usr/ucb/f
	rm -f ${DESTDIR}/usr/ucb/u
	ln -s users ${DESTDIR}/usr/ucb/u

clean:	FRC
	rm -f .depend a.out core *.s *.o ${STD} ${NSTD} ${PERM} vmstat
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

# Files listed in ${NSTD} have explicit make lines given below.

clear:
	${CC} -o $@ ${CFLAGS} clear.c -ltermlib

ul:
	${CC} -o $@ ${CFLAGS} ul.c -ltermlib

vacation:
	${CC} -o $@ ${CFLAGS} vacation.c -ldbm

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
