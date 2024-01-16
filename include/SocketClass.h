// For program start via IPC
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080

namespace landscape {

class CSocket
{
    int m_SockFd, m_NewSockFd, m_Portno;
    socklen_t m_CliLen;
    char a_Buffer[256];
    struct sockaddr_in m_ServAddr, m_CliAddr;
    int m_N;

public:
    CSocket()=default;
    bool initSocket();
    bool listenSocket();

    ~CSocket()=default;
};



}
