#include <stdbool.h>

#include "alphabet.h"
#include "../distance/distance.h"

// Monograph frequencies counted in strings with whitespace and punctuation removed.
// http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/english-letter-frequencies/
static const double enAlphabetFrequencies[alphabetSize] = {
	.0855, .0160, .0316, .0387, .1210, .0218, .0209, .0496, .0733,
	.0022, .0081, .0421, .0253, .0717, .0747, .0207, .0010, .0633,
	.0673, .0894, .0268, .0106, .0183, .0019, .0172, .0011};

double
freqScore(const double freqs[], distanceFunc f) {
	return f(freqs, enAlphabetFrequencies, alphabetSize);
}

bool
seemsLikeAPlaintextByMonographCount(const double freqs[], distanceFunc f) {
	return freqScore(freqs, f) <
	       monographCountPlaintextySumOfSquaresDistanceLimit;
}
