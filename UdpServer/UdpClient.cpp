#include "UdpClient.h"

UdpClient::UdpClient(UdpSocket& socket) {
	UdpClient::socket = socket;
	state = UdpClientState::RequestSession;

	char msg[] = "Server say \"HELLLO BITCH!\"";
	socket.sendDatagram(msg, sizeof(msg));
}

UdpClient::~UdpClient() {
}