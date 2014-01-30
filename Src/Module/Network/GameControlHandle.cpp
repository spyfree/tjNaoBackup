#include "GameControlHandle.h"
#include "altexttospeechproxy.h"
GameControlHandle::GameControlHandle()
{
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(GAMECONTROLLER_PORT);
	broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
}
GameControlHandle::~GameControlHandle()
{

}
void GameControlHandle::bind()
{

	sockn=SOCKETNS::socket(AF_INET, SOCK_DGRAM, 0);
	if(sockn==-1)
	{
		cout<<"error"<<endl;
		close() ;
	}
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(GAMECONTROLLER_PORT);
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(SOCKETNS::bind(sockn, (const struct sockaddr*)&bind_addr,
		sizeof(bind_addr)) == -1)
	{
		cout<<"error2"<<endl;
		close();
	}
	int on = 1;
	setsockopt(sockn, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));

	// Set socket to nonblocking io mode
	DWORD   ul=   1;

#ifdef WIN32

	ioctlsocket(sockn,FIONBIO,&ul);
#else

	ioctl(sockn,FIONBIO,&ul);
#endif
}
void GameControlHandle::close()
{

#ifdef WIN32
	if (sockn != -1)
		SOCKETNS::closesocket(sockn);
#else
	if (sockn != -1)
		SOCKETNS::close(sockn);
#endif
	// invalidate file descriptor
	sockn = -1;
}
bool GameControlHandle::read(RoboCupGameControlData&data)
{
	struct sockaddr_in recv_addr;
	int addr_len = sizeof(sockaddr_in);
	int size;
	RoboCupGameControlData buffer;
	//size = recv(sockn, (char*)&buffer, sizeof(buffer), 0);
	//data=buffer;

	bool received=false;
    while(0<(size=recv(sockn, (char*)&buffer, sizeof(buffer), 0)))
	{
		if (size==sizeof(buffer)&& !std::memcmp(&buffer, GAMECONTROLLER_STRUCT_HEADER, 4)
			&&buffer.version == GAMECONTROLLER_STRUCT_VERSION)
		{
			data=buffer;
			received=true;
		}
	}
	return received;
	//std::cout<<"buffer----score"<<buffer.teams[0].players[0].penalty<<std::endl;
	//AL::ALTextToSpeechProxy proxy("192.168.1.104",10000);
	//if (buffer.teams[0].players[0].penalty==0)
	//proxy.say("penalty");
	//else
	//proxy.say("unpenalty");

}




//don't use now
void GameControlHandle::write(char *buffer,int len)
{
	int result=SOCKETNS::sendto(sockn, buffer, len, 0, (struct sockaddr*)&broadcast_addr,
		sizeof(broadcast_addr));

}
int GameControlHandle::select()
{
	std::cout<<"udp select-------start"<<std::endl;
	int ret;
	FD_ZERO(&fdread);
	std::cout<<"udp select-------end1"<<std::endl;
	FD_SET(sockn,&fdread);
	std::cout<<"udp select-------end2"<<std::endl;
	ret=SOCKETNS::select(0,&fdread,NULL,NULL,NULL);
	std::cout<<"udp select-------end"<<std::endl;
	return ret;
}
bool GameControlHandle::insert()
{
	return FD_ISSET(sockn,&fdread);
}

void GameControlHandle::send()
{
	const unsigned char team=0;
	const unsigned char player=1;
	const unsigned char color=1;
	CommPacketHeader  datainfo;
	strcpy(datainfo.header, PACKET_HEADER);
	datainfo.team=1;
	datainfo.player=2;
	datainfo.color=1;
	char buffer[20];
	memcpy(&buffer[0], &datainfo, sizeof(CommPacketHeader));
	//memcpy(buf+sizeof(datainfo),&team,sizeof(unsigned char));
	write(&buffer[0],sizeof(CommPacketHeader));
}
