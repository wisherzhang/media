/*
 *  Copyright 2011 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_SERVER_DATA_SOCKET_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_SERVER_DATA_SOCKET_H_
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

#include <string>

class SocketBase {
public:
	SocketBase() : socket_(INVALID_SOCKET) { }
	explicit SocketBase(NativeSocket socket) : socket_(socket) { }
	~SocketBase() { Close(); }

	NativeSocket socket() const { return socket_; }
	bool valid() const { return socket_ != INVALID_SOCKET; }

	bool Create();
	void Close();

protected:
	NativeSocket socket_;
};




#endif