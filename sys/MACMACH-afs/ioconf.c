#include <sys/types.h>

#include <mac2/autoconf.h>

#define C (vm_offset_t)

extern via1init();
extern void level1intr();
extern via2init();
extern void level2intr();
extern scsiinit();
extern sccinit();
extern void level4intr();

struct devconf devconf[] = {
	{ "via", 1, C 0x50000000, 0x64, via1init, level1intr },
	{ "via", 2, C 0x50002000, 0x68, via2init, level2intr },
	{ "scsi", 0, C 0x50010000, 0x0, scsiinit, 0 },
	{ "scc", 0, C 0x50004000, 0x70, sccinit, level4intr },
	{ 0 }
};

