class UdpClient;

#pragma once
#include <iostream>
#include <conio.h>
#include <memory>
#include <thread>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include "UdpSocket.h"
#include "UdpServer.h"
#include "UdpCommunication.h"


class UdpClient{
	UdpClientState state;
	UdpSocket socket;
	sockaddr_in receiver_addr{0};

public:
	UdpClient(UdpSocket& socket);
	~UdpClient();
};

