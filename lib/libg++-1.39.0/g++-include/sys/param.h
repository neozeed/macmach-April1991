#ifndef param_h
#pragma once

#include <time.h>

extern "C"
{

#include "//usr/include/sys/param.h"

#ifndef param_h
#define param_h 1
#endif

/* kill commonly overloaded possible param.h macros */
#undef setbit
#undef clrbit
#undef isset
#undef isclr
#undef howmany
#undef roundup
#undef MIN
#undef MAX

}

#endif
