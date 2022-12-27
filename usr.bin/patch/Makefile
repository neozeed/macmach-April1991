# /usr/src/usr.bin/patch/Makefile for MacMach

CC = cc
bin = ${DESTDIR}/usr/bin
mansrc = ${DESTDIR}/usr/man/man1
manext = 1
CFLAGS =  -O
LDFLAGS = 

public = patch
private = 
manpages = patch.man
util = Makefile

c = patch.c pch.c inp.c version.c util.c

obj = patch.o pch.o inp.o util.o version.o

lintflags = -phbvxac

addedbyconf = Makefile.old bsd config.h config.sh eunice loc pdp11 usg v7

SHELL = /bin/sh

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all:	$(public) $(private) $(util)
	touch all

patch: $(obj)
	$(CC) $(LDFLAGS) $(obj) $(libs) -o patch

# won't work with csh
install: FRC
	export PATH || exit 1
	- mv $(bin)/patch $(bin)/patch.old
	- if test `pwd` != $(bin); then install -c -s $(public) $(bin); fi
	cd $(bin); chmod 755 $(public)
	- if test `pwd` != $(mansrc); then \
for page in $(manpages); do \
cp $$page $(mansrc)/`basename $$page .man`.$(manext); \
chmod 444 $(mansrc)/`basename $$page .man`.$(manext); \
done; \
fi

#/usr/man/manroff $$page > $(mansrc)/`basename $$page .man`.$(manext); \
#

clean:	FRC
	rm -f .depend *.o *.orig core

realclean:	FRC
	rm -f patch *.o *.orig core $(addedbyconf)

# The following lint has practically everything turned on.  Unfortunately,
# you have to wade through a lot of mumbo jumbo that can't be suppressed.
# If the source file has a /*NOSTRICT*/ somewhere, ignore the lint message
# for that spot.

patch.o: config.h common.h patch.c inp.h pch.h util.h version.h
pch.o: config.h common.h pch.c pch.h util.h
inp.o: config.h common.h inp.c inp.h util.h
util.o: config.h common.h util.c util.h
version.o: config.h common.h version.c version.h patchlevel.h util.h

FRC:
