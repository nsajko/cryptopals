#include <stddef.h>

#include "../buffer/buffer.h"

buffer *
xorAlloc(const buffer *in, unsigned char key) {
	buffer *out = bufferAlloc(in->l);
	if (out == NULL) {
		return out;
	}

	size_t i;
	for (i = 0; i < in->l; i++) {
		(out->b)[i] = (in->b)[i] ^ key;
	}

	out->l = in->l;
	return out;
}
