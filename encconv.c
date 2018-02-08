// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stddef.h>
#include <stdio.h>

#include "buffer/buffer.h"
#include "encoding/encoding.h"
#include "sizeofMacros.h"

typedef enum { hexToBase64Enum = 0, base64ToHexEnum } convProgEnum;

void
f(convProgEnum m) {
	typedef buffer *coderAlloc(const buffer *in);
	coderAlloc *(progArr[][2]) = {{hexDecodeAlloc, base64EncodeAlloc},
				      {base64DecodeAlloc, hexEncodeAlloc}};
	static unsigned char inArr[1 << 21];
	size_t n;
	buffer inBuf, *decodedBuf, *outBuf;

	n = FREAD(inArr, ARRAY_SIZE(inArr), stdin);
	if (!feof(stdin)) {
		fprintf(stderr, "stdin not at eof!\n");
		return;
	}
	inBuf.b = inArr;
	inBuf.l = n;
	decodedBuf = progArr[m][0](&inBuf);
	outBuf = progArr[m][1](decodedBuf);
	FWRITE(outBuf->b, outBuf->l, stdout);
	bufferFree(outBuf);
	bufferFree(decodedBuf);
}

int
main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Call as: %s hextobase64 or %s base64tohex\n", argv[0],
			argv[0]);
		return 1;
	}
	if (argv[1][0] == 'h') {
		f(hexToBase64Enum);
	} else {
		f(base64ToHexEnum);
	}
	return 0;
}
