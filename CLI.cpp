/**
 * 
 * Authors:
 * 317005403 David Shaulov,
 * 205544109 Yonatan Zilber
 * 
 * 
 */
#include "CLI.h"

enum COMMAND {
    UPLOAD_CSV = 1,
    ALG_SETTINGS = 2,
    DETECT_ANOMALIES = 3,
    DISPLAY_RESULTS = 4,
    UPLOAD_AND_ANALYZE = 5,

};

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->detector = new HybridAnomalyDetector();
    this->inputFile = new ifstream("./input.txt");
    this->report = new vector<AnomalyReport>();
    this->anomalyVector = new vector<Anomaly *>();
    this->TestCsvLen = -2; // one for title line and one for empty line at the bottom
    vector<Command *> commands = {
        new uploadTS(dio, inputFile, &TestCsvLen),
        new algSettings(dio, detector, inputFile),
        new detectAnomaly(dio, detector, &(this->report), this->anomalyVector),
        new displayResults(dio, &(this->report)),
        new uploadAndAnalyze(dio, detector, inputFile, &(this->report), &TestCsvLen, anomalyVector)
    };
    this->commands = commands;
}

void CLI::start(){
    while (true) {
        printMenu();
        string command;
        command = dio->read();
        int commandType = stoi(command);
        if (commandType < 1 || commandType > 5) {
            break;
        }
        commands[commandType - 1]->execute();
    }
}

void CLI::printMenu() {
    dio->write("Welcome to the Anomaly Detection Server.\n"); 
    dio->write("Please choose an option:\n");
    dio->write("1.upload a time series csv file\n");
    dio->write("2.algorithm settings\n");
    dio->write("3.detect anomalies\n");
    dio->write("4.display results\n");
    dio->write("5.upload anomalies and analyze results\n");
    dio->write("6.exit\n");
}

CLI::~CLI() {
}

