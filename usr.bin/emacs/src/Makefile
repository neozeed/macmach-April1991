# /usr/src/usr.bin/emacs/src/Makefile for MacMach

CPP = cc -E
MAKE = make  # BSD doesn't have it as a default.
#Note: an alternative is  CPP = /lib/cpp

all:	xmakefile doall

doall:	FRC
	$(MAKE) $(MAKEOVERRIDES) -f xmakefile ${MFLAGS} all

#This is used in making a distribution.
#Do not use it on development directories!
distclean:	FRC
	-rm -f paths.h config.h emacs-* temacs xemacs xmakefile core \
   *~ \#* *.o libXMenu11.a log*

clean:	FRC
	-rm -f temacs xemacs xmakefile core \#* *.o libXMenu11.a

xemacs: xmakefile doxemacs

doxemacs:	FRC
	$(MAKE) $(MAKEOVERRIDES) -f xmakefile ${MFLAGS} xemacs

temacs: xmakefile dotemacs

dotemacs:	FRC
	$(MAKE) $(MAKEOVERRIDES) -f xmakefile ${MFLAGS} temacs

# If you have a problem with cc -E here, changing
# the definition of CPP above may fix it.
xmakefile: ymakefile config.h
	rm -f xmakefile
	cp ymakefile junk.c
	$(CPP) junk.c | sed -e 's/^#.*//' -e 's/^[ \f\t][ \f\t]*$$//' -e 's/^ /	/' | \
	sed -n -e '/^..*$$/p' > xmakefile
	rm -f junk.c

tags:	FRC
	etags [a-z]*.h [a-z]*.c ../lisp/[a-z]*.el

FRC:
