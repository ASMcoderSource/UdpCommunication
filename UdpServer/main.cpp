#include <iostream>
#include "UdpSocket.h"




int main(){
	char msg[] = "Hello, world";
	try {
		UdpSocket socket;
		socket.makeConnection("127.0.0.1", 7777);
		do {
			socket.sendDatagram(msg, sizeof(msg), "127.0.0.1");
		} while (true);
	}
	catch (std::exception exc) {
		std::cout << exc.what() << std::endl;
	}
}
