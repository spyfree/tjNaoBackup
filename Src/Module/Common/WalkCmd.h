#ifndef __WalkCmd_h_
#define __WalkCmd_h_

#include "Tools/Math/Pose2D.h"

/**
* @class WalkCmd
* A class that represents a walk command.
*/
class WalkCmd
{
public:
	enum WalkType{
		eLIPMWalk ,
		eCartTableWalk,
		numWalkTypes
	};
	WalkType walkType;/**< The selected walk type. */
	Pose2D speed;/**< Requested walking speed,  in m/s and radians/s. */
	/**
	//static const double maxForwardSpeed;...
	*/
public:
	/*Default Constructor*/
	WalkCmd():walkType(eCartTableWalk), speed(0,0,0){}
	/**
	* The function returns names of WalkType.
	* @param walktype The walk type, the name of which is returned.
	* @return The corresponding name.
	*/
	static char* getWalkTypeName(int walktype)
	{
		switch(walktype)
		{
		case eLIPMWalk: return "LIPMWalk"; break;
		case eCartTableWalk: return "CartTableWalk"; break;
		default: return "Unknown WalkType";
		}
	}
	static WalkType getWalkTypeByName(const char* walkTypeName)
	{
        for(int i = 0; i < numWalkTypes; ++i)
        {
                if(strcmp(walkTypeName, getWalkTypeName(i))==0)
                    return (WalkType)i;
        }
        return numWalkTypes;
    }

};
#endif
