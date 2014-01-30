#include "VisionPercept.h"
#define Always_genclassifiedPic 0//1 open , 0 close
void VisionPercept::update(netVisionInterface* thenetVisionInterface)
{
#ifdef TJClassfiedPic
	if (cmd.RC_COMMAND==CPLANE_SEND || Always_genclassifiedPic)
	{
		///////goal///////////////////////////////////
		thenetVisionInterface->ownleftgoalSeen = (uint8)(theGoalPercept->getownleftgoalseen()?1:0);
		thenetVisionInterface->ownrightgoalSeen = (uint8)(theGoalPercept->getownrightgoalseen()?1:0);
		thenetVisionInterface->unknowowngoalSeen = (uint8)(theGoalPercept->getunowngoalseen()?1:0);
		thenetVisionInterface->oppleftgoalSeen=(uint8)(theGoalPercept->getoppleftgoalseen()?1:0);
		thenetVisionInterface->opprightgoalSeen=(uint8)(theGoalPercept->getopprightgoalseen()?1:0);
		thenetVisionInterface->unknowoppgoalSeen = (uint8)(theGoalPercept->getunoppgoalseen()?1:0);
		// netvision->HorizenInImage = (uint8)(theRegionpercept->horizonImage ?1:0);
		//for some reason , mutilp 100, when use the datas,remeber to divide 100
		thenetVisionInterface->ownleftGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OWN].positionOnField.x);
		thenetVisionInterface->ownleftGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OWN].positionOnField.y);
		thenetVisionInterface->ownrightGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OWN].positionOnField.x);
		thenetVisionInterface->ownrightGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OWN].positionOnField.y);
		thenetVisionInterface->unknowownGoalX = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OWN].positionOnField.x);
		thenetVisionInterface->unknowownGoalY = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OWN].positionOnField.y);
		thenetVisionInterface->oppleftGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OPPONENT].positionOnField.x);
		thenetVisionInterface->oppleftGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OPPONENT].positionOnField.y);
		thenetVisionInterface->opprightGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OPPONENT].positionOnField.x);
		thenetVisionInterface->opprightGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OPPONENT].positionOnField.y);
		thenetVisionInterface->unknowoppGoalX = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OPPONENT].positionOnField.x);
		thenetVisionInterface->unknowoppGoalY= (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OPPONENT].positionOnField.y);
		thenetVisionInterface->horizonStartX = (int32)theRegionpercept->horizonStart.x;
		thenetVisionInterface->horizonStartY = (int32)theRegionpercept->horizonStart.y;
		thenetVisionInterface->horizonEndX = (int32)theRegionpercept->horizonEnd.x;
		thenetVisionInterface->horizonEndY = (int32)theRegionpercept->horizonEnd.y;
		thenetVisionInterface->HorizenInImage=(uint8)theRegionpercept->horizonImage;

		//////////////////////ball////////////////////////////////////////
		thenetVisionInterface->ballSeen=theBallPercept->invion;
		thenetVisionInterface->balldis=(int32)(100*theBallPercept->getdistanceOfBall());
		thenetVisionInterface->ballx=(int32)(100*theBallPercept->relativePositionOnField.x);
		thenetVisionInterface->bally=(int32)(100*theBallPercept->relativePositionOnField.y);

		///////////////////line/////////////////////////

		thenetVisionInterface->CircleSeen=theLinesPercept->circleCenter.found;
		thenetVisionInterface->CircleCenterX=(int32)100*(theLinesPercept->circleCenter.pos.x);
		thenetVisionInterface->CircleCenterY=(int32)100*(theLinesPercept->circleCenter.pos.y);

		thenetVisionInterface->row = (int32)(theCameraMatrix->rotation.getXAngle()*1000000000);
		thenetVisionInterface->pitch = (int32)(theCameraMatrix->rotation.getYAngle()*1000000000);
		thenetVisionInterface->yaw = (int32)(theCameraMatrix->rotation.getZAngle()*1000000000);
		thenetVisionInterface->camX = (int32)(theCameraMatrix->translation.x*100000);
		thenetVisionInterface->camY = (int32)(theCameraMatrix->translation.y*100000);
		thenetVisionInterface->camZ = (int32)(theCameraMatrix->translation.z*100000);

		selfMessageQueue->SetMessageData(idVisionPercept,idRobotThread,idNetworkThread,
			sizeof(netVisionInterface),(char*)thenetVisionInterface);
	}
	//std::cout<<"-------------------------"<<std::endl;
#endif
}

void VisionPercept::update(TJImage* theTJImage)
{
#ifdef TJClassfiedPic
	//	//std::cout<<"ImageProvider::update , genclassfiedpic "<<std::endl;
	cmd.clear();
	selfMessageQueue->SearchMyMessage(idVisionCmd,idNetworkThread,idRobotThread,(char*)&cmd,sizeof(CommandData));
	//std::cout<<"VisionPercept  cmd.RC_COMMAND----------"<<cmd.RC_COMMAND<<std::endl;
	if (cmd.RC_COMMAND==CPLANE_SEND || Always_genclassifiedPic)
	{
		theTJImage->genclassifiedPic(*theTJImage);
		selfMessageQueue->SetMessageData(idClassifyImage,idRobotThread,idNetworkThread,
			IMAGE_HEIGHT*IMAGE_WIDTH,(char*)theTJImage->classifiedPic);
	}
#endif
}
MAKE_KS(VisionPercept)
