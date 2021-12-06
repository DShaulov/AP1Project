
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
	TimeSeries *ts1 = new TimeSeries(ts);
	SimpleAnomalyDetector::learnNormal(ts);
	// Go over all weakly correlated features
	printf("Hybrid learn normal\n");
	for (int i = 0; i < SimpleAnomalyDetector::cf.size(); i++) {
		if (!cf[i].stronglyCorrelated) {
			vector<float> firstFeatureData = ts1->getFeatureData(cf[i].feature1);
			vector<float> secondFeatureData = ts1->getFeatureData(cf[i].feature2);
			// Create a point array from data
			Point *arr[firstFeatureData.size()];
			for (int j = 0; j < firstFeatureData.size(); j++) {
				Point *p = new Point(firstFeatureData[j], secondFeatureData[j]);
				arr[j] = p;
			}
			vector<Point> pointsOnBoundry;
			Circle minCircle = emoWelzlAlgo(arr, pointsOnBoundry, firstFeatureData.size());
			cf[i].minCircle = &minCircle;
		}
	}
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts)  {
	TimeSeries *ts1 = new TimeSeries(ts);
	vector<AnomalyReport> reportVector = SimpleAnomalyDetector::detect(ts);
	// Go over all weakly correlated features
	printf("Hybrid detect\n");
	for (int i = 0; i < SimpleAnomalyDetector::cf.size(); i++) {
		if (!cf[i].stronglyCorrelated) {
			correlatedFeatures coFeatures = cf[i];
			vector<float> firstFeatureData = ts1->getFeatureData(coFeatures.feature1);
			vector<float> secondFeatureData = ts1->getFeatureData(coFeatures.feature2);
			// Check that each point falls within the min circle
			for (int j = 0; j < firstFeatureData.size(); j++){
				Point p(firstFeatureData[j], secondFeatureData[j]);
				if (!pointInCircle(*(coFeatures.minCircle), p)) {
					printf("Circle radius: %f\n", coFeatures.minCircle->radius);
					printf("Point not in circle\n");
					AnomalyReport report(cf[i].feature1 + "-" + cf[i].feature2, j + 1);
					reportVector.push_back(report);
				}
			}
		}
	}
	return reportVector;
}