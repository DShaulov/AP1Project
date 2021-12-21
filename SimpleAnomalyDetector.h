#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
//#include "timeseries.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	bool stronglyCorrelated;
	Circle *minCircle;
};



class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
	vector<correlatedFeatures> cf;
	float correlationThreshhold;
	float distanceThreshholdMult;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel();
	void setCorrelationThreshhold(int newThreshhold);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
