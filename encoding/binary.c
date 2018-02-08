#include <stddef.h> // size_t

#include <stdbool.h>

#include "../buffer/buffer.h"
#include "../sizeofMacros.h"

static size_t
binaryEncodedSize(size_t l) {
	return l * 8;
}

static size_t
binaryDecodedSize(size_t l) {
	return l / 8 + 1;
}

static void
binaryEncode(const buffer *in, buffer *out) {
	size_t i;
	for (i = 0; i < in->l; i++) {
		int j;
		for (j = 0; j < 8; j++) {
			out->b[i * 8 + j] = ((in->b[i] & (1 << j)) >> j) + '0';
		}
	}
}

buffer *
binaryEncodeAlloc(const buffer *in) {
	buffer *out = bufferAlloc(binaryEncodedSize(in->l));
	binaryEncode(in, out);
	return out;
}
