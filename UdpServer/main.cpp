#include <iostream>
#include "UdpServer.h"
#include "UdpSocket.h"




int main(){
	UdpServer server;
	server.startListening("0.0.0.0", 7777);
	while (true);
}
