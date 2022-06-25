#include "UdpCommunication.h"

QueryCreateSession::QueryCreateSession() {
	query_code = QueryCodes::CreateSession;
	datagram_id = 0;
}

QueryCloseSession::QueryCloseSession() {
	query_code = QueryCodes::CloseSession;
	datagram_id = -1;
}

QuerySessionAccepted::QuerySessionAccepted(int64_t packet_id, int64_t session_id) {
	query_code = QueryCodes::CloseSession;
	datagram_id = packet_id;
	QuerySessionAccepted::session_id = session_id;
}