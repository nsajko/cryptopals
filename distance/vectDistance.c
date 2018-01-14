static double
square(double x) {
	return x * x;
}

// Sum of squares distance, ie. Euclidean distance but without taking the square root.
double
sumOfSquares(const double v1[], const double v2[], int l) {
	double distance = 0;
	int i;
	for (i = 0; i < l; i++) {
		distance += square(v1[i] - v2[i]);
	}
	return distance;
}
