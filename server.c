// by v1ctor13

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 6565
#define DEFAULT_BUFLEN 1024

int main(){
    
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET acceptanceSocket = INVALID_SOCKET;

    int sFamily     = AF_INET;      // "ADRESS FAMILY INTERNET" IPv4
    int sType       = SOCK_STREAM;  // Two-way connection-based socket (uses TCP)
    int sProtocol   = IPPROTO_TCP;  // TCP protocol

    // Used to store functions results
    int result;

    WSADATA wsa;
    if( (result = WSAStartup(0b0000001000000010, &wsa) ) != 0){ // Max and Min version in two bytes in binary
        printf("[ERROR] Unable to initialize WSA structure. Error core: %d\n", result);
        return -1;
    }

    // ↓ DEALING WITH CLIENT CONNECTION ↓
    // listenSocket is used only by binding it to a port 
    // and after this, making it listen (wait for connections)

    if( (listenSocket = socket(sFamily, sType, sProtocol) ) == INVALID_SOCKET){
        printf("[ERROR] Unable to create listen socket. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    SOCKADDR_IN serverService;
    serverService.sin_family = sFamily;
    serverService.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverService.sin_port = htons(DEFAULT_PORT);

    if( bind (listenSocket, (SOCKADDR*) &serverService, sizeof(serverService) ) == SOCKET_ERROR){
        printf("[ERROR] Unable to bind listenSocket. Error code: %d\n", WSAGetLastError());
        if( closesocket(listenSocket) == SOCKET_ERROR){
            printf("[ERROR] Unable to close socket listenSocket. Error code: %d\n", WSAGetLastError());
            return -1;
        }
        WSACleanup();
        return -1;
    }

    if( listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        printf("[ERROR] Unable to listen. Error code: %d\n", WSAGetLastError());
        if(closesocket(listenSocket) == SOCKET_ERROR){
            printf("[ERROR] Unable to close socket listenSocket. Error code: %d\n", WSAGetLastError());
            return -1;
        }
        WSACleanup();
        return -1;
    }

    printf("$ Waiting for connections...\n");

    if( (acceptanceSocket = accept(listenSocket, NULL, NULL)) == INVALID_SOCKET){
        printf("[ERROR] Unable to accept connection. Error code: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("$ Connected to client.\n");

    char recvBuffer[DEFAULT_BUFLEN] = "";
    result = 0;
    do{
        memset(recvBuffer, 0, sizeof(recvBuffer));
        result = recv(acceptanceSocket, recvBuffer, DEFAULT_BUFLEN * sizeof(char), 0);
        if(result > 0){
            printf("\n$ Data received: ");
            puts(recvBuffer);
        }
        if(result == 0){
            printf("[!] Client closed connection.\n");
        }
        // printf("resultado: %d\n", result);
        // printf("Erro: %s\n", WSAGetLastError());
    }while(result != 0);



    printf("[!] Exiting...\n");



    return 0;
}