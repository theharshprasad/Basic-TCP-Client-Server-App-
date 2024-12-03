#include<iostream>
#include <WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>

using namespace std;

#pragma comment(lib,"ws2_32.lib") // link the Winsock library

// function to initialize WinSock
bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0; // initialize WinSock with version 2.2
}

int main() {
	int port = 12345; // define the port the server will listen on

	if (!Initialize()) { // initialize WinSock and check for errors
		cout << "Winsock Initialization Failed" << endl;
		return 1; // exit if initialization fails
	}

	// Create a socket
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); // use IPv4, TCP, and default protocol
	if (listenSocket == INVALID_SOCKET) { // check if socket creation failed
		cout << "Socket Creation Failed" << endl;
		return 1; // exit if socket creation fails
	}

	// create the address structure for the server
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET; // used IPv4
	serveraddr.sin_port = htons(port); // converted the port number to network byte order

	// convert and set the IP address to bind to (0.0.0.0 for all interfaces)
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << "Setting address structure failed" << endl;
		closesocket(listenSocket); // close the socket
		WSACleanup(); // cleanup WinSock
		return 1; // exit if address conversion fails
	}

	// bind the IP address and port to the socket
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "Bind Failed" << endl;
		closesocket(listenSocket); // close the socket
		WSACleanup(); // cleanup WinSock
		return 1; // exit if binding fails
	}

	// put the socket in listening mode
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) { // maximum connections allowed
		cout << "Listen Failed" << endl;
		closesocket(listenSocket); // close the socket
		WSACleanup(); // cleanup WinSock
		return 1; // exit if listening fails
	}

	cout << "Server has started listening on port: " << port << endl;

	// accept an incoming client connection
	SOCKET clientSocket = accept(listenSocket, nullptr, nullptr); // accept connection, no specific client info needed
	if (clientSocket == INVALID_SOCKET) { // check if client socket is invalid
		cout << "Invalid Client Socket" << endl;
		closesocket(listenSocket); // close the listening socket
		WSACleanup(); // cleanup WinSock
		return 1; // exit if accepting client connection fails
	}

	cout << "Client Connected" << endl;

	char buffer[4096]; // buffer to hold the received data

	// receive data from the client
	int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0); // receive data into buffer
	if (bytesrecvd > 0) { // check if data is received
		string message(buffer, bytesrecvd); // convert the received data into a string
		cout << "Message from client: " << message << endl; // print the client's message
	}

	// close the client socket
	closesocket(clientSocket);

	// close the listening socket
	closesocket(listenSocket);

	// cleanup WinSock
	WSACleanup();

	return 0; // exit the program
}
