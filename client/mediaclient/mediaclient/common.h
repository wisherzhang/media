#pragma once
#ifdef WIN32
#include <stdio.h>
#include <winsock2.h>
#include <WINSOCK2.H>
#include <STDIO.H>
#pragma  comment(lib,"ws2_32.lib")
typedef int socklen_t;
typedef SOCKET NativeSocket;
#else
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define closesocket close
typedef int NativeSocket;

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  static_cast<NativeSocket>(-1)
#endif
#endif
