#include <iostream>
#include "RunSimBehav.h"
#include "Module/Common/PyLED.h"

PyRobotSimInterface::PyRobotSimInterface()
{
	initialModule = true;	
}

PyRobotSimInterface::~PyRobotSimInterface()
{
	Py_XDECREF(behav_instance);
	Py_XDECREF(behav_module);
}

void PyRobotSimInterface::initializeVision(const GoalPercept*theGoalData,
										const BallPercept*theBallData,
										const RobotPercept*theRobotData)
{
	set_vision_reference(theGoalData,theBallData,theRobotData);
	c_init_Vision();
}

void PyRobotSimInterface::initializeLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel* theFreePartOfOpponentGoalModel)
{
	//set_locator_reference(theRobotPoseData,theBallModelData,theFreePartOfOpponentGoalModel);
	c_init_locator();
}

void PyRobotSimInterface::initializeCommon(const OutBehaviorData *_theCommonData)
{
	set_common_reference(_theCommonData);
	c_init_common();
}

void PyRobotSimInterface::initializeLED(LEDSimRequest *_theLEDRequest)
{
	set_led_reference(_theLEDRequest);
	c_init_led();
}

void PyRobotSimInterface::initializeTeamCfg(TeamCfgInfo *_theTeamData)
{
	set_teamcfg_reference(_theTeamData);
	c_init_teamcfg();
}

//void PyRobotSimInterface::initializeTeammate(const perceiveInfo* theLocData,const TeamMateInfo* theTeaMateData)
//{
//	set_teammate_reference(theLocData,theTeaMateData);
//	c_init_teammate();
//}

void PyRobotSimInterface::initializeMotion(MotionCmd *_theMotionCmd)
{
	set_motion_reference(_theMotionCmd);
	c_init_Motion();
}
void PyRobotSimInterface::initializeGameControl(const RoboCupGameControlData *_theGameControlData,const TeamCfgInfo*_theTeamCfgInfoData)
{
	set_GameControl_reference(_theGameControlData,_theTeamCfgInfoData);
	c_init_GameControl();

}
void PyRobotSimInterface::modifySysPath()
{
#ifndef NAO_DEBUG
	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("Pyaddr = os.path.join(os.getcwd(),'modules/python')");
	PyRun_SimpleString("print Pyaddr");
	PyRun_SimpleString("sys.path.append(Pyaddr)");
#else
	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("Pyaddr = os.path.join(os.getcwd(),'Python2010')");
	PyRun_SimpleString("print Pyaddr");
	PyRun_SimpleString("print os.getcwd()");
	PyRun_SimpleString("sys.path.append(Pyaddr)");
#endif
}

void PyRobotSimInterface::importmodules()
{
	modifySysPath();
	behav_module=PyImport_ImportModule("Behavior");
	if(!behav_module)
	{
		std::cout<<"import Behavior failed!"<<std::endl;
		return ;
	}
	getBehavInstance();
}

void PyRobotSimInterface::reloadmodules()
{
	if(behav_module == NULL)
	{
		std::cout<<"Error : Behavior module is not initialized"<<std::endl;
	}
	else
	{
		PyObject *oldModule = behav_module;
		behav_module = PyImport_ReloadModule(behav_module);
		if(behav_module == NULL)
		{
			std::cout<<"Error : New Behavior module can not be loaded, revert to old module"<<std::endl;
			behav_module = oldModule;
		}
		else
		{
			std::cout<<"New behavior module reloaded successfully!"<<std::endl;
		}
	}
	getBehavInstance();
}

void PyRobotSimInterface::getBehavInstance ()
{
	// Grab instantiate and hold a reference to a new Behavior.Behavior.Behavior()
	PyObject *dict = PyModule_GetDict(behav_module);
	if (dict == NULL)
		std::cout<<"get dict error,stop!!!"<<std::endl;

	PyObject *behav_class = PyDict_GetItemString(dict, "Behavior");
	if (PyCallable_Check(behav_class))
	{
		behav_instance = PyObject_CallObject(behav_class, NULL);
		std::cout<<"get behav_class successfully!!!"<<std::endl;
	}
	if (behav_instance == NULL)
	{
		if (PyErr_Occurred())
			PyErr_Print();
		else
			std::cout<<"  No error available"<<std::endl;
	}
	else
	{
		std::cout<<"get the behavInstance successful!!!"<<std::endl;
	}

}

void PyRobotSimInterface::processRun()
{
	// Call main run() method of Behavior
	if (behav_instance != NULL)
	{
		PyObject *func = PyObject_CallMethod(behav_instance, "run", NULL);
		if (func == NULL)
		{
			// report error
			std::cout<<"Error occurred in Behavior.Behavior.run() method"<<std::endl;
			if (PyErr_Occurred())
			{
				PyErr_Print();
			}
			else
			{
				std::cout<<"  No Python exception information available"<<std::endl;
			}
		}
		else
		{
			std::cout<<"begin to run behavior codes!!!"<<std::endl;
		}
	}
}

void PyRobotSimInterface::updataVision(const GoalPercept*theGoalData,const BallPercept*theBallData,const RobotPercept*theRobotData)
{
	set_vision_reference(theGoalData,theBallData,theRobotData);
}

void PyRobotSimInterface::updateLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel)
{
	//set_locator_reference(theRobotPoseData,theBallModelData,theFreePartOfOpponentGoalModel);
}


void PyRobotSimInterface::updateCommon(const OutBehaviorData *_theCommonData)
{
	set_common_reference(_theCommonData);
}

void PyRobotSimInterface::updateLED(LEDSimRequest*_theLEDRequest)
{
	set_led_reference(_theLEDRequest);
}

void PyRobotSimInterface::updateTeamCfg(TeamCfgInfo *_theTeamData)
{
	set_teamcfg_reference(_theTeamData);
}

void PyRobotSimInterface::updateMoiton(MotionCmd *_theMotionData)
{
	set_motion_reference(_theMotionData);
}
void PyRobotSimInterface::updateGameControl(const RoboCupGameControlData*_theGameControlData,const TeamCfgInfo *_theTeamCfgInfoData)
{
	set_GameControl_reference(_theGameControlData,_theTeamCfgInfoData);
}

//void PyRobotSimInterface::updateTeammate(const perceiveInfo* _theLocData,const TeamMateInfo* theTeammateData)
//{
//	set_teammate_reference(_theLocData,theTeammateData);
//}
