

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes

// you may edit this class
class Command{
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes
/**
 * Class corresponding to first menu option
 */
class uploadTS:public Command {
	ifstream *inputFile;
	int *TestCsvLen;
	private:
		enum uploadType {
			Train,
			Test
		};
	public:
		/**
		 * @brief Construct a new upload T S object
		 */
		uploadTS(DefaultIO *dio, ifstream *file, int *testLen) : Command(dio) {
			inputFile = file;
			TestCsvLen = testLen;
		};
		/**
		 * Prompts user for input and call createCSV function
		 */
		void execute() {
			printf("Please upload your local train CSV file.\n");
			createCSV(Train);
			printf("Upload complete.\n");
			printf("Please upload your local train CSV file.\n");
			createCSV(Test);
			printf("Upload complete.\n");
		}
		/**
		 * Reads from input.txt and writes to train/test csv file.
		 */
		void createCSV(uploadType type) {
			ofstream outputFile;
			if (type == Train) {
				outputFile.open("anomalyTrain.csv");
			}
			else {
				outputFile.open("anomalyTest.csv");
			}
			string line;
			getline(*inputFile, line);
			while (true) {
				outputFile << line;
				if (type == Test) {
					*TestCsvLen += 1;
				}
				getline(*inputFile, line);
				if (line == "done") {
					break;
				}
				outputFile << "\n";
			}

			
		}
};


/**
 * Class corresponding to second menu option
 */
class algSettings:public Command {
	HybridAnomalyDetector *detector;
	ifstream *inputFile;
	public:
		/**
		 * @brief Construct a new alg Settings object
		 */
		algSettings(DefaultIO *dio, HybridAnomalyDetector *ad, ifstream *ifile) : Command(dio) {
			detector = ad;
			inputFile = ifile;
		};
		/**
		 * @brief Displays the current correlation threshold and gives the option to change it
		 */
		void execute() {
			string line;
			float newThreshold;
			// Keep querying the user for a nubmer while it is not within 0 < x < 1
			while (true) { 
				cout << "The current correlation threshold is " << this->detector->correlationThreshhold << endl;
				getline(*inputFile, line);
				newThreshold = stof(line);
				if (0 < newThreshold && newThreshold < 1) {
					detector->correlationThreshhold = newThreshold;
					break;
				}
				cout << "please choose a value between 0 and 1.\n";
			}
		}
};
/**
 * Class corresponding to third menu option
 */
class detectAnomaly:public Command {
	HybridAnomalyDetector *detector;
	vector<AnomalyReport> **report;
	public:
		detectAnomaly(DefaultIO *dio, HybridAnomalyDetector *ad, vector<AnomalyReport> **reportPointer): Command(dio) {
			this->detector = ad;
			report = reportPointer;
		}	
		/**
		 * @brief Runs the hybrid algorithm on the uploaded CSV files
		 * 
		 */
		void execute() {
			TimeSeries *tsTrain = new TimeSeries("anomalyTrain.csv");
			TimeSeries *tsTest = new TimeSeries("anomalyTest.csv");
			this->detector->learnNormal(*tsTrain);
			vector<AnomalyReport> *rep = new vector<AnomalyReport>(this->detector->detect(*tsTest));
			*report = rep;
			cout << "Anomaly detection complete\n";	
		}
};
/**
 * Class corresponding to fourth menu option
 */
class displayResults:public Command {
	vector<AnomalyReport> **report;
	public:
		/**
		 * @brief Construct a new display Results object
		 */
		displayResults(DefaultIO *dio, vector<AnomalyReport> **rep): Command(dio) {
			this->report = rep;
		};
		/**
		 * @brief Prints the anomaly report
		 */
		void execute() {
			vector<AnomalyReport> rep = **(this->report);
			int vecLength = rep.size();
			for (int i = 0; i < vecLength; i++) {
				cout << rep[i].timeStep << "\t" << rep[i].description << endl;
			}
		}
};
/**
 * Class corresponding to fifth menu option
 */
class uploadAndAnalyze:public Command {
	HybridAnomalyDetector *detector;
	ifstream *inputFile;
	vector<AnomalyReport> **report;
	int *testCsvLen;
	struct Anomaly {
		int startTimeStep;
		int endTimeStep;
		int totalTimeSteps;
		int numTruePositives;
		bool isTruePositive;
	};
	public:
		uploadAndAnalyze(DefaultIO *dio, HybridAnomalyDetector *dt, ifstream *iFile, vector<AnomalyReport> **rep, int *testCsvLen): Command(dio) {
			this->detector = dt;
			this->inputFile = iFile;
			this->report = rep;
			this->testCsvLen = testCsvLen;
		};
		/**
		 * @brief Reads anomalies from user, and checks to see how many of them are actually anomalous
		 */
		void execute() {
			cout << "Please upload your local anomalies file.\n";
			vector<Anomaly *> *reportedAnomalies = new vector<Anomaly *>();
			string line;
			int totalReportedTimeSteps = 0;
			while(true) {
				getline(*inputFile, line);
				if (line == "done") {
					break;
				}
				Anomaly *report = new Anomaly();
				report->startTimeStep = stoi(line.substr(0, line.find(",")));
				report->endTimeStep = stoi(line.substr(line.find(",") + 1, line.length()));
				report->totalTimeSteps = report->endTimeStep - report->startTimeStep + 1;
				totalReportedTimeSteps += report->totalTimeSteps;
				reportedAnomalies->push_back(report);
			}
			int detectedPositives = 0;
			int numTruePositives = 0;
			vector<Anomaly *> *parsedAnomalies = parseAnomalies(&detectedPositives);
			int reportedAnomaliesSize = reportedAnomalies->size();
			int parsedAnomaliesSize = parsedAnomalies->size();
			// For each reported anomaly, compares it to a previously detected anomaly and checks the overlap
			for (int i = 0; i < reportedAnomaliesSize; i++) {
				Anomaly *rAnomaly = reportedAnomalies->at(i);
				for (int j = 0; j < parsedAnomaliesSize; j++) {
					Anomaly *pAnomaly = parsedAnomalies->at(j);
					if (rAnomaly->endTimeStep < pAnomaly->startTimeStep) {
						continue;
					}
					if (pAnomaly->endTimeStep < rAnomaly->startTimeStep) {
						continue;
					}
					else {
						int overlap = rAnomaly->endTimeStep - pAnomaly->startTimeStep + 1;
						rAnomaly->numTruePositives = rAnomaly->numTruePositives + overlap;
						numTruePositives += overlap;
						pAnomaly->isTruePositive = true;
					}
				}
			}
			// Count how many true positive reports are there
			int truePositiveCounter = 0;
			for (int i = 0; i < parsedAnomaliesSize; i++) {
				if (parsedAnomalies->at(i)->isTruePositive) {
					truePositiveCounter += 1;
				}
			}
			int falsePositiveCounter = parsedAnomaliesSize - truePositiveCounter;
			float truePositiveRate = static_cast<float>(truePositiveCounter) / static_cast<float>(reportedAnomaliesSize);
			double truePositiveRateRounded = floor(truePositiveRate * 1000.0) / 1000.0; 
			int N = *testCsvLen - totalReportedTimeSteps;
			double falsePositiveRate = (1.0*falsePositiveCounter) / (1.0*N);
			double falsePositiveRateRounded = floor(falsePositiveRate * 1000.0) / 1000.0; 

			printf("True Positive Rate: %.3g\n", truePositiveRateRounded);
			printf("False Positive Rate: %.3g\n", falsePositiveRateRounded);
		}

		/**
		 * @brief Uses anomaly report to parse for each anomaly start time step and end time step.
		 * 
		 * @return vector<Anomaly *>* 
		 */
		vector<Anomaly *> *parseAnomalies(int *counter) {
			vector<Anomaly *> *parsedAnomalies  = new vector<Anomaly *>();
			vector<AnomalyReport> *rep = *(this->report);
			int reportNum = rep->size();
			for (int i = 0; i < reportNum; i++) {
				AnomalyReport aRep = rep->at(i);
				Anomaly *anomaly = new Anomaly();
				anomaly->startTimeStep = aRep.timeStep;
				anomaly->endTimeStep = aRep.timeStep;
				for (int j = i + 1; j < reportNum; j++) {
					if (j == reportNum - 1) {
						anomaly->endTimeStep += 1;
						i = j;
						break;
					}
					if (rep->at(j).description != aRep.description) {
						i = j - 1;
						break;
					}
					anomaly->endTimeStep += 1;
				}
				anomaly->totalTimeSteps = anomaly->endTimeStep - anomaly->startTimeStep + 1;
				*counter += anomaly->totalTimeSteps;
				parsedAnomalies->push_back(anomaly);
			}
			return parsedAnomalies;
		}
};

#endif /* COMMANDS_H_ */
