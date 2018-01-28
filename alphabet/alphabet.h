#pragma once

#include <stdbool.h>

#include "../buffer/buffer.h"
#include "../distance/distance.h"

enum { alphabetSize = 26 };

static const double monographCountPlaintextySumOfSquaresDistanceLimit = 0.046875,
		    digraphCountPlaintextySumOfSquaresDistanceLimit = 0.00035;

extern double monographCount(const buffer *in, double freqsOut[]);
extern void monographFreqCount(const buffer *in, double freqsOut[]);
extern double digraphCount(const buffer *in, double freqsOut[][alphabetSize]);
extern void digraphFreqCount(const buffer *in, double freqsOut[][alphabetSize]);

extern double monographFreqScore(const double freqs[], distanceFunc f);
extern bool seemsLikeAPlaintextByMonographCount(const double freqs[], distanceFunc f);

extern double digraphFreqScore(const double freqs[][alphabetSize], distanceFunc f);
extern bool
seemsLikeAPlaintextByDigraphCount(const double freqs[][alphabetSize], distanceFunc f);
