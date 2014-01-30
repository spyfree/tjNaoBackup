#include "RemoteCmdProvider.h"
RemoteCmdProvider::RemoteCmdProvider()
{
    }
RemoteCmdProvider::~RemoteCmdProvider()
{
}

void RemoteCmdProvider::setMotionCmd(const MotionCmd& motionCmd_)
{
        std::cout<<"RemoteCmdProvider::setMotionCmd: Motion: "<<motionCmd_.motion<<", Type: "<<motionCmd_.walkCmd.walkType<<"spec cmd"<<motionCmd_.specActionCmd.specAction;
        std::cout<<", "<<motionCmd_.walkCmd.speed.translation.x<<", "<<motionCmd_.walkCmd.speed.translation.y<<std::endl;
		selfMessageQueue->SetMessageData(idMotionCmd,idMotionEngineWindow,idActuatorThread,
			sizeof(MotionCmd), (char*)(&motionCmd_));
}
