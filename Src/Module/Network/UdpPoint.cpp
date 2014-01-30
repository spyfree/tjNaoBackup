#include "UdpPoint.h"
#include "altexttospeechproxy.h"
UdpPoint::UdpPoint()
{
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(UDP_PORT);
	//broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	broadcast_addr.sin_addr.s_addr = inet_addr(UDP_BROADCAST);
	buf=(char *)malloc(UDP_BUF_SIZE);
	memset(buf,0,UDP_BUF_SIZE);
	gcTeam = 1;

	mreq.imr_multiaddr.s_addr=inet_addr(UDP_BROADCAST);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);

}
UdpPoint::~UdpPoint()
{

	delete buf;

}


void UdpPoint::close()
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
void UdpPoint::bind()
{

	sockn=SOCKETNS::socket(AF_INET, SOCK_DGRAM, 0);
	if(sockn==-1)
	{
		cout<<"error"<<endl;
		close() ;
	}
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(UDP_PORT);
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(SOCKETNS::bind(sockn, (const struct sockaddr*)&bind_addr,
		sizeof(bind_addr)) == -1)
	{
		cout<<"error2"<<endl;
		close();
	}
	int on = 1;
	setsockopt(sockn,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
	//setsockopt(sockn, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));

	// Set socket to nonblocking io mode
	DWORD   ul=   1;

//#ifdef COMM_LISTEN
  // Set socket to nonblocking io mode

//#endif

#ifdef WIN32

	ioctlsocket(sockn,FIONBIO,&ul);
#else

	ioctl(sockn,FIONBIO,&ul);
#endif
}
bool UdpPoint::readdata(TeamMateInfo &data)
{
    memset(&data,0,sizeof(TeamMateInfo));
	int result;
	char *buff =(char *)malloc(UDP_BUF_SIZE);
	int len = UDP_BUF_SIZE;
	bool received=false;
	int length = sizeof(bind_addr);
	while((result = recvfrom(sockn,buff,UDP_BUF_SIZE, 0,(struct sockaddr *) &bind_addr,(socklen_t *)&length))>0)
	{
		if(result == UDP_BUF_SIZE)
		{
			std::cout<<"###########GOTpacket#######"<<std::endl;
			received = true;
			if(validatePacket(buff,result)>0)
			{
				std::cout<<"###########GOTpacket form other robots"<<std::endl;
				parsePacket(buff);
				gotMateInfo++;
			}

			else if(validatePacket(buff,result) == 0)
			{
				std::cout<<"###########GOT own packet"<<std::endl;
				parsePacket(buff);
				gotMateInfo++;
			}
		}
	}
	free(buff);
	for(int j=0;j!=gotMateInfo&&j!=MAX_MESSAGE_MEMORY;++j)
	{
		TeamUDPData newInfo;
		newInfo = latest.back();
		if (newInfo.role==1)
		{
			memcpy(&(data.teamMateInfo1),&newInfo,sizeof(TeamUDPData));
			data.teamMateInfo1.received = true;
		}
		else if(newInfo.role==2)
		{
			memcpy(&(data.teamMateInfo2),&newInfo,sizeof(TeamUDPData));
			data.teamMateInfo2.received = true;
		}
		else if(newInfo.role==3)
		{
			memcpy(&(data.teamMateInfo3),&newInfo,sizeof(TeamUDPData));
			data.teamMateInfo3.received = true;
		}
		else if(newInfo.role==4)
		{
			memcpy(&(data.teamMateInfo4),&newInfo,sizeof(TeamUDPData));
			data.teamMateInfo4.received = true;
		}
		else if(newInfo.role==0)
		{
			memcpy(&(data.teamMateInfo0),&newInfo,sizeof(TeamUDPData));
			data.teamMateInfo0.received = true;
		}
		latest.pop_back();
	}
	if(gotMateInfo)
		return true;
	else
		return false;
}

void UdpPoint::write(char *buffer,int len)
{

	//std::cout<<"before write"<<std::endl;
	int result=SOCKETNS::sendto(sockn, buffer, len, 0, (struct sockaddr*)&broadcast_addr,sizeof(broadcast_addr));
	if(result == SOCKET_ERROR)
		std::cout<<"send failed"<<std::endl;
	else
		std::cout<<"packet send"<<std::endl;
	//std::cout<<"AAAAAAAAAAAAAAAAAA"<<WSAGetLastError()<<std::endl;
	//std::cout<<"after write"<<std::endl;
}
int UdpPoint::select()
{
	int ret;
	timeval tval;
	tval.tv_sec=0;
	tval.tv_usec=50000;
	gotMateInfo = 0;

	FD_ZERO(&fdread);
	FD_SET(sockn,&fdread);
	ret=SOCKETNS::select(sockn+1,&fdread,NULL,NULL,&tval);
	return ret;
}
bool UdpPoint::insert()
{
	return FD_ISSET(sockn,&fdread);
}
void UdpPoint::setOwnTeamInfo(TeamUDPData *ownTeam)
{
	ownTeamInfo = *ownTeam;
}
void UdpPoint::send()
{
	int bufSize = UDP_BUF_SIZE;
	char* sendbuffer = (char*)malloc(bufSize);

	memcpy(sendbuffer,&ownTeamInfo,sizeof(TeamUDPData));
	memset(sendbuffer+sizeof(TeamUDPData),'\0',UDP_BUF_SIZE-sizeof(TeamUDPData));
    write(sendbuffer,bufSize);
	free(sendbuffer);
}

void UdpPoint::sendTest()
{
	char Textp[] = "send infomation \n";
	sendto(sockn, Textp,strlen(Textp), 0, (struct sockaddr*)&broadcast_addr,sizeof(broadcast_addr));
}


int UdpPoint::validatePacket(const char* msg, int len)
{
	CommPacketHeader packet;
	TeamUDPData v;

	if (len < UDP_BUF_SIZE)
		return 0;

	memcpy(&packet,msg,sizeof(CommPacketHeader));
	memcpy(&v, msg, sizeof(TeamUDPData));


	if(v.role == ownTeamInfo.role)
		return 0;

	if(memcmp(packet.header, PACKET_HEADER, sizeof(PACKET_HEADER)) == 0)
		return 1;
	if(memcmp(packet.header,DEBUG_HEADER,sizeof(DEBUG_HEADER)) ==0)
		return 2;

	return 0;
}

void UdpPoint::parsePacket(const char* data)
{

	TeamUDPData v;
	memcpy(&v, data, sizeof(TeamUDPData));

	while (latest.size() >= MAX_MESSAGE_MEMORY)
		latest.pop_front();
	latest.push_back(v);
}
