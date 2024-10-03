#ifndef SOCKETCLASSCLIENT_H
#define SOCKETCLASSCLIENT_H

// For program start via IPC
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 8080

class CSocketClient
{
    int m_SockFd, m_Portno;
    char a_Buffer[256];
    struct sockaddr_in m_ServAddr;
    int m_N;
    struct hostent *m_ServerName;

public:
    CSocketClient()=default;
    bool initSocket();

    ~CSocketClient();
};


#endif // SOCKETCLASSCLIENT_H
