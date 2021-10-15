#include "anomaly_detection_util.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

float mean(float* x, int size) {
    float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
	}
    return (sum / size);
}
float var(float* x, int size) {
	float sumOfSquares = 0;
	for (int i = 0; i < size; i++) {
		sumOfSquares += pow(x[i], 2);
	}
	float meanSquared = pow(mean(x, size), 2);
	float equationFirstPart = ((1.0 / size) * sumOfSquares);
	return (equationFirstPart - meanSquared);
}
// 𝑐𝑜𝑣(𝑋,𝑌) = 𝐸((𝑋−𝐸(𝑋))(𝑌−𝐸(𝑌)), E is simple mean
float cov(float* x, float* y, int size) {
    float meanOfX = mean(x, size);
    float meanOfY = mean(y, size);
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
    Point *pointArr = *points;
    for (int i = 0; i < size; i++) {
        xVals[i] = pointArr[i].x;
        yVals[i] = pointArr[i].y;
    }
    float a = cov(xVals, yVals, size) / var(xVals, size);
    float b = mean(yVals, size) - (a * mean(xVals, size));
    delete[] xVals;
    delete[] yVals;
    Line line = Line(a, b);
    return line;
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
    // Expected: 116.68
    std::cout << cov(coTestArray3, coTestArray4, 5) << std::endl;

    // Pearson tests:
    // Expected: 0.936601
    std::cout << pearson(coTestArray3, coTestArray4, 5) << std::endl;
    // Expected: -0.9189
    std::cout << pearson(coTestArray2, coTestArray3, 5) << std::endl;
    // Linear reg test:
    Point *pointArray = (Point *) malloc(sizeof(Point) * 4);
    Point *pointArray2 = (Point *) malloc(sizeof(Point) * 4);
    pointArray[0] = Point(1, 2);
    pointArray[1] = Point(2, 3);
    pointArray[2] = Point(3, 4);
    pointArray[3] = Point(4, 5);
    pointArray[0] = Point(3, -50);
    pointArray[1] = Point(2, 8);
    pointArray[2] = Point(5, 20);
    pointArray[3] = Point(8, 3);
    Line line = linear_reg(&pointArray, 4);
    Line line2 = linear_reg(&pointArray, 4);
    std::cout << "Hello World" << std:: endl;
}