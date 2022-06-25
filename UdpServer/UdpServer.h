#pragma once
#include <iostream>
#include <vector>
#include <conio.h>
#include <memory>
#include <thread>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include "UdpClient.h"
#include "UdpSocket.h"
#include "UdpCommunication.h"

class UdpServer{
	UdpSocket listener_socket;
	std::thread* receive_thread;
	UdpClient** clients;
	int32_t clients_count = 1024;
	bool listening = false;
	bool can_be_deleted = true;

public:
	UdpServer();
	~UdpServer();

	void startListening(std::string interface_ip, int port);
	void stopListening();

protected:
	virtual void receiveCycleMethod();
	virtual void createNewSession(Datagram& datagram);
	virtual void closeSession(Datagram& datagram);
	virtual void newDatagramReceived(Datagram& datagram);

	virtual void newConnectionHandle();

private:
	int32_t getAvailableId();
};

