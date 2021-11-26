#include "minCircle.h"
#include <stdlib.h>
#include <math.h>

/**
 * Sets up the call to emoWelzAlgo function
 */
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> pointsAsVector;
    for (int i = 0; i < size; i++) {
        pointsAsVector.push_back(*points[i]);
    }
    vector<Point> pointsOnBoundry;
    return emoWelzlAlgo(pointsAsVector, pointsOnBoundry, size);
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
 * Create the trivial circle centered at (0,0) with radius 0 
 */
Circle createTrivialCircle() {
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
Circle createCircleFrom3Points(Point p1, Point p2, Point p3) {

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
Circle emoWelzlAlgo(vector<Point> points, vector<Point> pointsOnBoundry, size_t size) {
    if (points.size() == 0 && pointsOnBoundry.size() == 0) {
        return createTrivialCircle();
    }
    if (points.size() == 0 && pointsOnBoundry.size() == 1) {
        return createCircleFromPoint(pointsOnBoundry[0]);
    }
    if (points.size() == 0 && pointsOnBoundry.size() == 2) {
        return createCircleFrom2Points(pointsOnBoundry[0], pointsOnBoundry[1]);
    }
    if (pointsOnBoundry.size() == 3) {
        return createCircleFrom3Points(pointsOnBoundry[0], pointsOnBoundry[1], pointsOnBoundry[2]);
    }
    int randomIndex = rand() % size;
    Point randomPoint = points[randomIndex];
    points.erase(points.begin() + randomIndex);
    Circle circ = emoWelzlAlgo(points, pointsOnBoundry, size - 1);
    if (pointInCircle(circ, randomPoint)) {
        return circ;
    }
    pointsOnBoundry.push_back(randomPoint);
    return emoWelzlAlgo(points, pointsOnBoundry, size);
}
