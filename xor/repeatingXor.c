#include "../buffer/buffer.h"

buffer *
repeatingXorAlloc(const buffer *plain, const buffer *key) {
	size_t i, keyByte = 0;
	if (key->l == 0) {
		return NULL;
	}
	buffer *out = bufferAlloc(plain->l);
	if (out == NULL) {
		return out;
	}
	for (i = 0; i < plain->l; i++) {
		out->b[i] = plain->b[i] ^ key->b[keyByte];

		keyByte++;
		if (keyByte == key->l) {
			keyByte = 0;
		}
	}

	out->l = plain->l;
	return out;
}
