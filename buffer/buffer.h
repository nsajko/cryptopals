// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#pragma once

#include <stddef.h> // size_t

typedef struct {
	unsigned char *b;
	size_t l;
} buffer;

buffer *bufferAlloc(size_t l);
void bufferFree(buffer *buf);
