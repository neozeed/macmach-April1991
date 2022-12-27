#ifndef wait_h

#include <sys/resource.h>

#define wait WaitStatus

extern "C" {

#include "//usr/include/sys/wait.h"

#undef wait

#ifndef wait_h
#define wait_h 1
#endif

extern int wait3(WaitStatus*, int options, struct rusage*);
extern int wait4(int, WaitStatus*, int, struct rusage*);
}

#endif
