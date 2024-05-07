// hw1p1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData; // struct containing Winsock info
    WORD wVersionRequested = MAKEWORD(2, 2); // use version 2.2
    if (WSAStartup(wVersionRequested, &wsaData) != 0) { // initialize Winsock
        printf("WSAStartup error %d\n", WSAGetLastError());
        exit(-1);
    }

    // Task 1a
    // open socket
    // address family IPv4 (AF_INET)
    // sequenced, two-way connection-based byte stream (SOCK_STREAM)
    // protocol TCP (IPPROTO_TCP)
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("socket() error %d\n", WSAGetLastError());
        exit(-2);
    }

    // Task 2
    // struct for connecting to server
    struct sockaddr_in server;

    // determine IP addr of servers in URL
    char test_url[] = "irl.cs.tamu.edu";
    DWORD IP = inet_addr(test_url); // assume there is an IP address
    if (IP == INADDR_NONE)
    {
        // if no IP address, attempt through DNS
        struct hostent* remote = gethostbyname(test_url); // 4-byte IP addr
        if (remote == NULL) {
            // FQDN: fully-qualified domain names
            printf("Invalid string: neither FQDN, nor IP address\n");
            exit(-3);
        }
        else {
            memcpy((char*)&(server.sin_addr), remote->h_addr, remote->h_length);
        }

    }
    else {
        server.sin_addr.S_un.S_addr = IP;
    }

    // Task 3
    // connect socket to server on given port
    server.sin_family = AF_INET;
    server.sin_port = htons(80);		// host-to-network flips the byte order

    // connect to the server on port 80
    if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        printf("Connection error: %d\n", WSAGetLastError());
        exit(-4);
    }

    printf("Successfully connected to %s (%s) on port %d\n", test_url, inet_ntoa(server.sin_addr), htons(server.sin_port));

    // Task 4
    // send request
    const char* sendBuf = "GET /courses/ HTTP/1.0\r\nHost: irl.cs.tamu.edu\r\nConnection: close\r\n\r\n";
    if (send(sock, sendBuf, (int)strlen(sendBuf), 0) == SOCKET_ERROR)
    {
        printf("Send error: %d\n", WSAGetLastError());
        exit(-5);
    }

    // shutdown connection since no more data will be sent
    if (shutdown(sock, 1) == SOCKET_ERROR) {
        printf("Shutdown failed with error: %d\n", WSAGetLastError());
        exit(-6);
    }

    // Task 5
    // receive response
    char recvBuf[512];
    while (true) {
        int iRes = recv(sock, recvBuf, 512, 0);
        if (iRes > 0) {
            printf("%.*s", (int)iRes, recvBuf);
        }
        else if (iRes == 0) {
            printf("Connection closed\n");
            break;
        }
        else {
            printf("Recv failed with error: %d\n", WSAGetLastError());
            exit(-7);
        }
    }

    // Task 1.2
    // close socket
    closesocket(sock);

    WSACleanup();

    return 0;
}
