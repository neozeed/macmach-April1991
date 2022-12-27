# /usr/src/Makefile for MacMach

# WARNING: You should do all building as uid root!!!

# This is the top-level Makefile for the MacMach system.  Saying "make"
# will do everything according to the following parameters.  If the current
# working directory is not OBJDIR, it is assumed to be the top level source
# directory, in which case the OBJDIR will be flushed and re-created.

# DESTDIR specifies the destination directory for completed system objects.
# This needs about 60MB of disk space.  It will be created if it does not
# exist.
DESTDIR= /usr/os

# OBJDIR specifies where the compilation objects are to go, this should
# have a complete set of symbolic links to all source files wherever
# they are...  This needs about 90MB of disk space.  It will be created
# if it does not exist.  If the system build is started from a different
# directory, that directory is assumed to be the top level source directory,
# in which case the OBJDIR will be flushed and re-created.
OBJDIR= /usr/obj

# define INSTALL to be "install" if a system build should automatically
# install a system into DESTDIR.  define INSTALL to be "install config"
# if the installation to DESTDIR should produce a runnable system,
# otherwise DESTDIR is a system source directory that is a suitable
# input to the /etc/newsys system installer.
#INSTALL=
INSTALL=install
#INSTALL=install config

# order is important, old and man must be #1 and #2
SRCS= old man include lib usr.lib bin usr.bin etc ucb games X11 motif

# subdirectories in /usr/man than contain man pages
MAN=	man1 man2 man3 man3f man4 man5 man6 man7 man8 manx

# standard system directories in ${DESTDIR}
MKDIR=	/tmp /lib /bin /etc /dev /usr /usr/etc /usr/tmp /usr/include \
	/usr/lib /usr/bin /usr/ucb /usr/man /usr/man/doc /usr/man/doc/X11 \
	/usr/man/info /usr/spool /usr/adm /usr/old /usr/lib/tmac \
	/usr/lib/learn /usr/games /usr/games/lib /usr/games/hide \
	/usr/bin/X11 /usr/lib/X11 /usr/include/X11 /usr/users

# default action -- compile and install everything
# updates local libc, cc, /usr/include and /usr/lib
build:	startbuild Makefiles clean includes depend compiler libraries \
	compile vmunix tags ${INSTALL}
	@echo; echo -n "=== system build completed at: "; date

# before build, make sure ${OBJDIR} exists
startbuild: FRC
	@echo; echo -n "=== beginning system build at: "; date
	@echo "pwd     = `pwd`"
	@echo "OBJDIR  = ${OBJDIR}"
	@echo "DESTDIR = ${DESTDIR}"
	-@if [ `pwd` != ${OBJDIR} -o ! -d ${OBJDIR} ]; then \
	  echo; echo -n "=== creating ${OBJDIR} at: "; date; \
	  rm -rf ${OBJDIR}/*; fixit -ln `pwd` ${OBJDIR} >/dev/null ;\
	fi

# install everything in ${OBJDIR} to ${DESTDIR}
install: startinstall
	-@for i in ${SRCS}; do \
	  echo; echo -n "=== installing ${DESTDIR}/$$i at: "; date; \
	  (cd ${OBJDIR}/$$i; make ${MFLAGS} DESTDIR=${DESTDIR} install); \
	done
	@echo; echo -n "=== linking X to Xmacmach at: "; date
	@rm -rf ${DESTDIR}/usr/bin/X11/X
	@ln -s Xmacmach ${DESTDIR}/usr/bin/X11/X
	@echo; echo -n "=== installing X11 documentation at: "; date
	-@[ -d ${DESTDIR}/usr/man/doc/X11 ] || mkdir ${DESTIDR}/usr/man/doc/X11
	@cd ${OBJDIR}/X11/doc; \
	  tar -chf - * | (cd ${DESTDIR}/usr/man/doc/X11; tar -xf -)
	@rm -rf ${DESTDIR}/usr/man/doc/X11/Xlib/Xman
	@rm -rf ${DESTDIR}/usr/man/doc/X11/Xt/Xtman
	@cd ${OBJDIR}/X11; make ${MFLAGS} DESTDIR=${DESTDIR} install.man
	@cd ${OBJDIR}/motif; make ${MFLAGS} DESTDIR=${DESTDIR} install.man
	@cd ${OBJDIR}/motif/doc/man/source; \
	  for F in *.3X; do \
	    F=`basename $$F .3X`; \
	    install -c $$F.3X ${DESTDIR}/usr/man/man3/$$F.3Xm; \
	  done
	-@if [ -f ${OBJDIR}/sys/MACMACH/vmunix ]; then \
	  echo; echo -n "=== installing ${DESTDIR}/vmunix at: "; date; \
	  [ -f ${DESTDIR}/vmunix ] && \
	    mv ${DESTDIR}/vmunix ${DESTDIR}/vmunix.old; \
	  install -c -m 444 -o root -g bin \
	    ${OBJDIR}/sys/MACMACH/vmunix ${DESTDIR}/vmunix; \
	fi
	@echo; echo -n "=== installing ${DESTDIR}/usr/lib/tags at: "; date
	-@install -c -m 444 -o bin -g bin ${OBJDIR}/tags ${DESTDIR}/usr/lib/tags
	@echo; echo -n "=== setting /usr/include modes at: "; date
	@find ${DESTDIR}/usr/include -type d -exec chmod 555 {} \;
	@find ${DESTDIR}/usr/include -type f -exec chmod 444 {} \;
	@chown -R bin.bin ${DESTDIR}/usr/include
	@echo; echo -n "=== creating ${DESTDIR}/usr/man/whatis at: "; date
	@cd ${OBJDIR}/man; make DESTDIR=${DESTDIR} whatis
	@echo; echo -n "=== compressing man pages at: "; date
	@cd ${DESTDIR}/usr/man; compress -r ${MAN} doc
	@echo; echo -n "=== setting /usr/man modes at: "; date
	@cd ${DESTDIR}/usr/man; \
	  find ${MAN} doc info -type d -exec chmod 555 {} \; ;\
	  find ${MAN} doc info -type f -exec chmod 444 {} \; ;\
	  chown -R bin.bin ${MAN} doc info
	@echo; echo -n "=== updating ${DESTDIR}/.README at: "; date
	@{ \
	  echo "# /.README -- MacMach System File Checksums." ;\
	  echo "#" ;\
	  echo -n "# " ; eval "echo `cat ${OBJDIR}/VERSION`" ;\
	  echo "#" ;\
	  echo "# System created on `hostname` at `date`" ;\
	  echo "#" ;\
	  fixit -update ${DESTDIR} <${OBJDIR}/etc/etc.mac2/.README ;\
	} >${DESTDIR}/.README
	@echo; echo -n "=== installation complete at: "; date

# before install, make sure ${OBJDIR} and ${DESTDIR} exist
# make all the standard directories in DESTDIR
# make a standard /usr/guest as an example of a standard user account
startinstall: FRC
	@echo; echo -n "=== installing system at: "; date
	@echo "OBJDIR  = ${OBJDIR}"
	@echo "DESTDIR = ${DESTDIR}"
	@[ -d ${OBJDIR} ] || echo "missing object directory: ${OBJDIR}"
	-@[ -d ${DESTDIR} ] || { \
	  echo; echo -n "=== creating ${DESTDIR} at: "; date; \
	  mkdir ${DESTDIR}; \
	}
	chown root.bin ${DESTDIR}; chmod 755 ${DESTDIR}
	-for i in ${MKDIR}; do \
	  [ -d ${DESTDIR}$$i ] || mkdir ${DESTDIR}$$i; \
	  chown root.bin ${DESTDIR}$$i; chmod 755 ${DESTDIR}$$i; \
	done
	chmod 777 ${DESTDIR}/tmp ${DESTDIR}/usr/tmp
	chown games.bin ${DESTDIR}/usr/games/lib ${DESTDIR}/usr/games/hide
	-[ -r ${DESTDIR}/users ] || ln -s usr/users ${DESTDIR}/users
	chown root.wheel ${DESTDIR}/usr/users
	-[ -d ${DESTDIR}/usr/guest ] || mkdir ${DESTDIR}/usr/guest
	chown guest.staff ${DESTDIR}/usr/guest; chmod 775 ${DESTDIR}/usr/guest
	rm -f ${DESTDIR}/usr/guest/.login
	echo "source /etc/newconfig/system.login" >${DESTDIR}/usr/guest/.login
	chown guest.staff ${DESTDIR}/usr/guest/.login
	rm -f ${DESTDIR}/usr/guest/.cshrc
	echo "source /etc/newconfig/system.cshrc" >${DESTDIR}/usr/guest/.cshrc
	chown guest.staff ${DESTDIR}/usr/guest/.cshrc

# configure ${DESTDIR} for actual operation after installation
config:	FRC
	cd ${DESTDIR}/etc/newconfig; make DESTDIR=${DESTDIR} config
	cd ${DESTDIR}/etc/newconfig; make DESTDIR=${DESTDIR} dev

# compile everything into ${OBJDIR}, ignore include and library dependencies
compile: FRC
	cd ${OBJDIR}/X11; make ${MFLAGS} includes
	cd ${OBJDIR}/motif; make ${MFLAGS} includes
	-@for i in ${SRCS}; do \
	  echo; echo -n "=== compiling $$i at: "; date; \
	  ( cd ${OBJDIR}/$$i; make -k ${MFLAGS} ); \
	done

# make and install /usr/include
# note that DESTDIR is not used here, works with local file system only
# also note that some of the X11 includes are not installed until the
# libraries are installed, see libraries below
includes: FRC
	@echo; echo -n "=== building/installing include files at: "; date
	mv /usr/include /usr/include.rm
	cd ${OBJDIR}/include; make ${MFLAGS} install
	cd ${OBJDIR}/X11; make ${MFLAGS} includes
	cd ${OBJDIR}/X11/include; make ${MFLAGS} install
	cd ${OBJDIR}/motif; make ${MFLAGS} includes
	rm -rf /usr/include.rm

# make and install libraries in /usr/lib
# note that DESTDIR is not used here, installs to local file system only
# when installing X11 libraries, a bunch of include files are installed too
libraries: FRC
	@echo; echo -n "=== building libraries at: "; date
	cd ${OBJDIR}/usr.lib; make ${MFLAGS}
	cd ${OBJDIR}/usr.lib; make ${MFLAGS} install
	cd ${OBJDIR}/X11; make ${MFLAGS} includes
	cd ${OBJDIR}/X11/lib; make ${MFLAGS}
	cd ${OBJDIR}/X11/lib; make ${MFLAGS} install

# make and install /lib/libc.a and the C compiler twice
# note that DESTDIR is not used here, installs to local file system only
compiler: FRC
	@echo; echo -n "=== building libc first time at: "; date
	cd ${OBJDIR}/lib/libc; make ${MFLAGS}
	cd ${OBJDIR}/lib/libc; make ${MFLAGS} install
	@echo; echo -n "=== building compiler first time at: "; date
	cd ${OBJDIR}/bin/as; make ${MFLAGS}
	cd ${OBJDIR}/bin/as; make ${MFLAGS} install
	cd ${OBJDIR}/lib/gcc; make ${MFLAGS}
	cd ${OBJDIR}/lib/gcc; make ${MFLAGS} install
	cd ${OBJDIR}/lib; make ${MFLAGS} clean
	@echo; echo -n "=== building libc second time at: "; date
	cd ${OBJDIR}/lib/libc; make ${MFLAGS}
	cd ${OBJDIR}/lib/libc; make ${MFLAGS} install
	@echo; echo -n "=== building compiler second time at: "; date
	cd ${OBJDIR}/bin/as; make ${MFLAGS}
	cd ${OBJDIR}/bin/as; make ${MFLAGS} install
	cd ${OBJDIR}/lib/gcc; make ${MFLAGS}
	cd ${OBJDIR}/lib/gcc; make ${MFLAGS} install
	@echo; echo -n "=== building g++ compiler at: "; date
	cd ${OBJDIR}/lib/g++; make ${MFLAGS}
	cd ${OBJDIR}/lib/g++; make ${MFLAGS} install

# make X11 Makefiles in ${OBJDIR}
Makefiles: FRC
	@echo; echo -n "=== rebuilding X11 Makefiles at: "; date
	cd ${OBJDIR}/X11/config; make imake
	cd ${OBJDIR}/X11; make Makefiles
	cd ${OBJDIR}/motif/config; make imake
	cd ${OBJDIR}/motif; make Makefiles

# make X11 dependencies in ${OBJDIR}
depend: FRC
	cd ${OBJDIR}/X11; make ${MFLAGS} includes
	cd ${OBJDIR}/X11; make depend
	cd ${OBJDIR}/motif; make ${MFLAGS} includes
	cd ${OBJDIR}/motif; make depend

# build new kernel in ${OBJDIR}
vmunix:	FRC
	@echo; echo -n "=== building new kernel at: "; date
	cd ${OBJDIR}/sys; \
	  LPATH="/lib:/usr/lib:"; export LPATH; \
	  if [ -f afs/afs_buffer.c ]; then \
	    make CONFIG=MACMACH; \
	  else \
	    make CONFIG=MACMACH-afs; \
	  fi

# build new ramdisk kernel in ${OBJDIR}
ramdiskvmunix: FRC
	@echo; echo -n "=== building ramdisk kernel at: "; date
	cd ${OBJDIR}/sys; \
	  LPATH="/lib:/usr/lib:"; export LPATH; \
	  make CONFIG=MACMACH-macos_emul+ramdisk

# build a ramdisk image
ramdisk: FRC
	sh etc/etc.mac2/ramdisk.sh

# clean kernel source directory in ${OBJDIR}
cleanvmunix: FRC
	@echo; echo -n "=== cleaning sys at: "; date
	rm -rf ${OBJDIR}/sys/MACMACH*
	rm -f ${OBJDIR}/sys/bin/*
	cd ${OBJDIR}/sys/src; make clean
	cd ${OBJDIR}/sys/src/config; make clean

# clean up everything in ${OBJDIR}
clean:	cleanvmunix
	rm -f tags
	@for i in ${SRCS}; do \
	  echo; echo -n "=== cleaning $$i at: "; date; \
	  (cd ${OBJDIR}/$$i; make -k ${MFLAGS} clean); \
	done

# start a log file
log:	FRC
	-rm -rf log
	script log

# create tags file in ${OBJDIR}
tags:
	@echo; echo -n "=== creating tags file at: "; date
	cp /dev/null ${OBJDIR}/tags
	@for i in include lib usr.lib; do \
	  echo; echo -n "=== making tags for $$i at: "; date; \
	  (cd ${OBJDIR}/$$i; make ${MFLAGS} TAGSFILE=../tags tags); \
	done
	cd ${OBJDIR}; sort -u +0 -1 -o tags tags

FRC:
