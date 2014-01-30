#ifndef __RoboCupCtrl_h_
#define __RoboCupCtrl_h_
#include "Simulation/ODESimulation/SimulationCore/Controller/Controller.h"
#include "Tools/Math/Pose2D.h"
#include "Simulation/ODESimulation/SimulationCore/Tools/SimMath.h"
#include <list>
#include "Simulation/saTools/SaWidget.h"


class LocalRobot;
class LocalPart;

class RoboCupCtrl : public Controller
{
private:
	static RoboCupCtrl* controller;
protected:
	static std::string robotName;
public:
	static bool goToInitialStateFlag;

public:
	LocalRobot * getPLocalRobot(){return pLocalRobot;}
public:
	static int currentRobotNum;
protected:	
	LocalRobot* pLocalRobot;
	LocalPart*    myPart; //x

	std::list<LocalRobot*>robots;//x
	std::list<LocalPart*> parts;	//x

public:
	static std::list<std::string>robotNamesList;//x

	SimObject* ball;
	//	std::list<Robot*> robots; /**< The list of all robots. */
	SimObject* robot;
	SimObject* partOfRobot;//x
	// 	bool simTime;
	// 	bool dragTime;
	// 	unsigned lastTime;
	// 	std::string statusText;
	// 	int joystickID;
	void start();
	void stop();
public:
	RoboCupCtrl();
	~RoboCupCtrl();
	void execute();
	static RoboCupCtrl* getController() { 
		if(!controller){
			new RoboCupCtrl();
		}
		return controller; 
	}
	static std::string& getRobotFullName() {return robotName; }


	Pose2D getPose2D(SimObject* obj);
	//void broadcastPackage(const char* package, unsigned size);
	//int getNextJoystickID() { return joystickID ++;}
private:
	double checkRobotFall(SimObject* obj);
	void processRobotState();
	void randomShootBall();
	int fallDownFrames;//The Frames fall down last
	Matrix3d startFallRot;
	Vector3d startFallPos;
	Vector3d fallVect;
	bool startToFall, fallDown;
};
#endif