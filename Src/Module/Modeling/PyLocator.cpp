#include "PyLocator.h"
#include <iostream>

const static RobotPose* robotPose_reference = 0;
const static BallModel* ballModel_reference = 0;
const static FreePartOfOpponentGoalModel* freePartOfOpponentGoalModel_reference = 0;
const static OdometryData* OdometryData_reference = 0;

LocatorModule::LocatorModule()
{
	robotPose = robotPose_reference;
	ballModel = ballModel_reference;
	freePartOfOpponentGoalModel = freePartOfOpponentGoalModel_reference;
	odometryData = OdometryData_reference;
	lastOdometryData = Pose2D(0,0,0);
}

LocatorModule::~LocatorModule()
{

}

const double LocatorModule::getGlobalRobotX()
{
	return robotPose->translation.x;
}

const double LocatorModule::getGlobalRobotY()
{
	return robotPose->translation.y;
}

const double LocatorModule::getGlobalRobotR()
{
	return robotPose->rotation;
}

const double LocatorModule::getRobotPoseValidity()
{
	return robotPose->validity;
}
const double LocatorModule::getRobotPoseDeviation()
{
	return robotPose->deviation;
}

const double LocatorModule::getLocalBallPX()
{
	return ballModel->estimate.position.x;
}

const double LocatorModule::getLocalBallPY()
{
	return ballModel->estimate.position.y;
}

const double LocatorModule::getLocalBallVX()
{
	return ballModel->estimate.velocity.x;
}

const double LocatorModule::getLocalBallVY()
{
	return ballModel->estimate.velocity.y;
}

const double LocatorModule::getGlobalBallPX()
{
	return ballModel->estimateGlobe.position.x;
}

const double LocatorModule::getGlobalBallPY()
{
	return ballModel->estimateGlobe.position.y;
}

const double LocatorModule::getBallSqureLength()
{
	return ballModel->ballsquare.length;

}

const double LocatorModule::getBallSqureWidth()
{
	return ballModel->ballsquare.width;
}

const double LocatorModule::getBallSqureX()
{
	return ballModel->ballsquare.LeftUpPosition.x;
}
const double LocatorModule::getBallSqureY()
{
	return ballModel->ballsquare.LeftUpPosition.y;
}
const double LocatorModule::getGoalieStandSpotX()
{
	return ballModel->goalieStandSpot.x;
}

const double LocatorModule::getGoalieStandSpotY()
{
	return ballModel->goalieStandSpot.y;
}

const double LocatorModule::getFree()
{
	return freePartOfOpponentGoalModel->IsFree;
}
const double LocatorModule::getLeftEnd()
{
	return freePartOfOpponentGoalModel->LeftEnd.y;
	//double x(700);
	//return x;
}
const double LocatorModule::getRightEnd()
{
	return freePartOfOpponentGoalModel->RightEnd.y;
	//double x(-700);
	//return x;
}
const double LocatorModule::getCrossX()
{
	return robotPose->Cross.x;
}

const double LocatorModule::getCrossY()
{
	return robotPose->Cross.y;
}
const boost::python::tuple LocatorModule::getOdometryData()
{
	Pose2D odomeOffset = *odometryData - lastOdometryData;
	lastOdometryData = *odometryData;
	boost::python::tuple Data;
	Data[0]=odomeOffset.rotation;
	Data[1]=odomeOffset.translation.x;
	Data[2]=odomeOffset.translation.y;
	return Data;
}

void set_locator_reference(const RobotPose* _robotPose,const BallModel* _ballModel,const FreePartOfOpponentGoalModel*_freePartOfOpponentGoalModel,const OdometryData*_OdometryData)
{
	robotPose_reference = _robotPose;
	ballModel_reference = _ballModel;
	freePartOfOpponentGoalModel_reference = _freePartOfOpponentGoalModel;
	OdometryData_reference = _OdometryData;

}

BOOST_PYTHON_MODULE(PyLocator)
{
	class_<LocatorModule>("locator")
		.add_property("x",&LocatorModule::getGlobalRobotX)
		.add_property("y",&LocatorModule::getGlobalRobotY)
		.add_property("h",&LocatorModule::getGlobalRobotR)
		.add_property("RPValid",&LocatorModule::getRobotPoseValidity)
		.add_property("RPDev",&LocatorModule::getRobotPoseDeviation)

		.add_property("ballX",&LocatorModule::getLocalBallPX)
		.add_property("ballY",&LocatorModule::getLocalBallPY)
		.add_property("ballVelX",&LocatorModule::getLocalBallVX)
		.add_property("ballVelY",&LocatorModule::getLocalBallVY)
		.add_property("ballGX",&LocatorModule::getGlobalBallPX)
		.add_property("ballGY",&LocatorModule::getGlobalBallPY)
		.add_property("goalieSpotX",&LocatorModule::getGoalieStandSpotX)
		.add_property("goalieSpotY",&LocatorModule::getGoalieStandSpotY)
		.add_property("peakX",&LocatorModule::getBallSqureX)
		.add_property("peakY",&LocatorModule::getBallSqureY)
		.add_property("cellX",&LocatorModule::getBallSqureLength)
		.add_property("cellY",&LocatorModule::getBallSqureWidth)


		.add_property("Isfree",&LocatorModule::getFree)
		.add_property("LeftEnd",&LocatorModule::getLeftEnd)
		.add_property("RightEnd",&LocatorModule::getRightEnd)
		.add_property("OdometryData",&LocatorModule::getOdometryData)
		.add_property("CrossX",&LocatorModule::getCrossX)
		.add_property("CrossY",&LocatorModule::getCrossY)

		;
}

void c_init_locator()
{
	if (!Py_IsInitialized())
	{
		Py_Initialize();
	}
	try
	{
		initPyLocator();
	}
	catch (error_already_set)
	{
		PyErr_Print();
	}
}
