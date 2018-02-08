// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

package test

/*
#include "bitstringDistance.h"

#include "../buffer/buffer.c"
*/
import "C"

import "unsafe"

func bufferFromSlice(slice []byte) *C.buffer {
	retBuf := C.bufferAlloc(C.size_t(len(slice)))
	(*retBuf).l = C.size_t(len(slice))

	_ = copy((*[1 << 31]byte)(unsafe.Pointer((*retBuf).b))[0:len(slice)], slice)
	//for i, b := range slice {
	//	(*[1 << 31]byte)(unsafe.Pointer((*retBuf).b))[i] = b
	//}

	return retBuf
}

func hammingDistance(s1 []byte, s2 []byte) int {
	buf1, buf2 := bufferFromSlice(s1), bufferFromSlice(s2)
	retInt := C.hammingDistance(buf1, buf2)

	C.bufferFree(buf1)
	C.bufferFree(buf2)

	return int(retInt)
}

func generalizedHammingDistance(slice []byte, len int) float64 {
	buf := bufferFromSlice(slice)
	retFloat := C.generalizedHammingDistance(buf, C.size_t(len))

	C.bufferFree(buf)

	return float64(retFloat)
}
