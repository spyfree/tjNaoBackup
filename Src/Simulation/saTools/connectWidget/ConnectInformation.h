#ifndef CONNECTINFORMATION_H
#define CONNECTINFORMATION_H

#include <QWidget>	// add this just for QString

class  ConnectInformation
{
public:
	ConnectInformation();
public:
	QString masterNameString;
	QString ipString;
	QString portString;
protected:

private:

};

#endif  //CONNECTINFORMATION_H