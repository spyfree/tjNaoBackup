#include "TJSimRobot.h"

TJSimRobot::TJSimRobot(SmartPtr<Synchronizer> _synchro)
:Thread(_synchro, "TJRobot")
{
    std::cout<<"init robot thread"<<std::endl;
	theLocMessageData = new LocMessageData();
}
TJSimRobot::~TJSimRobot()
{
	if(theLocMessageData != NULL)
	{
		delete theLocMessageData;
		theLocMessageData = NULL;
	}

}
//------------function to control the thread----------//
int TJSimRobot::start()
{
	return Thread::start();
}

void TJSimRobot::sendLocData()
{
	theLocMessageData->lines.clear();
	theLocMessageData->lines = DrawBuffer::getInstance()->lines;
	theLocMessageData->ellipses.clear();
	theLocMessageData->ellipses = DrawBuffer::getInstance()->ellipses;
	theLocMessageData->polygons.clear();
	theLocMessageData->polygons = DrawBuffer::getInstance()->polygons;

	selfMessageQueue->SetMessageData(idLocDrawing,idBehaviorThread,idMainWindow,
		sizeof(LocMessageData),(char*)theLocMessageData);

	DrawBuffer::getInstance()->lines.clear();
	DrawBuffer::getInstance()->ellipses.clear();
	DrawBuffer::getInstance()->polygons.clear();
}

static const unsigned TJRobot_msPF = 200;
void TJSimRobot::run()
{
	std::cout<<"++++++++++++++++ start robot thread ++++++++++++++"<<std::endl;
	robotmanager.initRobotKs();
	running=true;
	spMyTrigger->on();
	while(running)
	{
		if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
		{
			theSynchronizer->getEventbyName("VOffline->Behavior")->wait();
		}
		unsigned startTime = SystemCall::getCurrentSystemTime();
		robotmanager.executeRobKs();

		if(SignalControl::getInstance()->visionSignal == SignalControl::VODE)
		{
			theSynchronizer->getEventbyName("Behavior->ImageView")->signal();
		}
		else if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
		{
			theSynchronizer->getEventbyName("Behavior->VOffline")->signal();
		}

		sendLocData();

		unsigned timeUsed = SystemCall::getTimeSince(startTime);
		std::cout<<"Time Used per Frame(TJRobot):"<<timeUsed<<" ms "<<std::endl;
        if (timeUsed < TJRobot_msPF)
        {

			SystemCall::sleep(TJRobot_msPF-timeUsed);
        }
	}

	Py_Finalize();
	robotmanager.destoryRobKs();
	spMyTrigger->off();
}
