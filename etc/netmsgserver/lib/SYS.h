/* /usr/src/etc/netmsgserver/lib/SYS.h */

/* similar to /usr/src/lib/libc/mac2/sys/SYS.h */

#define MACH_VMTP 1

#include <sys/syscall.h>

/* macros to profile assembly functions */

#ifdef	__STDC__
#ifdef	PROF
		    .globl  mcount
#define _MCOUNT	    link a6,\#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; \
		    .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,\#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; \
		    .text; jsr mcount; unlk a6
#else	/* PROF */
#define _MCOUNT
#define _MCOUNT2
#endif	/* PROF */
#define	ENTRY(x)    .globl _##x; _##x: _MCOUNT
#define	ENTRY2(x)    .globl _##x; _##x: _MCOUNT2
#else	/* __STDC__ */
#ifdef PROF
		    .globl  mcount
#define _MCOUNT	    link a6,#0; lea mcntr,a0; \
		    .data; .even; mcntr: .long 0; \
		    .text; jsr mcount; unlk a6
#define _MCOUNT2    link a6,#0; lea mcntr2,a0; \
		    .data; .even; mcntr2: .long 0; \
		    .text; jsr mcount; unlk a6
#else	/* PROF */
#define _MCOUNT
#define _MCOUNT2
#endif	/* PROF */
#define	ENTRY(x)    .globl _/**/x; _/**/x: _MCOUNT
#define	ENTRY2(x)   .globl _/**/x; _/**/x: _MCOUNT2
#endif	/* __STDC__ */

/* macros to make system calls with error returns */
#ifdef	__STDC__
#define	SYSCALL(x)  .globl cerror; err: jmp cerror; \
		    ENTRY(x); pea SYS_##x; trap \#0; jcs err
#define	PSEUDO(x,y) ENTRY(x); pea SYS_##y; trap \#0
#else	/* __STDC__ */
#define	SYSCALL(x)  .globl cerror; err: jmp cerror; \
		    ENTRY(x); pea SYS_/**/x; trap #0; jcs err
#define	PSEUDO(x,y) ENTRY(x); pea SYS_/**/y; trap #0
#endif	/* __STDC__ */

