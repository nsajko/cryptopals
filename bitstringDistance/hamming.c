// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <assert.h>
#include <stddef.h>

#include "../buffer/buffer.h"

static unsigned char
popcount(unsigned char i) {
	signed char count = 0;
	int j;

	for (j = 0; j <= 8; j++) {
		count += (i >> j) & 0x01;
	}

	// for (j = i; j != 0; j &= j - 1) {
	//	count++;
	//}

	return count;
}

/*
static unsigned char
popcount(unsigned char b) {
	// TODO: The popcounts array could be computed at compile time ...
	static char popcounts[1 << 8];
	static bool initialized = false;
	if (!initialized) {
		int i;
		initialized = true;
		for (i = 0; i < 1 << 8; i++) {
			signed char count = 0;
			int j;

			for (j = 0; j <= 8; j++) {
				count += (i >> j) & 0x01;
			}

			//for (j = i; j != 0; j &= j - 1) {
			//	count++;
			//}

			popcounts[i] = count;
		}
	}
	return popcounts[b];
}
*/

size_t
hammingDistance(const buffer *b1, const buffer *b2) {
	size_t i, dist = 0;
	assert(b1->l == b2->l);
	for (i = 0; i < b1->l; i++) {
		dist += popcount(b1->b[i] ^ b2->b[i]);
	}
	return dist;
}

/*
// Hamming distance per byte.
double
normalizedHammingDistance(const buffer *b1, const buffer *b2) {
	return (double)hammingDistance(b1, b2) / (double)(b1->l);
}
*/
