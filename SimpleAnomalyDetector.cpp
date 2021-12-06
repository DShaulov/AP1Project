/**
 * 
 * Authors:
 * 317005403 David Shaulov,
 * 205544109 Yonatan Zilber
 * 
 */

#include "SimpleAnomalyDetector.h"


SimpleAnomalyDetector::SimpleAnomalyDetector()
{
	vector<correlatedFeatures> vec;
	this->cf = vec;
	this->correlationThreshhold = 0.9;
	this->distanceThreshholdMult = 1.15;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
	// TODO Auto-generated destructor stub
}

/**
 * Potential Problems:
 * The correlation is always saved as positive because of absolute value, maybe should store negative ones as negative?
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts)
{
	TimeSeries *ts1 = new TimeSeries(ts);
	vector<vector<float>> entryMatrix = ts1->getMatrix();
	vector<string> features = ts1->getFeatures();

	// For each feature, go over all other features and find the one that is collrelated the most.
	// Add them to the vector of correlated features
	for (int i = 0; i < features.size(); i++) {
		string firstFeature = features[i];
		string secondFeature;
		float prevMaxCorrelation = 0;
		for (int j = i + 1; j < features.size(); j++) {
			// &entryMatrix[j][0] is a trick to convert vector<float> to float *
			float absolutePearson = abs(pearson(&entryMatrix[i][0], &entryMatrix[j][0], entryMatrix[i].size()));
			if (absolutePearson > prevMaxCorrelation) {
				prevMaxCorrelation = absolutePearson;
				secondFeature = features[j];
			}
		}
		// Check that the features are not too weakly correlated
		if (prevMaxCorrelation < 0.5) {
			continue;
		}
		bool stronglyCorrelated = true;
		// Check if the features are strongly correlated
		if (0.5 <= prevMaxCorrelation && prevMaxCorrelation < correlationThreshhold) {
			stronglyCorrelated = false;
		}
		correlatedFeatures relatedFeaturePair;
		relatedFeaturePair.feature1 = firstFeature;
		relatedFeaturePair.feature2 = secondFeature;
		relatedFeaturePair.stronglyCorrelated = stronglyCorrelated;
		relatedFeaturePair.corrlation = prevMaxCorrelation;
		vector<float> firstFeatureData = ts1->getFeatureData(firstFeature);
		vector<float> secondFeatureData = ts1->getFeatureData(secondFeature);
		relatedFeaturePair.lin_reg = linear_reg(&firstFeatureData[0], &secondFeatureData[0], firstFeatureData.size());
		relatedFeaturePair.threshold = farestPoint(&firstFeatureData[0], &secondFeatureData[0], firstFeatureData.size(), relatedFeaturePair.lin_reg);
		cf.push_back(relatedFeaturePair);
	}
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
	vector<AnomalyReport> reportVector;
	TimeSeries *ts1 = new TimeSeries(ts);
	// For each pair of correlated features, go over every row in the time series, 
	for (int i = 0; i < cf.size(); i++) {
		correlatedFeatures featurePair = cf[i];
		// Case where the feature pair is below correlation threshhold
		if (!featurePair.stronglyCorrelated) {
			continue;
		}
		vector<float> firstFeatureData = ts1->getFeatureData(featurePair.feature1);
		vector<float> secondFeatureData = ts1->getFeatureData(featurePair.feature2);
		int dataSize = firstFeatureData.size();
		for (int j = 0; j < dataSize; j++) {
			Point dataPoint(firstFeatureData[j], secondFeatureData[j]);
			float distanceFromLine = dev(dataPoint, featurePair.lin_reg);
			// If distance from the linear regression line is larger than expected, create anomaly report
			// Comparing distanceFromLine > featurePair.threshhold is too sensitive
			float threshhold = distanceThreshholdMult * featurePair.threshold;
			if (distanceFromLine > threshhold) {
				AnomalyReport report(featurePair.feature1 + "-" + featurePair.feature2, j + 1);
				reportVector.push_back(report);
			}
		}
	}
	return reportVector;
}

vector<correlatedFeatures> SimpleAnomalyDetector::getNormalModel()
{
	return this->cf;
}
