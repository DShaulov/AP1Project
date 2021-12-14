

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
/**
 * Reads input from CLI
 * Writes output to CLI
 */

//////////////// NOT SURE IF NEEDED //////////////////
// class nonSocketIO:public DefaultIO {
// 	public:
// 		string read() {
// 			string input;
// 			getline(cin, input);
// 			return input;
// 		}
// 		void write(string text) {
// 			cout << text << endl;
// 		}
// 		void write(float f) {
// 			cout << f << endl;
// 		}
// 		void read(float* f) {

// 		}
// };

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
	public:
		uploadTS(DefaultIO *dio)
		: Command(dio)
		{
			
		}

		void execute() {

		}
};
/**
 * Classs corresponding to second menu option
 */
class algSettings:public Command {
	void execute() {
		
	}
};
/**
 * Classs corresponding to third menu option
 */
class detectAnomaly:public Command {
	void execute() {
		
	}
};
/**
 * Classs corresponding to fourth menu option
 */
class displayResults:public Command {
	void execute() {
		
	}
};
/**
 * Classs corresponding to fifth menu option
 */
class exit:public Command {
	void execute() {
		
	}
};


#endif /* COMMANDS_H_ */
