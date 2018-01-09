#include <stdio.h>  // fputc
#include <string.h> // strlen

#include "buffer/buffer.h"
#include "encoding/encoding.h"
#include "xor/xor.h"

static void
pr(buffer *o) {
	size_t i;
	for (i = 0; i < o->l; i++) {
		fputc(o->b[i], stdout);
	}
}

static void
f(const char *s, const char *s1) {
	buffer si, si1;

	si.b = (unsigned char *)s;
	si.l = strlen(s);

	si1.b = (unsigned char *)s1;
	si1.l = strlen(s1);

	buffer *bufI = hexDecodeAlloc(&si);
	if (bufI == NULL) {
		fprintf(stderr, "error\n");
		return;
	}

	buffer *bufI1 = hexDecodeAlloc(&si1);
	if (bufI1 == NULL) {
		fprintf(stderr, "error\n");
		return;
	}

	buffer *out = fixedXor(bufI, bufI1);
	if (out == NULL) {
		fprintf(stderr, "error\n");
		return;
	}

	bufferFree(bufI);
	bufferFree(bufI1);


	/*
	buffer *o = base64EncodeAlloc(bufI);
	*/

	/*
	buffer *bufI = base64DecodeAlloc(&si);
	if (bufI == NULL) {
		fprintf(stderr, "error\n");
		return;
	}
	*/

	buffer *o = hexEncodeAlloc(out);
	if (out == NULL) {
		fprintf(stderr, "error\n");
		return;
	}

	bufferFree(out);

	pr(o);
	fputc('\n', stdout);

	/*
	bufferFree(bufI);
	*/

	bufferFree(o);
}

int
main(int argc, char *argv[]) {
	if (argc != 3) {
		return 1;
	}
	f(argv[1], argv[2]);
	return 0;
}
