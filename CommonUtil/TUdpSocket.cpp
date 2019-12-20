#include "TUdpSocket.h"
#include "stdio.h"
#include "string.h"
#include "SerializedPayload.h"

#ifdef LINUX
#include "unistd.h"
#endif



TUdpSocket::TUdpSocket()
{
	_socket_type = NONE_SOCKET_TYPE;
	_rcv_sock = NULL;

	/*
	SOCKET snd_sock;
	if((snd_sock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		printf("Error : Can't create unicast snd socket\n");
	}
	_snd_sock_list.push_back(snd_sock);
	*/
}

/*
unicast UDP 송신 소켓을 위한 생성자
*/
/*
TUdpSocket::TUdpSocket(char *dst_ip_str, unsigned short bind_port)
{
	sender_init(inet_addr(dst_ip_str), bind_port);
}

//Unicast UDP 수신 소켓을 위한 생성자
TUdpSocket::TUdpSocket(unsigned short bind_port)
{
	receiver_init(bind_port);
}

//Multicast UDP 송신 소켓을 위한 생성자
TUdpSocket::TUdpSocket(char *dst_ip_str, unsigned short bind_port, vector<string> &src_ip_list)
{
	sender_init(inet_addr(dst_ip_str), bind_port, src_ip_list);
}
//Multicast UDP 수신 소켓을 위한 생성자
TUdpSocket::TUdpSocket(vector<string> ucastAddrList, const char *mcast_addr, unsigned short bind_port)
{
	receiver_init(inet_addr(mcast_addr), bind_port, ucastAddrList);
}
*/
/*
unicast UDP 송신 소켓을 위한 생성자
*/
void TUdpSocket::sender_init(char *dst_ip_addr_str, unsigned short bind_port)
{
	sender_init(inet_addr(dst_ip_addr_str), bind_port);
}

void TUdpSocket::sender_init(int dst_ip_addr, unsigned short bind_port)
{
	_is_dont_wait = false;
	_socket_type = SEND_SOCKET_TYPE;
	_rcv_sock = NULL;
	
	SOCKET snd_sock = NULL;

	if ((snd_sock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		printf("Error : Can't create unicast snd socket\n");
	}

	//송신 소켓에 Dest주소정보를 포함하도록 수정 고려
	_snd_addr.sin_family = AF_INET;
	_snd_addr.sin_addr.s_addr = dst_ip_addr;
	_snd_addr.sin_port = htons(bind_port);
	_snd_sock_list.push_back(snd_sock);

	//set_snd_buf_size(SEND_SOCK_BUF_SIZE);
}

void TUdpSocket::sender_init(char *mcast_addr_str, unsigned short bind_port, vector<string>& src_ip_list)
{
	sender_init(inet_addr(mcast_addr_str), bind_port, src_ip_list);
}

/*
multicast UDP 송신 소켓을 위한 생성자
*/
void TUdpSocket::sender_init(int mcast_addr, unsigned short bind_port, vector<string> &src_ip_list)
{
	_is_dont_wait = false;
	_socket_type = SEND_SOCKET_TYPE;
	_snd_addr.sin_family = AF_INET;
	_snd_addr.sin_addr.s_addr = mcast_addr;
	_snd_addr.sin_port = htons(bind_port);

	for (unsigned int i = 0; i<src_ip_list.size(); i++)
	{
		SOCKET	snd_sock;

		if ((snd_sock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
		{
			printf("Can't create multicast snd socket\n");
		}

		int ttl = 32;

		int option = 1;
		// 소켓 재사용 옵션 지정
		setsockopt(snd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));

		/*멀태캐스트 메시지 TTL 값 32로 설정*/
		setsockopt(snd_sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));

		struct in_addr in_addr_multicast;
		in_addr_multicast.s_addr = inet_addr(src_ip_list[i].c_str());
		setsockopt(snd_sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&in_addr_multicast.s_addr, sizeof(struct in_addr));
		printf("setsockopt(%d) : IP_MUTICAST_IF - %s\n", snd_sock, src_ip_list[i].c_str());
		_snd_sock_list.push_back(snd_sock);
	}
	//set_snd_buf_size(SEND_SOCK_BUF_SIZE);
}

/* Unicast UDP 수신을 위한 설정*/
void TUdpSocket::receiver_init(unsigned short bind_port)
{
	_is_dont_wait = false;
	_socket_type = RECEIVE_SOCKET_TYPE;
	_rcv_sock = NULL;

	if((_rcv_sock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		printf("Error : Can't create unicast rcv socket\n");
	}

	int option = 1;
	// 소켓 재사용 옵션 지정
	setsockopt(_rcv_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));

	struct sockaddr_in sockaddr;
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(bind_port);

	if(bind(_rcv_sock, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in)) == -1)
	{
		printf("Fail: UDP Unicast socket bind (port:%u)\n", bind_port);
	}
	else
	{
		printf("Success : UDP Unicast socket bind (port:%u)\n", bind_port);
	}

	_bind_port = bind_port;
	//set_rcv_buf_size(RECEIVE_SOCK_BUF_SIZE);
}

void TUdpSocket::set_src_port_for_send(unsigned short src_port)
{
	if (src_port > 0)
	{
		struct sockaddr_in servaddr;
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(src_port);

		// 소켓 재사용 옵션 지정
		for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
		{
			int option = 1;
			setsockopt(_snd_sock_list[i], SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));

			if (bind(_snd_sock_list[i], (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR)
			{
				printf("Error : TCP(Send) bind error (port:%u)\n", src_port);
			}
		}
	}
}

void TUdpSocket::receiver_init(char *mcast_addr_str, unsigned short bind_port, vector<string> src_ip_list)
{
	receiver_init(inet_addr(mcast_addr_str), bind_port, src_ip_list);
}

//Multicast UDP 수신 소켓을 위한 생성자
void TUdpSocket::receiver_init(int mcast_addr, unsigned short bind_port, vector<string> src_ip_list)
{
	_is_dont_wait = false;
	_socket_type = RECEIVE_SOCKET_TYPE;
	if ((_rcv_sock = socket(PF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		printf("Can't create multicast rcv socket\n");
	}

	struct sockaddr_in sockaddr;
	_bind_port = bind_port;
	//strcpy(multicastAddr, mcast_addr);

	if (src_ip_list.size() != 1)
	{
		printf("%%%%%%%%%%%% UDP Receiver IP count : %d\n", src_ip_list.size());
	}

	sockaddr.sin_family = AF_INET;
	//sockaddr.sin_addr.s_addr = inet_addr(ucastAddrList[0].c_str());
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons((unsigned short)_bind_port);
	printf("bind receive ip addr : %s \n", src_ip_list[0].c_str());

	int option = 1;
	// 소켓 재사용 옵션 지정
	if (setsockopt(_rcv_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option))<0)
	{
		printf("Error : socket option(SO_REUSEADDR) setting error for Multicast\n");
		closesocket(_rcv_sock);
		return;
	}

	if (bind(_rcv_sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_in)) == -1)  /*Bind*/
	{
		printf("Error : UDP Multicast socket bind error(port:%d)\n", _bind_port);
		closesocket(_rcv_sock);
		return;
	}

	struct ip_mreq ipmr;

	ipmr.imr_multiaddr.s_addr = mcast_addr;
	//ipmr.imr_interface.s_addr = htonl(INADDR_ANY);
	
	//NIC이 여러개일경우 모든 NIC로 MulticastUdp를 수신
	for (unsigned int i = 0; i<src_ip_list.size(); i++)
	{
		ipmr.imr_interface.s_addr = inet_addr(src_ip_list[i].c_str());//htonl(INADDR_ANY);
		struct in_addr mcast_in_addr;
		mcast_in_addr.s_addr = mcast_addr;
		if (setsockopt(_rcv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipmr, sizeof(ipmr)) != 0) 
		{
			printf("setsockopt(%d) : IP_ADD_MEMBERSHIP error - %s - %s\n", _rcv_sock, src_ip_list[i].c_str(), inet_ntoa(mcast_in_addr));
			break;
		}
		else
		{
			printf("setsockopt(%d) : IP_ADD_MEMBERSHIP success - %s - %s\n", _rcv_sock, src_ip_list[i].c_str(), inet_ntoa(mcast_in_addr));
		}
	}
	//set_rcv_buf_size(RECEIVE_SOCK_BUF_SIZE);
}

TUdpSocket::~TUdpSocket(void)
{
#ifdef WIN32
	if(_rcv_sock!=NULL)
	{
		closesocket(_rcv_sock);
	}

	for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
	{
		closesocket(_snd_sock_list[i]);
	}
#else
	if(_rcv_sock!=0)
	{
		shutdown(_rcv_sock, SHUT_RDWR);
		close(_rcv_sock);
	}

	for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
	{
		close(_snd_sock_list[i]);
	}
#endif
}

void TUdpSocket::set_rcv_buf_size(unsigned int buf_size)
{
	setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVBUF, (char*)&buf_size, sizeof(buf_size));
}

void TUdpSocket::set_snd_buf_size(unsigned int buf_size)
{
	for (unsigned int i = 0; i< _snd_sock_list.size(); i++)
	{
		setsockopt(_snd_sock_list[i], SOL_SOCKET, SO_SNDBUF, (char*)&buf_size, sizeof(buf_size));
	}
}

/*
udp 소켓을 수신하는 함수
*/
int TUdpSocket::read(char *payload, unsigned int rcv_len)
{
	int len = -1;

	if(_socket_type == RECEIVE_SOCKET_TYPE)
	{
		struct sockaddr_in sender_addr;
#if defined(WIN32) || defined(VXWORKS)
		int sock_addr_size=sizeof(struct sockaddr_in);
#else
		socklen_t sock_addr_size =sizeof(struct sockaddr_in);
#endif

		if (_is_dont_wait == false)
		{
			len = recvfrom(_rcv_sock, payload, rcv_len, 0, (struct sockaddr *)&sender_addr, &sock_addr_size);
		}
		else
		{
#if defined(WIN32)
			len = recvfrom(_rcv_sock, payload, rcv_len, 0, (struct sockaddr *)&sender_addr, &sock_addr_size);
#else
			len = recvfrom(_rcv_sock, payload, rcv_len, MSG_DONTWAIT, (struct sockaddr *)&sender_addr, &sock_addr_size);
#endif
		}
			
	}
	else
	{
		printf("Error : UdpSocket's type is SendSocket!!\n");
	}

	return len;
}

int TUdpSocket::read(SerializedPayload &serialized_payload, unsigned int rcv_len)
{
	unsigned int len = read(serialized_payload.get_payload_ptr(), rcv_len);
	serialized_payload.set_payload_len(len);
	return len;
}


/*
udp 소켓으로 전송하는 함수
*/
int TUdpSocket::post(char *payload, unsigned int length)
{
	int sent_len = -1;
	if(_socket_type == SEND_SOCKET_TYPE)
	{
		for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
		{
			sent_len = sendto(_snd_sock_list[i], payload, length, 0, (struct sockaddr *)&_snd_addr, sizeof(struct sockaddr_in));
		}
	}
	else
	{
		printf("Error : UdpSocket's type is ReceiveSocket!!\n");
	}
	return sent_len;
}

int TUdpSocket::post(int dst_ip_addr, unsigned short bind_port, char *payload, int length)
{
	int sent_len = -1;
	if (_socket_type == SEND_SOCKET_TYPE)
	{
		struct sockaddr_in snd_addr_tmp;
		snd_addr_tmp.sin_family = AF_INET;
		snd_addr_tmp.sin_addr.s_addr = dst_ip_addr;
		snd_addr_tmp.sin_port = htons(bind_port);

		for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
		{
			sent_len = sendto(_snd_sock_list[i], payload, length, 0, (struct sockaddr *)&snd_addr_tmp, sizeof(struct sockaddr_in));
		}
	}
	else
	{
		printf("Error : UdpSocket's type is ReceiveSocket!!\n");
	}
	return sent_len;
}

int TUdpSocket::post(SerializedPayload &serialized_payload)
{
	return post(serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
}

int TUdpSocket::post(int dst_ip_addr, unsigned short bind_port, SerializedPayload &serialized_payload)
{
	return post(dst_ip_addr, bind_port, serialized_payload.get_payload_ptr(), serialized_payload.get_payload_len());
}


void TUdpSocket::set_snd_time_out(int milisec_time_out)
{
	for (unsigned int i = 0; i < _snd_sock_list.size(); i++)
	{
#ifdef WIN32
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_snd_sock_list[i], SOL_SOCKET, SO_SNDTIMEO, (char*)&milisec_time_out, opt_len);
#else
		struct timeval opt_val;
		opt_val.tv_sec = milisec_time_out / 1000;
		opt_val.tv_usec = (milisec_time_out % 1000) * 1000;
		int opt_len = sizeof(opt_val);
		int res = setsockopt(_snd_sock_list[i], SOL_SOCKET, SO_SNDTIMEO, (char*)&opt_val, opt_len);
		printf("set snd Time out - %d, (res:%d)", res, milisec_time_out);
#endif
	}
}

void TUdpSocket::set_dont_wait(bool is_dont_wait)
{
#ifdef WIN32
	set_rcv_time_out(1);
#else
	_is_dont_wait = is_dont_wait;
#endif
}

void TUdpSocket::set_rcv_time_out(int milisec_time_out)
{
	if (_rcv_sock)
	{
		
#ifdef WIN32
		int opt_len = sizeof(milisec_time_out);
		int res = setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&milisec_time_out, opt_len);
		
#else
		struct timeval opt_val;
		opt_val.tv_sec = milisec_time_out / 1000;
		opt_val.tv_usec = (milisec_time_out % 1000) * 1000;
		int opt_len = sizeof(opt_val);
		int res = setsockopt(_rcv_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt_val, opt_len);
		//printf("set rcv Time out - %d, (res:%d) \n", res, milisec_time_out);
#endif
	}
	else
	{
		printf("Error : RcvSock is not initialized!\n");
	}
}

unsigned short TUdpSocket::get_port()
{
	return _bind_port;
}