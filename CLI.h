/**
 * 
 * Authors:
 * 317005403 David Shaulov,
 * 205544109 Yonatan Zilber
 * 
 * 
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
#include <vector>

using namespace std;

class CLI {
	DefaultIO *dio;
	ifstream *inputFile;
	vector<Command *> commands;
	HybridAnomalyDetector *detector;
	vector<AnomalyReport> *report;
	vector<Anomaly *> *anomalyVector;
	int TestCsvLen;
public:
	CLI(DefaultIO* dio);
	void start();
	void printMenu();
	virtual ~CLI();
};

#endif /* CLI_H_ */
