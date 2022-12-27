# /usr/src/usr.lib/libm/common/Makefile for MacMach

CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	atan2.c sincos.c tan.c
OBJS=	atan2.o sincos.o tan.o

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

tags:	FRC
	ctags ${SRCS}

clean:	FRC
	rm -f .depend ${OBJS} a.out

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
