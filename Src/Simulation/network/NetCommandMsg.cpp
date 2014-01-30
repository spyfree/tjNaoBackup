#include "NetCommandMsg.h"
NetCommandMsg ::NetCommandMsg()
{
	init();
}
NetCommandMsg::NetCommandMsg(NetCMsgID commandmsg,double data1,double data2,double data3)
{
	RS_COMMAND=commandmsg;
	value1=data1;
	value2=data2;
	value3=data3;
}
void NetCommandMsg::init()
{
	RS_COMMAND = idUndefined;
	value1 = value2 = value3 = 0.0;
}