// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "SharedMemory.h"

unsigned int SharedMemory2::uiIndex = 0;

QString SharedMemory2::name() const {
	return a_ucData ? qsName : QString();
}

unsigned int SharedMemory2::size() const {
	return a_ucData ? uiSize : 0;
}

void SharedMemory2::setData(unsigned char* data, size_t length) {
	if (a_ucData)
		memcpy(a_ucData, data, length);
}

unsigned char *SharedMemory2::data() {
	return a_ucData;
}

const unsigned char *SharedMemory2::data() const {
	return a_ucData;
}

void SharedMemory2::erase() {
	if (a_ucData) {
		memset(a_ucData, 0, uiSize);
	}
}
