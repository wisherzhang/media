/*
 *  Copyright 2011 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "data_socket.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(WEBRTC_POSIX)
#include <unistd.h>
#endif
#include <assert.h>
// SocketBase
//

bool SocketBase::Create() {
	assert(!valid());
	socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	return valid();
}

void SocketBase::Close() {
	if (socket_ != INVALID_SOCKET) {
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}
}
//#if defined(WIN32)
//class WinsockInitializer {
//	static WinsockInitializer singleton;
//
//	WinsockInitializer() {
//		WSADATA data;
//		WSAStartup(MAKEWORD(1, 0), &data);
//	}
//
//public:
//	~WinsockInitializer() { WSACleanup(); }
//};
//WinsockInitializer WinsockInitializer::singleton;
//#endif
