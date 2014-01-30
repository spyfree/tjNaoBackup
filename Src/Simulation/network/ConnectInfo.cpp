#include "ConnectInfo.h"
ConnectInfo::ConnectInfo()
{
	SetInit();
}
ConnectInfo * ConnectInfo::operator =(ConnectInfo *robotinfo)
{
	return robotinfo;
}
void ConnectInfo::SetInit()
{
	nameString="nao_name";
	ipstring="192.168.1.42";

	port="21198";
}
void ConnectInfo::Copy(ConnectInfo robotinfo)
{
	nameString=robotinfo.nameString;
	ipstring=robotinfo.ipstring;
	port=robotinfo.port;
}
bool ConnectInfo::operator ==(ConnectInfo robotinfo)
{
	if(nameString==robotinfo.nameString)
		return true;
	else 
		return false;

}