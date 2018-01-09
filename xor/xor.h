#pragma once

#include "../buffer/buffer.h"

buffer *fixedXor(const buffer *in1, const buffer *in2);
buffer *xorAlloc(const buffer *in, unsigned char key);
