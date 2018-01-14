#pragma once

#include <stdbool.h>

#include "../buffer/buffer.h"
#include "../distance/distance.h"

enum { alphabetSize = 26 };

extern void freqCount(const buffer *in, double freqsOut[]);
extern void digraphFreqCount(const buffer *in, double freqsOut[][alphabetSize]);

extern double freqScore(const double freqs[], distanceFunc f);
extern bool seemsLikeAPlaintextByMonographCount(const double freqs[], distanceFunc f);

extern double digraphFreqScore(const double freqs[][alphabetSize], distanceFunc f);
extern bool
seemsLikeAPlaintextByDigraphCount(const double freqs[][alphabetSize], distanceFunc f);
