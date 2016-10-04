/*
 *  Copyright 2011 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "utils.h"

#include <stdio.h>
#include<sstream>

std::string int2str(int i) {
	std::string s;
	std::ostringstream oss;
	oss << std::boolalpha << i;
	s = oss.str();
    return s;
}

std::string size_t2str(size_t i) {
    std::string s;
	std::ostringstream oss;
	oss << std::boolalpha << i;
	s = oss.str();
    return s;
}
