// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#pragma once

#include "../buffer/buffer.h"

buffer *fixedXor(const buffer *in1, const buffer *in2);
buffer *repeatingXorAlloc(const buffer *plain, const buffer *key);
buffer *xorAlloc(const buffer *in, unsigned char key);
