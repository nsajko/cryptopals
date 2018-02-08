// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stddef.h>

#include "../buffer/buffer.h"

size_t hammingDistance(const buffer *b1, const buffer *b2);

double generalizedHammingDistance(const buffer *b, size_t strLen);
