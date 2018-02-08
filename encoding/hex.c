// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stddef.h> // size_t

#include <stdbool.h>

#include "../buffer/buffer.h"
#include "../sizeofMacros.h"

static const char index[] = {'0', '1', '2', '3', '4', '5', '6', '7',
			     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

static unsigned char
reverseIndex(char c, bool *found) {
	int i;
	for (i = 0; i < ARRAY_SIZE(index); i++) {
		if (index[i] == c) {
			break;
		}
	}
	if (i == ARRAY_SIZE(index)) {
		*found = false;
	} else {
		*found = true;
	}
	return i;
}

static size_t
hexEncodedSize(size_t l) {
	return l * 2;
}
static size_t
hexDecodedSize(size_t l) {
	return l / 2 + 1;
}

static void
hexEncode(const buffer *in, buffer *out) {
	size_t i, o = 0;
	unsigned char *outA = out->b;
	unsigned char *inA = in->b;
	for (i = 0; i < in->l; i++) {
		outA[o] = index[(inA[i] & 0xf0) >> 4];
		o++;
		outA[o] = index[inA[i] & 0x0f];
		o++;
	}
	out->l = o;
}

// Returns place where an error happened, or -1 if no error occured.
static int
hexDecode(const buffer *in, buffer *out) {
	size_t i, o = 0;
	unsigned char *outA = out->b;
	unsigned char *inA = in->b;
	for (i = 0; i + 1 < in->l; i += 2) {
		unsigned char nibble0, nibble1;
		bool found;
		nibble0 = reverseIndex(inA[i], &found);
		if (!found) {
			return i;
		}
		nibble1 = reverseIndex(inA[i + 1], &found);
		if (!found) {
			return i + 1;
		}
		outA[o] = (nibble0 << 4) | nibble1;
		o++;
	}
	if (in->l - i == 1) {
		unsigned char nibble;
		bool found;
		nibble = reverseIndex(inA[i], &found);
		if (!found) {
			return i;
		}
		outA[o] = nibble << 4;
		o++;
	}

	out->l = o;
	return -1;
}

buffer *
hexEncodeAlloc(const buffer *in) {
	buffer *out = bufferAlloc(hexEncodedSize(in->l));
	hexEncode(in, out);
	return out;
}

buffer *
hexDecodeAlloc(const buffer *in) {
	buffer *out = bufferAlloc(hexDecodedSize(in->l));
	if (hexDecode(in, out) != -1) {
		if (out != NULL) {
			bufferFree(out);
		}
		return NULL;
	}
	return out;
}
