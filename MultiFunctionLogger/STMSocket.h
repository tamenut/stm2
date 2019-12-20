#pragma once

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif

#ifndef WIN32
#define SOCKET_ERROR    -1
#define SOCKET                  int
#define closesocket             close
#endif

class  STMSocket
{
public:
	STMSocket(){};
	virtual ~STMSocket(){};
	virtual int post(char *payload, int payload_len) = 0;
	virtual int read(char *payload) = 0;
protected :

	SOCKET rcvSock;
	SOCKET sndSock;
};

