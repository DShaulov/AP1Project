/*
 * anomaly_detection_util.cpp
 *
 * Authors:
 * 317005403 David Shaulov,
 * 205544109 Yonatan Zilber
 * 
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
    float average = sum / size;
    return average;
}
// returns the variance of X and Y
float var(float* x, int size){
    if (size == 0) {
        throw "No points";
    }
	float sumOfSquares = 0;
	for (int i = 0; i < size; i++) {
		sumOfSquares += pow(x[i], 2);
	}
    float mean = avg(x, size);
    float meanSquared = mean * mean;
	float equationFirstPart = sumOfSquares / size;
    float result = equationFirstPart - meanSquared;
    if (result < 0) { // I replaced abs() because it was causing 5 tests to fail in practice mode.
        return -result;
    }
	return result;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    if (size == 0) {
        throw "No points";
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
    // Return 0 in case of division by 0
    if (sqrt(var(x, size)) == 0) {
        return 0;
    }
    if (sqrt(var(y, size)) == 0) {
        return 0;
    }
	return (cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size))));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	if (size == 0 || points == nullptr) {
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
    float covAll = cov(xVals, yVals, size);
    float varX = var(xVals, size);
    float varY = var(yVals, size);
    float a;
    if (varX == 0) {
        a = 0;
    }
    else {
        a = covAll / varX;
    }
    float b = avgY - (a * avgX);
    delete [] xVals;
    delete [] yVals;
    Line line = Line(a, b);
    return line;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    if (size == 0 || points == nullptr) {
        throw "No points";
    }
	Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	double expectedY = l.f(p.x);
    double actualY = p.y;
    double difference = expectedY - actualY;
    if (difference < 0) {
        return -difference; // maybe we should use absoult value? :: Tried it with abs in an earlier version, was failing one of the checks for some reason
    }
    return difference;
}