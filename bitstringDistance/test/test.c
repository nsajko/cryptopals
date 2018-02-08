// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <float.h> // DBL_MIN DBL_MAX
#include <stdio.h> // fprintf

#include <stdbool.h>

#include "../../buffer/buffer.h"
#include "../../sizeofMacros.h"
#include "../bitstringDistance.h"

// For any byte b, tests if the array of four bytes bArr contains two bytes with value b
// and two bytes with the negation of b.
static bool
twoAndTwo(const unsigned char bArr[]) {
	int byteCounts[2] = {1, 0};
	int i;
	for (i = 1; i < 4; i++) {
		if (bArr[i] == bArr[0]) {
			byteCounts[0]++;
		} else if (bArr[i] == (unsigned char)(~bArr[0])) {
			byteCounts[1]++;
		} else {
			// Got wrong byte values.

			return false;
		}
	}
	if (byteCounts[0] == 2 && byteCounts[1] == 2) {
		return true;
	}
	return false;
}

static bool
allBitPositionsSumToTwo(const unsigned char bArr[]) {
	signed char bitCounts[8] = {0};
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 8; j++) {
			bitCounts[j] += (bArr[i] & (0x01 << j)) >> j;
		}
	}
	for (j = 0; j < 8; j++) {
		if (bitCounts[j] != 2) {
			return false;
		}
	}
	return true;
}

static void
testGeneralizationMinMax(void) {
	unsigned char valuesOfByte0[] = {0x00, 0x01, 0x02, 0x0e, 0x0f,
					 0x10, 0xf0, 0xfe, 0xff};
	int i;
	for (i = 0; i < ARRAY_SIZE(valuesOfByte0); i++) {
		double minVal = DBL_MAX, maxVal = DBL_MIN;

		int byte1;
		for (byte1 = 0x00; byte1 <= 0xff; byte1++) {
			int byte2;
			for (byte2 = 0x00; byte2 <= 0xff; byte2++) {
				int byte3;
				for (byte3 = 0x00; byte3 <= 0xff; byte3++) {
					unsigned char arr[4];
					buffer buf;
					buf.b = arr;
					buf.l = ARRAY_SIZE(arr);
					arr[0] = valuesOfByte0[i];
					arr[1] = (unsigned char)byte1;
					arr[2] = (unsigned char)byte2;
					arr[3] = (unsigned char)byte3;

					bool allBytesEqual = byte1 == valuesOfByte0[i] &&
							     byte2 == valuesOfByte0[i] &&
							     byte3 == valuesOfByte0[i];

					double val = generalizedHammingDistance(&buf, 1);

					if ((val == 0.0) != allBytesEqual) {
						// ADD_FAILURE();
						fprintf(stdout,
							"Return value minimal, but bytes "
							"not equal or the reverse: %g, "
							"%2x %2x %2x %2x\n",
							val, arr[0], arr[1], arr[2],
							arr[3]);
					}
					if ((val == 8.0) != allBitPositionsSumToTwo(arr)) {
						// ADD_FAILURE();
						fprintf(stdout,
							"Return value maximal, but bytes "
							"not as they should be or the "
							"reverse: %g, %2x %2x %2x %2x\n",
							val, arr[0], arr[1], arr[2],
							arr[3]);
					}
					if (twoAndTwo(arr) && val != 8.0) {
						// ADD_FAILURE();
						fprintf(stdout,
							"Return value not maximal, but "
							"bytes are two and two: %g, %2x "
							"%2x %2x %2x\n",
							val, arr[0], arr[1], arr[2],
							arr[3]);
					}

					if (val < minVal) {
						minVal = val;
					}
					if (maxVal < val) {
						maxVal = val;
					}
				}
			}
		}

		fprintf(stdout, "Byte0 %2x done.\n", valuesOfByte0[i]);

		if (minVal != 0.0) {
			// ADD_FAILURE();
			fprintf(stdout, "min = %g, instead of 0.\n", minVal);
		}
		if (maxVal != 8.0) {
			// ADD_FAILURE();
			fprintf(stdout, "max = %g, instead of 8.\n", maxVal);
		}

		fprintf(stdout, "\n");
	}
}

static void
testGeneralization(void) {
	int byte0, byte1;
	for (byte0 = 0x00; byte0 <= 0xff; byte0++) {
		for (byte1 = 0x00; byte1 <= 0xff; byte1++) {
			unsigned char arr0[1];
			unsigned char arr1[1];
			buffer buf0;
			buffer buf1;

			unsigned char arr[2];
			buffer buf;

			buf0.b = arr0;
			buf1.b = arr1;
			buf0.l = ARRAY_SIZE(arr0);
			buf1.l = ARRAY_SIZE(arr1);
			arr0[0] = (unsigned char)byte0;
			arr1[0] = (unsigned char)byte1;

			buf.b = arr;
			buf.l = ARRAY_SIZE(arr);
			arr[0] = (unsigned char)byte0;
			arr[1] = (unsigned char)byte1;

			if ((double)hammingDistance(&buf0, &buf1) !=
			    generalizedHammingDistance(&buf, 1)) {
				// ADD_FAILURE();
				fprintf(stdout,
					"Supposed generalization not equal to Hamming "
					"distance for %2x %2x\n",
					byte0, byte1);
			}
		}
	}
}

int
main(void) {
	testGeneralizationMinMax();
	testGeneralization();
	return 0;
}
