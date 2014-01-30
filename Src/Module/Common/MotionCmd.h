/**
* @file Module/Common/MotionCmd.h
*
* The file provider a class to reperetent the motion command.
*
* @author XU Tao
*/
#ifndef __MotionCmd_h_
#define __MotionCmd_h_
#include "Module/Common/WalkCmd.h"
#include "Module/Common/HeadCmd.h"
#include "Module/Common/SpecActionCmd.h"
class MotionCmd
{
public:
	enum MotionID
	{
		Stand,
		Walk,
		SpecialAction,
		numOfMotions
	};
	MotionID motion;
	WalkCmd walkCmd;
	SpecActionCmd specActionCmd;
	HeadCmd headCmd;
public:
	MotionCmd():motion(Stand){}
    /**
	* The function returns names of Motion ID.
	* @param motionID The Motion ID, the name of which is returned.
	* @return The corresponding name.
	*/
	static char* getMotionNameByID(int motionID)
	{
		switch(motionID)
		{
		case Stand: return "Stand"; break;
		case Walk: return "Walk"; break;
        case SpecialAction: return "SpecialAction"; break;
		default: return "Unknown Motion ID";
		}
	}
};

class MotionPlan : public MotionCmd{};
#endif
