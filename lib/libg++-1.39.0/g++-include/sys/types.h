#ifndef types_h
#pragma once

#include <stddef.h>

extern "C"
{
#define size_t ____size_t
#define ptrdiff_t ____ptrdiff_t
#define wchar_t ____wchar_t
#include "//usr/include/sys/types.h"
#undef size_t
#undef ptrdiff_t
#undef wchar_t

#ifndef types_h
#define types_h 1
#endif

}


#endif
