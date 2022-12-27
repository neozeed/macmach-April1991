# /usr/src/bin/as/as.mac2/Makefile for MacMach

# This Makefile is a wrapper for the standard GNU Makefile.
# Note that ../gas should be a link to the appropriate gas (like gas-1.37).

DESTDIR= 

all:	FRC
	cd ../gas; make ${MFLAGS} a68

install: FRC
	cd ../gas; install -c -s -o bin -g bin -m 755 a68 ${DESTDIR}/bin/gas
	rm -f ${DESTDIR}/bin/as
	ln -s gas ${DESTDIR}/bin/as

clean:	FRC
	cd ../gas; make ${MFLAGS} clean

FRC:
