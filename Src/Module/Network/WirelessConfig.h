#ifndef WIRELESSCONFIG_H
#define WIRELESSCONFIG_H


#define TOOL_COMMANDING 0
#define TOOL_REQUESTING 1


#define CMD_TABLE      0
#define CMD_MOTION     1
#define CMD_HEAD       2
#define CMD_JOINTS     3




//nao
#define UDP_PORT  22297
#define TCP_SEND_PORT  21191
#define TCP_RECEIVE_PORT 21192
//#define TOOL_PORT 4002

#define UDP_BUF_SIZE 1024
#define TCP_BUF_SIZE 1048576 // 1MB for the Nao's


# define SOCKETNS

#define SOCKET_ERROR        (-1)

#define SIZEOF_REQUEST 10
struct DataRequest {
	bool info;
	bool joints;
	bool sensors;
	bool image;
	bool thresh;
	bool jpeg;
	bool objects;
	bool motion;
	bool local;
	bool comm;
};


#endif
