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


CSocketClient::~CSocketClient()
{
	close(m_SockFd);
}
