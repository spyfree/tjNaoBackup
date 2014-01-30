#ifndef ConnectInfo_H
#define  ConnectInfo_H
#include <QWidget>
class ConnectInfo
{
public:
	ConnectInfo();
	void Copy(ConnectInfo robotinfo);
	void SetInit();
	QString nameString;
	QString ipstring;
	QString port;
	ConnectInfo* operator=(ConnectInfo *robotinfo);
	bool operator==(ConnectInfo robotinfo);
};
#endif