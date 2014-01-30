#include "Actuator.h"
#include "Platform/SystemCall.h"
#include "Module/Sensor/NaoProvider.h"
#include <iostream>
#include <stdio.h>

#include "Module/Common/JointData.h"
#include "Module/Common/WalkingEngineOutput.h"
#include "Simulation/NaoProvider/ActuatorSimProvider.h"
#include "Module/Actuator/MotionDebugging.h"
#include "Platform/GetSetting.h"


#ifndef NAO_DEBUG
Actuator::Actuator(SmartPtr<Synchronizer> _synchro)
:Thread(_synchro, "Actuator"), theKSManager(0),
blackboard(new Blackboard)
{

}
Actuator::~Actuator()
{
	if(theKSManager)
	{
		delete theKSManager;
		theKSManager = NULL;
	}

	// TODO: delete blackboard? or this other place to delete the blackboard, maybe the blackboard need not to be deleted, as the blackboard only have pointers....
}
//The ms per frame of actuator frame
static const unsigned ACTUATOR_mSPF =20;
void Actuator::run()
{

	initKSSystem();
	spMyTrigger->on();
	running = true;
	while(running)
	{
		//std::cout<<"GUMPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPISGONE "<<std::endl;
		unsigned startTime = SystemCall::getCurrentSystemTime();
		SystemCall::sleep(100);
		//objectDetection.updateSonarsData();
		//objectDetection.walkBalanceCheck();
		//keeper.run();
		executeKS();
		unsigned timeUsed = SystemCall::getTimeSince(startTime);
		std::cout<<"========================Time Used per Frame(Actuator):"<<timeUsed<<" ms "<<std::endl;
	}

    //=======Destroy all KS and KS Stystem==================//
	destroyKSSystem();
	//MATLABHELP_DESTORY();
	spMyTrigger->off();
}

void Actuator::executeKS()
{
	// TODO: check whether the last cmd has been executed...
	theKSManager->execute();
}


#else
MDEBUG_DECLARE

Actuator::Actuator(SmartPtr<Synchronizer> _synchro)
:Thread(_synchro, "Actuator"), theKSManager(0),
blackboard(new Blackboard),
isConnected(false), ipAddress("0.0.0.0"),
jointsSend2Nao(false),
bodyHardness(0.8),
setRemoteBodyHardness(false)
{

}
Actuator::~Actuator()
{
	if(theKSManager)
	{
		delete theKSManager;
		theKSManager = NULL;
	}

	// TODO: delete blackboard? or this other place to delete the blackboard, maybe the blackboard need not to be deleted, as the blackboard only have pointers....
}
//The ms per frame of actuator frame
#ifndef NAO_DEBUG   //for Simulation
static const unsigned ACTUATOR_mSPF =20;
#else
static const unsigned ACTUATOR_mSPF =20;
#endif
void Actuator::run()
{
#ifndef NAO_DEBUG
	MDEBUG_OPEN_RECORD(true);
#endif
	MDEBUG_NEW()
		initKSSystem();
	//============//
#ifndef NAO_DEBUG
	isConnected=true;
	jointsSend2Nao=true;
#else
	ActuatorSimProvider::setConnected(isConnected);
	ActuatorSimProvider::setSend2Nao(jointsSend2Nao);
	//ActuatorSimProvider::setNaoProvider(naoProvider);
#endif
	//MATLABHELP_INIT();
	// First Reset walking Output angles;
	restWalkingEngineOutput();
	// TODO: make some start up motion....
	startupNao();
	spMyTrigger->on();
	running = true;
	while(running)
	{
#ifdef NAO_DEBUG
#ifdef ODESimulationActuator
		theSynchronizer->getEventbyName("ODESimulation->Actuator")->wait();
#endif
#endif
		unsigned startTime = SystemCall::getCurrentSystemTime();
		executeKS();
		//unsigned time1 = SystemCall::getTimeSince(startTime);
		MDEBUG_SAVE(MDebug::idJointCmd, *(Blackboard::theInstance->theJointCmd))
			MDEBUG_FINISH_FRAME(startTime)
			//unsigned time2 = SystemCall::getTimeSince(startTime) - time1;
#ifndef NAO_DEBUG
			if(isConnected && jointsSend2Nao)
				NaoProvider::finishFrame(ACTUATOR_mSPF);
#else
			if(isConnected && jointsSend2Nao)
				ActuatorSimProvider::finishFrame();
#ifdef ODESimulationActuator
#ifdef SendMotionBoradToODE
		ActuatorSimProvider::sendMotionBoardToODESimulation();
#else
		ActuatorSimProvider::sendToODESimulation(*(Blackboard::theInstance->theJointCmd));
#endif
#endif
#endif

#ifdef ODESimulationActuator
		theSynchronizer->getEventbyName("Actuator->ODESimulation")->signal();
#else
		unsigned timeUsed = SystemCall::getTimeSince(startTime);
		//std::cout<<"Time Used Mist(Actuator): Time1: "<<time1<<" ms, time2: "<<time2<<" ms"<<std::endl;
		//std::cout<<"Time Used per Frame(Actuator): "<<timeUsed<<" ms"<<std::endl;
		if(timeUsed < ACTUATOR_mSPF)
			SystemCall::sleep(ACTUATOR_mSPF - timeUsed);
#endif
	}
	//=======After Stop Thread, Check Whether to set body hardness====//
	if(isConnected && setRemoteBodyHardness){
#ifndef NAO_DEBUG
		NaoProvider::setBodyHardness(0.0);
#else
		ActuatorSimProvider::setBodyHardness(0.0);
#endif
		setRemoteBodyHardness = false;
	}
	//=======Destroy all KS and KS Stystem==================//
	destroyKSSystem();
#ifndef NAO_DEBUG
	std::string filename = std::string(GetSetting::GetConfigDir()) + "/test.mdata";
	//std::cout<<"Output Joints Log to "<<filename.c_str()<<std::endl;
	MDEBUG_FINISH_SAVE(filename.c_str());
#endif
	//MATLABHELP_DESTORY();
	spMyTrigger->off();
}

void Actuator::executeKS()
{
	// TODO: check whether the last cmd has been executed...
	theKSManager->execute();
}

void Actuator::restWalkingEngineOutput()
{

	if((Blackboard::theInstance !=NULL) && (Blackboard::theInstance->theWalkingEngineOutput != NULL))
	{
		WalkingEngineOutput*  walkoutput = const_cast<WalkingEngineOutput*>(Blackboard::theInstance->theWalkingEngineOutput);
		for(int i = JointData::HeadYaw; i < JointData::numOfJoints; ++i)
		{
			walkoutput->angles[i] = 0;
		}
	}

}

void Actuator::startupNao()
{
	/*FILE * teamConfigFile;*/
	//std::cout<<"startupNao begin"<<std::endl;
	//teamConfigFile = fopen("/opt/Config/team.cfg","r");
	//std::cout<<"startupNao end"<<std::endl;
	int roleFlag=0;
	std::ifstream stream((std::string(GetSetting::GetConfigDir()) + "/team.cfg").c_str());
	if (!stream.is_open())
	{
		std::cout<<" Open team configure file failed "<<std::endl;
	}
	stream>>roleFlag;

	double *jdataDeg=NULL;

	if (roleFlag == 1)
	{
		//std::cout<<"------49"<<std::endl;
		double jdataDeg1[] = {0, 0,
			93.16,-26.2, 2.70,18.112,//100, -30, 90, 90,
			92.9,26.4,  -2.9,  -18.2, //100, 30, -90, -90,
			0, 0, -40, 80, -40, 0,
			0, 0, -40, 80, -40, 0};
		jdataDeg=jdataDeg1;
	}
	else{
		//std::cout<<"------50"<<std::endl;
		double jdataDeg2[] = {0, 0,
			93.16,-26.2, 2.70,18.112,//100, -30, 90, 90,
			92.9,26.4,  -2.9,  -18.2, //100, 30, -90, -90,
			0, 0, -40, 80, -40, 0,
			0, 0, -40, 80, -40, 0};
		jdataDeg=jdataDeg2;
	}
	//for(int time = 0; time < 200; ++time){
	JointCmd*  jointCmd =  const_cast<JointCmd*>(Blackboard::theInstance->theJointCmd);

	for(int i = JointData::NoneJoint + 1; i < JointData::numOfJoints; ++i)
	{
		jointCmd->angles[i] = deg2rad(jdataDeg[i-1]) ;
	}
#ifndef NAO_DEBUG
	if(isConnected && jointsSend2Nao)
		NaoProvider::finishFrame(2000);
#else
	if(isConnected && jointsSend2Nao)
		ActuatorSimProvider::finishFrame();
#ifdef ODESimulationActuator
	ActuatorSimProvider::sendToODESimulation(*(Blackboard::theInstance->theJointCmd));
#endif
#endif
	SystemCall::sleep(20);
	//  }
	SystemCall::sleep(2000);
	//ActuatorSimProvider::setStartUpNao();
}

//========Connect and Disconnect======//
bool Actuator::doConnect(const std::string ipStr)
{
#ifndef NAO_DEBUG
	if(NaoProvider::connectNao(ipStr) == true){
#else
	if(ActuatorSimProvider::connectNao(ipStr) == true){
#endif
		ipAddress = ipStr;
		isConnected = true;
		return true;
	}
	else{
		ipAddress = "0.0.0.0";
		isConnected = false;
		return false;
	}
	return true;
}
void Actuator::disConnect()
{
	isConnected = false;
	ipAddress = "0.0.0.0";
}

void Actuator::setEmergencyStop()
{
	if(isRunning()){
		setRemoteBodyHardness = true;
		bodyHardness = 0.0;
		stop();
	}
}
void Actuator::setSendingJoints(bool sendjoints)
{
	jointsSend2Nao = sendjoints;
}

#endif