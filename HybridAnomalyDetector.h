

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"


class HybridAnomalyDetector:public SimpleAnomalyDetector {
	float relaxCircleModifier;
public:
	HybridAnomalyDetector();
	virtual vector<AnomalyReport> detect(const TimeSeries& ts) override;
	virtual ~HybridAnomalyDetector();
	double distance(Point a, Point b);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
