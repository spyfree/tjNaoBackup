#ifndef PyLocator_H
#define PyLocator_H
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Representations/Modeling/Freepartofopponentgoalmodel.h"
#include "Module/Representations/Modeling/BallModel.h"
#include "Module/Sensor/OdometryDataProvider.h"
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>
using namespace std;
using namespace boost::python;
using namespace boost;

class LocatorModule
{
public:
	LocatorModule();
	~LocatorModule();

	const double getRobotPoseValidity();
	const double getGlobalRobotX();
	const double getGlobalRobotY();
	const double getGlobalRobotR();
	const double getRobotPoseDeviation();

	const double getFree();
	const double getLeftEnd();
	const double getRightEnd();

	const double getLocalBallPX();
	const double getLocalBallPY();
	const double getLocalBallVX();
	const double getLocalBallVY();
	const double getGlobalBallPX();
	const double getGlobalBallPY();
	const double getBallSqureX();
	const double getBallSqureY();
	const double getBallSqureLength();
	const double getBallSqureWidth();
	const double getGoalieStandSpotX();
	const double getGoalieStandSpotY();
    const double getCrossX();
	const double getCrossY();
	const  boost::python::tuple getOdometryData();

private:
	const RobotPose* robotPose;
	const FreePartOfOpponentGoalModel* freePartOfOpponentGoalModel;
	const BallModel* ballModel;
	const OdometryData*odometryData;
	Pose2D lastOdometryData;

};

/*void set_locator_reference(const RobotPose* _robotPose);*/
void set_locator_reference(const RobotPose* _robotPose,const BallModel* _ballModel,const FreePartOfOpponentGoalModel* _freePartOfOpponentGoalModel,const OdometryData*_OdometryData);
void c_init_locator();
#endif
