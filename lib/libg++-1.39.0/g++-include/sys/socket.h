#ifndef socket_h
#pragma once

#include <time.h>

extern "C"
{

#define bind c_proto_bind
#define send c_proto_send
#include "//usr/include/sys/socket.h"
#undef bind
#undef send

#ifndef socket_h
#define socket_h 1
#endif

// void* in select, since different systems use int* or fd_set*
int       select(int, void*, void*, void*, struct timeval*);

int       connect(int, struct sockaddr*, int);
int       accept(int, struct sockaddr*, int*);
int       getsockname(int, struct sockaddr*, int*);
int       getpeername(int, struct sockaddr*, int*);  // 16 Oct 90 Talisman
}

#endif
