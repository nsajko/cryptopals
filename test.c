// Prints all possible single byte XOR codings of a string, sorted by proximity of the
// english letter frequency stats.

#include <stdio.h>  // fputc
#include <stdlib.h> // qsort
#include <string.h> // strlen

#include "alphabet/alphabet.h"
#include "buffer/buffer.h"
#include "encoding/encoding.h"
#include "sizeofMacros.h"
#include "xor/xor.h"

typedef struct {
	unsigned char key;
	buffer *b;
	double distance;
} sortingMember;

static sortingMember arr[0xff];

static int
qsortCompar(const void *m1, const void *m2) {
	if (((sortingMember *)m1)->distance < ((sortingMember *)m2)->distance) {
		return -1;
	}
	return 1;
}

static void
bruteAndSort(buffer *buf) {
	int k;
	for (k = 0; k < ARRAY_SIZE(arr); k++) {
		unsigned char key = k;
		double freqs[28];

		arr[key].key = key;
		arr[key].b = xorAlloc(buf, key);

		freqCount(arr[key].b, freqs);
		arr[key].distance = freqScore(freqs);
	}
	QSORT(arr, ARRAY_SIZE(arr), qsortCompar);
}

static void
freeArr(void) {
	int k;
	for (k = 0; k < ARRAY_SIZE(arr); k++) {
		bufferFree(arr[k].b);
	}
}

static void
pr(buffer *o) {
	size_t i;
	for (i = 0; i < o->l; i++) {
		fputc(o->b[i], stdout);
	}
}

static void
printArr(void) {
	int i;
	for (i = 0; i < ARRAY_SIZE(arr); i++) {
		if (arr[i].distance < 0.1) {
			fprintf(stdout, "%20g %2x ", arr[i].distance, arr[i].key);
			pr(arr[i].b);
			fputc('\n', stdout);
		}
	}
	fputc('\n', stdout);
}

static void
f(const char *s) {
	buffer si;

	si.b = (unsigned char *)s;
	si.l = strlen(s);

	buffer *bufI = hexDecodeAlloc(&si);
	if (bufI == NULL) {
		fprintf(stderr, "error\n");
		return;
	}

	bruteAndSort(bufI);
	printArr();

	freeArr();
	bufferFree(bufI);
}

int
main(int argc, char *argv[]) {
	if (argc != 2) {
		return 1;
	}
	f(argv[1]);
	return 0;
}
