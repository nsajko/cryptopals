// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stdio.h>  // fprintf fread feof ungetc
#include <stdlib.h> // malloc
#include <string.h> // strlen

#include "bitstringDistance/bitstringDistance.h"
#include "buffer/buffer.h"
#include "encoding/encoding.h"
#include "sizeofMacros.h"
#include "xor/xor.h"

static void
f(const char *plaintext, const char *key) {
	buffer plainBuf, keyBuf, *encBuf, *outBuf;

	plainBuf.b = (unsigned char *)plaintext;
	plainBuf.l = strlen(plaintext);
	keyBuf.b = (unsigned char *)key;
	keyBuf.l = strlen(key);
	encBuf = repeatingXorAlloc(&plainBuf, &keyBuf);
	outBuf = base64EncodeAlloc(encBuf);
	FWRITE(outBuf->b, outBuf->l, stdout);
	bufferFree(outBuf);
	bufferFree(encBuf);
}

int
main(int argc, char *argv[]) {
	if (argc != 3) {
		return 1;
	}
	f(argv[1], argv[2]);
	return 0;
}
