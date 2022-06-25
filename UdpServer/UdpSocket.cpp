#include "UdpSocket.h"


UdpSocket::UdpSocket() {
	// create buffer in heap
	try {
		buffer = std::make_unique<char>(buffer_size);
	}
	catch (...) {
		throw std::exception("Socket error: Memory allocation error");
	}

	// check Windows Sockets version
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
		throw std::exception("Socket error: Invalid version of Windows Sockets");

	// in the end, this socket object ready to use
	ready = true;
}

UdpSocket::~UdpSocket() {
	if (socket_owner == false)
		return;
	if (listening)
		throw std::exception("Socket error: Destruction active listening socket");
}

void UdpSocket::startListening(std::string interface_ip, int port) {
	// verify object possible to enter listening state
	if (!ready)
		throw std::exception("Socket error: Object isn't ready");
	if (listening)
		throw std::exception("Socket error: Multiple switching to listening mode");
	if (connected)
		throw std::exception("Socket error: Switching to listening mode in connected state");

	// save listening params
	UdpSocket::ip = interface_ip;
	UdpSocket::port = port;

	// prepare SOCKADDR_IN values
	addr = createSocketAddr(interface_ip, port);
	// create the socket, and bind interface address
	createSocket();
	bindSocket();

	listening = true;
}

void UdpSocket::makeConnection(std::string server_ip, int port) {
	// verify object possible to enter connection state
	if (!ready)
		throw std::exception("Socket error: Object isn't ready");
	if (listening)
		throw std::exception("Socket error: Switching to connection mode, while listening");
	if (connected)
		throw std::exception("Socket error: Switching to connection mode, while connected");

	// save listening params
	UdpSocket::ip = server_ip;
	UdpSocket::port = port;

	// prepare SOCKADDR_IN values
	other_addr = createSocketAddr(server_ip, port);
	addr = createSocketAddr(port);
	// create the socket, and bind interface address
	createSocket();
	bindSocket();

	connected = true;
}


SOCKADDR_IN UdpSocket::createSocketAddr(std::string ip, int port) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
	return addr;
}

SOCKADDR_IN UdpSocket::createSocketAddr( int port ) {
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	return addr;
}

void UdpSocket::createSocket() {
	socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket == SOCKET_ERROR)
		throw std::exception("Socket error: Socket creating error");
}

void UdpSocket::bindSocket() {
	if (bind(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != NULL)
		throw std::exception("Socket error: Binding is not succesful");
}

void UdpSocket::prepareToDelete() {
	ready = false;
	listening = false;
	connected = false;
	if( socket_owner )
		::closesocket(socket);
}

// RECEIVE DATAGRAM ------------------------------------------------------------------------------------

Datagram UdpSocket::receiveNextDatagram() {
	// verify object ready to receive datagrams
	if (ready == false)
		throw SOCKET_TERMINATED;
	if (!(listening || connected))
		throw std::exception("Socket error: Can't receive datagram without listening or connecting");

	// create and prepare receive datagrams
	Datagram receive_datagram{0};
	SOCKADDR addr;
	int addr_len = sizeof(sockaddr);
	int size = 0;

	// execute reading datagram, and check for errors
	size = recvfrom(socket, buffer.get(), buffer_size, 0, &addr, &addr_len);
	if (ready == false)
		throw SOCKET_TERMINATED;
	if (size == SOCKET_ERROR ) 
		throw std::exception("Socket error: Receiving error");

	receive_datagram.addr_size = addr_len;
	receive_datagram.addr = *reinterpret_cast<SOCKADDR_IN*>(&addr);
	receive_datagram.data_ptr = buffer.get();
	receive_datagram.data_size = size;

	if (listening)
		other_addr = receive_datagram.addr;
	return receive_datagram;
}

// SEND DATAGRAM ------------------------------------------------------------------------------------
int UdpSocket::sendDatagram(Datagram& datagram) {
	return sendDatagram(datagram.data_ptr, datagram.data_size, "");
}

int UdpSocket::sendDatagram(Datagram& datagram, std::string destination) {
	return sendDatagram(datagram.data_ptr, datagram.data_size, destination);
}

int UdpSocket::sendDatagram(void* data_ptr, int32_t data_size, std::string destination) {
	// verify object ready to send datagrams
	if (!(listening || connected))
		throw std::exception("Socket error: Sending isn't possible while socket isn't listen or connected");
	
	// Create receiver sockaddr
	SOCKADDR_IN receiver_addr;
	if (destination.length() != 0 ) 
		receiver_addr = createSocketAddr(destination, port);
	else 
		receiver_addr = other_addr;
	
	// send datagram, and check result value for error
	int len = sendto(socket, static_cast<char*>(data_ptr), data_size, 0, reinterpret_cast<sockaddr*>(&receiver_addr), sizeof(SOCKADDR));
	if (len == SOCKET_ERROR) {
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		throw std::exception("Socket error: Sending error");
	}

	return len;
}

UdpSocket& UdpSocket::operator = (UdpSocket& socket) {
	memcpy(this, &socket, sizeof(UdpSocket));
	socket_owner = false;
	buffer.release();

	// create buffer in heap
	try {
		buffer = std::make_unique<char>(buffer_size);
	}
	catch (...) {
		throw std::exception("Socket error: Memory allocation error");
	}
	return *this;
}