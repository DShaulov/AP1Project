#ifndef TIMESERIES_H_
#define TIMESERIES_H
#include <map>
#include <vector>

using namespace std;

class TimeSeries{
private:
	map<string, vector<float>> m;
public:
	/**
	 * Construct a new Time Series object
	 */
	TimeSeries(const char* CSVfileName);
	/**
	 * Get a specific entry 
	 */
	float getEntry(string category, int timeStep);
};



#endif /* TIMESERIES_H_ */
