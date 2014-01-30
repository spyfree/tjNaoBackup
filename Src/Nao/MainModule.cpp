/**
 * @author Jerome Monceaux
 * Aldebaran Robotics (c) 2007 All Rights Reserved. This is an example of use.\n
 *
 */

#include <iostream>
#include "MainModule.h"
#include "alproxy.h"
#include "alloggerproxy.h"
#ifdef WIN32
#include "windows.h"
#endif
//#include "Module/Behavior/RunBehav.h"
#include "Platform/SystemCall.h"
using namespace std;
using namespace AL;
ALPtr<ALBroker>test_pBroker;
//______________________________________________
// constructor
//______________________________________________
MainModule::MainModule(ALPtr<ALBroker> pBroker, std::string pName ): ALModule( pBroker, pName ),
//MainModule::MainModule(ALPtr<ALBroker> pBroker): ALModule( pBroker, "MainModule" ),
Thread(SmartPtr<Synchronizer>(new Synchronizer()), "MainModule")
{
  // Describe the module here
  setModuleDescription( "An TJArkNao world app." );
  test_pBroker=pBroker;
  // Define callable methods with there description
   initMainModule();

  initProvider();


 //-------add the Synchro to every thread
   //sensor=SmartPtr<Sensor>(new Sensor());

  network=SmartPtr<NetWork>(new NetWork(theSynchronizer));
  tjrobot=SmartPtr<TJRobot>(new TJRobot(theSynchronizer));
  actuator=SmartPtr<Actuator>(new Actuator(theSynchronizer));

//=Remote Cmd provider
// remoteCmdProvider = NULL;
// remoteCmdProvider = new RemoteCmdProvider();
}

//______________________________________________
// destructor
//______________________________________________
MainModule::~MainModule()
{
    std::cout<<"Destroying MainModule........";
	//std::vector<float> dataWalk(6,0);
	//setMotionCmd("Walk","eCartTableWalk",dataWalk);
// 	SystemCall::sleep(2000);  //2250
// 	std::vector<float> data;
// 	data.reserve(6);
// 	data.resize(6);
// 	setMotionCmd("SpecialAction","NewSitDown",data);
// 	SystemCall::sleep(5000);  //2250
//     if(remoteCmdProvider){
//         delete remoteCmdProvider;
//         }

    std::cout<<"DONE!"<<std::endl;
}
void MainModule::initProvider()
{
	//Online locator
	ADD_REQUIRES(LocSamplesToPc,Robotsamplebuffer)
	ADD_REQUIRES(LocSamplesToPc,Ballsamplebuffer)
	ADD_PROVIDES(LocSamplesToPc,ForSampleToPc)



	//ADD_REQUIRES(NetworkProvider,TeamCfgInfo)
	ADD_PROVIDES(NetworkProvider,RoboCupGameControlData)
	ADD_PROVIDES(NetworkProvider,TeamMateInfo)

	ADD_REQUIRES(GlobalInfoProvider,TeamMateInfo)
	ADD_PROVIDES(GlobalInfoProvider,GlobalInfo)

	//ADD_REQUIRES(NetworkProvider,TJImage)
	//vision Provider
	//ADD_REQUIRES(VisionProvider,TJImage)
	//ADD_PROVIDES(SensorComm,VisionInterface)
	//ADD_PROVIDES(NaoVisionConfigProvider, NaoConfig)
	ADD_REQUIRES(ImageProvider,CameraConfig)
	ADD_PROVIDES(ImageProvider,SafeJointData)
	ADD_PROVIDES(ImageProvider,SafeSensorData)
	ADD_PROVIDES(ImageProvider,KeyStates)
	ADD_REQUIRES(ImageProvider,SafeJointData)
	ADD_REQUIRES(ImageProvider,SafeSensorData)
	ADD_REQUIRES(ImageProvider,KeyStates)
	ADD_PROVIDES(ImageProvider,TJImage)
    ADD_PROVIDES(ImageProvider,FrameInfo)
	ADD_PROVIDES(ImageProvider,OdometryData)

	ADD_PROVIDES(NaoVisionConfigProvider, NaoConfig)
	ADD_PROVIDES(NaoVisionConfigProvider, CameraConfig)
	ADD_PROVIDES(NaoVisionConfigProvider, FieldDimensions)

	ADD_REQUIRES(CameraMatrixProvider, NaoStructure)
	ADD_PROVIDES(CameraMatrixProvider, CameraMatrix)
	//VISION
	//MAKE_KS(SensorComm)
	ADD_REQUIRES(SensorComm,TJImage)
	//ADD_REQUIRES(SensorComm,CameraConfig)
	ADD_REQUIRES(SensorComm,CameraMatrix)
	//ADD_REQUIRES(SensorComm,CameraTopMatrix)
	//ADD_REQUIRES(SensorComm,CameraMatrix)
	ADD_PROVIDES(SensorComm,Regionpercept)
	//ADD_PROVIDES(SensorComm,OdometryData)

	//MAKE_KS(LineSpecialist)
	ADD_REQUIRES(LineSpecialist,TJImage)
	ADD_REQUIRES(LineSpecialist,CameraMatrix)
	//ADD_REQUIRES(LineSpecialist,CameraConfig)
	//ADD_REQUIRES(LineSpecialist,CameraTopMatrix)
	//ADD_REQUIRES(LineSpecialist,CameraMatrix)
	ADD_PROVIDES(LineSpecialist,BallSpots)
	ADD_REQUIRES(LineSpecialist,Regionpercept)
	//ADD_REQUIRES(LineSpecialist,FrameInfo)
	ADD_PROVIDES(LineSpecialist,LinesPercept)
	ADD_PROVIDES(LineSpecialist,BlobPercept)

	//MAKE_KS(GoalSpecialist)
	ADD_REQUIRES(GoalSpecialist,TJImage)
	ADD_REQUIRES(GoalSpecialist,CameraMatrix)
	//ADD_REQUIRES(GoalSpecialist,CameraConfig)
	//ADD_REQUIRES(GoalSpecialist,CameraTopMatrix)
	ADD_REQUIRES(GoalSpecialist,RoboCupGameControlData)
	ADD_REQUIRES(GoalSpecialist,Regionpercept)
	ADD_REQUIRES(GoalSpecialist,FrameInfo)
	ADD_REQUIRES(GoalSpecialist,OdometryData)
	ADD_REQUIRES(GoalSpecialist,RobotPose)
	ADD_PROVIDES(GoalSpecialist,GoalPercept)

	//MAKE_KS(BallSpecialist)
	ADD_REQUIRES(BallSpecialist,TJImage)
	ADD_REQUIRES(BallSpecialist,CameraMatrix)
	//ADD_REQUIRES(BallSpecialist,CameraConfig)
	//ADD_REQUIRES(BallSpecialist,CameraTopMatrix)
	//ADD_REQUIRES(BallSpecialist,CameraMatrix)
	ADD_REQUIRES(BallSpecialist,BallSpots)
	ADD_REQUIRES(BallSpecialist,FrameInfo)
	ADD_PROVIDES(BallSpecialist,BallPercept)

	//RobotSpecialist
	ADD_REQUIRES(RobotSpecialist,BlobPercept)
	ADD_REQUIRES(RobotSpecialist,TJImage)
	ADD_REQUIRES(RobotSpecialist,CameraMatrix)
	ADD_PROVIDES(RobotSpecialist,RobotPercept)

	ADD_REQUIRES(BehaviorProvider,GoalPercept)
	ADD_REQUIRES(BehaviorProvider,BallPercept)
	ADD_REQUIRES(BehaviorProvider,RobotPose)
	ADD_REQUIRES(BehaviorProvider,BallModel)
	ADD_REQUIRES(BehaviorProvider,FreePartOfOpponentGoalModel)
	ADD_REQUIRES(BehaviorProvider,OdometryData)
	//ADD_REQUIRES(BehaviorProvider,TeamMateInfo)
	ADD_PROVIDES(BehaviorProvider,BehaviorData)
	ADD_PROVIDES(BehaviorProvider,MotionCmd)
	ADD_PROVIDES(BehaviorProvider,CameraConfig)
	ADD_PROVIDES(BehaviorProvider,RoboCupGameControlData)
	ADD_PROVIDES(BehaviorProvider,TeamCfgInfo)
	ADD_PROVIDES(BehaviorProvider,RobotInfo)
	ADD_PROVIDES(BehaviorProvider,TeamUDPData)
	ADD_PROVIDES(BehaviorProvider,Ballsamplebuffer)
	ADD_PROVIDES(BehaviorProvider,GlobalInfo)
	//ADD_PROVIDES(BehaviorProvider,LEDRequest)
	//outBeahviorData
	ADD_REQUIRES(OutBehaviorProvider,FallDownState)
	ADD_REQUIRES(OutBehaviorProvider,KeyStates)
	ADD_REQUIRES(OutBehaviorProvider,SafeJointData)
	ADD_REQUIRES(OutBehaviorProvider,NaoStructure)
	ADD_PROVIDES(OutBehaviorProvider,OutBehaviorData)
	//ADD_PROVIDES(BehaviorProvider,JointCmd)
	//-----sensor Provider--------//
	//ADD_REQUIRES(Sensor,NaoConfig)
	ADD_REQUIRES(Sensor,SafeJointData)
	ADD_REQUIRES(Sensor,SafeSensorData)
	ADD_PROVIDES(Sensor,NaoStructure)
	ADD_PROVIDES(Sensor,FallDownState)
	//NaoProvider
	ADD_PROVIDES(NaoProvider,SafeJointData)
    ADD_PROVIDES(NaoProvider,SafeSensorData)


	ADD_PROVIDES(NaoActuatorConfigProvider, NaoConfig)

	ADD_REQUIRES(MotionEngine, MotionCmd)
	ADD_REQUIRES(MotionEngine, SafeJointData)


	//ADD_PROVIDES(MotionEngine, MotionPlan)
	ADD_PROVIDES(MotionEngine, JointCmd)





		//ADD_PROVIDES(ActuatorSimProvider,JointCmd )
// 		ADD_PROVIDES(ActuatorSimProvider, SensoryJointData);
// 		ADD_PROVIDES(ActuatorSimProvider, SafeJointData);
// 		ADD_PROVIDES(ActuatorSimProvider, MotionCmd);
// 		//MAKE_KS(NaoActuatorConfigProvider)
// 		ADD_PROVIDES(NaoActuatorConfigProvider, NaoConfig)
// 		//MAKE_KS(MotionEngine)
// 		ADD_REQUIRES(MotionEngine, MotionCmd)
// 		ADD_REQUIRES(MotionEngine, SafeJointData)
// 		ADD_REQUIRES(MotionEngine, WalkingEngineOutput)
// 		ADD_REQUIRES(MotionEngine, SpecActionOutput)
// 		ADD_PROVIDES(MotionEngine, MotionPlan)
// 		ADD_PROVIDES(MotionEngine, JointCmd)
// 		//MAKE_KS(WalkingEngine)
// 		ADD_REQUIRES(WalkingEngine, MotionPlan)
// 		ADD_REQUIRES(WalkingEngine, NaoStructure)
// 		ADD_PROVIDES(WalkingEngine, WalkingEngineOutput)
// 		//SpecActionEngine
// 		ADD_REQUIRES(SpecActionEngine,MotionPlan)
// 		ADD_REQUIRES(SpecActionEngine,SensoryJointData)
// 		ADD_REQUIRES(SpecActionEngine,SafeJointData)
// 		ADD_PROVIDES(SpecActionEngine,SpecActionOutput)

		//locator

		ADD_REQUIRES(BallLocator,BallPercept)
		ADD_REQUIRES(BallLocator,OdometryData)
		ADD_REQUIRES(BallLocator,RobotPose)
		ADD_REQUIRES(BallLocator,CameraMatrix)
		ADD_REQUIRES(BallLocator,FieldDimensions)
		ADD_REQUIRES(BallLocator,FrameInfo)
		//ADD_REQUIRES(BallLocator,MotionCmd)
		ADD_PROVIDES(BallLocator,BallModel)
		ADD_PROVIDES(BallLocator,Ballsamplebuffer)



		//selfLocator
		ADD_REQUIRES(SelfLocator, OdometryData)
		ADD_REQUIRES(SelfLocator, GoalPercept)
		ADD_REQUIRES(SelfLocator, BallPercept)
		ADD_REQUIRES(SelfLocator, LinesPercept)
		ADD_REQUIRES(SelfLocator, RoboCupGameControlData)
		ADD_REQUIRES(SelfLocator, RobotInfo)
		//ADD_REQUIRES(SelfLocator, TeamCfgInfo)
		ADD_REQUIRES(SelfLocator, FrameInfo)
		ADD_REQUIRES(SelfLocator, FieldDimensions)
		ADD_REQUIRES(SelfLocator, CameraMatrix)
		ADD_PROVIDES(SelfLocator, RobotPose)
		ADD_PROVIDES(SelfLocator,Robotsamplebuffer)
		//ADD_PROVIDES(SelfLocator, RobotPoseHypotheses)
		//ADD_PROVIDES(SelfLocator, SelfLocatorSampleSet)

		//MAKE_KS(RobotPoseValidator)
		ADD_REQUIRES(RobotPoseValidator,RobotPose)
	    ADD_REQUIRES(RobotPoseValidator,CameraMatrix)
		ADD_REQUIRES(RobotPoseValidator,FieldDimensions)
	    ADD_REQUIRES(RobotPoseValidator,LinesPercept)
		ADD_REQUIRES(RobotPoseValidator,BallPercept)
	    ADD_REQUIRES(RobotPoseValidator,OdometryData)
		ADD_REQUIRES(RobotPoseValidator,GoalPercept)
		ADD_REQUIRES(RobotPoseValidator,RoboCupGameControlData)
	    ADD_REQUIRES(RobotPoseValidator,FallDownState)
		ADD_REQUIRES(RobotPoseValidator,FrameInfo)
		ADD_REQUIRES(RobotPoseValidator,TeamCfgInfo)
		ADD_REQUIRES(RobotPoseValidator,GlobalInfo)
		ADD_REQUIRES(RobotPoseValidator,Robotsamplebuffer)
		ADD_PROVIDES(RobotPoseValidator,RobotPose)
		ADD_PROVIDES(RobotPoseValidator,Robotsamplebuffer)

		//Ks FreePartOfOpponentGoal
		ADD_PROVIDES(FreePartOfOpponentGoalProvider,FreePartOfOpponentGoalModel)
		ADD_PROVIDES(FreePartOfOpponentGoalProvider,FreePartData);
		ADD_REQUIRES(FreePartOfOpponentGoalProvider,RobotPercept)
		ADD_REQUIRES(FreePartOfOpponentGoalProvider,CameraMatrix)
		ADD_REQUIRES(FreePartOfOpponentGoalProvider,RobotPose)
		ADD_REQUIRES(FreePartOfOpponentGoalProvider,BallModel)
		ADD_REQUIRES(FreePartOfOpponentGoalProvider,FieldDimensions)

		//OdometryDataProvider
		ADD_REQUIRES(OdometryDataProvider,NaoStructure)
		//ADD_REQUIRES(OdometryDataProvider,MotionPlan)
		ADD_REQUIRES(OdometryDataProvider,SafeSensorData)
		//ADD_PROVIDES(OdometryDataProvider,InertiaMatrix)
		ADD_PROVIDES(OdometryDataProvider,OdometryData)

		//RobotStateProvider
		ADD_REQUIRES(RobotStateProvider,NaoStructure)
		ADD_REQUIRES(RobotStateProvider,SensoryJointData)
		ADD_REQUIRES(RobotStateProvider,SafeSensorData)
		ADD_REQUIRES(RobotStateProvider,SensorData)
		ADD_REQUIRES(RobotStateProvider,ActuatorFrameInfo)
		ADD_PROVIDES(RobotStateProvider,RobotState)

		//VisionPercept
		ADD_REQUIRES(VisionPercept,CameraMatrix)
		ADD_REQUIRES(VisionPercept,BallPercept)
		ADD_REQUIRES(VisionPercept,LinesPercept)
		ADD_REQUIRES(VisionPercept,GoalPercept)
		ADD_REQUIRES(VisionPercept,Regionpercept)
		ADD_PROVIDES(VisionPercept,TJImage)
		ADD_PROVIDES(VisionPercept,netVisionInterface)
        //Online locator
		ADD_REQUIRES(LocSamplesToPc,Robotsamplebuffer)
		ADD_REQUIRES(LocSamplesToPc,Ballsamplebuffer)
		ADD_REQUIRES(LocSamplesToPc,FreePartData)
		ADD_PROVIDES(LocSamplesToPc,ForSampleToPc)


}
void MainModule::initMainModule()
{
    //std::cout<<"Binding methods to MainModule....";
	//std::cout<<"print getName()"<<getName()<<std::endl;
    //functionName( "NaoMain", getName(),  "print TJArkNao to the ALLogger module." );
	//BIND_METHOD( MainModule::NaoMain );
	functionName( "mainstart", getName(),  "print TJArkNao to the ALLogger module." );
	BIND_METHOD( MainModule::mainstart );
	functionName( "mainstop", getName(),  "print TJArkNao to the ALLogger module." );
	BIND_METHOD( MainModule::mainstop );
//	functionName( "mainWaitOn", "MainModule",  "print TJArkNao to the ALLogger module." );
//	BIND_METHOD( MainModule::mainWaitOn );
//	functionName( "mainWaitOff", "MainModule",  "print TJArkNao to the ALLogger module." );
//	BIND_METHOD( MainModule::mainWaitOff );
    functionName( "startActuator", getName(),  "Start Actuator Thread if is not running." );
	BIND_METHOD( MainModule::startActuator );
    functionName( "stopActuator", getName(),  "Stop Actuator Thread if is running." );
	BIND_METHOD( MainModule::stopActuator );

	functionName( "startTJRobot", getName(),  "Start TJRobot Thread if is not running." );
	BIND_METHOD( MainModule::startTJRobot );
	functionName( "stopTJRobot", getName(),  "Stop TJRobot Thread if is running." );
	BIND_METHOD( MainModule::stopTJRobot );



//     functionName("setMotionCmd", "MainModule", "Set Motion Cmd from remote.");
//  //   addParam( "motionID", "ID of motion: walk or special Action?");
//  //   addParam( " minType", "For Walk, it's WalkType; for Special Action it's action ID");
//  //   addParam( "data", "The first three are Head Cmd: pan, tilt and speed, if Walk, the latter three are walk speed.");
//     BIND_METHOD(MainModule::setMotionCmd);
//     functionName( "setEmergencyStop", "MainModule",  "Set actuator emergency stop, and set body hardness to zero." );
// 	BIND_METHOD( MainModule::setEmergencyStop );
	//functionName("processCMD","MainModule","send CMD from remote.");
	//BIND_METHOD(MainModule::processCMD);

    //functionName( "unregisterMainModule", "MainModule",  "Unregister MainModule and Exit TJPro" );
	//BIND_METHOD( MainModule::unregisterMainModule );

    std::cout<<"     DONE!"<<std::endl;
};
/**
 * helloworld Function
 */
void MainModule::NaoMain(  )
{

  try
  {
    getParentBroker()->getLoggerProxy()->info( "MainModule", "TJArkNao!" );
  }catch( ALError& )
  {
    std::cout << "could not create a proxy to ALLogger module" << std::endl;
  }
}
void MainModule::mainstart()
{
   SystemCall::sleep(5000);
    std::cout<<"Starting MainModule Running...."<<std::endl;
	Thread::start();


}
void MainModule::mainstop()
{
    std::cout<<"Stopping MainModule Running...."<<std::endl;
    Thread::stop();
}
void MainModule::mainWaitOn()
{
	getTrigger()->wait_on();
}
void MainModule::mainWaitOff()
{
	getTrigger()->wait_off();
}
void MainModule::startActuator()
{
if(!actuator->isRunning()){
	std::cout<<"=======startActuator======in MainModule======"<<endl;
    actuator->start();
    }
}
void MainModule::stopActuator()
{
if(actuator->isRunning()){
    actuator->stop();
    }
}
void MainModule::startTJRobot()
{
	if(!tjrobot->isRunning()){
		std::cout<<"=======startTJRobot======in MainModule======"<<endl;
		tjrobot->start();
	}
}
void MainModule::stopTJRobot()
{
	if(tjrobot->isRunning()){
		tjrobot->stop();
		std::cout<<"=======stopTJRobot======in MainModule======"<<endl;
	}
}
void MainModule::unregisterMainModule()
{
    if(Thread::isRunning()){
            mainstop();
    }
    std::cout<<"Unregistering MainModule....."<<std::endl;
    try{
        test_pBroker->unregisterModule("MainModule");
   }catch(AL::ALError &e){
		cout << "Failed to unregisterModule 'MainModule' error: "<<e.toString().c_str()<<endl;
	}
}

// void MainModule::setMotionCmd(const std::string& cmdstr, const std::string& miniTypestr, const std::vector<float>& data)
// {
// 	//std::cout<<"behavior send data to motion!!!"<<std::endl;
//     if(actuator != NULL && actuator->isRunning())
//     {
//         //MotionCmd mCmd;
//         bool error = false;
//         int motionID = 1;//(int)cmd[0];
//         int miniType = 1;//miniTypef;
//         //std::cout<<"MainModule::setMotionCmd: "<<cmdstr.c_str()<<" ,"<<miniTypestr.c_str()<<std::endl;
//         mCmd.motion = (MotionCmd::MotionID)motionID;
//         if(strcmp(cmdstr.c_str(), MotionCmd::getMotionNameByID(MotionCmd::Stand))==0)
//         {
//             //std::cout<<"MainModule::setMotionCmd Recived Stand Cmd"<<std::endl;
//             mCmd.motion = MotionCmd::Stand;
//
//             if((mCmd.walkCmd.walkType =WalkCmd::getWalkTypeByName(miniTypestr.c_str())) == WalkCmd::numWalkTypes)
//                 mCmd.walkCmd.walkType = WalkCmd::eCartTableWalk;
//             mCmd.headCmd.pan = 0;
//             mCmd.headCmd.tilt = 0;
//             mCmd.headCmd.speed = 0;
//             mCmd.walkCmd.speed.translation.x = 0;
//             mCmd.walkCmd.speed.translation.y = 0;
//             mCmd.walkCmd.speed.rotation = 0;
//         }
//         else if(strcmp(cmdstr.c_str(), MotionCmd::getMotionNameByID(MotionCmd::Walk))==0)
//         {
//             mCmd.motion = MotionCmd::Walk;
//             //std::cout<<"MainModule::setMotionCmd Recived Walk Cmd"<<std::endl;
//             if((mCmd.walkCmd.walkType =WalkCmd::getWalkTypeByName(miniTypestr.c_str())) == WalkCmd::numWalkTypes)
//                 mCmd.walkCmd.walkType = WalkCmd::eCartTableWalk;
// 			//std::cout<<"data.size()="<<data.size()<<std::endl;
//             if(data.size() ==6)
//             {
//                 mCmd.headCmd.pan = data[0];
//                 mCmd.headCmd.tilt = data[1];
//                 mCmd.headCmd.speed = data[2];
//                 mCmd.walkCmd.speed.translation.x = data[3];
//                 mCmd.walkCmd.speed.translation.y = data[4];
//                 mCmd.walkCmd.speed.rotation = data[5];
// 				/*std::cout<<"=====pan="<<mCmd.headCmd.pan<<std::endl;
// 				std::cout<<"=====tilt="<<mCmd.headCmd.tilt<<std::endl;*/
//             }else{
//                error = true;
//             }
//
//         }else if(strcmp(cmdstr.c_str(), MotionCmd::getMotionNameByID(MotionCmd::SpecialAction))==0)
//         {
//             mCmd.motion = MotionCmd::SpecialAction;
//             mCmd.specActionCmd.specAction =SpecActionCmd::getSpecActionIDbyName(miniTypestr.c_str());
//                 //std::cout<<"+++=id---"<< mCmd.specActionCmd.specAction <<endl;
//             if(mCmd.specActionCmd.specAction == SpecActionCmd::numSpecActions)
//
//                 mCmd.specActionCmd.specAction = SpecActionCmd::Stand;
//         }else{
//             error = true;
//         }
//         //for testing...
//         /*if(true){
//             error = false;
//             mCmd.motion = MotionCmd::Walk;
//             mCmd.walkCmd.walkType = WalkCmd::eCartTableWalk;
//             mCmd.headCmd.pan = 0;
//             mCmd.headCmd.tilt = 0;
//             mCmd.headCmd.speed = 0;
//             mCmd.walkCmd.speed.translation.x = 3.6;
//             mCmd.walkCmd.speed.translation.y = 0;
//             mCmd.walkCmd.speed.rotation = 0;
//         }*/
//         if(!error){
//             //std::cout<<"setting Remote Motion Cmd..."<<std::endl;
//             remoteCmdProvider->setMotionCmd(mCmd);
//         }else{
//            // std::cout<<"Remote Motion Cmd Error!!"<<std::endl;
//         }
//        // std::cout<<"REMOTE CMD: Motion: "<<mCmd.motion<<", Type: "<<miniTypestr.c_str();
//         for(int i = 0;i <6/* data.size()*/; ++i){
//             if(i<data.size())
//                 std::cout <<", "<<data[i];
//             else
//                 break;
//         }
//         std::cout<<std::endl;
//     }
//     // std::cout<<"mCmd.specActionCmd.specAction"<<mCmd.specActionCmd.specAction<<std::endl;
// }

// void MainModule::processCMD(const std::string& cmd)
// {
// 	if(cmd == "stopB")
// 		tjrobot->stop();
// 	else if(cmd == "startB")
// 		tjrobot->start();
// }
//
// void MainModule::setEmergencyStop()
// {
//     if(actuator->isRunning()){
//         actuator->setEmergencyStop();
//     }
// }

void MainModule::run()
{
	startSubThread();
	running=true;
	spMyTrigger->on();

	while(running)
	{
        SystemCall::sleep(1000);
	}

	stopSubThread();
	running=false;
   spMyTrigger->off();
   std::cout<<"Exit MainModule::run()...."<<std::endl;
   unregisterMainModule();
}
void MainModule::startSubThread()
{



 	if(network->start()!=0)
 	{
 		std::cout<<"network error"<<std::endl;
 	}
 	else
 	{
 		network->getTrigger()->wait_on();
 	}




	if(actuator->start()!=0)
	{
		std::cout<<"actuator error"<<std::endl;
	}
	else
	{
		actuator->getTrigger()->wait_on();
	}



	if(tjrobot->start()!=0)

	{
		std::cout<<"start run TJRobot Thread"<<std::endl;
	}
	else
	{
		tjrobot->getTrigger()->wait_on();

	}

}

void MainModule::stopSubThread()
{

   std::cout<<"Stopping tjrobot thread....."<<std::endl;
   if(tjrobot->isRunning())
   {
      tjrobot->stop();
       tjrobot->getTrigger()->wait_off();
  }
   std::cout<<"tjrobot thread stopped!"<<std::endl;


 std::cout<<"Stopping actuator thread....."<<std::endl;
    if(actuator->isRunning()){
        actuator->stop();
        actuator->getTrigger()->wait_off();
    }
    std::cout<<"Actuator thread stopped!"<<std::endl;
   if(network->isRunning())
   {
       network->stop();
       network->getTrigger()->wait_off();
   }

}





