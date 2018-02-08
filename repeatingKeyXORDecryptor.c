// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

// Prints all possible single byte XOR codings of a string, sorted by proximity of the
// english letter frequency stats.

// There are two unrelated kinds of "distance" used in this program, distances between
// arrays of doubles, and distances between bit strings. Both types of distance are
// recorded in type "double". Bit string distances (a generalization of Hamming distance to
// be more specific) are used to find the keysize; following which key bytes can be found
// using distances between arrays of doubles, which represent (plaintext) letter
// frequencies.

// TODO: how does this behave for keys where consecutive bytes differ only by single bits?

#include <assert.h>

#include <ctype.h>  // isdigit isxdigit
#include <float.h>  // DBL_MAX
#include <stdio.h>  // fprintf fread feof ungetc
#include <stdlib.h> // malloc
#include <string.h> // memcpy

#include "alphabet/alphabet.h"
#include "bitstringDistance/bitstringDistance.h"
#include "buffer/buffer.h"
#include "distance/distance.h"
#include "encoding/encoding.h"
#include "sizeofMacros.h"
#include "xor/xor.h"

// If chr is not a line ending symbol (EOF or '\n'), consume the current line on stdin.
static void
consumeStdinLine(int chr) {
	for (; chr != EOF && chr != '\n';) {
		chr = fgetc(stdin);
	}
}

static void
printResults(const buffer *key, const buffer *buf) {
	buffer *hexedKey;
	fprintf(stdout, "Selected key:   ");
	hexedKey = hexEncodeAlloc(key);
	FWRITE(hexedKey->b, hexedKey->l, stdout);
	bufferFree(hexedKey);
	fprintf(stdout, "\n\nDecryption:\n\n");
	FWRITE(buf->b, buf->l, stdout);
	fprintf(stdout, "\n\n\n\n");
}

// Key byte and associated letter/monograph counts.
typedef struct {
	unsigned char key;
	double monogDistance;
} keyByte;

enum {
	// How many bytes to try for each key byte position.
	trialKeyBytes = 2,
};

// probableKeys must be an array of trialKeyBytes keyByte-s.
static void
mostProbableKeyBytes(const buffer *buf, keyByte probableKeys[]) {
	size_t j;
	buffer tmpB;
	int key;

	for (j = 0; j < trialKeyBytes; j++) {
		probableKeys[j].monogDistance = DBL_MAX;
	}

	assert(buf->l != 0);
	MALLOC(tmpB.b, buf->l);
	tmpB.l = buf->l;

	for (key = 0x00; key <= 0xff; key++) {
		int i;
		keyByte kAndD;
		double monogFreqs[alphabetSize];

		// 0x20 is the ASCII letter case bit.
		if ((key & 0x20) != 0) {
			continue;
		}

		// MEMCPY(tmpB.b, buf.b, buf->l)
		for (j = 0; j < buf->l; j++) {
			// tmpB.b[j] ^= key;
			tmpB.b[j] = buf->b[j] ^ key;
		}
		monographFreqCount(&tmpB, monogFreqs);
		kAndD.monogDistance = monographFreqScore(monogFreqs, sumOfSquares);
		kAndD.key = key;

		// Search
		for (j = 0; j < trialKeyBytes &&
			    probableKeys[j].monogDistance < kAndD.monogDistance;
		     j++) {
		}

		for (i = trialKeyBytes - 1; j < i; i--) {
			MEMCPY(&(probableKeys[i]), &(probableKeys[i - 1]), 1)
		}
		if (j < trialKeyBytes) {
			MEMCPY(&(probableKeys[j]), &kAndD, 1)
		}
	}

	free(tmpB.b);
}

// Given the key size and an encrypted buffer, finds possible decryptions.
static void
decryptKeySizeBuf(const buffer *buf, unsigned char keySize) {
	int i;

	enum {
		// Key byte selected by index.
		KeyByIndex = 0,

		// The ASCII case bit, 0x20, has been selected to be set in the key
		// selected by index.
		CaseBit,

		// Key byte selected explicitly by value.
		KeyByValue,
	};
	struct {
		signed char index;

		// May be KeyByIndex, KeyByValue, or CaseBit.
		unsigned char modif;
	} * iArr;

	buffer key;
	size_t j;
	size_t smallBufSize;
	buffer smallBuf;

	// trialKeyBytes possible (most probable) key-bytes for each byte of the keysize.
	keyByte(*keyBytes)[trialKeyBytes];

	smallBufSize = buf->l / keySize;
	if (buf->l % keySize != 0) {
		smallBufSize++;
	}
	MALLOC(smallBuf.b, smallBufSize);
	MALLOC(keyBytes, keySize);
	for (i = 0; i < keySize; i++) {
		size_t smallBufI = 0;
		for (j = i; j < buf->l; j += keySize) {
			smallBuf.b[smallBufI] = buf->b[j];
			smallBufI++;
		}
		smallBuf.l = smallBufI;
		mostProbableKeyBytes(&smallBuf, keyBytes[i]);
	}
	free(smallBuf.b);

	CALLOC(iArr, keySize);
	MALLOC(key.b, keySize);
	key.l = keySize;
	for (i = 0; i < keySize; i++) {
		key.b[i] = keyBytes[i][iArr[i].index].key;
	}
	fprintf(stdout,
		"On display are the suggested values for bytes of the key. You may "
		"select\n"
		"any value for any byte of the key. Each position-suggested value\n"
		"coordinate points to two similar byte values which differ in the ASCII\n"
		"case bit, the bit set in byte 0x20.\n"
		"Usage instructions:\n"
		"Enter '1' to scroll through the suggested values of the key byte in "
		"position 1 (on horizontal axis, counting starts from zero).\n"
		"Enter '2 1' to select the key byte value in position 1 (on vertical "
		"axis) of the key byte in position 2.\n"
		"Enter '2 x3e' to select 3e (hexadecimal numeral) as the value of the key "
		"byte in position 2.\n");
	for (;;) {
		int k;
		int selectedKeyByte, n, chr;

		fprintf(stdout, "\nSuggested key bytes:\n  ");
		for (i = 0; i < keySize; i++) {
			fprintf(stdout, " %4d", i);
		}
		fprintf(stdout, "\n");
		for (k = 0; k < trialKeyBytes; k++) {
			fprintf(stdout, "%d ", k);
			for (i = 0; i < keySize; i++) {
				char selectedSign = ' ';
				if (iArr[i].modif != KeyByValue && iArr[i].index == k &&
				    iArr[i].modif != CaseBit) {
					selectedSign = '>';
				}
				fprintf(stdout, " %c %2x", selectedSign,
					keyBytes[i][k].key);
			}
			fprintf(stdout, "\n");

			fprintf(stdout, "  ");
			for (i = 0; i < keySize; i++) {
				char selectedSign = ' ';
				if (iArr[i].modif != KeyByValue && iArr[i].index == k &&
				    iArr[i].modif == CaseBit) {
					selectedSign = '>';
				}
				fprintf(stdout, " %c %2x", selectedSign,
					keyBytes[i][k].key ^ 0x20);
			}
			fprintf(stdout, "\n");
		}
		fprintf(stdout, "\n");

		buffer *out = repeatingXorAlloc(buf, &key);
		printResults(&key, out);
		bufferFree(out);

		// Let the user choose bytes of the key.
		chr = fgetc(stdin);
		if (chr == EOF) {
			break;
		}
		if (!isdigit(chr)) {
			fprintf(stderr, "Input not recognized.\n");
			consumeStdinLine(chr);
			continue;
		}
		for (n = chr - '0';; n = 10 * n + (chr - '0')) {
			if (keySize <= n) {
				fprintf(stderr, "Selected key byte position is invalid\n");
				chr = fgetc(stdin);
				consumeStdinLine(chr);
				goto continueLabel;
			}
			chr = fgetc(stdin);
			if (!isdigit(chr)) {
				break;
			}
		}
		selectedKeyByte = n;
		if (!(chr == EOF || chr == '\n')) {
			chr = fgetc(stdin);
		}
		if (chr == EOF || chr == '\n') {
			// Scrolling through the suggested key byte values.

			unsigned char caseBit = 0x20;
			if (iArr[selectedKeyByte].modif == KeyByValue) {
				iArr[selectedKeyByte].index = 0;
				iArr[selectedKeyByte].modif = KeyByIndex;
			} else if (iArr[selectedKeyByte].modif != CaseBit) {
				iArr[selectedKeyByte].modif = CaseBit;
			} else {
				iArr[selectedKeyByte].index++;
				if (trialKeyBytes <= iArr[selectedKeyByte].index) {
					iArr[selectedKeyByte].index = 0;
				}
				iArr[selectedKeyByte].modif = KeyByIndex;
			}
			if (iArr[selectedKeyByte].modif != CaseBit) {
				caseBit = 0x00;
			}
			key.b[selectedKeyByte] =
				keyBytes[selectedKeyByte][iArr[selectedKeyByte].index]
					.key ^
				caseBit;
		} else if (isdigit(chr)) {
			// Selection from suggested key byte values by index.

			for (n = chr - '0';; n = 10 * n + (chr - '0')) {
				if (trialKeyBytes <= n) {
					fprintf(stderr,
						"Selected suggested key byte value index "
						"is invalid\n");
					chr = fgetc(stdin);
					consumeStdinLine(chr);
					goto continueLabel;
				}
				chr = fgetc(stdin);
				if (!isdigit(chr)) {
					break;
				}
			}
			iArr[selectedKeyByte].index = n;
			iArr[selectedKeyByte].modif = KeyByIndex;
			key.b[selectedKeyByte] =
				keyBytes[selectedKeyByte][iArr[selectedKeyByte].index].key;
		} else {
			// Manual explicit key byte value selection as hex numeral.

			buffer *decodedByte;
			unsigned char hexByte[2];
			buffer hexByteBuf;
			hexByteBuf.b = hexByte;
			hexByteBuf.l = 2;

			chr = fgetc(stdin);
			if (!isxdigit(chr)) {
				fprintf(stderr, "Input is not a hexadecimal digit.\n");
				consumeStdinLine(chr);
				continue;
			}
			hexByte[0] = chr;
			chr = fgetc(stdin);
			if (!isxdigit(chr)) {
				fprintf(stderr, "Input is not a hexadecimal digit.\n");
				consumeStdinLine(chr);
				continue;
			}
			hexByte[1] = chr;

			// Consume the delimiter after the hex byte.
			fgetc(stdin);

			decodedByte = hexDecodeAlloc(&hexByteBuf);
			key.b[selectedKeyByte] = decodedByte->b[0];
			bufferFree(decodedByte);
			iArr[selectedKeyByte].modif = KeyByValue;
		}
	continueLabel:;

		// This code used to print all probable possibilities, but they were just
		// too many, except only for the very least key sizes.
		// double sum = 0, monogSumFreqs[alphabetSize] = {0};
		// for (i = 0; i < keySize; i++) {
		//	key.b[i] = keyBytes[i][iArr[i]].key;
		//	for (j = 0; j < alphabetSize; j++) {
		//		sum += keyBytes[i][iArr[i]].monogCounts[j];
		//		monogSumFreqs[j] += keyBytes[i][iArr[i]].monogCounts[j];
		//	}
		//}
		// for (j = 0; j < alphabetSize; j++) {
		//	monogSumFreqs[j] /= sum;
		//}
		// if (monographFreqScore(monogSumFreqs, sumOfSquares) <
		//    monographCountPlaintextySumOfSquaresDistanceLimit) {
		//	buffer *out = repeatingXorAlloc(buf, &key);
		//	printResults(&key, out);
		//	bufferFree(out);
		//}
		// for (i = keySize - 1; iArr[i] == trialKeyBytes - 1; i--) {
		//	if (i == 0) {
		//		goto breakLabel;
		//	}
		//	iArr[i] = 0;
		//}
		// iArr[i]++;
	}
	// breakLabel:

	fprintf(stdout,
		"\n\nDone.\n"
		"Printing key to stdout in case there is ASCII or UTF-8 in it:\n");
	FWRITE(key.b, key.l, stdout);
	fprintf(stdout, "\n");

	free(key.b);
	free(iArr);
	free(keyBytes);
}

static buffer *
probableKeySizes(const buffer *buf) {
	buffer *retBuf;
	signed char keySize, bestKeySize;

	// 8 bits per byte is the maximal normalized distance, thus the initialization
	// value needs to be greater than 8.0.
	double bestDistance = DBL_MAX;

	for (keySize = 1; keySize < 41; keySize++) {
		double normalizedDistance =
			generalizedHammingDistance(buf, keySize) / (double)keySize;
		fprintf(stderr, "%3d  %g\n", keySize, normalizedDistance); ///
		if (normalizedDistance < bestDistance || normalizedDistance < 4.3) {
			bestDistance = normalizedDistance;
			bestKeySize = keySize;
			if (normalizedDistance < 4.3) {
				break;
			}
		}
	}

	fprintf(stderr,
		"Key size was probably %3d, with normalized (per byte) generalized "
		"Hamming distance %9g, ergo that size is chosen for decryption.\n\n\n\n",
		bestKeySize, bestDistance);

	retBuf = bufferAlloc(1);
	retBuf->b[0] = (unsigned char)bestKeySize;
	retBuf->l = 1;
	return retBuf;
}

static void
g(const buffer *buf) {
	size_t i;
	buffer *keySizes = probableKeySizes(buf);
	for (i = 0; i < keySizes->l; i++) {
		decryptKeySizeBuf(buf, keySizes->b[i]);
	}
	bufferFree(keySizes);
}

static void
f(FILE *inFile) {
	buffer inputBuf, *base64DecodedBuf;
	static unsigned char inputArr[1 << 20];
	size_t len = FREAD(inputArr, ARRAY_SIZE(inputArr), inFile);
	if (!feof(inFile)) {
		fprintf(stderr, "Input file not at EOF\n");
	}
	inputBuf.b = inputArr;
	inputBuf.l = len;

	base64DecodedBuf = base64DecodeAlloc(&inputBuf);
	if (base64DecodedBuf == NULL) {
		fprintf(stderr, "Nil pointer\n");
	}
	g(base64DecodedBuf);
	bufferFree(base64DecodedBuf);
}

int
main(int argc, char *argv[]) {
	FILE *inFile;
	if (argc != 2) {
		return 1;
	}
	inFile = fopen(argv[1], "r");
	f(inFile);
	fclose(inFile);
	return 0;
}
