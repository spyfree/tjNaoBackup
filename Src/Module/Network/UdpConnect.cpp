#include "UdpConnect.h"
#ifndef WIN32
#include "UdpPoint.cpp"
#endif
UdpConnect::UdpConnect()
{

}
UdpConnect::~UdpConnect()
{
	udpPoint.close();
}
void UdpConnect::run()
{
	udpPoint.bind();
	cout<<"udprun"<<endl;

	//for(int i=0;i<10;i++)
	int ret;
	while(1)
	{
		ret=udpPoint.select();
		if(ret==SOCKET_ERROR)
		{
			cout<<"error"<<endl;
		}
		if(ret==0)
		{
			cout<<"time out"<<endl;
		}
		else if(ret>0)
		{
			if(udpPoint.insert())
				udpPoint.read();
		}


	}

}
