#include <ctype.h>
#include <stddef.h>

#include "../buffer/buffer.h"
#include "alphabet.h"

static unsigned char
alphaToIndex(unsigned char c) {
	return tolower(c) - 'a';
}

// Returns count of alphabetic characters.
double
monographCount(const buffer *in, double freqsOut[]) {
	double sum = 0;
	size_t i;
	const int l = alphabetSize;
	for (i = 0; i < l; i++) {
		freqsOut[i] = 0;
	}
	for (i = 0; i < in->l; i++) {
		if (isalpha(in->b[i])) {
			freqsOut[alphaToIndex(in->b[i])]++;
			sum++;
		}
	}
	return sum;
}

void
freqCount(const buffer *in, double freqsOut[]) {
	size_t i;
	const int l = alphabetSize;
	double sum = monographCount(in, freqsOut);
	if (sum != 0) {
		for (i = 0; i < l; i++) {
			freqsOut[i] /= sum;
		}
	}
}

// Returns count of alphabetic characters.
double
digraphCount(const buffer *in, double freqsOut[][alphabetSize]) {
	double sum = 0;
	size_t i, oldI;
	int j;
	const int l = alphabetSize;
	for (i = 0; i < l; i++) {
		for (j = 0; j < l; j++) {
			freqsOut[i][j] = 0;
		}
	}
	for (oldI = 0; oldI < in->l && !isalpha(in->b[oldI]); oldI++) {
	}
	for (i = oldI + 1; i < in->l; i++) {
		if (isalpha(in->b[i])) {
			freqsOut[alphaToIndex(in->b[oldI])][alphaToIndex(in->b[i])]++;
			sum++;
			oldI = i;
		}
	}
	return sum;
}

void
digraphFreqCount(const buffer *in, double freqsOut[][alphabetSize]) {
	int i, j;
	const int l = alphabetSize;
	double sum = digraphCount(in, freqsOut);
	if (sum != 0) {
		for (i = 0; i < l; i++) {
			for (j = 0; j < l; j++) {
				freqsOut[i][j] /= sum;
			}
		}
	}
}
