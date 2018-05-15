/*

   Example client program for sockets.  

   Adapted from https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx
 
   Copyright Simon D. Levy 2018

   MIT License
*/

#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

static const int BUFLEN = 512;

static const char * HOST = "localhost";
static const char * PORT = "20000";

static void error(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char buf[200];
	vsnprintf(buf, 200, fmt, ap);
	puts(buf);
	va_end(ap);

	while (true);
}

static SOCKET connect(void)
{
	SOCKET ConnectSocket = INVALID_SOCKET;

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		error("WSAStartup failed with error: %d\n", iResult);
	}

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	struct addrinfo *result = NULL;
	iResult = getaddrinfo(HOST, PORT, &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		error("getaddrinfo failed with error: %d\n", iResult);
	}

	// Attempt to connect to an address until one succeeds
	for (struct addrinfo * ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			WSACleanup();
			error("socket failed with error: %ld\n", WSAGetLastError());
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	return ConnectSocket;
}


int __cdecl main(int argc, char **argv)
{

	SOCKET ConnectSocket = connect();

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		error("Unable to connect to server %s:%s\n", HOST, PORT);
	}

	// Send an initial buffer
	const char *sendbuf = "this is a test";
	int iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		WSACleanup();
		error("send failed with error: %d\n", WSAGetLastError());
	}

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		WSACleanup();
		error("shutdown failed with error: %d\n", WSAGetLastError());
	}

	// Receive until the peer closes the connection
	do {

		char recvbuf[BUFLEN];
		int recvbuflen = BUFLEN;

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

