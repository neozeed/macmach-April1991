/* $Header: /afs/andrew.cmu.edu/usr24/zon/mac2_51/new/patch/RCS/version.c,v 1.1 89/10/12 15:41:36 zon Exp Locker: zon $
 *
 * $Log:	version.c,v $
 * Revision 1.1  89/10/12  15:41:36  zon
 * Initial revision
 * 
 * Revision 2.0  86/09/17  15:40:11  lwall
 * Baseline for netwide release.
 * 
 */

#include "EXTERN.h"
#include "common.h"
#include "util.h"
#include "INTERN.h"
#include "patchlevel.h"
#include "version.h"

/* Print out the version number and die. */

void
version()
{
    extern char rcsid[];

#ifdef lint
    rcsid[0] = rcsid[0];
#else
    fatal3("%s\nPatch level: %d\n", rcsid, PATCHLEVEL);
#endif
}
