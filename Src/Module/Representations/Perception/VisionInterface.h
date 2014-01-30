#ifndef __VisionInterface_h_
#define __VisionInterface_h_
#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include"Tools/Math/Common.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/LinesPercept.h"
//#include "Module/common/VisionHeader.h"
#include <vector>

	//这些最好在外面一点的地方写
// What is the certainty associated with the identification of the object
enum postidCertainty {
	NOT_SURE,
	MILDLY_SURE,
	_SURE
};

// Are the heights and widths from Vision to be trusted? Used in localization
enum postdistanceCertainty {
	BOTH_UNSURE,
	WIDTH_UNSURE,
	HEIGHT_UNSURE,
	BOTH_SURE
};


class netVisionInterface
{
public:
	uint8 ownleftgoalSeen;
	uint8 ownrightgoalSeen;
	uint8 unknowowngoalSeen;
	uint8 oppleftgoalSeen;
	uint8 opprightgoalSeen;
	uint8 unknowoppgoalSeen;



	int32 ownleftGoalX;
	int32 ownleftGoalY;
	int32 ownrightGoalX;
	int32 ownrightGoalY;
	int32 unknowownGoalX;
	int32 unknowownGoalY;


	int32 oppleftGoalX;
	int32 oppleftGoalY;
	int32 opprightGoalX;
	int32 opprightGoalY;
	int32 unknowoppGoalX;
	int32 unknowoppGoalY;

	int32 horizonStartX;
	int32 horizonStartY ;
	int32 horizonEndX ;
	int32 horizonEndY ;
	uint8 HorizenInImage;

	uint8 ballSeen;
	int32 ballx;
	int32 bally;
	int32 balldis;
    
	uint8 CircleSeen;
	int32 CircleCenterX;
	int32 CircleCenterY;

	int32 yaw;
	int32 pitch;
	int32 row;

	int32 camX;
	int32 camY;
	int32 camZ;


};
// class VisionInterface
// {
// public:
// 	 BallPercept ball;
// 	LinesPercept linesPercept;
//      GoalPercept  goalpercept;
// 	//GoalPercept goalperceptY;//
// 	//GoalPercept goalperceptB;//只需保留一个即可
// 	unsigned long frameNumber; /**< The frame number when perceived. */
// 
// 	VisionInterface();
// 	void reset(unsigned long frameNumber);
// };

#endif //__VisionInterface_h_
