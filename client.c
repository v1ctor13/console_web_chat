// by v1ctor13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 6565
#define DEFAULT_BUFLEN 1024

#define BACKSLASH 47

#define EQUAL_STRINGS 0

typedef enum e_cmd {
    SAIR
}COMMAND;

void quitChat(SOCKET clientSocket){
    if( closesocket(clientSocket) != 0){
        printf("[Error] Unable to close socket. Error code: %d", WSAGetLastError());
        WSACleanup();
        exit(0);
    }
    exit(0);
}

void commands(const char* sendBuffer, SOCKET clientSocket){
    if(strcmp(sendBuffer, "/quit") == EQUAL_STRINGS){
        printf("[!] Exiting...\n");
        quitChat(clientSocket);
    }
}

int main(){

    SOCKET clientSocket = INVALID_SOCKET;

    int sFamily     = AF_INET;      // "ADRESS FAMILY INTERNET" IPv4
    int sType       = SOCK_STREAM;  // Two-way connection-based socket (uses TCP)
    int sProtocol   = IPPROTO_TCP;  // TCP protocol

    // Used to store functions results
    int result;

    WSADATA wsa;
    if( (result = WSAStartup(0b0000001000000010, &wsa) ) != 0){ // Max and Min version in Binary
        printf("\n[ERROR] Unable to initialize WSA structure. Error core: %d\n", result);
        return -1;
    }


    // ↓ The client will try to connect to a port
    // the connection will only happen if there's a socket bound 
    // and listening on that port
    if( (clientSocket = socket(sFamily, sType, sProtocol) ) == INVALID_SOCKET ){
        printf("\n[ERROR] Unable to create listen socket. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    SOCKADDR_IN clientService;
    clientService.sin_family = sFamily;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(DEFAULT_PORT);

    printf("\n$ Trying to connect to server...\n");

    if( connect(clientSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR ){
        printf("\n[ERROR] Unable to connect socket. Error code: %d", WSAGetLastError());
        if(closesocket(clientSocket) == SOCKET_ERROR){
            printf("\n[Error] Unable to close socket. Error code: %d", WSAGetLastError());
            return -1;
        }
        WSACleanup();
        return -1;
    }

    printf("\n$ Connected to server.\n");

    char username[30];
    printf("\n$ Enter username: ");
    gets(username);

    char* sendBuffer = (char*)malloc(DEFAULT_BUFLEN * sizeof(char));
    while(1){
        printf("\n$ Send: ");
        gets(sendBuffer);

        if(sendBuffer[0] == BACKSLASH){
            commands(sendBuffer, clientSocket);
        }

        if( send (clientSocket, sendBuffer, (int)strlen(sendBuffer), 0) == SOCKET_ERROR){
            printf("\n[Error] Unable to send data. Error code: %d", WSAGetLastError());
            if( closesocket(clientSocket) != 0){
                printf("\n[Error] Unable to close socket. Error code: %d", WSAGetLastError());
                WSACleanup();
                return -1;
            }
            WSACleanup();
            return -1;
        }
    }
    // printf("[*] Data sent.\n");

    quitChat(clientSocket);

    WSACleanup();
    printf("\n[!] Quitting...\n");

    return 0;
}