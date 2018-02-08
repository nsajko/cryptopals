// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "../buffer/buffer.h"
#include "../sizeofMacros.h"

// A generalization of Hamming distance, takes as input two or more bit strings instead of
// just two.
//
// The input buffer b is interpreted as an array of arrays each long strLen bytes, except
// possibly the last.
//
// Some properties of the generalization:
//
// * In the case of b->l == strLen * 2, the generalization is equivalent to Hamming
// distance, that is the returned value equals the number of differing corresponding bits
// betweem the first and second halves of b->b.
//
// * The bit strings can appear in any order within b->b without the return value
// changing.
//
// * The minimal returned value (except in case of errors) is 0, the maximal value is
// strLen * 8.
//
// * The returned value is zero if and only if all the bit strings (of length strLen)
// interpreted as contained in b->b are equal.
//
// * If b->l / strLen is of even parity and b->l % strLen == 0 (in other words, there
// is an even number of bit strings and they are all of equal length), for any bit string
// s of length strLen, if half the bit strings equal s and the other half of bit strings
// equal the bit-wise complement of s, the returned value is strLen * 8.
//
// * The returned value is strLen * 8 if and only if there is an even number of bit strings
// and they are all of equal length, for all bit-positions in the bit strings the sum of
// bits in that position over all bit strings equals half the number of bit strings.
double
generalizedHammingDistance(const buffer *b, size_t strLen) {
	size_t i, byte;
	size_t m, n, sum;
	double half, ret;
	size_t numberOfStrings = b->l / strLen;
	assert(2 <= numberOfStrings);
	// if (b->l - i != 0) {
	if (b->l % strLen != 0) {
		numberOfStrings++;
	}

	size_t *bitCounts;
	CALLOC(bitCounts, strLen * 8);
	if (bitCounts == NULL) {
		return -1.0;
	}

	for (i = 0; i + strLen - 1 < b->l; i += strLen) {
		for (byte = 0; byte < strLen; byte++) {
			int bit;
			for (bit = 0; bit < 8; bit++) {
				bitCounts[byte * 8 + bit] +=
					((b->b[i + byte]) & (0x01 << bit)) >> bit;
			}
		}
	}

	for (byte = 0; byte < b->l - i; byte++) {
		int bit;
		for (bit = 0; bit < 8; bit++) {
			bitCounts[byte * 8 + bit] +=
				((b->b[i + byte]) & (0x01 << bit)) >> bit;
		}
	}

	// If strLen divides b->l we want b->l - i to equal strLen.
	if (b->l % strLen == 0) {
		i = b->l - strLen;
	}

	n = numberOfStrings;
	half = (double)n / 2.0;
	sum = 0;
	for (m = 0; m < (b->l - i) * 8; m++) {
		if (half < (double)bitCounts[m]) {
			bitCounts[m] = n - bitCounts[m];
		}
		sum += bitCounts[m];
	}
	ret = (double)sum / half;

	n = numberOfStrings - 1;
	half = (double)n / 2.0;
	sum = 0;
	for (; m < strLen * 8; m++) {
		if (half < (double)bitCounts[m]) {
			bitCounts[m] = n - bitCounts[m];
		}
		sum += bitCounts[m];
	}
	free(bitCounts);
	return ret + (double)sum / half;
}

/*
// Generalized Hamming distance per byte.
double
normalizedGeneralizedHammingDistance(const buffer *b, size_t strLen) {
	return generalizedHammingDistance(b, strLen) / (double)strLen;
}
*/
