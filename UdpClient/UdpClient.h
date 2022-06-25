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

class UdpClient{
	UdpSocket* socket;
	
public:
	UdpClient(UdpSocket* socket) {
		
	}
};

