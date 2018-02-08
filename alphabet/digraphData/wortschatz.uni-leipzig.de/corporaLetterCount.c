// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

// Reads from stdin while counting alphabetic letters and digraphs (as if non-alphabetic
// bytes were ignored), then prints counts and frequencies. "Alphabetic" means [A-Za-z]+
// (in regexp terms).

#include <ctype.h> // isalpha tolower
#include <stddef.h>
#include <stdio.h> // fread fprintf printf

#include <stdbool.h>

#include "../../../buffer/buffer.h"
#include "../../../sizeofMacros.h"

enum { alphabetSize = 'Z' - 'A' + 1 };

// Prints counts and normalized counts, the latter of which are to be used as standard
// frequencies in C source.
static void
printCounts(long monographCounts[], long digraphCounts[][alphabetSize]) {
	long sum = 0;
	int i, j;

	printf("\nMonograph counts\n\n");
	for (i = 0; i < alphabetSize; i++) {
		printf("%c  %ld\n", i + 'A', monographCounts[i]);
		sum += monographCounts[i];
	}
	printf("\nMonograph sum: %ld\n\n", sum);

	printf("static const double monographFreqs[] = {");
	for (i = 0; i < alphabetSize; i++) {
		printf("%.55g, ", (double)monographCounts[i] / (double)sum);
	}
	printf("};\n\n\n");

	sum = 0;

	printf("\nDigraph counts\n\n");
	for (i = 0; i < alphabetSize; i++) {
		for (j = 0; j < alphabetSize; j++) {
			printf("%c%c %ld\n", i + 'A', j + 'A', digraphCounts[i][j]);
			sum += digraphCounts[i][j];
		}
	}
	printf("\nDigraph sum: %ld\n\n", sum);

	printf("static const double digraphFreqs[][] = {");
	for (i = 0; i < alphabetSize; i++) {
		printf("{");
		for (j = 0; j < alphabetSize; j++) {
			printf("%.55g, ", (double)digraphCounts[i][j] / (double)sum);
		}
		printf("}, ");
	}
	printf("};\n\n\n");
}

static unsigned char
alphaToIndex(unsigned char c) {
	return tolower(c) - 'a';
}

static void
countLetters(const buffer *in, unsigned char bufBridge[], long monographCounts[],
	     long digraphCounts[][alphabetSize]) {
	size_t i, oldI;

	if (in->l == 0) {
		return;
	}

	for (oldI = 0; oldI < in->l && !isalpha(in->b[oldI]); oldI++) {
	}
	bufBridge[0] = alphaToIndex(in->b[oldI]);
	monographCounts[alphaToIndex(in->b[oldI])]++;
	for (i = oldI + 1; i < in->l; i++) {
		if (isalpha(in->b[i])) {
			digraphCounts[alphaToIndex(in->b[oldI])][alphaToIndex(in->b[i])]++;
			oldI = i;
			monographCounts[alphaToIndex(in->b[i])]++;
		}
	}
	bufBridge[1] = alphaToIndex(in->b[oldI]);
}

static void
readAndCountLetters(long monographCounts[], long digraphCounts[][alphabetSize]) {
	enum { bufSize = 1 << 22 };
	static unsigned char inputBuf[bufSize];
	buffer in;
	in.b = inputBuf;

	unsigned char inBridgeBuf[2];

	bool eof = false;

	{
		size_t n = FREAD(in.b, bufSize, stdin);
		if (n != bufSize) {
			fprintf(stderr, "EOF: %24lu.\n", n);
			eof = true;
		}
		in.l = n;
		countLetters(&in, inBridgeBuf, monographCounts, digraphCounts);
	}
	for (; !eof;) {
		unsigned char previousLastChar;
		size_t n = FREAD(in.b, bufSize, stdin);
		if (n != bufSize) {
			fprintf(stderr, "EOF: %24lu.\n", n);
			eof = true;
		}
		if (n == 0) {
			break;
		}
		in.l = n;

		// There is a very small bug here: we are assuming that an alphabetic
		// letter will be found in every buffer-load of data.
		previousLastChar = inBridgeBuf[1];
		countLetters(&in, inBridgeBuf, monographCounts, digraphCounts);
		digraphCounts[previousLastChar][inBridgeBuf[0]]++;
	}

	if (ferror(stdin)) {
		fprintf(stderr, "Error reading from stdin!\n");
	}
	if (!feof(stdin)) {
		fprintf(stderr, "Stdin not at EOF!\n");
	}
}

static void
f(void) {
	static long monographCounts[alphabetSize];
	static long digraphCounts[alphabetSize][alphabetSize];

	if (alphabetSize != 26) {
		fprintf(stderr, "alphabetSize not 26.\n");
		return;
	}

	readAndCountLetters(monographCounts, digraphCounts);

	printCounts(monographCounts, digraphCounts);
}

int
main(void) {
	f();
	return 0;
}
