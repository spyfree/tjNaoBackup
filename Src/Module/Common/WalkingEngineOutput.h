#ifndef __WalkingEngineOutput_H__
#define __WalkingEngineOutput_H__

#include "Module/Common/JointData.h"
#include "Tools/Math/Pose2D.h"

/**
* @class WalkingEnigeOutput
* A class that represents the output of the walking engine.
*/
class WalkingEngineOutput : public JointCmd
{
protected:
	virtual void serialize(In* in, Out* out)
	{  
		//STREAM_REGISTER_BEGIN();
		STREAM_BASE(JointCmd);
		STREAM(speed);
		STREAM(odometryOffset);
		//STREAM(isLeavingPossible);
		STREAM(ratioInWalkCycle);
		//STREAM_REGISTER_FINISH();
	}

public:
	Pose2D speed, /**< The current walking speed, m/s, radian/s*/
		odometryOffset; /**< The body motion feedback in this step, for localization mainly. */
	bool isLeavingPossible; /**< Is leaving the motion module possible now? */
	double ratioInWalkCycle; /**< The current ratio in the walk cycle in the range [0..1[. */

	/** 
	* Default constructor.
	*/
	WalkingEngineOutput() : ratioInWalkCycle(0), isLeavingPossible(true) {}
};

#endif // __WalkingEngineOutput_H__
