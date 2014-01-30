#ifndef VISIONPERCEPT_H
#define VISIONPERCEPT_H
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Module/Vision/TJImage.h"
#include "Module/Vision/Regionpercept.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Network/networkComm/NetworkMsg.h"
DEFINE_KS(VisionPercept)
REQUIRES(CameraMatrix)
REQUIRES(BallPercept)
REQUIRES(GoalPercept)
REQUIRES(LinesPercept)
REQUIRES(Regionpercept)
PROVIDES(TJImage)
PROVIDES(netVisionInterface)
END_KS
class VisionPercept : public VisionPerceptBase,BaseProvider
{
public:
	VisionPercept(){}
	~VisionPercept(){}
	void inCameraMatrix(){}
	void inBallPercept(){}
	void inGoalPercept(){}
	void inLinesPercept(){}
	void inRegionpercept(){}
	void update(netVisionInterface* thenetVisionInterface);
	void update(TJImage* theTJImage);
	void outnetVisionInterface(){};
	void outTJImage(){};
	CommandData cmd;

};
#endif
