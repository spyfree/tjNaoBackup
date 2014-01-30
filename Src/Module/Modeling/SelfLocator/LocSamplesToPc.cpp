#include "LocSamplesToPc.h"
const int RobotsamplebufferSize=sizeof(Robotsamplebuffer);
const int BallsamplebufferSize=sizeof(Ballsamplebuffer);
const int FreePartSize=sizeof(FreePartData);

void LocSamplesToPc::update(ForSampleToPc *theForSampleToPc)
{
	
	cmd.clear();
	selfMessageQueue->SearchMyMessage(idLocCmd,idNetworkThread,idRobotThread,(char*)&cmd,sizeof(CommandData));
	/*std::cout<<"##########################收到啦UpdateSamplesToPc cmd.RC_COMMAND----------"<<
		  "RobotRotation==="<<theRobotsamplebuffer->Robotsample[1].Robotrotation<<"Translation==="<<theRobotsamplebuffer->Robotsample[1].RobotTranslationX
		  <<"\t"<<theRobotsamplebuffer->Robotsample[1].RobotTranslationY<<std::endl;*/
	if(cmd.RC_COMMAND==Loc_Send)
	{
	  selfMessageQueue->SetMessageData(idLocSample,idRobotThread,idNetworkThread,
			 RobotsamplebufferSize,(char*)theRobotsamplebuffer);
	  selfMessageQueue->SetMessageData(idBallSample,idRobotThread,idNetworkThread,
		     BallsamplebufferSize,(char*)theBallsamplebuffer);
	  selfMessageQueue->SetMessageData(idFreePart,idRobotThread,idNetworkThread,
		     FreePartSize,(char*)theFreePartData);
	}
	theForSampleToPc->a=1;//随便更新；
	//std::cout<<"update(ForSampleToPc *theForSampleToPc) "<<std::endl;
}
MAKE_KS(LocSamplesToPc)