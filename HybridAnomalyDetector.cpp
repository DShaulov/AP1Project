
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	this->relaxCircleModifier = 0.9999;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts)  {
	TimeSeries *ts1 = new TimeSeries(ts);
	vector<AnomalyReport> reportVector = SimpleAnomalyDetector::detect(ts);
	// Go over all weakly correlated features
	for (int i = 0; i < SimpleAnomalyDetector::cf.size(); i++) {
		if (!cf[i].stronglyCorrelated) {
			correlatedFeatures coFeatures = cf[i];
			vector<float> firstFeatureData = ts1->getFeatureData(coFeatures.feature1);
			vector<float> secondFeatureData = ts1->getFeatureData(coFeatures.feature2);
			// Create a point array from data
			Point *arr[firstFeatureData.size()];
			for (int j = 0; j < firstFeatureData.size(); j++) {
				Point *p = new Point(firstFeatureData[j], secondFeatureData[j]);
				arr[j] = p;
			}
			vector<Point> pointsOnBoundry;
			Circle minCircle = emoWelzlAlgo(arr, pointsOnBoundry, firstFeatureData.size());
			// Check that each point falls within the min circle
			for (int j = 0; j < firstFeatureData.size(); j++){
				Point p(firstFeatureData[j], secondFeatureData[j]);
				double dist = distance(p, minCircle.center);
				double relaxedRadius = minCircle.radius * relaxCircleModifier;
				if (dist > relaxedRadius) {
					// printf("Point not in circle: (%f, %f)\n", p.x, p.y);
					// printf("Time step: %d\n", j);
					// printf("Distance from center: %f\n", dist);
					// printf("Circle radius: %f\n", minCircle.radius);
					// printf("Relaxed radius: %f\n", relaxedRadius);
					// printf("\n");
					AnomalyReport report(cf[i].feature1 + "-" + cf[i].feature2, j + 1);
					reportVector.push_back(report);
				}
			}
		}
	}
	return reportVector;
}

/**
 *	Returns the distance between two points 
 */
double HybridAnomalyDetector::distance(Point a, Point b) {
	double distX = (a.x - b.x) * (a.x - b.x);
	double distY = (a.y - b.y) * (a.y - b.y);
	double dist = sqrt(distX + distY);
	return dist;
}