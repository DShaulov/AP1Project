/**
 * 
 * Authors:
 * 317005403 David Shaulov,
 * 205544109 Yonatan Zilber
 * 
 * 
 */
#include "minCircle.h"
#include <stdlib.h>
#include <vector>
#include <math.h>

/**
 * Sets up the call to emoWelzAlgo function
 */
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> pointsOnBoundry;
    return emoWelzlAlgo(points, pointsOnBoundry, size);
}
/**
 *  Returns true if the point is inside the circle, false otherwise 
 */
bool pointInCircle(Circle circ, Point p) {
    Point center = circ.center;
    // Calculate distance of point from center
    double distance = sqrt((p.x - center.x)*(p.x - center.x) + (p.y - center.y)*(p.y - center.y));
    if (distance <= circ.radius) {
        return true;
    }
    return false;
}
/**
 * Create a circle centered at (0,0) with radius 0 
 */
Circle createAxisCircle() {
    Circle c(Point(0,0), 0);
    return c;
}
/**
 * Create a circle centerd at the point with radius 0 
 */
Circle createCircleFromPoint(Point p) {
    Circle c(p, 0);
    return c;
}
/**
 * Creates a circle where the center is the halfway point between the two points 
 */
Circle createCircleFrom2Points(Point p1, Point p2) {
    double midwayX = (p1.x + p2.x) / 2;
    double midwayY = (p1.y + p2.y) / 2;
    double radius = sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)) / 2;
    Circle c(Point(midwayX, midwayY), radius);
    return c;
}
/**
 *  First checks for case where one point can be contained in circle formed by other 2
 *  If not, creates a circle from 3 points based on the circumcircle of the 3 points 
 */
Circle createCircleFrom3Points(Point A, Point B, Point C) {
    // Check for case if circle from 2 points can contain the other one
    Circle c1 = createCircleFrom2Points(A, B);
    if (pointInCircle(c1, C)) {
        return c1;
    }
    Circle c2 = createCircleFrom2Points(A, C);
    if (pointInCircle(c2, B)) {
        return c2;
    }
    Circle c3 = createCircleFrom2Points(B, C);
    if (pointInCircle(c3, A)) {
        return c3;
    }
    // Create a circumcircle based on the 3 points
    // Calculate the center point (Based on a Wikipedia formula "Circumcenter Cartesian Coordinates")
    double d = 2 * (A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y));
    double x = ((A.x*A.x + A.y*A.y)*(B.y - C.y) + (B.x*B.x + B.y*B.y)*(C.y - A.y)+(C.x*C.x + C.y*C.y)*(A.y-B.y)) / d;
    double y = ((A.x*A.x + A.y*A.y)*(C.x - B.x) + (B.x*B.x + B.y*B.y)*(A.x - C.x)+(C.x*C.x + C.y*C.y)*(B.x-A.x)) / d;
    Point center(x,y);
    double radius = sqrt((center.x - A.x)*(center.x - A.x) + (center.y - A.y)*(center.y - A.y));
    Circle c4(center, radius);
    return c4;
}




/**
 * Based on the recursive algorithm by Emo Welzl:
 * Accepts 2 sets of points as input, one containing all points and one containing points on the boundry of the circle:
 * 1. Choose a point x at random
 * 2. Recursivly find minimal circle for points/{x}
 * 3. If the resulting circle contains x, then it is the minimal circle for all points, return it
 * 4. If the resulting circle does not contain x, then x must be on the boundry of the circle,
 *    Add x to the set of points on the boundry and call the algorithm again
 * Stopping conditions:
 * the point vector is empty or the number of points on boundry is 3 (Because the remaining points must be in the circle made up of points on the boundry)
 * 
 * 
 */
Circle emoWelzlAlgo(Point **points, vector<Point> pointsOnBoundry, size_t size) {
    if (size == 0 && pointsOnBoundry.size() == 0) {
        return createAxisCircle();
    }
    if (size == 0 && pointsOnBoundry.size() == 1) {
        return createCircleFromPoint(pointsOnBoundry[0]);
    }
    if (size == 0 && pointsOnBoundry.size() == 2) {
        return createCircleFrom2Points(pointsOnBoundry[0], pointsOnBoundry[1]);
    }
    if (pointsOnBoundry.size() == 3) {
        return createCircleFrom3Points(pointsOnBoundry[0], pointsOnBoundry[1], pointsOnBoundry[2]);
    }
    int randomIndex = rand() % size;
    Point *randomPoint = points[randomIndex];
    // Shuffle the pointer that is to be removed to the end of the pointer array to save time
    Point *tmp = points[size - 1];
    points[size - 1] = points[randomIndex];
    points[randomIndex] = tmp;
    Circle circ = emoWelzlAlgo(points, pointsOnBoundry, size - 1);
    if (pointInCircle(circ, *randomPoint)) {
        return circ;
    }
    pointsOnBoundry.push_back(*randomPoint);
    return emoWelzlAlgo(points, pointsOnBoundry, size - 1);
}
