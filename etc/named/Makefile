# /usr/src/etc/named/Makefile for MacMach

RES=
DEFINES= -DDEBUG -DSTATS
### -DALLOW_T_UNSPEC -Dmalloc=rt_malloc -Dfree=rt_free
### ALLOC=storage.o
CFLAGS=	-O ${DEFINES}
LDFLAGS= ${RES}
LIBC=	/lib/libc.a
SRCS=	db_dump.c db_load.c db_lookup.c db_reload.c db_save.c db_update.c \
	ns_forw.c ns_init.c ns_main.c ns_maint.c ns_req.c ns_resp.c \
	ns_sort.c ns_stats.c
HDRS=	db.h ns.h
OBJS=	db_dump.o db_load.o db_lookup.o db_reload.o db_save.o db_update.o \
	ns_forw.o ns_init.o ns_main.o ns_maint.o ns_req.o ns_resp.o \
	ns_sort.o ns_stats.o $(ALLOC)
SUBDIR=	tools

all:	named .depend

named:	${OBJS} ${LIBC}
	-sh newvers.sh
	${CC} -o $@ version.c ${OBJS} ${LDFLAGS}

install: FRC
	rm -f ${DESTDIR}/etc/named ${DESTDIR}/usr/etc/named
	install -c -s -o bin -g bin -m 755 named ${DESTDIR}/usr/etc/named
	ln -s ../usr/etc/named ${DESTDIR}/etc/named

clean:	FRC
	rm -f .depend ${OBJS} core named version.o version.c
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done

.depend:
	@echo "#`date`" >.depend
	mkdep -f .depend ${CFLAGS} ${SRCS}

-include .depend

FRC:
