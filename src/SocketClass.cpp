#include "SocketClass.h"
#include <iostream>

namespace landscape {

bool CSocket::initSocket(){
	m_SockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_SockFd < 0){
		std::cout << "ERROR opening socket" << std::endl;
		return false;
	}

	bzero((char *) &m_ServAddr, sizeof(m_ServAddr));
	m_Portno = PORT;
	m_ServAddr.sin_family = AF_INET;
	m_ServAddr.sin_addr.s_addr = INADDR_ANY;
	m_ServAddr.sin_port = htons(m_Portno);

	if (bind(m_SockFd, (struct sockaddr *) &m_ServAddr, sizeof(m_ServAddr)) < 0){
		std::cout << "ERROR on binding" << std::endl;
		return false;
	}

	return true;
}

bool CSocket::listenSocket() {
	bool bRet = true;

	listen(m_SockFd,5);
	m_CliLen = sizeof(m_CliAddr);
	m_NewSockFd = accept(m_SockFd,
			   (struct sockaddr *) &m_CliAddr,
			   &m_CliLen);
	if (m_NewSockFd < 0)
	{
		std::cout << "ERROR on accept" << std::endl;
		bRet = false;
	}
	return bRet;
}

bool CSocket::ReadMsg(void) {
	bool bRet = true;

	int iNum = read(m_NewSockFd, &m_aRxBuff[m_uRxMsgIdx], RX_BUFF_SIZE - m_uRxMsgIdx);
	if (iNum > 0 && iNum < RX_BUFF_SIZE) {
		m_uRxMsgIdx += iNum;
		std::cout << "Socket read " << iNum << " bytes." << std::endl;
		std::cout << "Content:" << m_aRxBuff << std::endl;
	} else {
		std::cout << "Error reading data from socket or receive buffer is full!" << std::endl;
		bRet = false;
	}

	return bRet;
}

bool CSocket::setTxData(const uint8_t *pTxBuff, uint8_t uTxMsgLen) {
    bool bRet = true;
    if (TX_BUFF_SIZE < m_uTxMsgIdx + uTxMsgLen) {
        bRet = false;
    } else {
        memcpy(m_aTxBuff + m_uTxMsgIdx, pTxBuff, uTxMsgLen);
        m_uTxMsgIdx += uTxMsgLen;
    }

    return bRet;
}

bool CSocket::setTxDataInt(const int iData) {
    bool bRet = true;
    const uint8_t *pData = reinterpret_cast<const uint8_t*>(&iData);
    if (TX_BUFF_SIZE < m_uTxMsgIdx + sizeof(int)) {
        bRet = false;
    } else {
        memcpy(m_aTxBuff + m_uTxMsgIdx, pData, sizeof(int));
        m_uTxMsgIdx += sizeof(int);
    }

    return bRet;
}

bool CSocket::SendMsg(void) {
    bool bRet = true;

    int n = write(m_NewSockFd, m_aTxBuff, m_uTxMsgIdx);
    if (n < m_uTxMsgIdx) {
        std::cout << "ERROR sending data to Bug app!" << std::endl;
        bRet = false;
    }

    return bRet;
}

const uint8_t CSocket::GetBuff( uint8_t*& pBuff ) {
	pBuff = m_aRxBuff;

	return m_uRxMsgIdx;
}

CSocket::~CSocket()
{
	std::cout << "Closing opened sockets." << std::endl;
	close(m_NewSockFd);
	close(m_SockFd);
}

}
