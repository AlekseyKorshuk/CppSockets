/*

Class declaration for threaded socket server

Adapted from http://web.eecs.utk.edu/~huangj/cs360/360/notes/Sockets/socketfun.c

Copyright Simon D. Levy 2018

MIT License
*/

#pragma once

class ThreadedSocketServer {

private:

	void * _sockinfo;

public:

	static const int BUFLEN = 512; // abitrary

	ThreadedSocketServer(int port);

	bool start(void);

	void stop(void);

	bool connected(void);

	int sendBuffer(char * buf, int len);

	int receiveBuffer(char * buf, int len);
};

