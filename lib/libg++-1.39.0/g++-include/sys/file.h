#ifndef file_h
#pragma once

/* Some folks need system types.h for things in file.h */
#include <stddef.h>
#include <sys/types.h>

extern "C"
{
#define open c_proto_open
#define fcntl c_proto_fcntl

#include "//usr/include/sys/file.h"

/* try to guess whether file.h actually gave the right defs */
#if !defined(O_RDONLY) || !defined(O_CREAT) || !defined(O_EXCL)
#include <sys/fcntl.h>
#endif

#ifndef file_h
#define file_h 1
#endif

#undef open
#undef fcntl

}

#endif
