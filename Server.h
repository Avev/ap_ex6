
#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        auto* sock = new SocketIO(clientID);
        CLI* cli = new CLI(sock);
        cli->start();
        delete cli;
        delete sock;
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	bool serverUp;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
