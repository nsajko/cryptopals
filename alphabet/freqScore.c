#include <ctype.h>
#include <stddef.h>

#include "../buffer/buffer.h"
#include "../sizeofMacros.h"

// http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/english-letter-frequencies/
static const double enAlphabetFrequencies[] = {
	.0855, .0160, .0316, .0387, .1210, .0218, .0209, .0496, .0733,
	.0022, .0081, .0421, .0253, .0717, .0747, .0207, .0010, .0633,
	.0673, .0894, .0268, .0106, .0183, .0019, .0172, .0011};

void
freqCount(const buffer *in, double freqsOut[]) {
	double sum = 0;
	size_t i;
	const int l = ARRAY_SIZE(enAlphabetFrequencies);
	for (i = 0; i < l; i++) {
		freqsOut[i] = 0;
	}
	for (i = 0; i < in->l; i++) {
		if (isalpha(in->b[i])) {
			freqsOut[tolower(in->b[i]) - 'a']++;
			sum++;
		}
	}
	for (i = 0; i < l; i++) {
		freqsOut[i] /= sum;
	}
}

static double
square(double x) {
	return x * x;
}

double
freqScore(const double freqs[]) {
	double distance = 0;
	int i;
	const int l = ARRAY_SIZE(enAlphabetFrequencies);
	for (i = 0; i < l; i++) {
		distance += square(freqs[i] - enAlphabetFrequencies[i]);
	}
	return distance;
}
