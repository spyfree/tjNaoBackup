#include "PyMotion.h"


static MotionCmd* interface_reference = 0;
class PyWalkCmd
{
public:
	PyWalkCmd(int motionID,int walktype, boost::python::tuple speed)
	{
		_motionID = static_cast<MotionCmd::MotionID>(motionID);
		_walktype = static_cast<WalkCmd::WalkType>(walktype);
		speedVector.rotation = extract<float>(speed[2]);
		speedVector.translation.x = extract<float>(speed[0]);
		speedVector.translation.y = extract<float>(speed[1]);
	}
	double getLeft(){return speedVector.translation.x;}
	double getForward(){return speedVector.translation.y;}
	double getTurnCCW(){return speedVector.rotation;}
	WalkCmd::WalkType getWalktype(){return _walktype;}
	MotionCmd::MotionID getMotionID(){return _motionID;}
private:
	Pose2D speedVector;
	WalkCmd::WalkType _walktype;
	MotionCmd::MotionID _motionID;
};

class PyHeadCmd
{
public:
	PyHeadCmd(double pan,double tilt,double speed)
	{
		_pan = pan;
		_tilt = tilt;
		_speed = speed;
	}
	double getPan(){return _pan;}
	double getTilt(){return _tilt;}
	double getSpeed(){return _speed;}
private:
	double _pan;
	double _tilt;
	double _speed;
};

class PySpecialAction
{
public:
	PySpecialAction(int motionID,int specialAction)
	{
		_motionID = static_cast<MotionCmd::MotionID>(motionID);
		_specialAction = specialAction;
	}
	MotionCmd::MotionID getMotionID(){return _motionID;}
	int getSpecialAction(){return _specialAction;}
private:
	int _specialAction;
	MotionCmd::MotionID _motionID;
};

class PyMotionInterface
{
public:
	PyMotionInterface()
	{
		motionCmd = interface_reference;
	}
	void setWalkCmd(PyWalkCmd* command)
	{
		motionCmd->motion = command->getMotionID();
		motionCmd->walkCmd.walkType = command->getWalktype();
		motionCmd->walkCmd.speed.rotation = command->getTurnCCW();
		motionCmd->walkCmd.speed.translation.x = command->getLeft();
		motionCmd->walkCmd.speed.translation.y = command->getForward();
	}

	void setHeadCmd(PyHeadCmd* command)
	{
		motionCmd->headCmd.pan = command->getPan();
		motionCmd->headCmd.tilt = command->getTilt();
		motionCmd->headCmd.speed = command->getSpeed();
	}
	
	void setSpecialAction(PySpecialAction* command)
	{
		motionCmd->motion = command->getMotionID();
		motionCmd->specActionCmd.specAction = command->getSpecialAction();
	}

private:
	MotionCmd* motionCmd;
};

BOOST_PYTHON_MODULE(PyMotion)
{

	class_<PyWalkCmd>("WalkCmd",init<int,int,boost::python::tuple>());
	class_<PyHeadCmd>("HeadCmd",init<double,double,double>());
	class_<PySpecialAction>("SpecialAction",init<int,int>());
	class_<PyMotionInterface>("motion")
		.def("setWalkCmd", &PyMotionInterface::setWalkCmd)
		.def("setHeadCmd",&PyMotionInterface::setHeadCmd)
		.def("setSpecialAction",&PyMotionInterface::setSpecialAction)
		;

}

void c_init_Motion()
{
	if (!Py_IsInitialized())
		Py_Initialize();

	try 
	{
		initPyMotion();
	} 
	catch (error_already_set) 
	{
		PyErr_Print();
	}
}

void set_motion_reference (MotionCmd* _motionCmd)
{
	interface_reference = _motionCmd;
}
