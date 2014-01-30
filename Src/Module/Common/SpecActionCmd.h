#ifndef __SpecActionCmd_h_
#define __SpecActionCmd_h_

/**
* @class SpecActionCmd
* The class represents special actions command
*/
class SpecActionCmd
{
public:
	SpecActionCmd() : specAction(Stand){}
public:
	/**ID for all special actions*/
	enum SpecActionID
	{
		playDead,
		Stand,
		StandUpBack,
		StandUpBackNao,
		StandUpFront,
		StandUpFrontNao,
		grandPaLeft,
		kickLeftNao,
		kickRightNao,
		kickSideLeftNao,
		kickSideRightNao,
		lookAtFloorInFront,
		lookStraightInFront,
		demoJesus,
		demoUpright,
		NewSitDown,
		NewAKeeperSitDown,
		NewAKeeperGetUp,
		numSpecActions

	};

	int specAction;
public:
	/**
	* The function returns names for special action ids.
	* @param id The special action id the name of which is returned.
	* @return The corresponding name.
	*/
	static const char* getSpecActionName(int id)
	{
		switch(id)
		{
		case playDead: return "playDead";
		case Stand: return "Stand";
		case StandUpBack: return "StandUpBack";
		case StandUpBackNao: return "StandUpBackNao";
		case StandUpFront: return "StandUpFront";
		case StandUpFrontNao: return "StandUpFrontNao";
		case kickLeftNao: return "kickLeftNao";
		case kickRightNao: return "kickRightNao";
		case kickSideLeftNao: return "kickSideLeftNao";
		case kickSideRightNao: return "kickSideRightNao";
		case grandPaLeft: return "grandPaLeft";
		case lookAtFloorInFront: return "lookAtFloorInFront";
		case lookStraightInFront: return "lookStraightInFront";
		case demoJesus: return "demoJesus";
		case demoUpright: return "demoUpright";
		case NewSitDown: return "NewSitDown";
		case NewAKeeperSitDown: return "NewAKeeperSitDown";
		case NewAKeeperGetUp: return "NewAKeeperGetUp";
		default: return "unknown";
		}
	}
	/**
	* The function searches the id for a special action name.
	* @param name The name of the special action.
	* @return The corresponding id if found, or numOfSpecialActions if not found.
	*/
	static SpecActionID getSpecActionIDbyName(const char* name)
	{
		for(int i = 0; i < numSpecActions; ++i)
		{
			if(strcmp(name, getSpecActionName(i)) == 0)
				return SpecActionID(i);

		}
		return numSpecActions;
	}
};

#endif //__SpecActionCmd_h_
