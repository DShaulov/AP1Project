
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	this->relaxCircleModifier = 1.1;
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
			// Check that each point falls within the min circle
			for (int j = 0; j < firstFeatureData.size(); j++){
				Point p(firstFeatureData[j], secondFeatureData[j]);
				double dist = distance(p, coFeatures.minCircle->center);
				if (dist > coFeatures.minCircle->radius) {
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