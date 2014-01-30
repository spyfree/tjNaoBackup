#ifndef __SpecActionOutput_h_
#define __SpecActionOutput_h_

#include "Module/Common/JointData.h"
#include "Tools/Math/Pose2D.h"
#include "Module/Common/SpecActionCmd.h"
class SpecActionOutput : public JointCmd
{
public:
	Pose2D odometryOffset;
	bool isLeavingPossible;
	//bool isMotionStable;
	int executedSpecAction;/**< The SpecActionCmd currently executed*/
	
	SpecActionOutput():
	isLeavingPossible(true), executedSpecAction(SpecActionCmd::numSpecActions){}
};


#endif//__SpecActionOutput_h_