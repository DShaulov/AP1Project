

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
	private:
		enum uploadType {
			Train,
			Test
		};
	public:
		/**
		 * @brief Construct a new upload T S object
		 */
		uploadTS(DefaultIO *dio, ifstream *file) : Command(dio) {
			inputFile = file;
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
			while (true) {
				getline(*inputFile, line);
				if (line == "done") {
					break;
				}
				outputFile << line << "\n";
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
			TimeSeries *tsTrain = new TimeSeries("anomalyTest.csv");
			TimeSeries *tsTest = new TimeSeries("anomalyTrain.csv");
			this->detector->learnNormal(*tsTrain);
			vector<AnomalyReport> aReport = this->detector->detect(*tsTest);
			vector<AnomalyReport> *rep = new vector<AnomalyReport>(aReport);
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
	public:
		uploadAndAnalyze(DefaultIO *dio): Command(dio) {};
		void execute() {
			cout << "TODO\n";
		}
};
/**
 * Class corresponding to sixth menu option
 */
class exit:public Command {
	void execute() {
		
	}
};


#endif /* COMMANDS_H_ */
