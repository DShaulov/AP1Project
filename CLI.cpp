#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

void CLI::start(){
    cout << "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n" << endl; 
    cout << "1. upload a time series csv file\n" << endl;
    cout << "2. algorithm settings\n" << endl;
    cout << "3. detect anomalies\n" << endl;
    cout << "4. display results\n" << endl;
    cout << "5. upload anomalies and analyze results\n" << endl;
    cout << "6. exit\n" << endl;
    // Get command number from user
    int commandType;
    cin >> commandType;
    printf("Hey this is your command: %d\n", commandType);
}


CLI::~CLI() {
}

