
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
Circle emoWelzlAlgo(Point** points, vector<Point> pointsOnBoundry, size_t size);
Circle createTrivialCircle();
Circle createCircleFromPoint(Point p);
Circle createCircleFrom2Points(Point p1, Point p2);
Circle createCircleFrom3Points(Point p1, Point p2, Point p3);
bool pointInCircle(Circle circ, Point p);

#endif /* MINCIRCLE_H_ */
