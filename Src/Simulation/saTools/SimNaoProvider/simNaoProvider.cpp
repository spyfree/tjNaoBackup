#include "simNaoProvider.h"
#include "Simulation/saTools/sensorWidget/sensorsData.h"
#include "Tools/Math/Common.h"

#include "almotionproxy.h"
#include "almemoryproxy.h"
#include "dcmproxy.h"
#include <albroker.h>

#include <almodule.h>
#include "alproxy.h"
#include <alptr.h>

using namespace std;
PROCESS_WIDE_STORAGE SimNaoProvider* SimNaoProvider::theInstance = 0;
float angleMotionSpeed=0.06;
//=========
//=========Functions
SimNaoProvider::SimNaoProvider()
{
	SimNaoProvider::theInstance = this;
	isConnected = false;
	dcmWayString = "ClearAll";
	
	simDCMProxy=NULL;
	simMotionProxy=NULL;
	simMemoryProxy=NULL;
}
SimNaoProvider::~SimNaoProvider()
{
	if (simDCMProxy!=NULL)
	{
		delete simDCMProxy;
		simDCMProxy = NULL;
	}
	if (simMemoryProxy!=NULL)
	{
		delete simMemoryProxy;
		simMemoryProxy=NULL;
	}
	if (simMotionProxy!=NULL)
	{
		delete simMotionProxy;
		simMotionProxy=NULL;
	}
}
void SimNaoProvider::doConnect(string ipString)
{
	std::string brokerName = "saTools";
	std::string brokerIP = "";
	int brokerPort = 0 ;
	std::string parentBrokerIP = ipString;		//"127.0.0.1";  	// Default parent broker IP
	int parentBrokerPort = 9559;  //kBrokerPort=9559			// Default parent broker port

	std::cout << "Try to connect to parent Broker at ip :" << parentBrokerIP
		<< " and port : " << parentBrokerPort << std::endl;
	std::cout << "Start the server bind on this ip :  " << brokerIP
		<< " and port : " << brokerPort << std::endl;
	//--------Broker
	try
	{
		broker = AL::ALBroker::createBroker(brokerName, brokerIP, brokerPort, parentBrokerIP, parentBrokerPort);
		//ALModule::createModule<MyModule>( broker ,  "MyModule");
	}
	catch(ALError &e)
	{
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << "Creation of broker failed. Application will exit now." << std::endl;
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << e.toString() << std::endl;
		exit(0);
	}
	//--------MotionProxy
	try
	{
		simMotionProxy = new AL::ALMotionProxy(broker);
	} catch (ALError& )
	{
		std::cout<<"ALMotionProxy Creationg Problem!!"<<std::cout;
	}
	//--------MemoryProxy
	try
	{
		simMemoryProxy = new AL::ALMemoryProxy(broker);
	} catch(ALError&)
	{
		std::cout<<"ALMemoryProxy Creationg Problem!!"<<std::cout;
	}
	//--------DCMProxy
	try
	{
		simDCMProxy = new AL::DCMProxy(broker);
		initDcmProxy();
	} catch (ALError& )
	{
		std::cout<<"DCMProxy Creationg Problem!!"<<std::cout;
	}
}

void SimNaoProvider::disConnect()
{
	//ALBrokerManager::getInstance()->killAllBroker();
	//@todo:解决注销broker的问题
}

//========MemoryProxy Funtions
std::vector<float> SimNaoProvider::getAllSensorValueMemory()
{
	std::vector<float> sensorValues;
	for (int i=SensorsData::gyroX;i<=SensorsData::fsrRBR;i++)
	{
		const char* tempSensorName = SensorsData::getNaoSensorName(SensorsData::Sensor(i));
		ALValue tempALValue = simMemoryProxy->getData(tempSensorName,0);
		if (i>=SensorsData::accX & i<=SensorsData::accZ)
		{
			sensorValues.push_back(*(tempALValue.getPtrValue())/6);
		}
		else if(i>=SensorsData::angleX & i<=SensorsData::angleY)
		{
			sensorValues.push_back(rad2deg(*(tempALValue.getPtrValue())));
		}
		else if(i>=SensorsData::fsrLFL & i<=SensorsData::fsrRBR)
		{
			sensorValues.push_back(*(tempALValue.getPtrValue())*9.81);
		}
		else
		{
			sensorValues.push_back(*(tempALValue.getPtrValue()));
		}
	}
	return sensorValues;
};
//========MotionProxy Functions
void SimNaoProvider::setStiffnessMotion(float stiffness )
{
	simMotionProxy->setStiffnesses("Body",stiffness);
}

void SimNaoProvider::setChainStiffnessMotion(string pChainName, float pStiffness)
{
	simMotionProxy->setStiffnesses(pChainName,pStiffness);
}

void SimNaoProvider::setJointStiffnessMotion(string pJointName,float pStiffness)
{
	simMotionProxy->setStiffnesses(pJointName,pStiffness);
}

void SimNaoProvider::setManyJointsAnglesMotion(string pName,std::vector<float> manyAngles)
{
	simMotionProxy->setAngles(pName,manyAngles,angleMotionSpeed);
}

void SimNaoProvider::setJointAngleMotion(string pJointName, float pAngle)
{
	simMotionProxy->setAngles(pJointName,deg2rad(pAngle),angleMotionSpeed);
}

// std::vector<float> SimNaoProvider::getJointLimits(string jointName)
// {
// //api			return simMotionProxy->getJointLimits(jointName);
// }

// std::vector<std::string> SimNaoProvider::getBodyJointNames()
// {
// //api			return	simMotionProxy->getBodyJointNames();
// }

std::vector<float> SimNaoProvider::getAllAnglesMotion()
{
//api			return	simMotionProxy->getBodyAngles();
	return simMotionProxy->getAngles("Body",TRUE);
}

std::vector<float> SimNaoProvider::getAllStiffnessMotion()
{
	return simMotionProxy->getStiffnesses("Body");
}
//======Dcm Functions
void SimNaoProvider::initDcmProxy()
{
	dcmActionTime = 2000;	//@todo:通过面板控制
	bodySiffness = 0.9;		//@todo:通过面板控制
	createDCMAliases();
	createDCMCommands();
}

void SimNaoProvider::createDCMAliases()
{
	ALValue angleAlias;
	angleAlias.arraySetSize(2);
	angleAlias[0] = string("AllJointsAngles");

	ALValue stiffnessAlias;
	stiffnessAlias.arraySetSize(2);
	stiffnessAlias[0] = string("AllJointsStiffness");

	angleAlias[1].arraySetSize(JointsData::numOfJoints);
	stiffnessAlias[1].arraySetSize(JointsData::numOfJoints);

	for (int i=0;i<JointsData::numOfJoints;i++)
	{
		angleAlias[1][i] = string( JointsData::getJointName(JointsData::Joint(i)) ) + "/Position/Actuator/Value";
		stiffnessAlias[1][i] = string( JointsData::getJointName(JointsData::Joint(i)) ) + "/Hardness/Actuator/Value";
	}
	//--------
	try
	{
		ALValue tempA = simDCMProxy->createAlias(angleAlias);
		ALValue tempS = simDCMProxy->createAlias(stiffnessAlias);
	}catch (AL::ALError& a)
	{
		cout << "Alias Creation Problem!!" << endl;
		//cout << tempS.toString() << endl;
	}
}
void SimNaoProvider::createDCMCommands()
{
	allJointsAngleCommand.arraySetSize(6);
	allJointsAngleCommand[0] = string("AllJointsAngles");
	allJointsAngleCommand[1] = string("ClearAll");		//@todo:几种方式需要进行分析比较
	allJointsAngleCommand[2] = string("time-separate");  //@todo:几种方式需要进行分析比较
	allJointsAngleCommand[3] = 0; //command importance level
	allJointsAngleCommand[4].arraySetSize(1); //list of time to send commands
	allJointsAngleCommand[5].arraySetSize(JointsData::numOfJoints);

	allJointsStiffnessCommand.arraySetSize(6);
	allJointsStiffnessCommand[0] = string("AllJointsStiffness");
	allJointsStiffnessCommand[1] = string("ClearAll");
	allJointsStiffnessCommand[2] = string("time-separate");
	allJointsStiffnessCommand[3] = 0; //command importance level
	allJointsStiffnessCommand[4].arraySetSize(1); //list of time to send commands
	allJointsStiffnessCommand[5].arraySetSize(JointsData::numOfJoints);
}
//--DCM Control Functions
void SimNaoProvider::setAllJointForSaPlayerDCM(int startStep,int endStep,QVector<JointsData> comInfo)
{
	int numOfSteps = endStep-startStep+1;
	ALValue saPlayerCommand;
	saPlayerCommand.arraySetSize(6);
	saPlayerCommand[0] = string("AllJointsAngles");
	saPlayerCommand[1] = string("ClearAll");
	saPlayerCommand[2] = string("time-separate");
	saPlayerCommand[3] = 0;
	saPlayerCommand[4].arraySetSize(numOfSteps);
	saPlayerCommand[5].arraySetSize(JointsData::numOfJoints);
	//------------
	int timeTotal = 0;
	for (int i=0;i<numOfSteps;i++)
	{
		timeTotal += comInfo[startStep+i].timeOfAction;
		saPlayerCommand[4][i] = simDCMProxy->getTime(timeTotal);
	}
	for (int m=0;m<JointsData::numOfJoints;m++)
	{
		saPlayerCommand[5][m].arraySetSize(numOfSteps);
		for (int n=0;n<numOfSteps;n++)
		{
			saPlayerCommand[5][m][n] = deg2rad(comInfo[startStep+n].jointsAngle[m]);
		}
	}
	//--------
	try
	{
		simDCMProxy->setAlias(saPlayerCommand);
	}catch (AL::ALError& a)
	{
		cout<<"SaPlayer DCM set error  "<<endl;
	}
}

void SimNaoProvider::setAllStiffnessDCM(float stiffness)
{
	allJointsStiffnessCommand[4][0] = simDCMProxy->getTime(0);	
	for (int i = 0;i<JointsData::numOfJoints;i++)
	{
		allJointsStiffnessCommand[5][i].arraySetSize(1);
		allJointsStiffnessCommand[5][i][0]= stiffness;		
	}
	//-------
	try
	{
		simDCMProxy->setAlias(allJointsStiffnessCommand);
	}
	catch (AL::ALError& a)
	{
		cout<<"DCM_Hardness set error  "<<endl;
	}
}

//===================for debug
void SimNaoProvider::bug()
{
	cout<<"tim = "<<simDCMProxy->getTime(0)<<endl;
}