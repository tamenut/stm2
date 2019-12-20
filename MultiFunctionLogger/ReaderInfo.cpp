#include "ReaderInfo.h"
#include "TUdpSocket.h"
#include "TTcpSocket.h"

namespace MF_LOG
{
ReaderInfo::ReaderInfo(ServerInfo &_server_info)
{
	server_info = _server_info;
	send_log_flag = false;
	struct in_addr server_addr;
	server_addr.s_addr = server_info.ip_addr;

	if(server_info.socket_type == UDP_SOCKET_TYPE)
	{
		snd_sock = new TUdpSocket();
		snd_sock->sender_init(server_addr.s_addr, server_info.port);
	}
	else
	{
		snd_sock = new TTcpSocket();
		snd_sock->sender_init(server_addr.s_addr, server_info.port);
	}
	
}

ReaderInfo::~ReaderInfo(void)
{
	delete snd_sock;
}

void ReaderInfo::set_send_log_flag(bool flag)
{
	send_log_flag = flag;
}

int ReaderInfo::write(char *msg, unsigned int msg_len)
{
	if(send_log_flag == true)
		return snd_sock->post(msg, msg_len);
	else
		return 0;
}

ServerInfo &ReaderInfo::get_server_info()
{
	return server_info;
}
}