#pragma once

#include <stdio.h>

#if defined(WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")


#if defined(DLL_EXPORT)
    #include <Winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <windows.h>
#endif

#include <io.h>
#elif LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <sys/timeb.h>


/*
#if defined (WIN32)	// Windows
	#if defined (SDDS_BUILD_DLL)	// SDDS DLL Build Option
		#define SDDS_DLL_EXPORT		__declspec (dllexport)
		#define SDDS_USR_DLL_EXPORT
	#elif defined (SDD_USR_DLL)		// SDDS User DLL Build Option
		#define SDDS_DLL_EXPORT		__declspec (dllimport)
		#define SDDS_USR_DLL_EXPORT	__declspec (dllexport)
	#else
		#define SDDS_DLL_EXPORT
		#define SDDS_USR_DLL_EXPORT
	#endif
#else	// Linux
	#define SDDS_DLL_EXPORT
	#define SDDS_USR_DLL_EXPORT
#endif
*/

namespace STM {

enum ENDPOINT_TYPE
{
	NONE_ENDPOINT_TYPE = 0,
	WRITER_ENDPOINT_TYPE,
	READER_ENDPOINT_TYPE
};

enum FLAG_ENDIAN
{
	FLAG_BIG_ENDIAN  = false,//0,
	FLAG_LITTLE_ENDIAN  = true//1
};

#define MAX_TOPIC_NAME_SIZE 128
#define MAX_CONF_MSG_LENGTH 128

#define IP_ADDR_LEN 32
#define FRAGMENT_MSG_SIZE 10*1024

#define DISCOVERY_PORT_BASE 35000
#define USER_DATA_PORT_BASE 36000
#define DISCOVERY_IP_ADDR "225.10.1.1"
#define PARTICIPANTID_GAIN	100

#define DISCOVERY_PKT_SIGNATURE1		0x454D4154L
#define DISCOVERY_PKT_SIGNATURE2		0x4454554EL

#define USER_PKT_SIGNATURE1		0x454D4154L
#define USER_PKT_SIGNATURE2		0x5554554EL

#define ACK_PKT_SIGNATURE1		0x454D4154L
#define ACK_PKT_SIGNATURE2		0x4154554EL

#define PARTICIPANT_TIMEOUT	12000
#define SEND_DISCOVERY_PERIOD 3000

#define SEND_SOCKET_BUFFER_SIZE 1*1024*1024
#define RECEIVE_SOCKET_BUFFER_SIZE 1*1024*1024

#define ACK_RECEIVE_TIME_OUT	1000
#define ACK_PAYLOAD_SIZE 512


#ifndef LINUX
typedef char			int8_t;
typedef unsigned char	uint8_t;
typedef short			int16_t;
typedef unsigned short	uint16_t;
typedef int				int32_t;
typedef unsigned int	uint32_t;
typedef long long		int64_t;
typedef unsigned long long uint64_t;
#endif

}






