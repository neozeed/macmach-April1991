#if defined(LIBC_SCCS)
	.asciz "@(#)alloca.s MacMach 5.1"
	.even
#endif /* LIBC_SCCS and not lint */

/* allocate bytes in current stack frame */

#include "DEFS.h"

ENTRY(alloca)
	movl    sp@,a0		/* save return addr */
	movl    sp@(4),d0	/* align on longword boundary */
	addqw   #3,d0
	andl	#0xfffffffc,d0
	subl    d0,a7		/* adjust stack */
	movl    sp,d0
	addqw   #8,d0		/* remember ret addr and argument */
	jmp     a0@
