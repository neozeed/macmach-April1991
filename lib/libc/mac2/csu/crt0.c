/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if defined(VERSION) && !defined(lint)
/* VERSION is a identifying string to exist in all executables */
static char version[] = VERSION;
#endif

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)crt0.c	5.4 (Berkeley) 1/18/88";
#endif LIBC_SCCS and not lint

/*
 *	C start up routine.
 */

/* Define EXIT_ZERO if main() should ignore return value and exit zero */
#define EXIT_ZERO

/* Define MACH to do MACH initializations. */

#ifdef mac2

/*
  start() is jmp'ed to and the stack looks like:
	sp +0  [kargc]
	   +4  [kargv0]
  Since kargc is in the place that the compiler expects a return address,
  it can not be accessed as an argument to start().  The compiler sets up
  a stack frame for start().  The (argc, argv, envp) arguments to main()
  are allocated as local variables to start().  The stack then looks like:
	   -12  [argc]
	   -8   [argv]
	   -4   [envp]
	a6 +0   [old a6]
	   +4   [kargc]
	   +8   [kargv0]
  An asm() is used to get the value for argc that is at a6+4.  After argv
  and envp are loaded, another asm() is used to clear a6.  This prepares
  the linked list of frame pointers for a debugger.  Since its arguments
  are already on the stack, main() is called without arguments.  Note that
  the return value from main() is ignored.  The only way to get a non-zero
  exit value is with an explicit call to exit().
*/
#define MAIN main()

#else

/*
 *	Robert Henry, UCB, 20 Oct 81
 *
 *	We make the following (true) assumptions:
 *	1) when the kernel calls start, it does a jump to location 2,
 *	and thus avoids the register save mask.  We are NOT called
 *	with a calls!  see sys1.c:setregs().
 *	2) The only register variable that we can trust is sp,
 *	which points to the base of the kernel calling frame.
 *	Do NOT believe the documentation in exec(2) regarding the
 *	values of fp and ap.
 *	3) We can allocate as many register variables as we want,
 *	and don't have to save them for anybody.
 *	4) Because of the ways that asm's work, we can't have
 *	any automatic variables allocated on the stack, because
 *	we must catch the value of sp before any automatics are
 *	allocated.
 */
#define MAIN main(kfp->kargc, argv, environ)

#endif

#ifdef MACH
/* MACH function pointers are initialized non-zero by the MACH libraries */
void (*mach_init_routine)();
void (*_cthread_init_routine)();
int (*_cthread_exit_routine)();
void (*_StrongBox_init_routine)();
#endif

char **environ = (char **)0;
static int fd;

extern	unsigned char	etext;
extern	unsigned char	eprol; /* Not really extern, see asm() below. */

#ifdef mac2
static start(kargv0)
char *kargv0;
#else
start()
#endif
{
#ifdef mac2
	char **envp;
	char **argv;
	int argc;
#else
	struct kframe {
		int	kargc;
		char	*kargv[1];	/* size depends on kargc */
		char	kargstr[1];	/* size varies */
		char	kenvstr[1];	/* size varies */
	};
	/*
	 *	ALL REGISTER VARIABLES!!!
	 */
	register int r11;		/* needed for init */
	register struct kframe *kfp;	/* r10 */
	register char **targv;
	register char **argv;
#endif
	extern int errno;

#ifdef lint
#ifdef mac2
	envp = 0;
	argv = envp;
	argc = 0;
#else
	kfp = 0;
	initcode = initcode = 0;
#endif
#else not lint
#ifndef mac2
	asm("	movl	sp,r10");	/* catch it quick */
#endif
#endif not lint
#ifdef mac2
	asm("	movl a6@(4),a6@(-12)");
	argv = &kargv0;
	environ = envp = &argv[argc + 1];
	asm("	lea 0,a6");
#else
	for (argv = targv = &kfp->kargv[0]; *targv++; /* void */)
		/* void */ ;
	if (targv >= (char **)(*argv))
		--targv;
	environ = targv;
#endif

#ifdef MACH
	if (mach_init_routine) (*mach_init_routine)();
#endif

asm("_eprol:");

#ifdef paranoid
	/*
	 * The standard I/O library assumes that file descriptors 0, 1, and 2
	 * are open. If one of these descriptors is closed prior to the start 
	 * of the process, I/O gets very confused. To avoid this problem, we
	 * insure that the first three file descriptors are open before calling
	 * main(). Normally this is undefined, as it adds two unnecessary
	 * system calls.
	 */
	do	{
		fd = open("/dev/null", 2);
	} while (fd >= 0 && fd < 3);
	close(fd);
#endif paranoid

#ifdef MCRT0
	monstartup(&eprol, &etext);
#endif MCRT0

#ifdef MACH
	if (_cthread_init_routine) (*_cthread_init_routine)();
	if (_StrongBox_init_routine) (*_StrongBox_init_routine)();
#endif

	errno = 0;

#ifdef EXIT_ZERO

	(void) MAIN;
#ifdef MACH
	if (_cthread_exit_routine) _exit((*_cthread_exit_routine)(0));
#endif
	exit(0);

#else

#ifdef MACH
	if (_cthread_exit_routine) _exit((*_cthread_exit_routine)(MAIN));
#endif
	exit(MAIN);

#endif

}

#ifdef MCRT0
/*ARGSUSED*/
exit(code)
	register int code;	/* r11 */
{
	monitor(0);
	_cleanup();
	_exit(code);
}
#endif MCRT0

#ifdef CRT0
/*
 * null mcount and moncontrol,
 * just in case some routine is compiled for profiling
 */
moncontrol(val)
	int val;
{

}
#ifdef mac2
asm("	.text");
asm("	.globl	mcount");
asm("mcount:	rts");
#else
asm("	.globl	mcount");
asm("mcount:	rsb");
#endif
#endif CRT0

/* change _start to start */
asm(".set start, _start; .globl start");
