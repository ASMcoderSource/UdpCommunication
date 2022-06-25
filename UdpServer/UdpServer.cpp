#include "UdpServer.h"

UdpServer::UdpServer() {
	clients = new UdpClient*[clients_count];
	memset(clients, 0, sizeof(UdpClient*) * clients_count);
}

UdpServer::~UdpServer() {
	stopListening();
	for (int32_t i = 0; i < clients_count; i++) {
		if (clients[i] == nullptr)
			continue;
		delete clients[i];
	}
}

void UdpServer::startListening(std::string interface_ip, int port) {
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

void UdpServer::stopListening() {
	listener_socket.prepareToDelete();
	listening = false;
	while (can_be_deleted != true)
		std::this_thread::yield();
}

int32_t UdpServer::getAvailableId() {
	for (int32_t i = 0; i < clients_count; i++) {
		if (clients[i] == nullptr)
			return i;
	}
	return -1;
}


void UdpServer::receiveCycleMethod() {
	listening = true;
	while (listening) {

		// DATAGRAM PACKET HANDLER ______________________________________
		try {
			Datagram datagram = listener_socket.receiveNextDatagram();
			BaseQuery* query;
			query = reinterpret_cast<BaseQuery*>(datagram.data_ptr);
			switch (query->query_code) {
			case QueryCodes::CreateSession:
				this->createNewSession(datagram);
				break;
			case QueryCodes::CloseSession:
				this->closeSession(datagram);
				break;
			};
		}
		// _______________________________________________________________

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

void UdpServer::createNewSession(Datagram& datagram) {
	QueryCreateSession* query = reinterpret_cast<QueryCreateSession*>(datagram.data_ptr);
	int32_t id = getAvailableId();
	if ( id == -1 ) {
		// server can't accept new connection
		// to many connections
		return;
	}
	UdpSocket client_socket;
	client_socket = listener_socket;
	clients[id] = new UdpClient(client_socket);
	this->newConnectionHandle();
}

void UdpServer::closeSession(Datagram& datagram) {
	QueryCloseSession* query = reinterpret_cast<QueryCloseSession*>(datagram.data_ptr);

}


//------------------------------------

void UdpServer::newConnectionHandle() {
	std::cout << "New connection!" << std::endl;
}

void UdpServer::newDatagramReceived(Datagram& datagram) {
	std::cout << "New datagram received!" << std::endl;
}