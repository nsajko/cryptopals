// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stdio.h>
#include <string.h>

#include "buffer/buffer.h"
#include "encoding/encoding.h"

void
f(const char *s) {
	size_t i;
	buffer hexBuf, *decodedBuf, *binaryBuf;

	hexBuf.l = strlen(s);
	hexBuf.b = s;

	decodedBuf = hexDecodeAlloc(&hexBuf);
	binaryBuf = binaryEncodeAlloc(decodedBuf);

	for (i = 0; i < decodedBuf->l; i++) {
		fprintf(stdout, "%8lu ", i);
	}
	fprintf(stdout, "\n");

	for (i = 0; i < decodedBuf->l; i++) {
		fprintf(stdout, "%8.2s ", &(hexBuf.b[i * 2]));
	}
	fprintf(stdout, "\n");

	for (i = 0; i < decodedBuf->l; i++) {
		fprintf(stdout, "%8.8s ", &(binaryBuf->b[i * 8]));
	}
	fprintf(stdout, "\n");

	bufferFree(decodedBuf);
	bufferFree(binaryBuf);
}

int
main(int argc, char *argv[]) {
	if (argc != 2) {
		return 1;
	}
	f(argv[1]);
	return 0;
}
