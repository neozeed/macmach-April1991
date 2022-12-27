#ifndef resource_h
#pragma once
#ifndef USG
#include <time.h>

extern "C"
{

#include "//usr/include/sys/resource.h"

#ifndef resource_h
#define resource_h 1
#endif

int getrusage(int, struct rusage*);
int getrlimit (int resource, struct rlimit *rlp);
int setrlimit (int resource, struct rlimit *rlp);

}

#endif /* USG */
#endif 
