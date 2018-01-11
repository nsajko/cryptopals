#pragma once

#include "../buffer/buffer.h"

buffer *fixedXor(const buffer *in1, const buffer *in2);
buffer *repeatingXorAlloc(const buffer *plain, const buffer *key);
buffer *xorAlloc(const buffer *in, unsigned char key);
