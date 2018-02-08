// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

#include <stdio.h>

#include "digraphTableRead.h"

// Returns less than zero on error.
int
fillArray(double arr[][sz]) {
	int i, j;
	for (;;) {
		int r;
		int curSym = fgetc(stdin);
		if (curSym == EOF) {
			return 0;
		}
		if (curSym < 'A' || 'Z' < curSym) {
			return -1;
		}
		i = curSym - 'A';
		curSym = fgetc(stdin);
		if (curSym < 'A' || 'Z' < curSym) {
			return -2;
		}
		j = curSym - 'A';
		curSym = fgetc(stdin);
		if (curSym != ' ') {
			return -3;
		}
		r = scanf("%lf", &(arr[i][j]));
		if (r != 1) {
			return -4;
		}
		curSym = fgetc(stdin);
		if (curSym == EOF) {
			return 0;
		}
		if (curSym != '\n') {
			return -5;
		}
	}
}
