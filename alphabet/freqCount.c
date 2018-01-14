#include <ctype.h>
#include <stddef.h>

#include "alphabet.h"
#include "../buffer/buffer.h"

static unsigned char
alphaToIndex(unsigned char c) {
	return tolower(c) - 'a';
}

void
freqCount(const buffer *in, double freqsOut[]) {
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
	if (sum != 0) {
		for (i = 0; i < l; i++) {
			freqsOut[i] /= sum;
		}
	}
}

void
digraphFreqCount(const buffer *in, double freqsOut[][alphabetSize]) {
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
	if (sum != 0) {
		for (i = 0; i < l; i++) {
			for (j = 0; j < l; j++) {
				freqsOut[i][j] /= sum;
			}
		}
	}
}
