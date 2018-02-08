// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <ctype.h>  // isalnum
#include <stdio.h>  // fputc
#include <stdlib.h> // qsort

#include "alphabet/alphabet.h"
#include "buffer/buffer.h"
#include "distance/distance.h"
#include "sizeofMacros.h"

static void
brute(buffer *buf) {
	static double digraphFreqs[alphabetSize][alphabetSize];
	double freqs[alphabetSize];

	monographFreqCount(buf, freqs);
	printf("Monograph freq: %g\n", monographFreqScore(freqs, sumOfSquares));

	digraphFreqCount(buf, digraphFreqs);
	printf("Digraph freq: %g\n", digraphFreqScore(digraphFreqs, sumOfSquares));
}

static void
pr(buffer *o) {
	size_t i;
	for (i = 0; i < o->l; i++) {
		fputc(o->b[i], stdout);
	}
}

static unsigned char stringBuf[1 << 25];

static void
f(void) {
	for (;;) {
		buffer si;
		int i = 0;
		int chr;
		for (;;) {
			chr = fgetc(stdin);
			if (chr == EOF || !isalnum(chr)) {
				break;
			}
			stringBuf[i] = chr;
			i++;
		}
		for (; chr != EOF;) {
			chr = fgetc(stdin);
			if (isalnum(chr)) {
				ungetc(chr, stdin);
				break;
			}
		}
		si.b = stringBuf;
		si.l = i;

		brute(&si);

		pr(&si);
		fputc('\n', stdout);

		if (chr == EOF) {
			break;
		}
	}
}

int
main(void) {
	f();
	return 0;
}
