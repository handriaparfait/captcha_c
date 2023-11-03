#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main() {
    int clientSocket;
    char buffer[1024];
	char buffer1[1024];
    char testcaptcha[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Connect to the server
    addr_size = sizeof serverAddr;
    result = connect(clientSocket, (struct sockaddr*)&serverAddr, addr_size);
    if (result == SOCKET_ERROR) {
        perror("Connect failed");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    
    // Receive a message from the server
    result = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (result == SOCKET_ERROR) {
        perror("Receive failed");
    } else if (result == 0) {
        printf("Connection closed by the server.\n");
    } else {
        buffer[result] = '\0';  // Null-terminate the received data
        printf("%s\n", buffer);
    }

    // Input a response
    scanf("%s", testcaptcha);

    // Send the response to the server
    result = send(clientSocket, testcaptcha, strlen(testcaptcha), 0);
    if (result == SOCKET_ERROR) {
        perror("Send failed");
    }


	result = recv(clientSocket, buffer1, sizeof(buffer1), 0);
    if (result == SOCKET_ERROR) {
        perror("Receive failed");
    } else if (result == 0) {
        printf("Connection closed by the server.\n");
    } else {
        buffer1[result] = '\0';  // Null-terminate the received data
        printf("%s\n", buffer1);
    }
	

    // Close the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
