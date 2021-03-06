// TestBlend.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "./Sockets/DatagramSocket.h"
#include "./Sockets/IPAddress.h"
#include <iostream>

#include "FileTransport.h"
using namespace NetCom;
int main()
{
	FileTransport ft;
	SocketAddress m_receiveAddrss(IPAddress(), 518);
	DatagramSocket m_receiveSocket(m_receiveAddrss);
	char buffer[1024 * 2];
	while (1)
	{
		memset(buffer, 0, sizeof(buffer));
		int n = m_receiveSocket.receiveBytes(buffer, sizeof(buffer) - 1);
		//std::cout << "接收数据=" << std::string(buffer) << std::endl;
		ft.process(buffer);
	}

    return 0;
}

