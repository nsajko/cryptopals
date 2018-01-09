#include "../buffer/buffer.h"

buffer *
fixedXor(const buffer *in1, const buffer *in2) {
	if (in1->l != in2->l) {
		return NULL;
	}

	buffer *out = bufferAlloc(in1->l);
	if (out == NULL) {
		return out;
	}

	int i;
	for (i = 0; i < in1->l; i++) {
		(out->b)[i] = (in1->b)[i] ^ (in2->b)[i];
	}

	out->l = in1->l;
	return out;
}
