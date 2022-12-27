# /usr/src/games/hack/Makefile for MacMach

# Hack or Quest Makefile.

# on some systems the termcap library is in -ltermcap
TERMLIB = -ltermlib

# make hack
GAME = hack
GAMEDIR = /usr/games/lib/hackdir
CFLAGS = -O
HACKCSRC = hack.Decl.c\
	hack.apply.c hack.bones.c hack.c hack.cmd.c hack.do.c\
	hack.do_name.c hack.do_wear.c hack.dog.c hack.eat.c hack.end.c\
	hack.engrave.c hack.fight.c hack.invent.c hack.ioctl.c\
	hack.lev.c hack.main.c hack.makemon.c hack.mhitu.c\
	hack.mklev.c hack.mkmaze.c hack.mkobj.c hack.mkshop.c\
	hack.mon.c hack.monst.c hack.o_init.c hack.objnam.c\
	hack.options.c hack.pager.c hack.potion.c hack.pri.c\
	hack.read.c hack.rip.c hack.rumors.c hack.save.c\
	hack.search.c hack.shk.c hack.shknam.c hack.steal.c\
	hack.termcap.c hack.timeout.c hack.topl.c\
	hack.track.c hack.trap.c hack.tty.c hack.unix.c\
	hack.u_init.c hack.vault.c\
	hack.wield.c hack.wizard.c hack.worm.c hack.worn.c hack.zap.c\
	hack.version.c rnd.c alloc.c

CSOURCES = $(HACKCSRC) makedefs.c

HSOURCES = hack.h hack.mfndpos.h config.h\
	def.edog.h def.eshk.h def.flag.h def.func_tab.h def.gold.h\
	def.mkroom.h\
	def.monst.h def.obj.h def.objclass.h def.objects.h\
	def.permonst.h def.rm.h def.trap.h def.wseg.h

SOURCES = $(CSOURCES) $(HSOURCES)

AUX = data help hh rumors hack.6 hack.sh

DISTR = $(SOURCES) $(AUX) READ_ME Makefile date.h hack.onames.h

HOBJ = hack.Decl.o hack.apply.o hack.bones.o hack.o hack.cmd.o hack.do.o\
	hack.do_name.o hack.do_wear.o hack.dog.o hack.eat.o hack.end.o\
	hack.engrave.o hack.fight.o hack.invent.o hack.ioctl.o\
	hack.lev.o hack.main.o hack.makemon.o hack.mhitu.o hack.mklev.o\
	hack.mkmaze.o hack.mkobj.o hack.mkshop.o hack.mon.o\
	hack.monst.o hack.o_init.o hack.objnam.o hack.options.o\
	hack.pager.o hack.potion.o hack.pri.o\
	hack.read.o hack.rip.o hack.rumors.o hack.save.o\
	hack.search.o hack.shk.o hack.shknam.o hack.steal.o\
	hack.termcap.o hack.timeout.o hack.topl.o\
	hack.track.o hack.trap.o\
	hack.tty.o hack.unix.o hack.u_init.o hack.vault.o hack.wield.o\
	hack.wizard.o hack.worm.o hack.worn.o hack.zap.o\
	hack.version.o rnd.o alloc.o

all:	hack .depend
hack: ${HOBJ} ${LIBC}
	@echo "Loading ..."
	@cc -X -o $@ $(HOBJ) $(TERMLIB)

makedefs:	makedefs.c
	${CC} ${CFLAGS} -o $@ makedefs.c

hack.onames.h:	makedefs def.objects.h
	./makedefs > hack.onames.h

diff:
	@- for i in $(SOURCES) $(AUX) ; do \
	  cmp -s $$i $D/$$i || \
	    ( echo diff $D/$$i $$i ; diff $D/$$i $$i ; echo ) ; \
	done

distribution: Makefile
	@- for i in READ_ME $(SOURCES) $(AUX) Makefile date.h hack.onames.h; do \
	  cmp -s $$i $D/$$i || \
	    ( echo cp $$i $D ; cp $$i $D ) ; \
	done
# the distribution directory also contains the empty files perm and record.

install: FRC
	install -c -s -o games -g bin -m 4744 hack ${DESTDIR}/usr/games/hide
	(cd ${DESTDIR}/usr/games; rm -f hack; ln -s dm hack; chown games.bin hack)
	-[ -d ${DESTDIR}/usr/games/lib/hackdir ] || mkdir ${DESTDIR}/usr/games/lib/hackdir
	chmod 755 ${DESTDIR}/usr/games/lib/hackdir
	chown games.bin ${DESTDIR}/usr/games/lib/hackdir
	install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/hackdir/perm
	install -c -o games -g bin -m 644 /dev/null ${DESTDIR}/usr/games/lib/hackdir/record
	install -c -o games -g bin -m 644 hh ${DESTDIR}/usr/games/lib/hackdir/hh
	rm -f ${DESTDIR}/usr/games/lib/hackdir/bones*

clean:	FRC
	rm -f .depend *.o hack makedefs

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${CSOURCES}

-include .depend

FRC:
