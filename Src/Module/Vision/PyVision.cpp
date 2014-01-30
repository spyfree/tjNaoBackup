#include "PyVision.h"
const static GoalPercept* goalPercept_reference=0;
const static BallPercept* ballPercept_reference=0;
const static RobotPercept* robotPercept_reference=0;
VisionModule::VisionModule()
{
	goalPercept = goalPercept_reference;
	ballPercept = ballPercept_reference;
	robotPercept = robotPercept_reference;
}
VisionModule::~VisionModule()
{


}

//==============================for Ball=================================
const int VisionModule::getBallCenterX()
{
	/*return visionInterface->frameNumber;*/
	return ballPercept->BallCenter.x;
}

const int VisionModule::getBallCenterY()
{
	return ballPercept->BallCenter.y;
}

const double VisionModule::getLocalX()
{
	return ballPercept->coordinateRobot.x;
}

const double VisionModule::getLocalY()
{
	return ballPercept->coordinateRobot.y;
}

const double VisionModule::getLocalZ()
{
	return ballPercept->coordinateRobot.z;
}

const double VisionModule::getpercentOfOrange()
{
	return ballPercept->percentOfOrange;
}

const double VisionModule::getradius()
{
	return ballPercept->radius;
}

const bool VisionModule::getConfidence()
{
	return ballPercept->invion;
}

//==============================for Goal=================================
const int VisionModule::getOppGoalCenterX()
{
	return goalPercept->MiddleOppPointImage.x;
}
const int VisionModule::getOppGoalCenterY()
{
	return goalPercept->MiddleOppPointImage.y;
}
const double VisionModule::getDistanceForOppoGoal()
{
	double Oppomiddlex;
	double Oppomiddley;

	if (goalPercept->posts[0].positionOnField.x == 0 || goalPercept->posts[1].positionOnField.x == 0)
	{
		Oppomiddlex = goalPercept->posts[0].positionOnField.x + goalPercept->posts[1].positionOnField.x;
		Oppomiddley = goalPercept->posts[0].positionOnField.y + goalPercept->posts[1].positionOnField.y;
	}
	else
	{
		Oppomiddlex = (goalPercept->posts[0].positionOnField.x + goalPercept->posts[1].positionOnField.x) / 2;
		Oppomiddley = (goalPercept->posts[0].positionOnField.y + goalPercept->posts[1].positionOnField.y) / 2;
	}
	return sqr(Oppomiddlex * Oppomiddlex + Oppomiddley * Oppomiddley);
}

const double VisionModule::getOpGoalLeftX()
{
	return goalPercept->posts[0].positionOnField.x;
}

const double VisionModule::getOpGoalRightX()
{
	return goalPercept->posts[1].positionOnField.x;
}

const double VisionModule::getDistanceForOwnGoal()
{
	double Ownmiddlex;
	double Ownmiddley;

	if (goalPercept->posts[2].positionOnField.x == 0 || goalPercept->posts[3].positionOnField.x == 0)
	{
		Ownmiddlex = goalPercept->posts[2].positionOnField.x + goalPercept->posts[3].positionOnField.x;
		Ownmiddley = goalPercept->posts[2].positionOnField.y + goalPercept->posts[3].positionOnField.y;
	}
	else
	{
		Ownmiddlex = (goalPercept->posts[2].positionOnField.x + goalPercept->posts[3].positionOnField.x) / 2;
		Ownmiddley = (goalPercept->posts[2].positionOnField.y + goalPercept->posts[3].positionOnField.y) / 2;
	}
	return sqr(Ownmiddlex * Ownmiddlex + Ownmiddley * Ownmiddley);

}

const bool VisionModule::getOwnGoalSeen()
{
	if (goalPercept->posts[2].perceptionType == 0 || goalPercept->posts[3].perceptionType ==0 )
		return true;
	else
		return false;
}

const bool VisionModule::getOppoGoalSeen()
{
	if(goalPercept->posts[0].perceptionType == 0 || goalPercept->posts[1].perceptionType == 0)
		return true;
	else
		return false;
}

const double VisionModule::getAngleForLeftPost()
{
	return goalPercept->LeftShotAngle.x;
}

const double VisionModule::getAngleForRightPost()
{
	return goalPercept->RightShotAngle.x;
}
const double VisionModule::getAngleForMiddleGoal()
{
	return goalPercept->MiddleShotAngle.x;
}
void set_vision_reference(const GoalPercept* _goalPercept,const BallPercept* _ballPercept,const RobotPercept* _robotPercept)
{
	goalPercept_reference = _goalPercept;
	ballPercept_reference = _ballPercept;
	robotPercept_reference = _robotPercept;
}
const double VisionModule::getAngleForOwnLeft()
{
	return goalPercept->LeftOwnAngle.x;
}
const double VisionModule::getAngleForOwnRight()
{
	return goalPercept->RightOwnAngle.x;
}
const bool VisionModule::getOwnLeftSeen()
{
	return goalPercept->posts[2].perceptionType ==0;
}
const bool VisionModule::getOwnRightSeen()
{
	return goalPercept->posts[3].perceptionType ==0;
}
const int VisionModule::getOwnGoalLeftInImage()
{
	return goalPercept->posts[2].positionInImage.x;
}
const int VisionModule::getOwnGoalRightInImage()
{
	return goalPercept->posts[3].positionInImage.y;
}
const double VisionModule::getDistOwnLeft()
{
	return sqrt(double(sqr(goalPercept->posts[2].positionOnField.x)+sqr(goalPercept->posts[2].positionOnField.y)));
}
const double VisionModule::getDistOwnRight()
{
	return sqrt(double(sqr(goalPercept->posts[3].positionOnField.x)+sqr(goalPercept->posts[3].positionOnField.y)));
}
const double VisionModule::getOwnLeftAngle()
{
	return atan2(double(goalPercept->posts[2].positionOnField.y),double(goalPercept->posts[2].positionOnField.x));
}
const double VisionModule::getOwnRightAngle()
{
	return atan2(double(goalPercept->posts[3].positionOnField.y),double(goalPercept->posts[3].positionOnField.x));
}
const bool VisionModule::getUnknowOppoPostSeen()
{
	return (goalPercept->unknownPosts[0].perceptionType ==0);
}
const double VisionModule::getUnknowOppoPostX()
{
	return goalPercept->unknownPosts[0].positionOnField.x;
}
const double VisionModule::getUnknowOppoPostY()
{
	return goalPercept->unknownPosts[0].positionOnField.y;
}

const bool VisionModule::getUnknowOwnPostSeen()
{
	return (goalPercept->unknownPosts[1].perceptionType==0);
}
const int VisionModule::getUnknowOwnPostXinImage()
{
	return goalPercept->unknownPosts[1].positionInImage.x;
}
const int VisionModule::getUnknowOwnPostYinImage()
{
	return goalPercept->unknownPosts[1].positionInImage.y;
}
const double VisionModule::getDistUnknowOwn()
{
	return sqrt(double(sqr(goalPercept->unknownPosts[1].positionOnField.x)+sqr(goalPercept->unknownPosts[1].positionOnField.y)));
}
const double VisionModule::getUnknowOwnAngle()
{
	return atan2(double(goalPercept->unknownPosts[1].positionOnField.y),double(goalPercept->unknownPosts[1].positionOnField.x));
}

//=============================for robot================================//

const int VisionModule::getLocRobotNums()
{
	return robotPercept->robots.size();
}
const int VisionModule::getUnLocRobotNums()
{
	return robotPercept->unlocalizableRobots.size();
}
const int VisionModule::getFirstRobotLocX()
{
	if (robotPercept->robots.size() == 0)
		return 0;
	else 
		return robotPercept->robots[0].relativePositionOnField.x;
}
const int VisionModule::getFirstRobotLocY()
{
	if (robotPercept->robots.size() == 0)
		return 0;
	else 
		return robotPercept->robots[0].relativePositionOnField.y;
}
const int VisionModule::getFirstRobotTeam()
{
	if(robotPercept->robots.size() == 0)
		return -1;
	else 
		return robotPercept->robots[0].team;
}
const int VisionModule::getSecondRobotLocX()
{
	if (robotPercept->robots.size() < 2 )
		return 0;
	else
		return robotPercept->robots[1].relativePositionOnField.x;
}
const int VisionModule::getSecondRobotLocY()
{
	if (robotPercept->robots.size() < 2 )
		return 0;
	else
		return robotPercept->robots[1].relativePositionOnField.y;
}
const int VisionModule::getSecondRobotTeam()
{
	if (robotPercept->robots.size() < 2 )
		return -1;
	else
		return robotPercept->robots[1].team;
}
const int VisionModule::getFirstUnknowTeam()
{
	if (robotPercept->unlocalizableRobots.size() == 0)
		return -1;
	else
		return robotPercept->unlocalizableRobots[0].team;
}
const int VisionModule::getSecondUnknowTeam()
{
	if (robotPercept->unlocalizableRobots.size() < 2)
		return -1;
	else
		return robotPercept->unlocalizableRobots[1].team;
}

BOOST_PYTHON_MODULE(PyVision)
{

	class_<VisionModule>("vision")
		.def("ballCenterX",&VisionModule::getBallCenterX)
		.def("ballCenterY",&VisionModule::getBallCenterY)
		.def("radius",&VisionModule::getradius)
		.def("ballX",&VisionModule::getLocalX)
		.def("ballY",&VisionModule::getLocalY)
		.def("ballZ",&VisionModule::getLocalZ)
		.def("ballPer",&VisionModule::getpercentOfOrange)
		.def("ballCf",&VisionModule::getConfidence)

		.def ("lPLocalA",&VisionModule::getAngleForLeftPost)
		.def ("RPLocalA",&VisionModule::getAngleForRightPost)
		.def ("MPLocalA",&VisionModule::getAngleForMiddleGoal)
		.def("distOppo",&VisionModule::getDistanceForOppoGoal)
		.def("cfOppo",&VisionModule::getOppoGoalSeen)
		.def("OpLeftGoalX",&VisionModule::getOpGoalLeftX)
		.def("OpRightGoalX",&VisionModule::getOpGoalRightX)

		.def("OpCenterX",&VisionModule::getOppGoalCenterX)
		.def("OpCenterY",&VisionModule::getOppGoalCenterY)


		.def("distOwn",&VisionModule::getDistanceForOwnGoal)
		.def("cfOwn",&VisionModule::getOwnGoalSeen)

		.def("leftAngleOwnGoal",&VisionModule::getAngleForOwnLeft)
		.def("rightAngleOwnGoal",&VisionModule::getAngleForOwnRight)
		.def("OwnLeftAngle",&VisionModule::getOwnLeftAngle)
		.def("OwnRightAngle",&VisionModule::getOwnRightAngle)
		.def("LeftOwnInImage",&VisionModule::getOwnGoalLeftInImage)
		.def("RightOwnInImage",&VisionModule::getOwnGoalRightInImage)
		.def("OwnLeftSeen",&VisionModule::getOwnLeftSeen)
		.def("OwnRightSeen",&VisionModule::getOwnRightSeen)
		.def("DistOwnLeft",&VisionModule::getDistOwnLeft)
		.def("DistOwnRight",&VisionModule::getDistOwnRight)


		.def("unknownOppoPostSeen",&VisionModule::getUnknowOppoPostSeen)
		.def("unknownOppoPostX",&VisionModule::getUnknowOppoPostX)
		.def("unknownOppoPostY",&VisionModule::getUnknowOppoPostY)
		.def("unknownOwnPostSeen",&VisionModule::getUnknowOwnPostSeen)
		.def("unknownOwnPostXinImage",&VisionModule::getUnknowOwnPostXinImage)
		.def("unknownOwnPostYinImage",&VisionModule::getUnknowOwnPostYinImage)
		.def("unknownDist",&VisionModule::getDistUnknowOwn)
		.def("unknownAngle",&VisionModule::getUnknowOwnAngle)

		.def("robotNum",&VisionModule::getLocRobotNums)
		.def("unknownNum",&VisionModule::getUnLocRobotNums)
		.def("FirstRobotLocX",&VisionModule::getFirstRobotLocX)
		.def("FirstRobotLocY",&VisionModule::getFirstRobotLocY)	
		.def("FirstRobotTeam",&VisionModule::getFirstRobotTeam) //0 for blue,1 for red,-1 for nothing
		.def("SecondRobotLocX",&VisionModule::getSecondRobotLocX)
		.def("SecondRobotLocY",&VisionModule::getSecondRobotLocY)
		.def("SecondRobotTeam",&VisionModule::getSecondRobotTeam)
		.def("FirstUnknowTeam",&VisionModule::getFirstUnknowTeam)
		.def("SecondUnknowTeam",&VisionModule::getSecondUnknowTeam)
		;
}
void c_init_Vision()
{
	if (!Py_IsInitialized())
	{
		Py_Initialize();
	}
	try {
		Py_Initialize();
		initPyVision();
	} catch (error_already_set) {
		PyErr_Print();
	}
}



















