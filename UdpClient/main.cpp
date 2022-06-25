#include <iostream>
#include "UdpServer.h"
#include "UdpSocket.h"


int main(){
	QueryCreateSession query_create_session;
	
	UdpSocket client;
	client.makeConnection("127.0.0.1", 7777);
	client.sendDatagram(reinterpret_cast<char*>(&query_create_session), sizeof(QueryCreateSession));
	auto datagram = client.receiveNextDatagram();
	std::cout << "Datagram received!\n";
	for (int i = 0; i < datagram.data_size; i++) {
		std::cout << ((char*)datagram.data_ptr)[i];
	}
	while (true);
}
