#pragma once
#include <iostream>

#include <memory>
#include <thread>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>

struct Datagram {
	void* data_ptr;
	int32_t data_size;
	sockaddr addr;
	int addr_size;
};

class UdpSocket {
	// buffer variables
	int buffer_size = 1400;
	std::unique_ptr<char> buffer;

	// socket variables
	SOCKET socket = NULL;
	SOCKADDR_IN addr;
	SOCKADDR_IN other_addr;
	int port;
	std::string ip;


	// object state variables
	bool ready = false;
	bool connected = false;
	bool listening = false;

public:
	UdpSocket();
	virtual ~UdpSocket();

	// start listening method
	void startListening(std::string interface_ip, int port);

	// make connection method
	void makeConnection(std::string server_ip, int port);

	Datagram receiveNextDatagram();
	int sendDatagram(Datagram& datagram);
	int sendDatagram(Datagram& datagram, std::string destination);
	int sendDatagram(void* data_ptr, int32_t data_size, std::string destination = "");

protected:
	// prepare SOCKADDR_IN values
	virtual SOCKADDR_IN createSocketAddr(std::string ip, int port);
	virtual SOCKADDR_IN createSocketAddr( int port );

	// create socket, and save descriptor to socket variable
	virtual void createSocket();

	// bind socket with preparedSocketAddr
	virtual void bindSocket();
};