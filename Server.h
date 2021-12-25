
/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include "CLI.h"
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO: public DefaultIO{
	int clientSocket;
	public:
		SocketIO(int clientSocket) {
			this->clientSocket = clientSocket;
		}
		/**
		 * Reads from socket until getting to \n
		 */
		virtual string read() {
			string clientString = "";
			char c;
			while (true) {
				recv(clientSocket, &c, sizeof(char), 0);
				if (c == '\n') {
					break;
				}
				clientString += c;
			}
			return clientString;
		}
		virtual void write(string text) {
			const char *cString = text.data();
			int sizeOfText = strlen(cString);
			int sendCode = send(clientSocket, cString, sizeOfText, 0);
			if (sendCode < 0) {
				cout << "Send Exception\n";
			}
		}
		virtual void write(float f) {
			// Possible bug, conversion not always accurate
			string floatToString = to_string(f);
			// Format the float string
			int stringSize = floatToString.size();
			if (stringSize > 5) {
				floatToString = floatToString.substr(0, 5);
			}
			// Strip trailing zeroes or '.' in case of whole number
			for (int i = 4; 0 < i ; i--) {
				if (floatToString[i] == '0' || floatToString[i] == '.') {
					floatToString.pop_back();
				}
				else {
					break;
				}
			}
			write(floatToString);

		}
		virtual void read(float* f) {
			recv(clientSocket, f, sizeof(float), 0);
		}
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		SocketIO *io = new SocketIO(clientID);
		CLI *clientCLI = new CLI(io);
		clientCLI->start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	ClientHandler *clientHandler;
	int port;
	int socketFD;
	sockaddr_in serverInfo;
	int serverInfoSize = sizeof(serverInfo);
	sockaddr_in clientInfo;
	socklen_t clientInfoSize = sizeof(this->clientInfo);
	bool isStopped = 0;
	int socketTimeoutDuration = 2;
	struct timeval timeout;
	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
