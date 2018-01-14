#include <stdio.h>

#include "digraphTableRead.h"

extern int fillArray(double arr[][sz]);

extern void printSortedTable(const double arr[][sz]);

static double freqs[sz][sz];

static void
arrayNormalizeAndPrint(void) {
	double sum = 0;
	int i, j;
	for (i = 0; i < sz; i++) {
		for (j = 0; j < sz; j++) {
			sum += freqs[i][j];
		}
	}
	if (sum == 0) {
		printf("Sum is zero\n");
		return;
	}

	for (i = 0; i < sz; i++) {
		for (j = 0; j < sz; j++) {
			freqs[i][j] = freqs[i][j] / sum;
		}
	}

	printf("static const double enDigraphFreqs[][] = {");
	for (i = 0; i < sz; i++) {
		printf("{");
		for (j = 0; j < sz; j++) {
			printf("%.55g, ", freqs[i][j]);
		}
		printf("}, ");
	}
	printf("};\n");

	// printSortedTable(freqs);
}

int
f(void) {
	int n = fillArray(freqs);

	arrayNormalizeAndPrint();

	return n;
}

int
main(void) {
	return f();
}
