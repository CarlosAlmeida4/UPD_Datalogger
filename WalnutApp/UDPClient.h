#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <thread>

#include "DirtTwo.h"
#include "EASportsWRC.h"

#pragma comment(lib, "ws2_32.lib")

extern DirtTwo l_DirtTwo;
extern EASportsWRC l_EASportsWRC;

class UDPClient
{
private:
	std::string  SERVER_IP_s;
	int PORT_i;
	SOCKET serverSocket;
	std::thread m_NetworkThread;
public:
	bool isRunning_b = false;
private:

public:
	UDPClient(std::string serverIP_l, int port_l) : SERVER_IP_s(serverIP_l), PORT_i(port_l) {}
	~UDPClient();
	int startClient();
	void stopClient();

	void receiveData();
};

