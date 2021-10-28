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
        throw "No points"; // there is no var for 0 points
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
// we used the formula: 
//COV(X,Y) = E((X-E(X))Y-E(Y)) of final sample so E(X) is simple avarage.
float cov(float* x, float* y, int size){
    if (size == 0) {
        throw "No points"; // there isn't cov for 0 points
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
    // from the formula: Pearson = COV(X,Y)/(VAR(X)^0.5*VAR(Y)^0.5)
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	if (size == 0 || points == nullptr) {
        throw "Empty points array"; // cant create line
    }
    // get all points x and y values to array
    float* xVals = new float[size];
    float* yVals = new float[size];
    for (int i = 0; i < size; i++) {
        Point p = *points[i];
        xVals[i] = p.x;
        yVals[i] = p.y;
    }
    // find x and y axis avarage, and use the formula to create line
    // the formula: y = ax+b, when a = COV (X,Y)/VAR(X), and b = avgY - a*avgX
    float avgX = avg(xVals, size);
    float avgY = avg(yVals, size);

    float covAll = cov(xVals, yVals, size);
    float varX = var(xVals, size);
    float varY = var(yVals, size);
    float a = covAll / varX; // will it for sure return float and not int? just want to make sure :: Yes
    float b = avgY - (a * avgX);
    // free memory:
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
	float expectedY = l.f(p.x);
    float actualY = p.y;
    float difference = expectedY - actualY;
    if (difference < 0) {
        return -difference; 
    }
    return difference;
}