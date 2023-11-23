#include "UDPClient.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "DirtTwo.h"
#include "EASportsWRC.h"

DirtTwo l_DirtTwo;
EASportsWRC l_EASportsWRC;


int UDPClient::startClient()
{
    if (!isRunning_b)
    {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return 1;
        }

        // Create a UDP socket
        serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket" << std::endl;
            WSACleanup();
            return 1;
        }

        // Set up the server address
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(PORT_i);

        // Bind the socket to the specified port
        if (bind(serverSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Error binding socket" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "UDP server listening on port " << PORT_i << std::endl;
        isRunning_b = true;

        m_NetworkThread = std::thread([this]() { receiveData(); });

    }

}

void UDPClient::stopClient()
{
    closesocket(serverSocket);
    WSACleanup();
    isRunning_b = false;
    if (m_NetworkThread.joinable())
        m_NetworkThread.join();
}

void UDPClient::receiveData()
{
    while (isRunning_b)
    {
        // Wait for incoming messages
        char buffer[265];
        uint8_t uintBuffer[265];
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        int bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
        //int bytesRead = recvfrom(serverSocket, uintBuffer, sizeof(uintBuffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);

        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Error receiving message" << std::endl;
        }
        else {
            //buffer[bytesRead] = '\0';
            //std::cout << "Size of buffer" << bytesRead << std::endl;
            if (bytesRead <= 264) //Full size packet for Dirt Rally 2
            {
                //check if firts activation
                if (!l_EASportsWRC.GetOnStage()) l_EASportsWRC.StartStage();
                memcpy(l_EASportsWRC.UDPReceiveArray.data(), buffer, sizeof(buffer));
                //std::cout << "Received message from client: " << l_DirtTwo.UDPReceiveArray.data() << std::endl;
                l_EASportsWRC.HandleArray();
            }
            else
            {
                std::cout << "Received package with a different size " << sizeof(buffer) << std::endl;
            }
        }
    }
   
}

UDPClient::~UDPClient()
{
    closesocket(serverSocket);
    WSACleanup();
    isRunning_b = false;
    if (m_NetworkThread.joinable())
        m_NetworkThread.join();
}