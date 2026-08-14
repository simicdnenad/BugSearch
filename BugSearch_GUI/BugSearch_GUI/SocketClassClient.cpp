#include "SocketClassClient.h"
#include <iostream>

bool CSocketClient::initSocket(){
    m_SockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_SockFd < 0){
        std::cout << "ERROR opening socket" << std::endl;
        return false;
    }

    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname failed");
        return false;
    }

    m_ServerName = gethostbyname(hostname);
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

    // set time-out interval
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(m_SockFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

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

bool CSocketClient::ReceiveMsg(void) {
    bool bRet = true;
    int i = read(m_SockFd, &m_aRxBuff[m_uRxMsgIdx], RX_BUFF_SIZE - m_uRxMsgIdx);

    if (i > 0) {
        m_uRxMsgIdx += i;
    } else {
        std::cout << "ERROR receiving data from Bug app!" << std::endl;
        bRet = false;
    }

    return bRet;
}

unsigned CSocketClient::getNumberOfBugs() {
    unsigned uNOfBugs = 0;

    if (m_uRxMsgIdx >= sizeof(unsigned)) {
        uNOfBugs = *((unsigned*)&m_aRxBuff[m_uRxMsgIdx - sizeof(unsigned)]);
        m_uRxMsgIdx = m_uRxMsgIdx - sizeof(unsigned);
    } else {
        std::cout << "Number of Bug patterns found not received!" << std::endl;
    }

    return uNOfBugs;
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
    std::cout << "Closing opened socket." << std::endl;
    close(m_SockFd);
}
