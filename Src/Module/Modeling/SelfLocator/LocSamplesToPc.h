#ifndef LocSamplesToPc_H
#define LocSamplesToPc_H
#include "Module/Representations/KS.h"
#include "Module/Representations/KSManager.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/Modeling/BallModel.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include "Module/Network/networkComm/NetworkMsg.h"
//该ks平时不开启
DEFINE_KS(LocSamplesToPc)
REQUIRES(Robotsamplebuffer)
REQUIRES(Ballsamplebuffer)
REQUIRES(FreePartData)
//REQUIRES(MotionCmd)
PROVIDES(ForSampleToPc)
END_KS

class LocSamplesToPc : public LocSamplesToPcBase,public BaseProvider
{
public:
	LocSamplesToPc(){}
	~LocSamplesToPc(){}
	void inRobotsamplebuffer(){}
	void inBallsamplebuffer(){}
	void inFreePartData(){}
	void update(ForSampleToPc* theForSampleToPc);
	void outForSampleToPc(){}
    CommandData cmd;
};
#endif 