#include "SocketClassClient.h"
#include <iostream>

bool CSocketClient::initSocket(){
    m_SockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_SockFd < 0){
        std::cout << "ERROR opening socket" << std::endl;
        return false;
    }

    m_ServerName = gethostbyname("nenad-Lenovo");
    if (NULL == m_ServerName) {
        std::cout << "ERROR, no such host" << std::endl;
        return false;
    }

    bzero((char *) &m_ServAddr, sizeof(m_ServAddr));
    m_ServAddr.sin_family = AF_INET;
    bcopy((char *)m_ServerName->h_addr,
         (char *)&m_ServAddr.sin_addr.s_addr,
         m_ServerName->h_length);
    m_ServAddr.sin_port = htons(PORT);

    if (connect(m_SockFd,(struct sockaddr *) &m_ServAddr,sizeof(m_ServAddr)) < 0) {
        std::cout << "ERROR connecting" << std::endl;
        return false;
    }
    return true;
}

bool CSocketClient::SendMsg(void) {
    bool bRet = true;
    int n = write(m_SockFd, m_aTxBuff, m_uTxMsgIdx);
    if (n < m_uTxMsgIdx) {
        std::cout << "ERROR sending data to Bug app!" << std::endl;
        bRet = false;
    }

    return bRet;
}

bool CSocketClient::setTxData(const uint8_t *pTxBuff, uint8_t uTxMsgLen) {
    bool bRet = true;
    if (TX_BUFF_SIZE < m_uTxMsgIdx + uTxMsgLen) {
        bRet = false;
    } else {
        memcpy(m_aTxBuff + m_uTxMsgIdx, pTxBuff, uTxMsgLen);
        m_uTxMsgIdx += uTxMsgLen;
    }

    return bRet;
}


CSocketClient::~CSocketClient()
{
    close(m_SockFd);
}
