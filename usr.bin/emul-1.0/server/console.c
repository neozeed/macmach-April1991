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
 *	File: emul/server/console.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"

#include <cthreads.h>

#include <sys/file.h>
#include <sys/ioctl.h>

/*
 * These routines redirect
 * console output to a file.
 * Probably will change with MACH 3.0
 */

cthread_t	console_cthread;

int	log;

int		pty = -1, tty = -1;
static char	ttyname[MAXPATHLEN+1];
char		logfilename[MAXPATHLEN+1];

any_t	console_loop();

console_init()
{
    register		n;
    register char	l;
    int		fd;
    char	ptyname[MAXPATHLEN+1];

    for (l = 'p'; l <= 't'; l++) {
	for (n = 0; n <= 0xf; n++) {
	    sprintf(ptyname, "/dev/pty%c%1x", l, n);
	    pty = open(ptyname, O_RDWR);
	    
	    if (pty >= 0) {
		sprintf(ttyname, "/dev/tty%c%01x", l, n);
		fd = open(ttyname, O_RDWR);
		if (fd < 0) {
		    close(pty);
		    pty = -1;
		}
		else {
		    close(fd);
		    break;
		}
	    }
	}

	if (pty >= 0)
	    break;
    }

    if (pty < 0)
	return;

    tty_disown();

    tty = open(ttyname, O_RDWR);
    if (tty < 0) {
	close(pty);
	pty = -1;
	return;
    }

    if (logfilename[0] == '\0')
	sprintf(logfilename, "/tmp/console.%d", getpid());

    log = open(logfilename, O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (log < 0) {
	close(pty);
	pty = -1;
    }
}

tty_disown()
{
    int		fd;

    fd = open("/dev/tty", O_RDWR);
    if (fd < 0)
	return;

    ioctl(fd, TIOCNOTTY, 0);

    close(fd);
}

console_redirect()
{
    int		arg = TIOCVCONS_ENABLE;

    if (nolog)
	return;

    if (console_cthread == NO_CTHREAD) {
	cthread_init();

	console_init();
	if (pty < 0)
	    return;

	console_cthread = cthread_fork(console_loop, 0);
	cthread_detach(console_cthread);
    }

    if (ioctl(tty, TIOCVCONS, &arg) < 0)
	perror("ioctl TIOCVCONS enable");
}

console_restore()
{
    int		arg = TIOCVCONS_DISABLE;

    if (pty < 0)
	return;

    if (ioctl(tty, TIOCVCONS, &arg) < 0)
	perror("ioctl TIOCVCONS disable");
}

any_t
console_loop()
{
    char	buf[256];
    int		n;

    for (;;) {
	n = read(pty, buf, sizeof (buf));
	if (n > 0)
	    write(log, buf, n);
	else {
	    console_restore();
	    break;
	}
#ifdef notdef
	fsync(log);
#endif
    }
    cthread_exit(0);
}
