#ifndef __Actuator_h_
#define __Actuator_h_

#include "Module/Core/Thread.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/KSManager.h"
#include "Module/Actuator/ObjectDetection.h"
#include "Module/Actuator/KeeperMotion.h"

#include "Simulation/NaoProvider/ActuatorSimProvider.h"
#include "Module/Actuator/WalkingEngine.h"
#include "Module/Actuator/MotionEngine.h"
#include "Module/Actuator/SpecActionEngine.h"

#ifndef NAO_DEBUG
class Actuator : public Thread
{
public:
	Actuator(SmartPtr<Synchronizer> _synchro);
	~Actuator();
	void run();
	void waitOn(){spMyTrigger->wait_on();}
private:
	void initKSSystem()
	{
		setGlobal();
		initKSManager();
		updateKSManager();
	}
	void setGlobal() {Blackboard::theInstance = blackboard;}
	void initKSManager(){theKSManager = new KSManager("Acuator");}
	void updateKSManager(){theKSManager->update(0);}
	void destroyKSSystem()
	{
		theKSManager->destroy();
		if(theKSManager){
			delete theKSManager;
			theKSManager = NULL;
		}
	}
	void executeKS();
	Blackboard* blackboard;
	KSManager* theKSManager;
	//keeperMotion keeper;
	
};

#else
class Actuator : public Thread
{
public:
	Actuator(SmartPtr<Synchronizer> _synchro);
	~Actuator();
	void run();
	/**
	* Called by another thread that need to be synchronized with this thread
	* wait until trigger on event.
	*/
	void waitOn(){spMyTrigger->wait_on();}

	bool doConnect(const std::string ipStr);
	void disConnect();
	void setEmergencyStop();
	void setSendingJoints(bool sendjoints);
private:
	void initKSSystem()
	{
		setGlobal();
		initKSManager();
		updateKSManager();
	}
	void setGlobal() {Blackboard::theInstance = blackboard;}
	void initKSManager(){theKSManager = new KSManager("Acuator");}
	void updateKSManager(){theKSManager->update(0);}
	void destroyKSSystem()
	{
		theKSManager->destroy();
		if(theKSManager){
			delete theKSManager;
			theKSManager = NULL;
		}
	}
	/**
	* Execute the KSes as the sequence defined in the config file.
	*/
	void executeKS();
	/**
	* Init the joints of walking output data, used especially in PC_Debug Mode
	*/
	void restWalkingEngineOutput();
	void startupNao();
private:
	/**
	* The Blackboard entity of this thread
	*/
	Blackboard* blackboard;
	/**
	* Manager of the KSes in this thread
	*/
	KSManager* theKSManager;

private:
	std::string ipAddress;
	bool isConnected;
	bool jointsSend2Nao; /**< Indicate wheather we will send joints to Nao*/
	float bodyHardness;
	bool setRemoteBodyHardness;
};

#endif

#endif
