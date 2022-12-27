# 
# Mach Operating System
# Copyright (c) 1989 Carnegie-Mellon University
# Copyright (c) 1988 Carnegie-Mellon University
# Copyright (c) 1987 Carnegie-Mellon University
# All rights reserved.  The CMU software License Agreement specifies
# the terms and conditions for use and redistribution.
#
#
# HISTORY
# $Log:	Makefile,v $
# Revision 2.6  89/05/01  17:03:18  rpd
# 	Cleaned up, to match mach/Makefile.
# 	[89/05/01  14:53:13  rpd]
# 
# Revision 2.5  89/02/25  18:42:53  gm0w
# 	Changes for cleanup.
# 
# Revision 2.4  89/02/15  23:56:08  rpd
# 	Moved to mach_debug/ca/.
# 
# Revision 2.3  89/01/12  07:39:04  rpd
# 	Added rules for mach_debug_ca.defs.
# 	[89/01/12  04:27:46  rpd]
# 
# 09-May-87  Mike Accetta (mja) at Carnegie-Mellon University
#	Created (for archiving).
#	[ V5.1(XF11) ]
#
#

MIG=LPATH=$$LPATH:/usr/mach/lib PATH=$$PATH:/usr/mach/bin mig
MIGOPTS = -I../.. -DKERNEL
MIGKERNEL = -DKERNEL_SERVER
MIGUKERNEL = -DKERNEL_USER

MACH_DEBUG_CA_INTERFACE =  mach_debug_ca_server.c

all: $(MACH_DEBUG_CA_INTERFACE)

$(MACH_DEBUG_CA_INTERFACE):	mach_debug_ca.defs
	$(MIG) $(MIGOPTS) $(MIGKERNEL) -server mach_debug_ca_server.c -user /dev/null -header /dev/null mach_debug_ca.defs

clean:
	rm -f $(MACH_DEBUG_CA_INTERFACE)
