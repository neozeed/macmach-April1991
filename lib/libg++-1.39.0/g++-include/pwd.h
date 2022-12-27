#ifndef pwd_h
#ifdef __GNUG__
#pragma once
#pragma interface
#endif

// the Interviews-based standard kludge again

extern "C" {

#define getpwent c_proto_getpwent
#define getpwuid c_proto_getpwuid
#define getpwnam c_proto_getpwnam
#define setpwent c_proto_setpwent
#define endpwent c_proto_endpwent

#include "//usr/include/pwd.h"

#ifndef pwd_h
#define pwd_h 1
#endif

#undef getpwent
#undef getpwuid
#undef getpwnam
#undef setpwent
#undef endpwent

extern struct passwd* getpwent();
extern struct passwd* getpwuid(int);
extern struct passwd* getpwnam(char*);
extern int            setpwent();
extern int            endpwent();

}

#endif
