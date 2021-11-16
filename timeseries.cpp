#include "timeseries.h"
#include <stdio.h>
#include <fstream>
#include <algorithm>


using namespace std;

/**
 * @brief Construct a new Time Series:: Time Series object
 * Reads from provided CSV file and writes the data to a std::map.
 * Keys of map are the features as strings and the values are the measurements at each time-step as vectors.
 * For example, if the CSV file is:
 * Time (seconds),Altitude (feet),Air Speed (knots),Heading (deg)
 * 0.1,12000.2,250,45.0
 * 0.2,12001.0,250,45.3
 * 0.3,12000.5,250,45.8
 * then map["Time (seconds)"] = [0.1, 0.2, 0,3] (a vector<float> object)
 */
TimeSeries::TimeSeries(const char* CSVfileName){
    fstream file(CSVfileName);
    string line;
    getline(file, line);
    int numOfCols = count(line.begin(), line.end(), ',') + 1;
    int numOfRows = 1;
    while (getline(file, line)) {
        numOfRows += 1;
    }
    file.clear();
    file.seekg(0);
    vector<string> features(numOfCols);
    vector<vector<float>> entries(numOfCols);
    for (int i = 0; i < numOfCols; i++) {
        entries[i].resize(numOfRows - 1);
    }
    string entry;
    for (int i = 0; i < numOfRows; i++) {
        for (int j = 0; j < numOfCols; j++) {
            // Case where its the feature row
            if (i == 0) {
                if (j == numOfCols - 1) {
                    getline(file, entry);
                    features[j] = entry;
                    continue;
                }
                getline(file, entry, ',');
                features[j] = entry;
                continue;
            }
            if (j == numOfCols - 1) {
                    getline(file, entry);
                    float entryAsFloat = stof(entry);
                    entries[j][i-1] = entryAsFloat;
                    continue;
            }
            getline(file, entry, ',');
            float entryAsFloat = stof(entry);
            entries[j][i-1] = entryAsFloat;
        }
    }
    for (int i = 0; i < numOfCols; i++) {
        pair<string, vector<float>> p = make_pair(features[i], entries[i]);
        m.insert(p);
    }
    this->numOfCols = numOfCols;
    this->numOfRows = numOfRows;
}

/**
 * Returns the entry from the specified feature at the specified time-step
 */
float TimeSeries::getEntry(string feature, int timeStep) {
    return m[feature][timeStep];
}

/**
 * Returns a vector of all features
 */
vector<string> TimeSeries::getFeatures() {
    vector<string> features;
    for (map<string, vector<float>>::iterator iter = m.begin(); iter != m.end(); iter++) {
        string feature = iter->first;
        features.push_back(feature);
    }
    return features;
}

// returns the vector of one feature
vector<float> TimeSeries::getFeatureData(string feature) {
    vector<float> vec =  m[feature];
    //cout<<m[feature]<< endl;
   // cout << "Output of begin and end: \n";
   // for (auto i = vec.begin(); i != vec.end(); ++i)
     //   cout << *i << " "<<endl;
    //float* toRet = vec.data();
    return vec;

}


// return the number of types of parameters (like time, altitude etc.)
int TimeSeries::getNumOfCols(){

    return numOfCols;
}
// return the number of information dots we have (for example: if time is 0.1 0.2 0.3 0.4 it will return 4)
	int TimeSeries::getNumOfRows(){
        return numOfRows-1;
    }
