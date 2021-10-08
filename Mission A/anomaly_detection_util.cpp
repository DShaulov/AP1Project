#include "anomaly_detection_util.h"
#include <stdio.h>
#include <math.h>

float var(float* x, int size) {
	float sum = 0;
	float sumOfSquares = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
		sumOfSquares += pow(x[i], 2);
	}
	float mean = pow(((1 / size) * sum), 2);
	float equationFirstPart = ((1 / size) * sumOfSquares);
	return (equationFirstPart - mean);
}