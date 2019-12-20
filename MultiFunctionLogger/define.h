#pragma once

#define DISPLAY_NOT_READ	-1		
#define DISPLAY_NONE	0		
#define DISPLAY_CONSOLE 1	// 1<<1
#define DISPLAY_FILE	2	// 1<<2

//#define NOT_USE_REDUNDANT -1
#define MAX_LINE_LENGTH 1024
#define MAX_CONF_MSG_LENGTH 1024
#define MAX_FILE_NAME_LENGTH 256


#define SERVER_DISCOVERY_PORT_BASE 28000
#define CLIENT_DISCOVERY_PORT_BASE 28100
#define SERVER_PORT_BASE 28300
#define CONFIG_MSG_PORT_BASE 28200

#define DISCOVERY_IP_ADDR "225.10.2.1"
#define SEND_DISCOVERY_PERIOD 200
#define DISCOVERY_TIMEOUT 3000
#define MAX_LOG_PAYLOAD_SIZE 10*1024

#define DISCOVERY_PKT_SIGNATURE1		0x454D4154L
#define DISCOVERY_PKT_SIGNATURE2		0x44474F4CL

#define USER_PKT_SIGNATURE1		0x454D4154L
#define USER_PKT_SIGNATURE2		0x55474F4CL
#define SEND_SOCKET_BUFFER_SIZE		1024*1024
#define RECEIVE_SOCKET_BUFFER_SIZE	1024*1024

#define IP_ADDR_LEN				16

#define CONFIG_NONE -1
#define CONFIG_FALSE 0
#define CONFIG_TRUE 1
#define MAX_LOG_LENGTH 512

#define MAX_FILE_SIZE 10485760	//5120000
