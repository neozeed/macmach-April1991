# /usr/src/old/uudiff/Makefile for MacMach

CFLAGS=-O

all:	uudiff

uudiff: lenrem locpart printable run1 trim

lenrem: lenrem.c
	cc $(CFLAGS) lenrem.c -o lenrem

locpart: locpart.c
	cc $(CFLAGS) locpart.c -o locpart

printable: printable.c
	cc $(CFLAGS) printable.c -o printable

run1: run1.c
	cc $(CFLAGS) run1.c -o run1

trim: trim.c
	cc $(CFLAGS) trim.c -o trim

install:	FRC
	: stuff lives here!

clean:	FRC
	rm -f *.o lenrem locpart printable run1 trim

FRC:
