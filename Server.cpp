
#include "Server.h"
/**
 * @brief Handles SIGALRM signal emitted by alarm timeout
 * 
 */
void handleTimeOut(int signalCode) {
	printf("Accept timed out\n");
}

/**
 * @brief Construct a new Server:: Server object
 * Creates a socket for the server
 * 
 * @param port 
 */
Server::Server(int port)throw (const char*) {
	// Create a TCP socket over IP protocol
	this->socketFD = socket(AF_INET, SOCK_STREAM, 0);
	this->serverInfo.sin_family = AF_INET;
	// Convert to Big Endian
	this->serverInfo.sin_port = htons(port);
	// Bind server to any available IP
	this->serverInfo.sin_addr.s_addr = INADDR_ANY;
}

/**
 * @brief Accepts clients and handles them until told to stop
 * 
 * @param ch 
 */
void Server::start(ClientHandler& ch)throw(const char*){
	// Bind the socket	
	int bindCode = bind(this->socketFD, (struct sockaddr *)&(this->serverInfo), this->serverInfoSize);
	if (bindCode < 0) {
		throw "bind Exception\n";
	}
	// Start listening for connections
	int listenCode = listen(this->socketFD, 4);
	if (listenCode < 0) {
		throw "listen Exception\n";
	}
	// Set socket timeout time
	// this->timeout.tv_sec = this->socketTimeoutDuration;
	// this->timeout.tv_usec = 0;
	// int socketOptionSetCode = setsockopt(this->socketFD, SOL_SOCKET, SO_RCVTIMEO, &(this->timeout), sizeof(this->timeout));
	// if (socketOptionSetCode < 0) {
	// 	throw "setsockopt Exception";
	// }
	auto clientCallable = [&ch](int clientID) -> void {
		ch.handle(clientID);
		close(clientID);
	};
	// Create a callable object for the thread
	auto serverCallable = [this, clientCallable]() -> void {
		while(true) {
			if (this->isStopped) {
				break;
			}
			int clientID = accept(this->socketFD,(struct sockaddr*)&(this->clientInfo), &(this->clientInfoSize));
			if (clientID > 0) {
				thread *clientThread = new thread(clientCallable, clientID);
			}
		}
		close(this->socketFD);
	};
	this->t = new thread(serverCallable);
	
}

void Server::stop(){
	this->isStopped = true;
	t->join(); // do not delete this!
}

Server::~Server() {
}

