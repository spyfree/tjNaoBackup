#include "RoboCupCtrl.h"
#include "Simulation/SimObject.h"
#include "Simulation/Simulation.h"
#include "Tools/Surface.h"
#include "LocalRobot.h"
#include "LocalPart.h"
#include "SimFTimer.h"
#include <iostream>
#include "Module/Actuator/MatlabHelp/MatlabHelp.h"//x

RoboCupCtrl* RoboCupCtrl::controller = 0;
std::string RoboCupCtrl::robotName;
std::list<std::string> RoboCupCtrl::robotNamesList;
int RoboCupCtrl::currentRobotNum = 1;
bool RoboCupCtrl::goToInitialStateFlag = false;

CONNECT_CONTROLLER_TO_SCENE(RoboCupCtrl, "RoboCup");

RoboCupCtrl::RoboCupCtrl():pLocalRobot(0),myPart(0),fallDownFrames(0), startToFall(false), fallDown(false)
{
	controller = this;
	robot = 0;
	SimObject* robotGroup_Group = getObjectReference("RoboCup.robots");
	if(robotGroup_Group)
	{
		for(unsigned currentRobot = 0; currentRobot < robotGroup_Group->getNumberOfChildNodes(); ++ currentRobot)
		{
			robot = robotGroup_Group->getChildNode(currentRobot);
			robotNamesList.push_back(robot->getFullName());
			robots.push_back( new LocalRobot(robot) );
		}
		//@todo  这三个就是权宜之计，因为一旦改动，牵扯太大
		pLocalRobot = (*  robots.begin() );
		robot = pLocalRobot->objOfThisRobot;
		robotName = pLocalRobot->objOfThisRobot->getFullName();
		//for(std::list<LocalPart*>::iterator i = parts.begin(); i != parts.end(); ++i)
		//	(*i)->update();
	}

	//x   set SimObject <partOfRobot>
	SimObject* partOfRobot_Group = getObjectReference("RoboCup.partOfRobots");
	if( partOfRobot_Group)
	{
		for(unsigned currentPart = 0; currentPart < partOfRobot_Group->getNumberOfChildNodes(); ++currentPart)
		{
			partOfRobot = partOfRobot_Group->getChildNode(currentPart);
			parts.push_back( new LocalPart(partOfRobot) );
		}
	}

	//balls	
	if(getObjectReference("RoboCup.robots")->getNumberOfChildNodes())
		ball = getObjectReference("RoboCup.balls.ball");
	else
		ball = 0;

	setGUIUpdateDelay(100);
	SIMFTIMER_INIT();
}
RoboCupCtrl::~RoboCupCtrl()
{
	stop();
}
void RoboCupCtrl::start()
{
	//SIMFTIMER_SET(SimFTimer::tImpulseRobot, 200);
}
void RoboCupCtrl::stop()
{
	if(pLocalRobot)
	{
		delete pLocalRobot;
		pLocalRobot = 0;
	}

	for(std::list<LocalPart*>::iterator i = parts.begin(); i != parts.end(); ++i)
	{
		if(*i)	
		{
			delete (*i);
			(*i) = 0;
		}
	}

	controller = 0;
}
void RoboCupCtrl::execute()
{
	if(currentRobotNum<=robots.size()  & currentRobotNum>=1)
	{
		std::list<LocalRobot*>::iterator robotIte = robots.begin();		
		for (int i = 1;i<currentRobotNum;i++)
		{
			robotIte++;
		}

		if (	pLocalRobot != ( *robotIte )		)
		{
			pLocalRobot = ( * robotIte );
			robot = pLocalRobot->objOfThisRobot;
			robotName = pLocalRobot->objOfThisRobot->getFullName();
		}
	}

	// TODO: update joitns date...
	if(pLocalRobot){
		//====Process RobotState ========//
		//processRobotState();
		if(goToInitialStateFlag)
		{
			//	Vector3d 	lu =	robot->getPosition();
			// 	Matrix3d hu = robot->getRotation();
			simulation.moveObject(robot, Vector3d(-0.3,0,0.32738654494285579), Vector3d(0,0,0));
			goToInitialStateFlag = false;
		}
		//====Execute Robot Control====//
		pLocalRobot->update();
	}


	for(std::list<LocalPart*>::iterator i = parts.begin(); i != parts.end(); ++i)
	{
		(*i)->update();


		//SensorData temp;
		//temp=(*i)->getSensorData();
		//@todo:1、将物体的名字加到matlab的变量名里，这样好区分  2、LocalPart里的sensor的port改成用查表的方式
		//MATLABHELP_PLOT(temp.data[SensorData::fsrRFL],"myluan");
		//MATLABHELP_PLOT(temp.data[SensorData::fsrLFL],"myshui");
	}

	SIMFTIMER_COUNTDOWN();

}
Pose2D RoboCupCtrl::getPose2D(SimObject* obj)
{
	double offset = obj->getNumberOfChildNodes() && obj->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
	return Pose2D(normalize(obj->getRotation().getZAngle() + offset),
		Vector2<double>(obj->getPosition().v[0], obj->getPosition().v[1]));
}

/*
double RoboCupCtrl::checkRobotFall(SimObject* obj)
{
Vector3d up(0,0,1);
up = obj->getRotation() * up;
return up.v[2];
}

void RoboCupCtrl::processRobotState()
{
Matrix3d curRot;
Vector3d curPos;
// 	double ax, ay, az;
// 	double ax2, ay2, az2;
// 	SimObject* body = simulation.getObjectReference(RoboCupCtrl::getRobotFullName()+".body2");
// 	ax = robot->getRotation().getXAngle();
// 	ay = robot->getRotation().getYAngle();
// 	az = robot->getRotation().getZAngle();

// 	std::cout<<"==>>"<<ax<<" "<<ay<<" "<<az<<std::endl;

// 	ax2 = body->getRotation().getXAngle();
// 	ay2 = body->getRotation().getYAngle();
//az2 = body->getRotation().getZAngle();
//====Reset Robot If Fall======
double f = checkRobotFall(robot); 
if( f < 0.3 && !fallDown){
fallDown = true;
startToFall = false;
pLocalRobot->setFallState(fallDown, fallVect.v[0],fallVect.v[1],fallDownFrames * 20);
pLocalRobot->setUpdateModleFlag(true);
pLocalRobot->setRobotRecoverd(false);
fallDownFrames = 0;
if(!SIMFTIMER_ISACTIVE(SimFTimer::tDoRecover)){
SIMFTIMER_RESETALL();
SIMFTIMER_SET(SimFTimer::tDoRecover, 200);
}
std::cout<<"--------------Fall Donw-------Score Robot---"<<std::endl;
}else if(f < 0.9 && !startToFall && !fallDown)
{
std::cout<<"========Starting to Fall============="<<std::endl;
startFallRot = robot->getRotation();//recode start fall world rotation.
startFallPos = robot->getPosition();//recode start fall world position.
startToFall = true;
fallDownFrames = 0;
}else if(f < 0.9 && startToFall)
{
//=====calculate falldown critia============
curPos = robot->getPosition();
Vector3d tmp = curPos - startFallPos;
Matrix3d rot;
rot.setZRotation(-(startFallRot.getZAngle()-pi/2));
tmp = rot * tmp;//transfer to robot coordinate of starting falling state.
std::cout<<tmp.v[0]<<" "<<tmp.v[1]<<std::endl;
fallVect = tmp;
fallVect.v[2] = 0;
fallDownFrames ++;
}
if(SIMFTIMER_CHECK(SimFTimer::tDoRecover))
{
double offset = robot->getNumberOfChildNodes() && robot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
simulation.moveObject(robot, Vector3d(0,0,0.3), Vector3d(0,0,pi/2));
simulation.setObjectAngularVelocity(robot, 0,0,0);
SIMFTIMER_SET(SimFTimer::tNoticeRecovered, 100);
SIMFTIMER_RESET(SimFTimer::tDoRecover);
std::cout<<"Recover Robot"<<std::endl;
}
if(SIMFTIMER_CHECK(SimFTimer::tNoticeRecovered))
{
//set recovered signal to actuator
fallDown = false;
pLocalRobot->setFallState(fallDown);
pLocalRobot->setRobotRecoverd(true);
pLocalRobot->setUpdateModleFlag(false);
SIMFTIMER_SET(SimFTimer::tImpulseRobot, 200);
SIMFTIMER_RESET(SimFTimer::tNoticeRecovered);
std::cout<<"Robot Recovered"<<std::endl;
}
if(SIMFTIMER_CHECK(SimFTimer::tImpulseRobot))
{
//shoot ball to robot
randomShootBall();
SIMFTIMER_SET(SimFTimer::tScoreRobot, 200);
SIMFTIMER_RESET(SimFTimer::tImpulseRobot);
std::cout<<"Shoot Random Ball"<<std::endl;
}
if(SIMFTIMER_CHECK(SimFTimer::tScoreRobot))
{
pLocalRobot->setFallState(false, 0,0,0);
pLocalRobot->setUpdateModleFlag(true);
//score robot and set score
SIMFTIMER_RESETALL();
SIMFTIMER_SET(SimFTimer::tDoRecover, 200);
std::cout<<"NoFall, Score Robot"<<std::endl;
}
}

void RoboCupCtrl::randomShootBall()
{
SimObject* objectBall = simulation.getObjectReference("RoboCup.balls.ball");
SimObject* objectRobot = simulation.getObjectReference(RoboCupCtrl::getRobotFullName());
if(objectBall){
Vector3d direction(randomDouble()-0.5, randomDouble()-0.5, 0);
direction.normalize();
//simulation->resetDynamicsForObject(objectBall);
double offset = objectRobot->getNumberOfChildNodes() && objectRobot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
Vector3d rPos = objectRobot->getPosition();
Matrix3d rRot = objectRobot->getRotation();
rRot.rotateZ(offset);
Vector3d startPos = rPos +rRot * (Vector3d(0,0,0.14) - direction);
Vector3d vBall = rRot * direction * 9;
simulation.setObjectLinearVelocity(objectBall, 0, 0, 0);
simulation.setObjectAngularVelocity(objectBall, 0, 0, 0);
simulation.moveObject(objectBall, startPos);
simulation.setObjectLinearVelocity(objectBall, vBall.v[0], vBall.v[1], 0);

}
}
*/

double RoboCupCtrl::checkRobotFall(SimObject* obj)
{
	Vector3d up(0,0,1);
	up = obj->getRotation() * up;
	return up.v[2];
}

void RoboCupCtrl::processRobotState()
{
	Matrix3d curRot;
	Vector3d curPos;
	// 	double ax, ay, az;
	// 	double ax2, ay2, az2;
	// 	SimObject* body = simulation.getObjectReference(RoboCupCtrl::getRobotFullName()+".body2");
	// 	ax = robot->getRotation().getXAngle();
	// 	ay = robot->getRotation().getYAngle();
	// 	az = robot->getRotation().getZAngle();

	// 	std::cout<<"==>>"<<ax<<" "<<ay<<" "<<az<<std::endl;

	// 	ax2 = body->getRotation().getXAngle();
	// 	ay2 = body->getRotation().getYAngle();
	//az2 = body->getRotation().getZAngle();
	//====Reset Robot If Fall======
	double f = checkRobotFall(robot); 
	if( f < 0.3 && !fallDown){
		fallDown = true;
		startToFall = false;
		pLocalRobot->setFallState(fallDown, fallVect.v[0],fallVect.v[1],fallDownFrames * 20);
		pLocalRobot->setUpdateModleFlag(true);
		pLocalRobot->setRobotRecoverd(false);
		fallDownFrames = 0;
		if(!SIMFTIMER_ISACTIVE(SimFTimer::tDoRecover)){
			SIMFTIMER_RESETALL();
			SIMFTIMER_SET(SimFTimer::tDoRecover, 300);
		}
		std::cout<<"--------------Fall Donw-------Score Robot---"<<std::endl;
	}else if(f < 0.9 && !startToFall && !fallDown)
	{
		std::cout<<"========Starting to Fall============="<<std::endl;
		startFallRot = robot->getRotation();//recode start fall world rotation.
		startFallPos = robot->getPosition();//recode start fall world position.
		startToFall = true;
		fallDownFrames = 0;
	}else if(f < 0.9 && startToFall)
	{
		//=====calculate falldown critia============
		curPos = robot->getPosition();
		Vector3d tmp = curPos - startFallPos;
		Matrix3d rot;
		rot.setZRotation(-(startFallRot.getZAngle()-pi/2));
		tmp = rot * tmp;//transfer to robot coordinate of starting falling state.
		//std::cout<<tmp.v[0]<<" "<<tmp.v[1]<<std::endl;
		fallVect = tmp;
		fallVect.v[2] = 0;
		fallDownFrames ++;
	}
	if(SIMFTIMER_CHECK(SimFTimer::tDoRecover))
	{
		double offset = robot->getNumberOfChildNodes() && robot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
		simulation.moveObject(robot, Vector3d(0,0,0.5), Vector3d(0,0,pi/2));
		simulation.setObjectAngularVelocity(robot, 0,0,0);
		SIMFTIMER_SET(SimFTimer::tNoticeRecovered, 200);
		SIMFTIMER_RESET(SimFTimer::tDoRecover);
		//SystemCall::sleep(100);
		std::cout<<"Recover Robot"<<std::endl;
	}
	if(SIMFTIMER_CHECK(SimFTimer::tNoticeRecovered))
	{
		//set recovered signal to actuator
		if(f > 0.6){
			fallDown = false;
			pLocalRobot->setFallState(fallDown);
			pLocalRobot->setRobotRecoverd(true);
			pLocalRobot->setUpdateModleFlag(false);
			SIMFTIMER_SET(SimFTimer::tImpulseRobot, 200);
			SIMFTIMER_RESET(SimFTimer::tNoticeRecovered);
			std::cout<<"Robot Recovered"<<std::endl;
		}else
		{	
			SIMFTIMER_RESET(SimFTimer::tNoticeRecovered);
			SIMFTIMER_SET(SimFTimer::tDoRecover, 300);
		}
	}
	if(SIMFTIMER_CHECK(SimFTimer::tImpulseRobot))
	{
		//shoot ball to robot
		//randomShootBall();
		//SIMFTIMER_SET(SimFTimer::tScoreRobot, 400);
		SIMFTIMER_RESET(SimFTimer::tImpulseRobot);
		std::cout<<"Shoot Random Ball"<<std::endl;
	}
	if(SIMFTIMER_CHECK(SimFTimer::tScoreRobot))
	{
		pLocalRobot->setFallState(false, 0,0,0);
		pLocalRobot->setUpdateModleFlag(true);
		//score robot and set score
		SIMFTIMER_RESETALL();
		SIMFTIMER_SET(SimFTimer::tDoRecover, 200);
		std::cout<<"NoFall, Score Robot"<<std::endl;
	}
}

void RoboCupCtrl::randomShootBall()
{
	SimObject* objectBall = simulation.getObjectReference("RoboCup.balls.ball");
	SimObject* objectRobot = simulation.getObjectReference(RoboCupCtrl::getRobotFullName());
	if(objectBall){
		Vector3d direction(randomDouble()-0.5, randomDouble()-0.5, 0);
		direction.normalize();
		//simulation->resetDynamicsForObject(objectBall);
		double offset = objectRobot->getNumberOfChildNodes() && objectRobot->getChildNode(0)->getName() != "CM5Box" ? -pi_2 : 0;
		Vector3d rPos = objectRobot->getPosition();
		Matrix3d rRot = objectRobot->getRotation();
		rRot.rotateZ(offset);
		Vector3d startPos = rPos +rRot * (Vector3d(0,0,0.18) - direction);
		Vector3d vBall = rRot * direction * 9*sqrt(2.0);
		simulation.setObjectLinearVelocity(objectBall, 0, 0, 0);
		simulation.setObjectAngularVelocity(objectBall, 0, 0, 0);
		simulation.moveObject(objectBall, startPos);
		simulation.setObjectLinearVelocity(objectBall, vBall.v[0], vBall.v[1], 0);

	}
}
