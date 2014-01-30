#ifndef PYVISION
#define PYVISION
#include "Module/Representations/Perception/GoalPercetp.h"
#include "Module/Representations/Perception/BallPercept.h"
#include "Module/Representations/Perception/RobotPercept.h"
#include <boost/python.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;
class VisionModule
{
public:
	VisionModule();
	~VisionModule();

	const int getBallCenterX();
	const int getBallCenterY();
	const double getLocalX();
	const double getLocalY();
	const double getLocalZ();
	const double getradius();
	const double getpercentOfOrange();
	const bool getConfidence();


	const double getAngleForLeftPost();
	const double getAngleForRightPost();
	const double getAngleForMiddleGoal();
	const double getDistanceForOppoGoal();
	const bool getOppoGoalSeen();
	const double getOpGoalLeftX();
	const double getOpGoalRightX();

	const int getOppGoalCenterX();
	const int getOppGoalCenterY();
	
	const double getDistanceForOwnGoal();
	const bool getOwnGoalSeen();
	const double getAngleForOwnLeft();
	const double getAngleForOwnRight();
	const bool getOwnLeftSeen();
	const bool getOwnRightSeen();
	const double getDistOwnLeft();
	const double getDistOwnRight();
	const int getOwnGoalLeftInImage();
	const int getOwnGoalRightInImage();
	const double getOwnLeftAngle();
	const double getOwnRightAngle();

	const bool getUnknowOppoPostSeen();
	const double getUnknowOppoPostX();
	const double getUnknowOppoPostY();

	const bool getUnknowOwnPostSeen();
	const double getDistUnknowOwn();
	const int getUnknowOwnPostXinImage();
	const int getUnknowOwnPostYinImage();
	const double getUnknowOwnAngle();

	const int getLocRobotNums();
	const int getUnLocRobotNums();

	const int getFirstRobotLocX();
	const int getFirstRobotLocY();
	const int getFirstRobotTeam();

	const int getSecondRobotLocX();
	const int getSecondRobotLocY();
	const int getSecondRobotTeam();

	const int getFirstUnknowTeam();
	const int getSecondUnknowTeam();


private:
	const GoalPercept* goalPercept;
	const BallPercept* ballPercept;
	const RobotPercept* robotPercept;
};

void set_vision_reference(const GoalPercept* _goalPercept,const BallPercept* _ballPercept,const RobotPercept* _robotPercept);
void c_init_Vision();

#endif
