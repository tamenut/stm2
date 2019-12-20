#include "TSocket.h"
#include <stdio.h>


TSocket::TSocket(void)
{
	wsa_startup();
	_rcv_sock = 0;
}

TSocket::~TSocket(void)
{
}




