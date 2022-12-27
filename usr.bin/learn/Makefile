# /usr/src/usr.bin/learn/Makefile for MacMach

# Flag BSD4_2 means 4.2 or later.
CFLAGS=	-O -DBSD4_2
LIBC=	/lib/libc.a
LLIB=	${DESTDIR}/usr/lib/learn/bin
SRCS=	copy.c dounit.c getlesson.c learn.c list.c mem.c makpipe.c maktee.c \
	mysys.c selsub.c selunit.c start.c whatnow.c wrapup.c
OBJS=	copy.o dounit.o getlesson.o learn.o list.o mem.o makpipe.o maktee.o \
	mysys.o selsub.o selunit.o start.o whatnow.o wrapup.o

all:	learn lrntee lcount getline.o getnum.o .depend

learn:	${OBJS} ${LIBC}
	${CC} -o $@ ${CFLAGS} ${OBJS}

lcount: lcount.c ${LIBC}
	${CC} ${CFLAGS} -o $@ lcount.c

lrntee: lrntee.c ${LIBC}
	${CC} ${CFLAGS} -o $@ lrntee.c

install: FRC
	-[ -d ${LLIB} ] || mkdir ${LLIB}
	chmod 755 ${LLIB}
	install -c -s -o bin -g bin -m 755 learn ${DESTDIR}/usr/bin/learn
	install -c -s -o bin -g bin -m 755 learn ${LLIB}/learn
	install -c -s -o bin -g bin -m 755 lrntee ${LLIB}/lrntee
	install -c -s -o bin -g bin -m 755 lcount ${LLIB}/lcount
	install -c -o bin -g bin -m 644 getline.o ${LLIB}/getline.o
	install -c -o bin -g bin -m 644 getnum.o ${LLIB}/getnum.o
# assume LLIB is ${DESTDIR}/usr/lib/learn/bin
	cd ${LLIB}; rm -f ../C/get*.o; \
	  ln -s ../bin/getline.o ../C/getline.o; \
	  ln -s ../bin/getnum.o ../C/getnum.o
	install -c -o bin -g bin -m 644 getline.c ${LLIB}/../C/getline.c
	install -c -o bin -g bin -m 644 getnum.c ${LLIB}/../C/getnum.c
	chmod 755 ${LLIB}/../*/Init

clean:	FRC
	rm -f .depend ${OBJS} core learn lrntee lcount getline.o getnum.o

.depend: lcount.c lrntee.c getline.c getnum.c
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS} lcount.c lrntee.c getline.c getnum.c

-include .depend

FRC:
