/* $Header: /afs/andrew.cmu.edu/usr24/zon/mac2_51/new/patch/RCS/inp.h,v 1.1 89/10/12 15:41:06 zon Exp Locker: zon $
 *
 * $Log:	inp.h,v $
 * Revision 1.1  89/10/12  15:41:06  zon
 * Initial revision
 * 
 * Revision 2.0  86/09/17  15:37:25  lwall
 * Baseline for netwide release.
 * 
 */

EXT LINENUM input_lines INIT(0);	/* how long is input file in lines */
EXT LINENUM last_frozen_line INIT(0);	/* how many input lines have been */
					/* irretractibly output */

bool rev_in_string();
void scan_input();
bool plan_a();			/* returns false if insufficient memory */
void plan_b();
char *ifetch();

