  /**
 * @author Jerome Monceaux
 * Aldebaran Robotics (c) 2007 All Rights Reserved. This is an example of use.\n
 *
 */

#ifndef MainModule_H
# define MainModule_H
# include "main.h"


#include <string>
#include "Module/Core/Thread.h"
#include "Module/Behavior/TJRobot.h"
#include "Module/Network/Network.h"
#include "Module/Sensor/Sensor.h"
#include "Module/Vision/ImageProvider.h"
#include "Module/Sensor/NaoProvider.h"
#include "Module/Vision/CameraMatrixProvider.h"
#include "Module/Vision/SensorComm.h"
#include "Module/Vision/NaoVisionConfigProvider.h"
#include "Module/Actuator/Actuator.h"
#include "Module/Actuator/NaoActuatorConfigProvider.h"
#include "Module/Sensor/RemoteCmdProvider.h"
#include "Module/Representations/Infrastructure/OutBehaviorProvider.h"
#include "Module/Network/NetworkProvider.h"
#include "Module/Network/GlobalInfoProvider.h"
#include "Module/Modeling/BallLocator/BallLocator.h"
#include "Module/Sensor/OdometryDataProvider.h"
#include "Module/Actuator/RobotStateProvider.h"
#include "Module/Modeling/SelfLocator/SelfLocator.h"
#include "Module/Modeling/SelfLocator/RobotPoseValidator.h"
#include "Module/Vision/VisionPercept.h"
#include "Module/Vision/RobotSpecialist.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include "Module/Modeling/SelfLocator/LocSamplesToPc.h"
#include "Module/Modeling/SelfLocator/FreePartOfOpponentGoalModelProvider.h"
#include "Module/Sensor/NaoProvider.h"

#include "Module/Actuator/MotionEngine.h"

namespace AL
{
	class ALProxy;
	class ALMotionProxy;
	class ALValue;
	class ALBroker;
};
using namespace AL;
/**
 * A basic class example.
 */
class MainModule : public AL::ALModule,public Thread
{

  public:

    /**
     * Default Constructor.
     */
    MainModule( ALPtr<ALBroker> pBroker, std::string pName );

    /**
     * Destructor.
     */
    virtual ~MainModule();

    /**
     * helloWorld, print "TJArkNao!" to the ALLogger module
     */
    void NaoMain(  );

    /**
     * version
     * @return version of alcommon
     */
    //std::string version(){return ALTOOLS_VERSION( NAO );};

    /**
     * innerTest
     * @return True is all the tests passed
     */
    bool innerTest(){ return true;};

    /* dataChanged. Called by stm when subcription
     * has been modified.
     * @param pDataName Name of the suscribed data
     * @param pValue Value of the suscribed data
     * @param pMessage Message written by user during subscription
     */
    void dataChanged(const std::string& pDataName, const ALValue& pValue, const std::string& pMessage){};
	//-------initMainModule--------//
	void initMainModule();
	//----thread run---//

	void mainstart();
	void mainstop();
	void mainWaitOn();
	void mainWaitOff();
	void startActuator();
	void stopActuator();
	void startTJRobot();
	void stopTJRobot();
	void unregisterMainModule();
    /**
    * Set Motion Cmd from remote.
    * @param motionID ID of motion: walk or special Action?
    * @param minType For Walk, it's WalkType; for Special Action it's action ID
    * @param data The first three are Head Cmd: pan, tilt and speed, if Walk, the latter three are walk speed.
    */
//     void setMotionCmd(const std::string& cmd, const std::string& miniTypef, const std::vector<float>& data);
//     void setEmergencyStop();
/*	void processCMD(const std::string& cmd);*/
private:
	void run();
	void initProvider();
	void startSubThread();
	void stopSubThread();
	SmartPtr<TJRobot>tjrobot;


    SmartPtr<NetWork>network;
	SmartPtr<Actuator>actuator;
// 	RemoteCmdProvider* remoteCmdProvider;
// 	//========Tempory buffer for message queue use====//
// 	MotionCmd mCmd;
	//---add for messageTest-----//
	//SmartPtr<MessageQueue>initMessageQueue;




};


#endif // MyModule_H
