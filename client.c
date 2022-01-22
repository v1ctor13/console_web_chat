// by v1ctor13

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 6565
#define DEFAULT_BUFLEN 1024

int main(){

    SOCKET clientSocket = INVALID_SOCKET;

    int sFamily = AF_INET;          // "ADRESS FAMILY INTERNET" IPv4
    int sType = SOCK_STREAM;        // Two-way connection-based socket (uses TCP)
    int sProtocol = IPPROTO_TCP;    // TCP protocol

    // Used to store functions results
    int result;

    WSADATA wsa;
    if( (result = WSAStartup(0b0000001000000010, &wsa) ) != 0){ // Max and Min version in Binary
        printf("[ERROR] Unable to initialize WSA structure. Error core: %d\n", result);
        return -1;
    }


    // ↓ The client will try to connect to a port
    // the connection will only happen if there's a socket bound 
    // and listening on that port
    if( (clientSocket = socket(sFamily, sType, sProtocol) ) == INVALID_SOCKET ){
        printf("[ERROR] Unable to create listen socket. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    SOCKADDR_IN clientService;
    clientService.sin_family = sFamily;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(DEFAULT_PORT);

    printf("[*] Trying to connect to server...\n");

    if( connect(clientSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR ){
        printf("[ERROR] Unable to connect socket. Error code: %d", WSAGetLastError());
        if(closesocket(clientSocket) == SOCKET_ERROR){
            printf("[Error] Unable to close socket. Error code: %d", WSAGetLastError());
            return -1;
        }
        WSACleanup();
        return -1;
    }

    printf("[*] Connected to server.\n");

    char* sendBuffer = (char*)malloc(DEFAULT_BUFLEN * sizeof(char));
    printf("\n> Send: ");
    gets(sendBuffer);

    if( send (clientSocket, sendBuffer, (int)strlen(sendBuffer), 0) == SOCKET_ERROR){
        printf("[Error] Unable to send data. Error code: %d", WSAGetLastError());
        if( closesocket(clientSocket) != 0){
            printf("[Error] Unable to close socket. Error code: %d", WSAGetLastError());
            WSACleanup();
            return -1;
        }
        WSACleanup();
        return -1;
    }

    // printf("[*] Data sent.\n");

    if( closesocket(clientSocket) != 0){
        printf("[Error] Unable to close socket. Error code: %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    WSACleanup();
    printf("[!] Exiting...\n");

    return 0;
}