// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

// https://tools.ietf.org/html/rfc4648

#include <stddef.h> // size_t

#include <stdbool.h>

#include "../buffer/buffer.h"
#include "../sizeofMacros.h"

static const char index[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

static const char padSymbol = '=';

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
base64EncodedSize(size_t l) {
	return (l + 2) / 3 * 4;
}
static size_t
base64DecodedSize(size_t l) {
	// Assuming l to be divisible by four.
	return l / 4 * 3;
}

static void
base64Encode(const buffer *in, buffer *out) {
	size_t i, o = 0;
	unsigned char *outA = out->b;
	unsigned char *inA = in->b;
	for (i = 0; i + 2 < in->l; i += 3) {
		outA[o] = index[(inA[i + 0] & 0xfc) >> 2];
		o++;
		outA[o] = index[((inA[i + 0] & 0x03) << 4) | ((inA[i + 1] & 0xf0) >> 4)];
		o++;
		outA[o] = index[((inA[i + 1] & 0x0f) << 2) | ((inA[i + 2] & 0xc0) >> 6)];
		o++;
		outA[o] = index[(inA[i + 2] & 0x3f) << 0];
		o++;
	}
	if (in->l - i == 1) {
		outA[o] = index[(inA[i + 0] & 0xfc) >> 2];
		o++;
		outA[o] = index[(inA[i + 0] & 0x03) << 4];
		o++;
		outA[o] = padSymbol;
		o++;
		outA[o] = padSymbol;
		o++;
	} else if (in->l - i == 2) {
		outA[o] = index[(inA[i + 0] & 0xfc) >> 2];
		o++;
		outA[o] = index[((inA[i + 0] & 0x03) << 4) | ((inA[i + 1] & 0xf0) >> 4)];
		o++;
		outA[o] = index[(inA[i + 1] & 0x0f) << 2];
		o++;
		outA[o] = padSymbol;
		o++;
	}
	out->l = o;
}

// Does not ignore '\r' or '\n'. Use: tr -d '\n\r'
// Returns place where an error happened, or -1 if no error occured.
static int
base64Decode(const buffer *in, buffer *out) {
	if (in->l % 4 != 0) {
		return 0;
	}

	size_t i, o = 0;
	unsigned char *outA = out->b;
	unsigned char *inA = in->b;
	for (i = 0; i < in->l - 4; i += 4) {
		unsigned char sextet0, sextet1, sextet2, sextet3;
		bool found;
		sextet0 = reverseIndex(inA[i], &found);
		if (!found) {
			return i;
		}
		sextet1 = reverseIndex(inA[i + 1], &found);
		if (!found) {
			return i + 1;
		}
		outA[o] = sextet0 << 2 | ((sextet1 & 0x30) >> 4);
		o++;

		sextet2 = reverseIndex(inA[i + 2], &found);
		if (!found) {
			return i + 2;
		}
		outA[o] = ((sextet1 & 0x0f) << 4) | ((sextet2 & 0x3c) >> 2);
		o++;

		sextet3 = reverseIndex(inA[i + 3], &found);
		if (!found) {
			return i + 3;
		}
		outA[o] = ((sextet2 & 0x03) << 6) | sextet3;
		o++;
	}
	unsigned char sextet0, sextet1, sextet2, sextet3;
	bool found;
	sextet0 = reverseIndex(inA[i], &found);
	if (!found) {
		return i;
	}
	sextet1 = reverseIndex(inA[i + 1], &found);
	if (!found) {
		return i + 1;
	}
	outA[o] = sextet0 << 2 | ((sextet1 & 0x30) >> 4);
	o++;

	if (inA[i + 2] == padSymbol) {
		out->l = o;
		return -1;
	}

	sextet2 = reverseIndex(inA[i + 2], &found);
	if (!found) {
		return i + 2;
	}
	outA[o] = ((sextet1 & 0x0f) << 4) | ((sextet2 & 0x3c) >> 2);
	o++;

	if (inA[i + 3] == padSymbol) {
		out->l = o;
		return -1;
	}

	sextet3 = reverseIndex(inA[i + 3], &found);
	if (!found) {
		return i + 3;
	}
	outA[o] = ((sextet2 & 0x03) << 6) | sextet3;
	o++;

	out->l = o;
	return -1;
}

buffer *
base64EncodeAlloc(const buffer *in) {
	buffer *out = bufferAlloc(base64EncodedSize(in->l));
	base64Encode(in, out);
	return out;
}

buffer *
base64DecodeAlloc(const buffer *in) {
	buffer *out = bufferAlloc(base64DecodedSize(in->l));
	if (base64Decode(in, out) != -1) {
		if (out != NULL) {
			bufferFree(out);
		}
		return NULL;
	}
	return out;
}
