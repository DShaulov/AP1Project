#include "anomaly_detection_util.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

float var(float* x, int size) {
	float sum = 0;
	float sumOfSquares = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
		sumOfSquares += pow(x[i], 2);
	}
	float mean = pow(((1.0 / size) * sum), 2);
	float equationFirstPart = ((1.0 / size) * sumOfSquares);
	return (equationFirstPart - mean);
}
// 𝑐𝑜𝑣(𝑋,𝑌) = 𝐸((𝑋−𝐸(𝑋))(𝑌−𝐸(𝑌)), E is simple mean
float cov(float* x, float* y, int size) {
    float sumOfX = 0;
    float sumOfY = 0;
    for (int i = 0; i < size; i++) {
		sumOfX += x[i];
		sumOfY += y[i];
	}
    float meanOfX = sumOfX / size;
    float meanOfY = sumOfY / size;
    float covSum = 0;
    for (int i = 0; i < size; i++) {
        covSum += (x[i] - meanOfX) * (y[i] - meanOfY);
    }
    return (covSum / size);
}
float pearson(float* x, float* y, int size) {
    return (cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size))));
}
Line linear_reg(Point** points, int size) {
    if (*points == nullptr) {
        throw "Empty points array";
    }
    float* xVals = new float[size];
    float* yVals = new float[size];


    delete[] xVals;
    delete[] yVals;
}

// TESTS
int main() {
    // Variance tests:
    float floatArray1[4] = {1, 2, 3, 4};
    float floatArray2[4] = {1.25, 4.46, 7.89, 10.45};
    float floatArray3[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float floatArray4[5] = {2, 4, 6, 8, 10};

    // Expected: 1.25
    std::cout << var(floatArray1, 4) << std::endl;
    // Expected: 12.077
    std::cout << var(floatArray2, 4) << std::endl;
    // Expected: 8.25
    std::cout << var(floatArray3, 10) << std::endl;
    // Expected: 8
    std::cout << var(floatArray4, 5) << std::endl;
    // Covariance tests:
    float coTestArray1[5] = {2, 4, 6, 8, 10};
    float coTestArray2[5] = {12, 11, 8, 3, 1};
    float coTestArray3[5] = {5, 12, 18, 23, 45};
    float coTestArray4[5] = {2, 8, 18, 20, 28};

    // Expected: -12
    std::cout << cov(coTestArray1, coTestArray2, 5) << std::endl;
    // Expected: 146.1
    std::cout << cov(coTestArray3, coTestArray4, 5) << std::endl;

    // Pearson tests:
    // Expected: 0.936601
    std::cout << pearson(coTestArray3, coTestArray4, 5) << std::endl;
    // Expected: -0.9189
    std::cout << pearson(coTestArray2, coTestArray3, 5) << std::endl;

}