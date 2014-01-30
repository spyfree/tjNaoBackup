#include <iostream>
#include "RunBehav.h"

PyRobotInterface::PyRobotInterface()
{
    std::cout<<"begin to initialize Python"<<std::endl;
	initialModule = true;
}

PyRobotInterface::~PyRobotInterface()
{
	Py_XDECREF(behav_instance);
	Py_XDECREF(behav_module);
}

void PyRobotInterface::initializeVision(const GoalPercept*theGoalData,
										const BallPercept*theBallData,
										const RobotPercept*theRobotData)
{
	set_vision_reference(theGoalData,theBallData,theRobotData);
	c_init_Vision();
}

void PyRobotInterface::initializeGlobalInfo(GlobalInfo* theGlobalInfo)
{
	set_GlobalInfo_reference(theGlobalInfo);
	c_init_GlobalInfo();
}

//void PyRobotInterface::initializeLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData)
//{
//	set_locator_reference(theRobotPoseData,theBallModelData);
//	c_init_locator();
//}
void PyRobotInterface::initializeLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel* theFreePartOfOpponentGoalModel,const OdometryData*theOdometryData)
{
	set_locator_reference(theRobotPoseData,theBallModelData,theFreePartOfOpponentGoalModel,theOdometryData);
	c_init_locator();
}
void PyRobotInterface::initializeCommon(const OutBehaviorData *_theCommonData)
{
    set_common_reference(_theCommonData);
    c_init_common();
}

void PyRobotInterface::initializeLED(LEDRequest *_theLEDRequest)
{
	set_led_reference(_theLEDRequest);
	c_init_led();
}

void PyRobotInterface::initializeCamera(const CameraConfig *_theCameraData)
{
	set_camera_reference(_theCameraData);
	c_init_camera();
}

void PyRobotInterface::initializeTeamCfg(TeamCfgInfo *_theTeamData)
{
	set_teamcfg_reference(_theTeamData);
	c_init_teamcfg();
}

//void PyRobotInterface::initializeTeammate(const perceiveInfo* theLocData,const TeamMateInfo* theTeaMateData)
//{
//	set_teammate_reference(theLocData,theTeaMateData);
//	c_init_teammate();
//}

void PyRobotInterface::initializeMotion(MotionCmd *_theMotionCmd)
{
	set_motion_reference(_theMotionCmd);
	c_init_Motion();
}
void PyRobotInterface::initializeTeamUDPDate(TeamUDPData *_theTeamUDPData,TeamMateInfo *_teammateData)
{
	set_teamUDP_reference(_theTeamUDPData,_teammateData);
	c_init_teamUDP();
}
void PyRobotInterface::initializeGameControl(const RoboCupGameControlData *_theGameControlData,const TeamCfgInfo*_theTeamCfgInfoData)
{
	set_GameControl_reference(_theGameControlData,_theTeamCfgInfoData);
	c_init_GameControl();

}
void PyRobotInterface::modifySysPath()
{
#ifndef NAO_DEBUG
	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("Pyaddr = os.path.join('/home/nao/naoqi/','Python2011')");
	PyRun_SimpleString("print Pyaddr");
	PyRun_SimpleString("sys.path.append(Pyaddr)");
#else
	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("Pyaddr = os.path.join(os.getcwd(),'Python')");
	PyRun_SimpleString("print Pyaddr");
	PyRun_SimpleString("sys.path.append(Pyaddr)");
#endif
}

void PyRobotInterface::importmodules()
{
	modifySysPath();
#ifdef MOTION_ONLY
	behav_module=PyImport_ImportModule("NoBehavior");
#else
	std::cout<<"import Python module"<<std::endl;
    behav_module=PyImport_ImportModule("Behavior");
	//behav_module=PyImport_ImportModule("leeTest");
#endif
	if(!behav_module)
	{
		std::cout<<"import Behavior failed!"<<std::endl;
		return ;
	}
	getBehavInstance();
}

void PyRobotInterface::reloadmodules()
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

void PyRobotInterface::getBehavInstance ()
{
	std::cout<<"get bahavior instanse"<<std::endl;
	// Grab instantiate and hold a reference to a new Behavior.Behavior.Behavior()
	PyObject *dict = PyModule_GetDict(behav_module);
	if (dict == NULL)
		std::cout<<"get dict error,stop!!!"<<std::endl;

	PyObject *behav_class = PyDict_GetItemString(dict, "Behavior");
	//PyObject *behav_class = PyDict_GetItemString(dict, "leeTest");
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

void PyRobotInterface::processRun()
{
	// Call main run() method of Behavior
	if (behav_instance != NULL)
	{
		//std::cout<<"get the run method"<<std::endl;
		PyObject *func = PyObject_CallMethod(behav_instance, "run", NULL);
		//std::cout<<"after get run method"<<std::endl;
		if (func == NULL)
		{
			// report error
			std::cout<<"Error occurred in Behavior.Behavior.run() method"<<std::endl;
			if (PyErr_Occurred())
			{
				std::cout<<"pyError occurred"<<std::endl;
				PyErr_Print();
			}
			else
			{
				std::cout<<"  No Python exception information available"<<std::endl;
			}
		}
		else
		{
			//std::cout<<"begin to run behavior codes!!!"<<std::endl
			;
		}
	}
}

void PyRobotInterface::updataVision(const GoalPercept*theGoalData,
									const BallPercept*theBallData,
									const RobotPercept*theRobotData)
{
	set_vision_reference(theGoalData,theBallData,theRobotData);
}

void PyRobotInterface::updataGlobalInfo( GlobalInfo* theGlobalInfo)
{
	set_GlobalInfo_reference(theGlobalInfo);
}

//void PyRobotInterface::updateLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData)
//{
//	set_locator_reference(theRobotPoseData,theBallModelData);
//}

void PyRobotInterface::updateLocator(const RobotPose* theRobotPoseData,const BallModel* theBallModelData,const FreePartOfOpponentGoalModel*theFreePartOfOpponentGoalModel,const OdometryData*theOdometryData)
{
	set_locator_reference(theRobotPoseData,theBallModelData,theFreePartOfOpponentGoalModel,theOdometryData);
}

void PyRobotInterface::updateCommon(const OutBehaviorData *_theCommonData)
{
    set_common_reference(_theCommonData);
}

void PyRobotInterface::updateLED(LEDRequest*_theLEDRequest)
{
	set_led_reference(_theLEDRequest);
}

void PyRobotInterface::updateCamera(const CameraConfig *_theCameraData)
{
	set_camera_reference(_theCameraData);
}

void PyRobotInterface::updateTeamCfg(TeamCfgInfo *_theTeamData)
{
	set_teamcfg_reference(_theTeamData);
}

void PyRobotInterface::updateMoiton(MotionCmd *_theMotionData)
{
	set_motion_reference(_theMotionData);
}
void PyRobotInterface::updateTeamUDPData(TeamUDPData *_theTeamUDPData,TeamMateInfo *_teammateData)
{
	set_teamUDP_reference(_theTeamUDPData,_teammateData);
}
void PyRobotInterface::updateGameControl(const RoboCupGameControlData*_theGameControlData,const TeamCfgInfo *_theTeamCfgInfoData)
{
	set_GameControl_reference(_theGameControlData,_theTeamCfgInfoData);
}

//void PyRobotInterface::updateTeammate(const perceiveInfo* _theLocData,const TeamMateInfo* theTeammateData)
//{
//	set_teammate_reference(_theLocData,theTeammateData);
//}

