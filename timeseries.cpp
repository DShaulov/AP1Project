#include "timeseries.h"
#include <stdio.h>
#include <fstream>
#include <algorithm>

using namespace std;

/**
 * @brief Construct a new Time Series:: Time Series object
 * Reads from provided CSV file and writes the data to a std::map.
 * Keys of map are the categories as strings and the values are the measurements at each time-step as vectors.
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
    vector<string> categories(numOfCols);
    vector<vector<float>> rowEntries(numOfCols);
    for (int i = 0; i < numOfCols; i++) {
        rowEntries[i].resize(numOfRows - 1);
    }
    string entry;
    for (int i = 0; i < numOfRows; i++) {
        for (int j = 0; j < numOfCols; j++) {
            // Case where its the category row
            if (i == 0) {
                if (j == numOfCols - 1) {
                    getline(file, entry);
                    categories[j] = entry;
                    continue;
                }
                getline(file, entry, ',');
                categories[j] = entry;
                continue;
            }
            if (j == numOfCols - 1) {
                    getline(file, entry);
                    float entryAsFloat = stof(entry);
                    rowEntries[j][i-1] = entryAsFloat;
                    continue;
            }
            getline(file, entry, ',');
            float entryAsFloat = stof(entry);
            rowEntries[j][i-1] = entryAsFloat;
        }
    }
    for (int i = 0; i < numOfCols; i++) {
        pair<string, vector<float>> p = make_pair(categories[i], rowEntries[i]);
        m.insert(p);
    }
}