#ifndef TIMESERIES_H_
#define TIMESERIES_H
#include <map>
#include <vector>

using namespace std;

class TimeSeries{
private:
	map<string, vector<float>> m;
	int numOfCols;
	int numOfRows;
public:
	/**
	 * Construct a new Time Series object
	 */
	TimeSeries(const char* CSVfileName);
	/**
	 * Get a specific entry 
	 */
	float getEntry(string feature, int timeStep);
	/**
	 * Returns a vector of all features
	 */
	vector<string> getFeatures();
};



#endif /* TIMESERIES_H_ */
