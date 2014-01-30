#include "ObjectDetection.h"
#include "almotionproxy.h"
#include "almemoryproxy.h"
#include "Platform/GetSetting.h"
#include <fstream>
#include <string>

static std::list<int> *statesList = 0;

ObjectDetection::ObjectDetection()
{
	std::string path = std::string(GetSetting::GetConfigDir()) + "/WalkCfg/WalkBalance.cfg";
	params.readCfgFile(path);
	params.massAssign();
	stateList.resize(3,0);
	statesList = &stateList;
}


ObjectDetection::~ObjectDetection(){}

void ObjectDetection::Parameters::massAssign()
{
	ASSIGN(ULimt);
	ASSIGN(DLimt);
	ASSIGN(UnableLimt);

	ASSIGN(angleXmax);
	ASSIGN(angleXmin);
	ASSIGN(angleYmax);
	ASSIGN(angleYmin);

	ASSIGN(newAngleXmax);
	ASSIGN(newAngleXmin);
	ASSIGN(newAngleYmax);
	ASSIGN(newAngleYmin);

	ASSIGN(leftSonarData);
	ASSIGN(rightSonarData);
	ASSIGN(angleX);
	ASSIGN(angleY);
}
void ObjectDetection::updateSonarsData()
{
	params.leftSonarData = myProxyContainer.almemoryProxy->getData("Device/SubDeviceList/US/Left/Sensor/Value", 0);
	params.rightSonarData = myProxyContainer.almemoryProxy->getData("Device/SubDeviceList/US/Right/Sensor/Value", 0);
}

bool ObjectDetection::isObjectThere()
{
	updateSonarsData();

// 	if (leftSonarData>ULimt or rightSonarData>ULimt)
// 		return false;//N
// 	else if (leftSonarData<DLimt or rightSonarData<DLimt)
// 		return true;//Y
// 	else if (leftSonarData < UnableLimt or rightSonarData < UnableLimt)
// 		return false;//U
// 	else:
// 		return false;//L

	if (params.leftSonarData<params.DLimt || params.rightSonarData<params.DLimt)
		return true;
	else
		return false;
}

void  ObjectDetection::updateBodyAnlge()
{
	params.angleX = myProxyContainer.almemoryProxy->getData("Device/SubDeviceList/InertialSensor/AngleX/Sensor/Value",0);
	params.angleY = myProxyContainer.almemoryProxy->getData("Device/SubDeviceList/InertialSensor/AngleY/Sensor/Value",0);
}
void ObjectDetection::updateBumperData()
{
	params.rightBumper = myProxyContainer.almemoryProxy->getData("RightBumperPressed",0);
	params.leftBumper = myProxyContainer.almemoryProxy->getData("LeftBumperPressed",0);
}
void ObjectDetection::walkBalanceCheck()
{
	if(myProxyContainer.motionProxy->walkIsActive() )
	{
		updateBumperData();
		if (params.rightBumper == 1 ||params.leftBumper == 1)
		{
			myProxyContainer.motionProxy->setWalkTargetVelocity(0,0,0,0);
			std::cout<<"bumper walk stop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
			stateList.push_back(1);
			stateList.pop_front();
			//statesList = &stateList;
			return;
		}
		updateBodyAnlge();

		if (isObjectThere())
		{
			if(params.angleX > params.newAngleXmax || params.angleX < params.newAngleXmin ||
		params.angleY >params.newAngleYmax ||params.angleY < params.newAngleYmin)
			{
				myProxyContainer.motionProxy->setWalkTargetVelocity(0,0,0,0);
				std::cout<<"sonar walk stop !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
				stateList.push_back(1);
				stateList.pop_front();
				//statesList = &stateList;
				return;
			}
		}
		else
		{
			if(params.angleX > params.angleXmax || params.angleX <params.angleXmin ||
				params.angleY > params.angleYmax ||params.angleY < params.angleYmin)
			{
				myProxyContainer.motionProxy->setWalkTargetVelocity(0,0,0,0);
				std::cout<<"walk stop !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
				stateList.push_back(0);
				stateList.pop_front();
				//statesList = &stateList;
				return;
			}
		}
        stateList.push_back(0);
		stateList.pop_front();
		//statesList = &stateList;
	}
	else
		stateList.resize(3,0);


}

boost::python::list getObjectState()
{
	boost::python::list list;
	for (std::list<int>::iterator it = (*statesList).begin();it!=(*statesList).end();++it)
	{
		list.append(*it);
	}
	return list;
}

//BOOST_PYTHON_MODULE(PyObjectDetection)
//{
//	def("getObjectState",&getObjectState);
//}
//
//void c_init_ObjectDetection()
//{
//	if (!Py_IsInitialized())
//	{
//		std::cout<<"00000000000"<<std::endl;
//		Py_Initialize();
//	}
//
//	try
//	{
//		std::cout<<"aaaaaaaaaaaaaaaaa"<<std::endl;
//		initPyObjectDetection();
//		std::cout<<"bbbbbbbbbbbbbbbbb"<<std::endl;
//	}
//	catch (error_already_set)
//	{
//		PyErr_Print();
//	}
//}
