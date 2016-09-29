#pragma once

#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
typedef SOCKET NativeSocket;
#else
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#define closesocket close
typedef int NativeSocket;

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  static_cast<NativeSocket>(-1)
#endif
#endif
