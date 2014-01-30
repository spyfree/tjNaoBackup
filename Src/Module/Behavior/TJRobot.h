#ifndef TJRobot_H
#define TJRobot_H
#include "Module/Core/Thread.h"
#include "RobotProvider.h"

class TJRobot:public Thread
{
public:
	TJRobot(SmartPtr<Synchronizer> _synchro);
	~TJRobot();
	int start();
	void stop();
private:
	void run();
	RobotManager robotmanager;
};
#endif
