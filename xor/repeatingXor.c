// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include "../buffer/buffer.h"

static void
repeatingXor(const buffer *plain, buffer *out, const buffer *key) {
	size_t i, keyByte = 0;
	for (i = 0; i < plain->l; i++) {
		out->b[i] = plain->b[i] ^ key->b[keyByte];

		keyByte++;
		if (keyByte == key->l) {
			keyByte = 0;
		}
	}
}

buffer *
repeatingXorAlloc(const buffer *plain, const buffer *key) {
	buffer *out;
	if (key->l == 0) {
		return NULL;
	}
	out = bufferAlloc(plain->l);
	if (out == NULL) {
		return out;
	}
	repeatingXor(plain, out, key);
	out->l = plain->l;
	return out;
}
