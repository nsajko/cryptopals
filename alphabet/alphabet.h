#pragma once

#include "../buffer/buffer.h"

extern void freqCount(const buffer *in, double freqsOut[]);
extern double freqScore(const double freqs[]);
