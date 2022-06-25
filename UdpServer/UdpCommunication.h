enum class UdpClientState;

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


enum QueryCodes {
	ConnectionAlive,
	CreateSession,
	SessionAccepted,
	CloseSession,
};

#pragma pack(push, 1)
struct BaseQuery {
	int64_t query_code;
	int64_t datagram_id;
};

struct QueryCreateSession : BaseQuery {
	const char magik_value[14] = "CreateSession";
	QueryCreateSession();
};

struct QueryCloseSession : BaseQuery {
	QueryCloseSession();
};

struct QuerySessionAccepted : BaseQuery {
	int64_t session_id;
	QuerySessionAccepted(int64_t packet_id, int64_t session_id);
};
#pragma pack(pop)

enum class UdpClientState {
	Invalid,
	RequestSession,
	WaitForQueryReplay,
	SessionActive,
	CloseSession
};