#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;

// link winsock library
#pragma comment(lib, "ws2_32")

// function to initialize winsock
bool Initialize() {
    WSADATA data;
    // start winsock with version 2.2
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main() {
    cout << "Client Program" << endl;

    // initialize winsock
    if (!Initialize()) {
        cout << "Winsock initialization failed" << endl;
        return 1; // exit if initialization fails
    }

    // create a socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        cout << "Failed to create socket" << endl;
        WSACleanup(); // cleanup winsock if socket creation fails
        return 1;
    }

    // specify port number
    int port = 12345;
    // specify server address
    string serveraddress = "127.0.0.1";

    // setup server address structure
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET; // address family: IPv4
    serveraddr.sin_port = htons(port); // convert port number to network byte order
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr)); // convert IP address to binary form

    // connect to the server
    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        cout << "Not able to connect to server" << endl;
        closesocket(s); // close the socket on failure
        WSACleanup(); // cleanup winsock on failure
        return 1;
    }

    cout << "Successfully connected" << endl;

    // prepare the message to send
    string message = "Hello There!!";
    // send the message to the server
    int bytesent = send(s, message.c_str(), message.length(), 0);

    // check if message was sent successfully
    if (bytesent == SOCKET_ERROR) {
        cout << "Send failed" << endl;
    }
    else {
        cout << "Message sent successfully" << endl;
    }

    // close the socket and cleanup winsock
    closesocket(s);
    WSACleanup();

    return 0;
}
