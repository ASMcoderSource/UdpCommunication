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


enum QueryCodes {
	ConnectionAlive,
	CreateSession,
	CloseSession,
};

#pragma pack(push, 1)
struct BaseQuery {
	int64_t query_code;
	int64_t datagram_id;
};

struct QueryCreateSession : BaseQuery {
	char magik_value[14] = "CreateSession";
	QueryCreateSession() {
		query_code = QueryCodes::CreateSession;
		datagram_id = 0;
	}
};

struct QueryCloseSession : BaseQuery {
	QueryCloseSession() {
		query_code = QueryCodes::CloseSession;
		datagram_id = -1;
	}
};
#pragma pack(pop)

class UdpServer{
	UdpSocket listener_socket;
	std::thread* receive_thread;

	bool listening = false;
	bool can_be_deleted = true;

public:
	UdpServer() {
	
	}

	~UdpServer() {
		stopListening();
	}

	void startListening(std::string interface_ip, int port) {
		if (listening)
			throw std::exception("UdpServer error: Is already listening");
		try {
			listener_socket.startListening(interface_ip, port);
			can_be_deleted = false;
			receive_thread = new std::thread([&, this]() {
				receiveCycleMethod();
				});
			receive_thread->detach();
		}
		catch (std::exception exception) {
			std::cout << exception.what() << std::endl;
			std::cout << "WSA last error = " << WSAGetLastError() << std::endl;
			_getch();
			exit(1);
		}
	}

	void stopListening() {
		listener_socket.prepareToDelete();
		listening = false;
		while (can_be_deleted != true)
			std::this_thread::yield();
	}

protected:
	virtual void receiveCycleMethod() {
		listening = true;
		while (listening) {
			try {
				Datagram datagram = listener_socket.receiveNextDatagram();
				this->newDatagramReceived(datagram);
			}
			catch (std::exception exception) {
				std::cout << exception.what() << std::endl;
				std::cout << "WSA last error = " << WSAGetLastError() << std::endl;
				_getch();
				exit(1);
			}
			catch (int code) {
				if (code == SOCKET_TERMINATED)
					break;
			}
		}
		can_be_deleted = true;
		terminate();
	}

	virtual void newDatagramReceived( Datagram& datagram ) {
		std::cout << "New datagram received!" << std::endl;
	}
};

