# /usr/src/include/Makefile

DESTDIR=

# standard files in /usr/include
STD=	a.out.h ar.h assert.h cthreads.h ctype.h curses.h dbm.h disktab.h \
	exception_all.h fcntl.h fstab.h grp.h lastlog.h mach.h mach_error.h \
	mach_exception.h mach_init.h mach_interface.h math.h memory.h \
	mntent.h mp.h mtab.h ndbm.h netdb.h nlist.h pcc.h pwd.h ranlib.h \
	regexp.h resolv.h setjmp.h sgtty.h stab.h stdio.h string.h strings.h \
	struct.h sysexits.h time.h ttyent.h tzfile.h utmp.h varargs.h vfont.h \
	mach_tahoe_fs.h

# links in /usr/include to /usr/include/sys
LINKS=	errno.h signal.h syslog.h

# subdirectories in /usr/include
SUBDIRS=arpa protocols cmu

# network stuff taken from ../sys
NETDIRS=net netimp netinet netns

# mach stuff taken from ../sys
MACHDIRS=mach mach_debug mach/mac2 mach/sun3 mach/vax mach/ca

# /usr/include/servers files built with mig
SERVERS=errorlib.h netname.h netname_defs.h env_mgr.h ipcx_types.h \
	emdefs.h service.h netmemory.h netmemory_defs.h

# mac2 stuff taken from ../sys
MAC2DIRS=mac2 mac2dev mac2if mac2slotmgr nfs rpc afs vfs

TAGSFILE=tags

all:

install: mkdir mach ${MACHINE} ${STD}
	-for i in ${STD}; do \
          ( echo "/* `cat ../VERSION` */"; echo ""; cat $$i ) > /tmp/I; \
	  cmp -s /tmp/I ${DESTDIR}/usr/include/$$i || \
	    { echo $$i; \
              install -c -m 444 /tmp/I ${DESTDIR}/usr/include/$$i; }; \
	done
	-for i in ${SUBDIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	  (cd $$i; for j in *.[ih]; do \
            ( echo "/* `cat ../../VERSION` */"; echo ""; cat $$j ) > /tmp/I; \
	    cmp -s /tmp/I ${DESTDIR}/usr/include/$$i/$$j || \
	      { echo $$i/$$j; \
                install -c -m 444 /tmp/I ${DESTDIR}/usr/include/$$i/$$j; }; \
	  done); \
	done
	-rm -f /tmp/I
	-for i in ${LINKS}; do \
	  rm -f ${DESTDIR}/usr/include/$$i; \
	  echo $$i; ln -s sys/$$i ${DESTDIR}/usr/include/$$i; \
	done
	rm -f ${DESTDIR}/usr/include/machine
	ln -s ${MACHINE} ${DESTDIR}/usr/include/machine
	find ${DESTDIR}/usr/include -type d -exec chmod 555 {} \;
	find ${DESTDIR}/usr/include -type f -exec chmod 444 {} \;
	chown -R bin.bin ${DESTDIR}/usr/include

mkdir:	FRC
	-[ -d ${DESTDIR}/usr/include ] || mkdir ${DESTDIR}/usr/include
	chmod 755 ${DESTDIR}/usr/include
	chown bin.bin ${DESTDIR}/usr/include

mac2:	FRC
	-for i in ${NETDIRS} ${MAC2DIRS} sys; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	  (cd ../sys/$$i; for j in *.[ih]; do \
	    cmp -s $$j ${DESTDIR}/usr/include/$$i/$$j || { \
	      echo $$i/$$j; \
	      install -c -m 444 $$j ${DESTDIR}/usr/include/$$i/$$j; \
	    }; \
	  done); \
	done
	rm -f ${DESTDIR}/usr/include/sys/dkstat.h
	ln -s dk.h ${DESTDIR}/usr/include/sys/dkstat.h

mach: machdirs ${SERVERS}
	-[ -d ${DESTDIR}/usr/include/servers ] || \
	  mkdir ${DESTDIR}/usr/include/servers
	-for i in ${SERVERS}; do \
	  cmp -s $$i ${DESTDIR}/usr/include/servers/$$i || { \
	    echo $$i; \
	    install -c -m 444 $$i ${DESTDIR}/usr/include/servers/$$i; \
	  }; \
	done

machdirs: FRC
	-for i in ${MACHDIRS}; do \
	  [ -d ${DESTDIR}/usr/include/$$i ] || \
	    mkdir ${DESTDIR}/usr/include/$$i; \
	  (cd ../sys/$$i; for j in *.[ihd]*; do \
	    cmp -s $$j ${DESTDIR}/usr/include/$$i/$$j || { \
	      echo $$i/$$j; \
	      install -c -m 444 $$j ${DESTDIR}/usr/include/$$i/$$j; \
	    }; \
	  done); \
	done
	rm -f ${DESTDIR}/usr/include/mig_errors.h
	ln -s mach/mig_errors.h ${DESTDIR}/usr/include/mig_errors.h
	rm -f ${DESTDIR}/usr/include/mach/machine
	ln -s ${MACHINE} ${DESTDIR}/usr/include/mach/machine

mach_interface.h: ../sys/mach/mach.defs
	mig $? -header $@ -server /dev/null -user /dev/null

service.h: ../etc/mach_init/service.defs
	mig $? -header $@ -server /dev/null -user /dev/null

netmemory.h: ../etc/netmemoryserver/netmemory.defs
	mig $? -header $@ -server /dev/null -user /dev/null

errorlib.h: ../usr.lib/libmach/errorlib.h
	rm -f $@; ln -s $? $@

netname.h: ../etc/netmsgserver/server/netname.defs
	mig $? -header $@ -server /dev/null -user /dev/null

netname_defs.h: ../etc/netmsgserver/server/netname_defs.h
	rm -f $@; ln -s $? $@

env_mgr.h: ../etc/envmgr/env_mgr.defs
	mig $? -header $@ -server /dev/null -user /dev/null

netmemory_defs.h: ../etc/netmemoryserver/netmemory_defs.h
	rm -f $@; ln -s $? $@

emdefs.h: ../etc/envmgr/emdefs.h
	rm -f $@; ln -s $? $@

ipcx_types.h: ../etc/ipcexecd/ipcx_types.h
	rm -f $@; ln -s $? $@

tags:	FRC
	cwd=/usr/include; \
	for i in ${STD} ${LINKS}; do ctags -a -f ${TAGSFILE} $$cwd/$$i; done

clean:	FRC
	rm -f tags mach_interface.h ${SERVERS}

FRC:
