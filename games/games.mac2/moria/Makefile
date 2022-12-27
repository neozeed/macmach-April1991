# /usr/src/games/games.mac2/moria/Makefile for MacMach

# BINDIR is the directory where the moria binary while be put
# LIBDIR is where the other files (scoreboard, news, hours) will be put
#  LIBDIR must be the same directory defined in config.h
BINDIR = ${DESTDIR}/usr/games
LIBDIR = ${DESTDIR}/usr/games/lib/moria

CFLAGS = -O -DKEY_BINDINGS=ROGUE_LIKE

SRCS = main.c misc1.c misc2.c store1.c files.c io.c create.c desc.c\
	generate.c sets.c dungeon.c creature.c death.c eat.c help.c magic.c\
	potions.c prayer.c save.c staffs.c wands.c scrolls.c spells.c\
	wizard.c store2.c signals.c moria1.c moria2.c monsters.c\
	treasure1.c treasure2.c variables.c

OBJS = main.o misc1.o misc2.o store1.o files.o io.o create.o desc.o\
	generate.o sets.o dungeon.o creature.o death.o eat.o help.o magic.o\
	potions.o prayer.o save.o staffs.o wands.o scrolls.o spells.o\
	wizard.o store2.o signals.o moria1.o moria2.o monsters.o\
  	treasure1.o treasure2.o variables.o

all:	moria .depend

moria:	$(OBJS)
	cc -o moria $(CFLAGS) $(OBJS) -lm -lcurses -ltermcap

install: FRC
	cp moria $(BINDIR)
	-[ -d $(LIBDIR) ] || mkdir $(LIBDIR)
	cp Moria_hours $(LIBDIR)
	cp Moria_news $(LIBDIR)
	cp Highscores $(LIBDIR)
	chmod 4555 $(BINDIR)/moria
	chmod 644 $(LIBDIR)/Highscores
	chmod 444 $(LIBDIR)/Moria_news
	chmod 444 $(LIBDIR)/Moria_hours
	chmod 755 $(LIBDIR)

clean:	FRC
	rm -f .depend *.o moria

.depend:
	@echo "#`date`" >.depend
	mkdep -p -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
