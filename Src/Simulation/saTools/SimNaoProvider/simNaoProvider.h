#ifndef SIMNAOPROVIDER_H
#define SIMNAOPROVIDER_H
//-----C++
#include <string>
#include <vector>
using std::string;
#include <iostream>
#include "Platform/Win32/SystemCall.h"
//-----Aldebaran
#include "alptr.h"
#include "albroker.h"
#include "alvalue.h"
//#include "alproxy.h"  //这两个太囧了
//#include "almotionproxy.h"
using namespace AL;
//------TJArkNao
#include "Simulation/saTools/jointsData.h"
#include <QVector>
//===========
class SimNaoProvider
{
	public:
		SimNaoProvider();
		~SimNaoProvider();

		static SimNaoProvider * getInstance()
		{
			if(theInstance==NULL)
			{
				theInstance=new SimNaoProvider();
			}
			return theInstance;
		}

	public:
		void doConnect(string ipString);
		void disConnect();
		//-----MotionFunctions
		void setStiffnessMotion(float);
		void setChainStiffnessMotion(string,float);
		void setJointStiffnessMotion(string,float);
		void setJointAngleMotion(string,float);
		void setManyJointsAnglesMotion(string,std::vector<float>);
		std::vector<std::string> getBodyJointNames();
		std::vector<float> getJointLimits(string);
		std::vector<float>getAllAnglesMotion();
		std::vector<float>getAllStiffnessMotion();
		//-----MemoryFunctions
		std::vector<float> getAllSensorValueMemory();
		//-----DCMFunctions
		void setAllStiffnessDCM(float);		
		void setAllJointForSaPlayerDCM(int startStep,int endStep,QVector<JointsData> comInfo);
	
	public://for debug~~
		void bug();

	private:
		void initDcmProxy();
		void createDCMAliases();
		void createDCMCommands();

	private:
		AL::ALBroker::Ptr broker;
		AL::ALMotionProxy * simMotionProxy;
		AL::DCMProxy * simDCMProxy;
		AL::ALMemoryProxy * simMemoryProxy;
		AL::ALValue  allJointsAngleCommand;
		AL::ALValue  allJointsStiffnessCommand;
		AL::ALValue  allSensorsCommand;

// 		//myTest 5.19
// 		ALPtr<ALProxy> myMot;
// 		AL::ALProxy * simProxyMotion;
// 		AL::ALModule	* myModule;
// 		AL::ALProxy * myProxy;
// 		AL::ALPtr<AL::DCMProxy> dcmProxy;
// 		AL::ALProxy* badProxy;

	public:	//@todo:这里的量要加到面板上
		int dcmActionTime;	
		float bodySiffness;
		string dcmWayString;
		
	private:
		PROCESS_WIDE_STORAGE_STATIC SimNaoProvider * theInstance; /**< The only instance of this module. */
		bool isConnected;
};

#endif	//SIMNAOPROVIDER_H