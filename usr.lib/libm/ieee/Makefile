# /usr/src/usr.lib/libm/ieee/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	cabs.c cbrt.c support.c
OBJS=	cabs.o cbrt.o support.o

.c.o:
	${CC} -p ${CFLAGS} -c $*.c
	-ld -X -r $*.o
	mv a.out ../profiled/$*.o
	${CC} ${CFLAGS} -c $*.c
	-ld -x -r $*.o
	mv a.out $*.o

all:	../libm.a ../libm_p.a .depend

../libm.a ../libm_p.a: ${OBJS}
	cd ../profiled; ar cru ../libm_p.a ${OBJS}
	ar cru ../libm.a ${OBJS}

clean:	FRC
	rm -f .depend ${OBJS} a.out

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
