/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: emul/server/signal.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

/*
 * Setup signal actions
 */

char *signames[] =
{
    0,
    "HUP", "INT", "QUIT", "ILL", "TRAP", "IOT", "EMT", "FPE",
    "KILL", "BUS", "SEGV", "SYS", "PIPE", "ALRM", "TERM", "URG",
    "STOP", "TSTP", "CONT", "CHLD", "TTIN", "TTOUT", "IO", "XCPU",
    "XFSZ", "VTALRM", "PROF", "WINCH", "29", "USR1", "USR2",
};

#define	ndef	SIG_DFL

#define ign	SIG_IGN

int (*emul_sig_actions[])() = {
    0,
    ign, ign, ign, ndef, ndef, ndef, ndef, ndef,
    ndef, ndef, ndef, ndef, ndef, ndef, ign, ndef,
    ign, ign, ign, ign, ndef, ndef, ndef, ndef,
    ndef, ndef, ndef, ign, ndef, ndef, ndef,
};

int	fatal();
int	stop();

int (*server_sig_actions[])() = {
    0,
    fatal, stop, stop, fatal, fatal, fatal, fatal, fatal,
    fatal, fatal, fatal, fatal, fatal, fatal, fatal, fatal,
    stop, stop, ign, ign, stop, stop, fatal, fatal,
    fatal, fatal, fatal, ign, ndef, ndef, ndef,
};

/*
 * Initialize signal handlers
 */
signal_init(server)
{
    struct sigvec	sv;
    register		sig;
    register		(**sigrouts)();

    if (server)
	sigrouts = server_sig_actions;
    else
	sigrouts = emul_sig_actions;

    sv.sv_mask = 0; sv.sv_flags = 0;

    for (sig = SIGHUP; sig < NSIG; sig++) {
	sv.sv_handler = sigrouts[sig];
	sigvec(sig, &sv, 0);
    }
}

fatal(sig, code, scp)
int			sig, code;
struct sigcontext	*scp;
{
    fprintf(stderr, "Fatal signal %s code %x at PC %08x\n",
	    signames[sig], code, scp->sc_pc);

    other_error_exit("emulator terminated");
}
