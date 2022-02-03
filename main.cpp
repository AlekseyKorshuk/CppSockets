// Your First C++ Program

#include <iostream>
#include "TcpClientSocket.hpp"


int main() {

    TcpClientSocket client("wgforge-srv.wargaming.net", 443);

    client.openConnection();

    char message[100] = "Hello from client!";

    client.sendData(message, strlen(message));

    client.receiveData(message, 100);
    return 0;
}

