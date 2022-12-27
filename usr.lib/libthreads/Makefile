# /usr/src/usr.lib/libthreads/Makefile for MacMach

CFLAGS=	-O -DMTHREAD

SRCS=	cprocs.c cthreads.c fork.c ipc.c malloc.c \
	mig_support.c stack.c stack_base.c sync.c \
	thread.c time.c unix.c

# ipc.o for coroutines only
OBJS=	cprocs.o cthreads.o malloc.o mig_support.o \
	stack.o sync.o time.o unix.o fork.o \
	stack_base.o thread.o lock.o

all:	libthreads libthreads_p .depend

libthreads libthreads_p: ${OBJS}
	@echo building profiled libthreads
	@cd profiled; ar cu ../libthreads_p ${OBJS}
	ranlib libthreads_p
	@echo building normal libthreads
	@ar cu libthreads ${OBJS}
	ranlib libthreads

threadtest: threadtest.c libthreads
	cc -O -c threadtest.c
	ld -o threadtest \
	  /lib/crt0.o \
	  threadtest.o \
	  ./libthreads \
	  /usr/lib/libmach.a \
	  /lib/libc.a
	rm threadtest.o

.S.o:
	@cc -c -DPROF ${CFLAGS} $*.S
	@ld -x -o profiled/$*.o -r $*.o
	cc ${CFLAGS} -c $*.S
	@ld -x -r $*.o
	@mv a.out $*.o

.c.o:
	@cc -S -c -pg ${CFLAGS} $*.c
	@awk -f cthread_inline.awk $*.s > $*.S; rm $*.s
	@cc -c $*.S; rm $*.S
	@ld -x -o profiled/$*.o -r $*.o
	cc -S ${CFLAGS} -c $*.c
	@awk -f cthread_inline.awk $*.s > $*.S; rm $*.s
	@cc -c $*.S; rm $*.S
	@ld -x -r $*.o
	@mv a.out $*.o

install: FRC
	install -c -o bin -g bin -m 644 libthreads ${DESTDIR}/usr/lib/libthreads.a
	ranlib ${DESTDIR}/usr/lib/libthreads.a
	install -c -o bin -g bin -m 644 libthreads_p ${DESTDIR}/usr/lib/libthreads_p.a
	ranlib ${DESTDIR}/usr/lib/libthreads_p.a

clean:	FRC
	rm -f .depend *.s ${OBJS} profiled/*.o libthreads libthreads_p threadtest

tags:	FRC
	ctags ${SRCS}

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
