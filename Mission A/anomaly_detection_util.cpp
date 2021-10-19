/*
 * animaly_detection_util.cpp
 *
 * Authors:
 * 317005403, David Shaulov,
 *  
 */

#include <math.h>
#include "anomaly_detection_util.h"
#include <iostream>

// Calculates the average value of the float array
float avg(float* x, int size){
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += x[i];
	}
    float average = static_cast<float>(sum) / static_cast<float>(size);
    return average;
}
// Rounds a float to 3 decimal places
double round3DecimalPts(float f) {
    double num = (int)(f * 1000 + 0.5);
    double rounded = (double)num / 1000;
    return rounded;
}

// returns the variance of X and Y
float var(float* x, int size){
    if (size == 0) {
        throw "No points";
    }
    if (size == 1) {
        throw "Must enter atleast 2 values";
    }
	float sumOfSquares = 0;
	for (int i = 0; i < size; i++) {
		sumOfSquares += pow(x[i], 2);
	}
	float meanSquared = pow(avg(x, size), 2);
	float equationFirstPart = ((1.0 / size) * sumOfSquares);
	return (equationFirstPart - meanSquared);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    if (size == 0) {
        throw "No points";
    }
    if (size == 1) {
        throw "Must enter atleast 2 values each";
    }
	float meanOfX = avg(x, size);
    float meanOfY = avg(y, size);
    float covSum = 0;
    for (int i = 0; i < size; i++) {
        covSum += (x[i] - meanOfX) * (y[i] - meanOfY);
    }
    return (covSum / size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return (cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size))));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	if (size == 0 || points == NULL) {
        throw "Empty points array";
    }
    float* xVals = new float[size];
    float* yVals = new float[size];
    for (int i = 0; i < size; i++) {
        Point p = *points[i];
        xVals[i] = p.x;
        yVals[i] = p.y;
    }
    float avgX = avg(xVals, size);
    float avgY = avg(yVals, size);
    float covAll = cov(xVals, yVals, 10);
    float varX = var(xVals, 10);
    float varY = var(yVals, 10);
    float a = covAll / varX;
    float b = avgY - (a * avgX);
    delete[] xVals;
    delete[] yVals;
    Line line = Line(a, b);
    return line;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    if (size == 0 || points == NULL) {
        throw "No points";
    }
	Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	double expectedY = l.f(p.x);
    double actualY = p.y;
    double difference = fabs(expectedY - actualY);
    double rounded = round3DecimalPts(difference);
    return rounded;
}