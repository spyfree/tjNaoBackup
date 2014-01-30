#ifndef GameControlHandle_H
#define GameControlHandle_H
#include "Module/Common/CommonData.h"
#include "networkComm/NetworkInclude.h"
#include "networkComm/WdataInfo.h"
#include "WirelessConfig.h"
#include <fcntl.h>
#include"RoboCupGameControlData.h"
class GameControlHandle
{
public:
	GameControlHandle();
	~GameControlHandle();
	void bind();
	bool read(RoboCupGameControlData&data);
	void write(char *buffer ,int len);
	int select();
	bool insert();
	void close();
	void send();
private:
	int sockn;

	fd_set fdread;
	struct sockaddr_in  bind_addr;
	struct sockaddr_in  broadcast_addr;
};


#endif
