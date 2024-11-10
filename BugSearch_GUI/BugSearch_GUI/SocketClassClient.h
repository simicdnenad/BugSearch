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
#include <cstdint>
#define PORT 8080
#define TX_BUFF_SIZE 256

class CSocketClient
{
    int m_SockFd, m_Portno;
    struct sockaddr_in m_ServAddr;
    struct hostent *m_ServerName;
    uint8_t m_aTxBuff[TX_BUFF_SIZE] = {0}; //!< tx buffer
    uint8_t m_uTxMsgIdx = 0;               //!< current message index

public:
    CSocketClient()=default;
    bool initSocket();
    bool setTxData(const uint8_t *pTxBuff, uint8_t uTxMsgLen);
    bool SendMsg(void);
    ~CSocketClient();
};


#endif // SOCKETCLASSCLIENT_H
