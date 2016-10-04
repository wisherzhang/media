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

#define STACK_ARRAY(TYPE, LEN) static_cast<TYPE*>(::alloca((LEN)*sizeof(TYPE)))

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
static int HexPairValue(const char * code) {
	int value = 0;
	for (const char * pch = code; pch < code + 2; ++pch) {
		value <<= 4;
		int digit = *pch;
		if (digit >= '0' && digit <= '9') {
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') {
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') {
			value += digit - 'a' + 10;
		}
		else {
			return -1;
		}
	}
	return value;
}
static int InternalUrlDecode(const char *source, char *dest,
	bool encode_space_as_plus) {
		char * start = dest;

		while (*source) {
			switch (*source) {
			case '+':
				if (encode_space_as_plus) {
					*(dest++) = ' ';
				} else {
					*dest++ = *source;
				}
				break;
			case '%':
				if (source[1] && source[2]) {
					int value = HexPairValue(source + 1);
					if (value >= 0) {
						*(dest++) = static_cast<char>(value);
						source += 2;
					}
					else {
						*dest++ = '?';
					}
				}
				else {
					*dest++ = '?';
				}
				break;
			default:
				*dest++ = *source;
			}
			source++;
		}

		*dest = 0;
		return static_cast<int>(dest - start);
}
std::string
	InternalUrlDecodeString(const std::string & encoded,
	bool encode_space_as_plus) {
		size_t needed_length = encoded.length() + 1;
		char* buf = STACK_ARRAY(char, needed_length);
		InternalUrlDecode(encoded.c_str(), buf, encode_space_as_plus);
		return buf;
}
std::string
	UrlDecodeString(const std::string & encoded) {
		return InternalUrlDecodeString(encoded, true);
}
