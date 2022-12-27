# Makefile for g++ library test files

# Copyright (C) 1988 Free Software Foundation
#   written by Doug Lea (dl@rocky.oswego.edu)

# This file is part of GNU CC.

# GNU CC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.  No author or distributor
# accepts responsibility to anyone for the consequences of using it
# or for whether it serves any particular purpose or works at all,
# unless he says so in writing.  Refer to the GNU CC General Public
# License for full details.

# Everyone is granted permission to copy, modify and redistribute
# GNU CC, but only under the conditions described in the
# GNU CC General Public License.   A copy of this license is
# supposed to have been given to you along with GNU CC so you
# can know your rights and responsibilities.  It should be in a
# file named COPYING.  Among other things, the copyright notice
# and this notice must be preserved on all copies.

# make parameters -- these should normally be inherited from parent make

# ------ source locations

# source directory for libg++.a
SRCDIR = ../src

# source include directory 
SRCIDIR= ../g++-include

# ------ installation destinations
# ------ You will require write-permission on the destination directories
# ------ in order to `make install'


prefix =/usr/gnu

# libg++.a destination
LIBDIR = $(prefix)/lib

# executables directory: location to install the genclass class generator
BINDIR = $(prefix)/bin

# location to install include file directory
IDIR = $(prefix)/lib/g++-include


# ------- System-dependent defines
# ------- use the second form of each for SystemV (USG)

# g++ flags
OSFLAG=
#OSFLAG = -DUSG

# ld or ld++ flags
OSLDFLAG =
#OSLDFLAG= -lPW

# how to install
INSTALL=install -c
#INSTALL=cp

# ranlib if necessary
RANLIB=ranlib
#RANLIB=echo

# which make?
MAKE=make

#which ar?
AR=ar

# not used, but convenient for those who preprocess things while compiling
SHELL=/bin/sh


# ------ compiler names

# GNU C++ compiler name
GXX = g++
#GXX=gcc

# GNU CC compiler name (needed for some .c files in libg++.a)
CC = gcc

# GNU loader
LDXX = $(LIBDIR)/gcc-ld -L$(SRCDIR)

# crt0+.o location (for dynamic loading tests)
GXXCRT1=$(LIBDIR)/crt1+.o

# ------ Other compilation flags
# ------ modify as you like -- the ones here are sheer overkill

GXX_OPTIMIZATION_FLAGS= -O -fstrength-reduce  -felide-constructors -fschedule-insns -fdelayed-branch  -fsave-memoized 
#GXX_OPTIMIZATION_FLAGS=

GCC_OPTIMIZATION_FLAGS= -O -fstrength-reduce -fdelayed-branch 

DEBUG_FLAGS= -g

#use this only if you like to look at lots of useless messages
VERBOSITY_FLAGS= -Wall -v

GXX_INCLUDE_DIRS= -I$(SRCIDIR)

GCC_INCLUDE_DIRS= -I$(prefix)/lib/gcc-include -I/usr/include -I$(SRCIDIR)

PIPE_AS= -pipe

# Flags for all C++ compiles
GXXFLAGS = $(OSFLAG) $(GXX_INCLUDE_DIRS) $(DEBUG_FLAGS) $(GXX_OPTIMIZATION_FLAGS) $(VERBOSITY_FLAGS) $(PIPE_AS)

# Flags for all C compiles
CFLAGS= $(OSFLAG) $(GCC_INCLUDE_DIRS) $(DEBUG_FLAGS) $(GCC_OPTIMIZATION_FLAGS) $(VERBOSITY_FLAGS) $(PIPE_AS)

# g++ load time flags 
GXXLDFLAGS = $(VERBOSITY_FLAGS) -L$(SRCDIR) -lg++ -lm $(OSLDFLAG)

# these flags tell test0 where ld++ and crt1+.o are
TFLAGS = -DLDXX='"$(LDXX)"' -DCRT1X='"$(GXXCRT1)"'

# Comment out the next line to disable incremental linking test
#TEST0=test0
TEST0=

# g++ files should have extension .cc
.SUFFIXES: .cc
.cc.o:
	$(GXX) $(GXXFLAGS) -c  $<

PROTODIR=../g++-include/gen
GENCLASS=../genclass

all: checktests


# compilation notes:
# The following g++ warnings are expected to appear:
#
# 2. Several warnings from genclass about classes with only .h, no .cc files
#
# 3. A few harmless reminder warnings that some classes have no data members.
#
# 4. Many harmless warnings that arguments to vector-delete are unnecessary
#    when deleting arrays of builtin types.

# test files
TSRCS =  tFile.cc tObstack.cc tString.cc \
 tInteger.cc tRational.cc \
 tComplex.cc tBitSet.cc tBitString.cc tRandom.cc tList.cc tPlex.cc tLList.cc \
 tVec.cc tStack.cc tQueue.cc tDeque.cc tPQ.cc tSet.cc  tBag.cc \
 tMap.cc tFix.cc tFix16.cc tFix24.cc \
 test.hello.cc test.bye.cc test.bye2.cc test.shell.cc \
 tCurses.cc tGetOpt.cc

# executables
TOUTS =  $(TEST0) tFile tObstack tString \
  tInteger  tRational tComplex tBitSet tBitString \
  tRandom tFix tFix16 tFix24 tCurses tGetOpt \
  tList tPlex tLList tVec tStack tQueue tDeque tPQ tSet \
  tBag  tMap 

# files for archived prototype classes
LOBJS = \
 iList.o iSLList.o iDLList.o iVec.o iAVec.o \
 iPlex.o  iFPlex.o  iXPlex.o iRPlex.o iMPlex.o \
 iSet.o iBag.o iMap.o iPQ.o \
 iXPSet.o  iOXPSet.o  iSLSet.o  iOSLSet.o  iBSTSet.o \
 iAVLSet.o  iSplaySet.o  iVHSet.o  iVOHSet.o  iCHSet.o \
 iXPBag.o  iOXPBag.o  iSLBag.o  iOSLBag.o  iSplayBag.o \
 iVHBag.o  iCHBag.o \
 iVHMap.o  iCHMap.o  iSplayMap.o  iAVLMap.o \
 iRAVLMap.o \
 iSplayPQ.o  iPHPQ.o  iXPPQ.o \
 iVStack.o \
 iVQueue.o iStack.o iQueue.o iDeque.o \
 iXPStack.o iSLStack.o \
 iXPQueue.o  iSLQueue.o \
 iXPDeque.o iDLDeque.o


LHDRS =  idefs.h 

tests: $(SRCDIR)/libg++.a $(GENCLASS) $(TOUTS)

runtests: tests
	for i in $(TOUTS); do \
		case $$i in \
			test0) echo "Must run test0 from tty";; \
			tCurses) echo "Must run tCurses from tty" ;; \
			tGetOpt) echo $$i;  ./$$i -abc -de10 -2000 -h3i ;; \
			*) echo $$i;  ./$$i < $$i.inp;; \
		esac \
	done

checktests: tests
	-$(MAKE) -s runtests > my.out 2>&1
	-diff -b expected.out my.out > checktests.diffs
	cat checktests.diffs

	
libtest.a: $(LHDRS) $(LOBJS)
	rm -f libtest.a
	$(AR) r libtest.a $(LOBJS)
	$(RANLIB) libtest.a

test0: test0.h  test.hello.o test.bye test.bye2 test.shell
	$(GXX)  test.hello.o $(GXXLDFLAGS) -o test0

test.hello.o: test0.h test.hello.cc
	$(GXX) -c $(GXXFLAGS) $(TFLAGS) test.hello.cc -o test.hello.o

test.bye: test0.h test.bye.cc
	$(GXX) -c $(GXXFLAGS) $(TFLAGS) test.bye.cc -o test.bye

test.bye2: test0.h test.bye2.cc
	$(GXX) -c $(GXXFLAGS) $(TFLAGS) test.bye2.cc -o test.bye2

test.shell: test0.h test.shell.cc
	$(GXX) -c $(GXXFLAGS) $(TFLAGS) test.shell.cc -o test.shell


tFile:  tFile.o
	$(GXX)  tFile.o  -o tFile $(GXXLDFLAGS)

tObstack:  tObstack.o
	$(GXX)  tObstack.o  -o tObstack  $(GXXLDFLAGS)

tString:  tString.o
	$(GXX)  tString.o  -o tString  $(GXXLDFLAGS)

tInteger:  tInteger.o
	$(GXX)  tInteger.o  -o tInteger  $(GXXLDFLAGS)

twrapper:  twrapper.o
	$(GXX)  twrapper.o  -o twrapper  $(GXXLDFLAGS)

tgwrapper:   tgwrapper.o
	$(GXX)  tgwrapper.o  -o tgwrapper  $(GXXLDFLAGS)

tRational:  tRational.o
	$(GXX)  tRational.o  -o tRational  $(GXXLDFLAGS)

tComplex:  tComplex.o
	$(GXX)  tComplex.o  -o tComplex  $(GXXLDFLAGS)

tBitSet:  tBitSet.o
	$(GXX)  tBitSet.o  -o tBitSet  $(GXXLDFLAGS)

tBitString:  tBitString.o
	$(GXX)  tBitString.o  -o tBitString  $(GXXLDFLAGS)

tRandom:  tRandom.o
	$(GXX)  tRandom.o  -o tRandom  $(GXXLDFLAGS)

tFix:  tFix.o
	$(GXX)  tFix.o  -o tFix  $(GXXLDFLAGS)

tFix16:  tFix16.o
	$(GXX)  tFix16.o  -o tFix16  $(GXXLDFLAGS)

tFix24:  tFix24.o
	$(GXX)  tFix24.o  -o tFix24  $(GXXLDFLAGS)

tCurses:  tCurses.o
	$(GXX)  tCurses.o  -o tCurses -lg++ -lcurses -ltermcap $(GXXLDFLAGS)

tGetOpt:  tGetOpt.o
	$(GXX)  tGetOpt.o  -o tGetOpt  $(GXXLDFLAGS)

tList:  libtest.a  tList.o
	$(GXX)  tList.o -o tList -L. -ltest  $(GXXLDFLAGS)

tPlex:  libtest.a  tPlex.o
	$(GXX)  tPlex.o -o tPlex -L. -ltest  $(GXXLDFLAGS)

tLList:  libtest.a tLList.o
	$(GXX)  tLList.o -o tLList -L. -ltest  $(GXXLDFLAGS)

tVec: libtest.a tVec.o
	$(GXX)  tVec.o -o tVec -L. -ltest  $(GXXLDFLAGS)

tStack: libtest.a tStack.o
	$(GXX)  tStack.o -o tStack -L. -ltest  $(GXXLDFLAGS)

tQueue: libtest.a tQueue.o
	$(GXX)  tQueue.o -o tQueue -L. -ltest  $(GXXLDFLAGS)

tDeque: libtest.a tDeque.o
	$(GXX)  tDeque.o -o tDeque -L. -ltest  $(GXXLDFLAGS)

tPQ: libtest.a tPQ.o
	$(GXX)  tPQ.o -o tPQ -L. -ltest  $(GXXLDFLAGS)

tSet: libtest.a tSet.o
	$(GXX)  tSet.o -o tSet -L. -ltest  $(GXXLDFLAGS)

tBag:  libtest.a tBag.o
	$(GXX)  tBag.o -o tBag -L. -ltest  $(GXXLDFLAGS)

tMap:  libtest.a tMap.o
	$(GXX)  tMap.o -o tMap -L. -ltest  $(GXXLDFLAGS)

idefs.h:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val defs i
iSet.h iSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Set i
iXPSet.h iXPSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPSet i
iOXPSet.h iOXPSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val OXPSet i
iSLSet.h iSLSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SLSet i
iOSLSet.h iOSLSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val OSLSet i
iBSTSet.h iBSTSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val BSTSet i
iAVLSet.h iAVLSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val AVLSet i
iSplaySet.h iSplaySet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SplaySet i
iVHSet.h iVHSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val VHSet i
iVOHSet.h iVOHSet.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val VOHSet i
iCHSet.h iCHSet.cc : 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val CHSet i
iBag.h iBag.cc : 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Bag i
iXPBag.h iXPBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPBag i
iOXPBag.h iOXPBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val OXPBag i
iSLBag.h iSLBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SLBag i
iOSLBag.h iOSLBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val OSLBag i
iSplayBag.h iSplayBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SplayBag i
iVHBag.h iVHBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val VHBag i
iCHBag.h iCHBag.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val CHBag i
iMap.h iMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val Map i
iVHMap.h iVHMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val VHMap i
iCHMap.h iCHMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val CHMap i
iSplayMap.h iSplayMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val SplayMap i
iAVLMap.h iAVLMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val AVLMap i
iRAVLMap.h iRAVLMap.cc :
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) -2 int val int val RAVLMap i
iPQ.h iPQ.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val PQ i
iPHPQ.h iPHPQ.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val PHPQ i
iSplayPQ.h iSplayPQ.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SplayPQ i
iXPPQ.h iXPPQ.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPPQ i
iStack.h iStack.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Stack i
iQueue.h iQueue.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Queue i
iDeque.h iDeque.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Deque i
iSLStack.h iSLStack.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SLStack i
iSLQueue.h iSLQueue.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SLQueue i
iDLDeque.h iDLDeque.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val DLDeque i
iList.h iList.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val List i
iPlex.h iPlex.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Plex i
iFPlex.h iFPlex.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val FPlex i
iXPlex.h iXPlex.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPlex i
iMPlex.h iMPlex.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val MPlex i
iRPlex.h iRPlex.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val RPlex i
iFPStack.h iFPStack.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val FPStack i
iXPStack.h iXPStack.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPStack i
iFPQueue.h iFPQueue.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val FPQueue i
iXPQueue.h iXPQueue.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPQueue i
iXPDeque.h iXPDeque.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val XPDeque i
iSLList.h iSLList.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val SLList i
iDLList.h iDLList.cc: 
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val DLList i
iVec.h iVec.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val Vec i
iAVec.h iAVec.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val AVec i
iVStack.h iVStack.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val VStack i
iVQueue.h iVQueue.cc:
	PROTODIR=$(PROTODIR); export PROTODIR; $(GENCLASS) int val VQueue i

relink:
	(rm -f $(TOUTS); make tests)

clean:
	rm -f *.o core
	rm -f plot.out ffile streamfile i*.cc i*.h C++.hack* my.out checktests.diffs

realclean: clean
	rm -f test.bye test.bye2 test.shell libtest.a $(TOUTS) 

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

iAVLMap.o : iAVLMap.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iAVLMap.h iMap.h \
  $(SRCIDIR)/Pix.h idefs.h 
iAVLSet.o : iAVLSet.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iAVLSet.h iSet.h \
  $(SRCIDIR)/Pix.h idefs.h 
iAVec.o : iAVec.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iAVec.h iVec.h 
iBSTSet.o : iBSTSet.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iBSTSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h 
iBag.o : iBag.cc $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h iBag.h $(SRCIDIR)/Pix.h \
  idefs.h 
iCHBag.o : iCHBag.cc iCHBag.h iBag.h $(SRCIDIR)/Pix.h idefs.h 
iCHMap.o : iCHMap.cc iCHMap.h iMap.h $(SRCIDIR)/Pix.h idefs.h 
iCHSet.o : iCHSet.cc iCHSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h 
iDLList.o : iDLList.cc $(SRCIDIR)/values.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/streambuf.h \
  iDLList.h $(SRCIDIR)/Pix.h idefs.h 
iFPlex.o : iFPlex.cc iFPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/Pix.h idefs.h 
iList.o : iList.cc $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h iList.h $(SRCIDIR)/Pix.h 
iMPlex.o : iMPlex.cc iMPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/Pix.h idefs.h 
iMap.o : iMap.cc $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h iMap.h $(SRCIDIR)/Pix.h \
  idefs.h 
iOSLBag.o : iOSLBag.cc iOSLBag.h iBag.h $(SRCIDIR)/Pix.h \
  idefs.h iSLList.h 
iOSLSet.o : iOSLSet.cc iOSLSet.h iSet.h $(SRCIDIR)/Pix.h \
  idefs.h iSLList.h 
iOXPBag.o : iOXPBag.cc iOXPBag.h iBag.h $(SRCIDIR)/Pix.h \
  idefs.h iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h 
iOXPSet.o : iOXPSet.cc iOXPSet.h iSet.h $(SRCIDIR)/Pix.h \
  idefs.h iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h 
iPHPQ.o : iPHPQ.cc $(SRCIDIR)/values.h iPHPQ.h iPQ.h \
  $(SRCIDIR)/Pix.h idefs.h 
iPQ.o : iPQ.cc $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h iPQ.h $(SRCIDIR)/Pix.h \
  idefs.h 
iPlex.o : iPlex.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iPlex.h $(SRCIDIR)/Pix.h idefs.h 
iRAVLMap.o : iRAVLMap.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iRAVLMap.h iMap.h \
  $(SRCIDIR)/Pix.h idefs.h 
iRPlex.o : iRPlex.cc iRPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/Pix.h idefs.h 
iSLBag.o : iSLBag.cc iSLBag.h iBag.h $(SRCIDIR)/Pix.h idefs.h \
  iSLList.h 
iSLList.o : iSLList.cc $(SRCIDIR)/values.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/streambuf.h \
  iSLList.h $(SRCIDIR)/Pix.h idefs.h 
iSLSet.o : iSLSet.cc iSLSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h \
  iSLList.h 
iSet.o : iSet.cc $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h iSet.h $(SRCIDIR)/Pix.h \
  idefs.h 
iSplayBag.o : iSplayBag.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iSplayBag.h iBag.h \
  $(SRCIDIR)/Pix.h idefs.h 
iSplayMap.o : iSplayMap.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iSplayMap.h iMap.h \
  $(SRCIDIR)/Pix.h idefs.h 
iSplayPQ.o : iSplayPQ.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iSplayPQ.h iPQ.h \
  $(SRCIDIR)/Pix.h idefs.h 
iSplaySet.o : iSplaySet.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iSplaySet.h iSet.h \
  $(SRCIDIR)/Pix.h idefs.h 
iVHBag.o : iVHBag.cc iVHBag.h iBag.h $(SRCIDIR)/Pix.h idefs.h 
iVHMap.o : iVHMap.cc iVHMap.h iMap.h $(SRCIDIR)/Pix.h idefs.h 
iVHSet.o : iVHSet.cc iVHSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h 
iVOHSet.o : iVOHSet.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iVOHSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h 
iVQueue.o : iVQueue.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iVQueue.h iQueue.h idefs.h 
iVStack.o : iVStack.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iVStack.h iStack.h idefs.h 
iVec.o : iVec.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iVec.h 
iXPBag.o : iXPBag.cc iXPBag.h iBag.h $(SRCIDIR)/Pix.h idefs.h \
  iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h 
iXPPQ.o : iXPPQ.cc iXPPQ.h iPQ.h $(SRCIDIR)/Pix.h idefs.h \
  iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h 
iXPSet.o : iXPSet.cc iXPSet.h iSet.h $(SRCIDIR)/Pix.h idefs.h \
  iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h 
iXPlex.o : iXPlex.cc iXPlex.h iPlex.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/Pix.h idefs.h 
tBag.o : tBag.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iBag.h $(SRCIDIR)/Pix.h \
  idefs.h $(SRCIDIR)/MLCG.h $(SRCIDIR)/RNG.h \
  iXPBag.h iXPlex.h iPlex.h iSLBag.h iSLList.h iVHBag.h iCHBag.h iOXPBag.h \
  iOSLBag.h iSplayBag.h 
tBitSet.o : tBitSet.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/BitSet.h $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h 
tBitString.o : tBitString.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/BitString.h $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h 
tComplex.o : tComplex.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/Complex.h $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h 
tCurses.o : tCurses.cc $(SRCIDIR)/CursesW.h \
  $(SRCIDIR)/curses.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h 
tDeque.o : tDeque.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iDeque.h idefs.h iXPDeque.h iXPlex.h \
  iPlex.h $(SRCIDIR)/Pix.h iDLDeque.h iDLList.h 
tFile.o : tFile.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/SFile.h $(SRCIDIR)/PlotFile.h \
  $(SRCIDIR)/assert.h 
tFix.o : tFix.cc $(SRCIDIR)/Fix.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h $(SRCIDIR)/Integer.h 
tFix16.o : tFix16.cc $(SRCIDIR)/Fix16.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h 
tFix24.o : tFix24.cc $(SRCIDIR)/Fix24.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h 
tGetOpt.o : tGetOpt.cc $(SRCIDIR)/GetOpt.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/stdio.h 
tInteger.o : tInteger.cc $(SRCIDIR)/Integer.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h $(SRCIDIR)/assert.h 
tLList.o : tLList.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h iSLList.h \
  $(SRCIDIR)/Pix.h idefs.h iDLList.h 
tList.o : tList.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h iList.h $(SRCIDIR)/Pix.h \
  $(SRCIDIR)/MLCG.h $(SRCIDIR)/RNG.h 
tMap.o : tMap.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iMap.h $(SRCIDIR)/Pix.h \
  idefs.h $(SRCIDIR)/MLCG.h $(SRCIDIR)/RNG.h \
  iSplayMap.h iVHMap.h iCHMap.h iAVLMap.h iRAVLMap.h 
tObstack.o : tObstack.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h $(SRCIDIR)/Obstack.h \
  $(SRCIDIR)/ctype.h 
tPQ.o : tPQ.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iPQ.h $(SRCIDIR)/Pix.h \
  idefs.h $(SRCIDIR)/MLCG.h $(SRCIDIR)/RNG.h \
  iXPPQ.h iXPlex.h iPlex.h iPHPQ.h iSplayPQ.h 
tPlex.o : tPlex.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iPlex.h $(SRCIDIR)/Pix.h \
  idefs.h iFPlex.h iXPlex.h iMPlex.h iRPlex.h 
tQueue.o : tQueue.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iQueue.h idefs.h iXPQueue.h iXPlex.h \
  iPlex.h $(SRCIDIR)/Pix.h iVQueue.h iSLQueue.h iSLList.h 
tRandom.o : tRandom.cc $(SRCIDIR)/assert.h \
  $(SRCIDIR)/ACG.h $(SRCIDIR)/RNG.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/MLCG.h $(SRCIDIR)/SmplStat.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/SmplHist.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/streambuf.h $(SRCIDIR)/Binomial.h \
  $(SRCIDIR)/Random.h $(SRCIDIR)/Erlang.h \
  $(SRCIDIR)/Geom.h \
  $(SRCIDIR)/HypGeom.h \
  $(SRCIDIR)/NegExp.h $(SRCIDIR)/Normal.h \
  $(SRCIDIR)/LogNorm.h $(SRCIDIR)/Poisson.h \
  $(SRCIDIR)/Uniform.h \
  $(SRCIDIR)/DiscUnif.h \
  $(SRCIDIR)/Weibull.h 
tRational.o : tRational.cc $(SRCIDIR)/Rational.h \
  $(SRCIDIR)/Integer.h $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h 
tSet.o : tSet.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iSet.h $(SRCIDIR)/Pix.h \
  idefs.h $(SRCIDIR)/MLCG.h $(SRCIDIR)/RNG.h \
  iXPSet.h iXPlex.h iPlex.h iSLSet.h iSLList.h iVHSet.h iVOHSet.h iCHSet.h \
  iOXPSet.h iOSLSet.h iBSTSet.h iAVLSet.h iSplaySet.h 
tStack.o : tStack.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  $(SRCIDIR)/assert.h iStack.h idefs.h iXPStack.h iXPlex.h \
  iPlex.h $(SRCIDIR)/Pix.h iVStack.h iSLStack.h iSLList.h 
tString.o : tString.cc $(SRCIDIR)/String.h \
  $(SRCIDIR)/stream.h $(SRCIDIR)/File.h \
  $(SRCIDIR)/builtin.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/math.h $(SRCIDIR)/values.h \
  $(SRCIDIR)/streambuf.h $(SRCIDIR)/assert.h 
tVec.o : tVec.cc $(SRCIDIR)/stream.h \
  $(SRCIDIR)/File.h $(SRCIDIR)/builtin.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stdio.h $(SRCIDIR)/math.h \
  $(SRCIDIR)/values.h $(SRCIDIR)/streambuf.h \
  iVec.h iAVec.h $(SRCIDIR)/MLCG.h \
  $(SRCIDIR)/RNG.h $(SRCIDIR)/assert.h 
test.bye.o : test.bye.cc $(SRCIDIR)/Incremental.h test0.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/stdio.h 
test.bye2.o : test.bye2.cc $(SRCIDIR)/Incremental.h test0.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/stdio.h 
test.hello.o : test.hello.cc test0.h $(SRCIDIR)/std.h \
  $(SRCIDIR)/stddef.h $(SRCIDIR)/stdio.h \
  $(SRCIDIR)/sys/file.h $(SRCIDIR)/sys/types.h 
test.shell.o : test.shell.cc $(SRCIDIR)/Incremental.h test0.h \
  $(SRCIDIR)/std.h $(SRCIDIR)/stddef.h \
  $(SRCIDIR)/stdio.h 

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
