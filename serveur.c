#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
    WSADATA wsaData;
    SOCKET welcomeSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int result;

    // Initialize Winsock
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    // Create a socket
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (welcomeSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    result = bind(welcomeSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        perror("Bind failed");
        closesocket(welcomeSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(welcomeSocket, 5) == SOCKET_ERROR) {
        perror("Error in listening");
        closesocket(welcomeSocket);
        WSACleanup();
        return 1;
    }
    else {
        printf("Listening\n");
    }

    // Accept incoming connections
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr*)&serverStorage, &addr_size);
    if (newSocket == INVALID_SOCKET) {
        perror("Accept failed");
        closesocket(welcomeSocket);
        WSACleanup();
        return 1;
    }

    // Send a message
    const char* message = "1 + 1 equals? ";
    send(newSocket, message, strlen(message), 0);

    // Receive a response
    char buffer[1024];
    result = recv(newSocket, buffer, sizeof(buffer), 0);
    if (result == SOCKET_ERROR) {
        perror("Receive failed");
    }
    else if (result == 0) {
        printf("Connection closed by client.\n");
    }
    else {
        buffer[result] = '\0';  // Null-terminate the received data
        printf("Client response: %s\n", buffer);
		const char* responseMessage;
		if (strcmp(buffer, "2") == 0) {
			responseMessage = "Vous êtes connectés";
		} else {
			responseMessage = "Vous êtes un robot";
		}

		// Send the appropriate response message
		send(newSocket, responseMessage, strlen(responseMessage), 0);

    }

	
    // Clean up and close the sockets
    closesocket(newSocket);
    closesocket(welcomeSocket);
    WSACleanup();

    return 0;
}
