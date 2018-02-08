// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "digraphTableRead.h"
#include "../../../sizeofMacros.h"

static double sortingArr[sz * sz];

static int
qsortCompar(const void *m1, const void *m2) {
	if (*(double *)m1 < *(double *)m2) {
		return -1;
	}
	return 1;
}

void
printSortedTable(const double arr[][sz]) {
	int i;

	// memcpy(sortingArr, arr, sizeof(sortingArr));
	for (i = 0; i < ARRAY_SIZE(sortingArr); i++) {
		sortingArr[i] = arr[i / sz][i % sz];
	}

	for (i = 0; i < ARRAY_SIZE(sortingArr); i++) {
		printf("%20g", sortingArr[i]);
	}
	printf("\n\n");

	QSORT(sortingArr, ARRAY_SIZE(sortingArr), qsortCompar);

	printf("\n");

	for (i = 0; i < ARRAY_SIZE(sortingArr); i++) {
		// The casts spill Intel x87 doubles to memory to make them 64-bit properly
		// ... ?
		// printf("%20g %lx %ld", sortingArr[i], (unsigned long)sortingArr[i],
		// (long)sortingArr[i]);
		printf("%20g", sortingArr[i]);
	}
	printf("\n");
}
