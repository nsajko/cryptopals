#include <stdio.h>  // fputc
#include <string.h> // strlen

#include "../buffer/buffer.h"
#include "encoding.h"

void
pr(buffer *o) {
	size_t i;
	for (i = 0; i < o->l; i++) {
		fputc(o->b[i], stdout);
	}
}

void
f(const char *s) {
	buffer si;
	si.b = (unsigned char *)s;
	si.l = strlen(s);

	/*
	buffer *i = hexDecodeAlloc(&si);
	if (i == NULL) {
		fprintf(stderr, "error\n");
		return;
	}
	buffer *o = base64EncodeAlloc(i);
	*/

	buffer *i = base64DecodeAlloc(&si);
	if (i == NULL) {
		fprintf(stderr, "error\n");
		return;
	}
	buffer *o = hexEncodeAlloc(i);

	pr(o);
	fputc('\n', stdout);
	bufferFree(i);
	bufferFree(o);
}

int
main(int argc, char *argv[]) {
	if (argc != 2) {
		return 1;
	}
	f(argv[1]);
	return 0;
}
