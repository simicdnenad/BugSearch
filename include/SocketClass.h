// For program start via IPC
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080
#define RX_BUFF_SIZE 256

namespace landscape {

class CSocket
{
    int m_SockFd, m_NewSockFd, m_Portno;
    socklen_t m_CliLen;
    struct sockaddr_in m_ServAddr, m_CliAddr;
    uint8_t m_aRxBuff[RX_BUFF_SIZE] = {0};	//!< rx buffer
    uint8_t m_uRxMsgIdx = 0;               	//!< current message index

public:
    CSocket()=default;
    bool initSocket();
    bool listenSocket();
    bool ReadMsg(void);

    ~CSocket();
};



}
