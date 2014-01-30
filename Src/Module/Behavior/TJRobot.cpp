#include "TJRobot.h"

TJRobot::TJRobot(SmartPtr<Synchronizer> _synchro)
:Thread(_synchro, "TJRobot")
{
    std::cout<<"init robot thread"<<std::endl;
}
TJRobot::~TJRobot()
{

}
//------------function to control the thread----------//
int TJRobot::start()
{
	return Thread::start();
}
void TJRobot::stop()
{
	Thread::stop();
}

static const unsigned TJRobot_msPF = 200;
void TJRobot::run()
{
	std::cout<<"++++++++++++++++ start robot thread ++++++++++++++"<<std::endl;
	robotmanager.initRobotKs();
	running=true;
	spMyTrigger->on();
	while(running)
	{
		unsigned startTime = SystemCall::getCurrentSystemTime();
		robotmanager.executeRobKs();

		unsigned timeUsed = SystemCall::getTimeSince(startTime);
		std::cout<<"========================Time Used per Frame(TJRobot):"<<timeUsed<<" ms "<<std::endl;
        /*if (timeUsed < TJRobot_msPF)
        {

			SystemCall::sleep(TJRobot_msPF-timeUsed);
        }*/
	}

	running=false;
	robotmanager.destoryRobKs();
	spMyTrigger->off();
}
