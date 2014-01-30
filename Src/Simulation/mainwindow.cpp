#include <Qt>
#include "mainwindow.h"
#include "Module/Sensor/Sensor.h"
#include "Module/Vision/NaoVisionConfigProvider.h"
#include "Module/Vision/CameraMatrixProvider.h"
#include "Module/Actuator/NaoActuatorConfigProvider.h"
#include "Module/Actuator/RobotStateProvider.h"
#include "Module/Actuator/MotionEngine.h"
#include "Module/Actuator/WalkingEngine.h"
#include "Module/Actuator/SpecActionEngine.h"
#include "Module/Actuator/RobotStateProvider.h"
#include "Module/Sensor/OdometryDataProvider.h"
#include "Simulation/NaoProvider/ActuatorSimProvider.h"
#include "Simulation/ODESimulation/SimulationGUI/Document.h"
#include "Simulation/ODESimulation/SimulationGUI/ObjectWindow.h"
#include "Module/Vision/VisionProvider.h"
#include "Module/Modeling/LogPercept.h"
#include "Module/Modeling/SelfLocator/SelfLocator.h"
#include "Module/Modeling/BallLocator/BallLocator.h"
#include "Module/Modeling/SelfLocator/FreePartOfOpponentGoalModelProvider.h"
#include "Module/Modeling/SelfLocator/RobotPoseValidator.h"
#include "Module/Vision/RobotSpecialist.h"
#include "Simulation/ODESimulation/ODEprocessor/ODEmassage.h"
#include "Simulation/ODESimulation/ODEprocessor/ODECameraProvider.h"
#include "Simulation/ODESimulation/ODEprocessor/ODEOutBehaviorProvider.h"
#include "Module/Behavior/RobotSimProvider.h"
#include "Simulation/NaoProvider/VisionSimulation.h"
#include "Simulation/SignalControlWnd.h"
//#include "Module/Sensor/OdometryDataProvider.h"

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent), document(0), objectWindow(0)

{
	setWindowTitle(tr("TJRobot"));
	setWindowIcon(QIcon(":/icons/Simulator.png"));
	setGeometry (0, 30, 1200, 600);//100, 100, 800, 600
	//setFixedSize(800,600);
	//resize(800, 600);
	///////pointer init
	selfMenuBar=NULL;
	selfMenuBar=new MenuBar(this);
	setMenuBar(selfMenuBar);
	selfToolBar=NULL;
	selfToolBar =new ToolBar(this);
	addToolBar(selfToolBar);

	spSynchronizer = SmartPtr<Synchronizer>(new Synchronizer());
	actuatorThread = new Actuator(spSynchronizer);
	tjRobotThread = new TJSimRobot(spSynchronizer);
	//============register synchronizer===============
	spSynchronizer->registerEvent("Actuator->ODESimulation");
	spSynchronizer->registerEvent("ODESimulation->Actuator");
	spSynchronizer->registerEvent("Behavior->ImageView");
	spSynchronizer->registerEvent("Behavior->VOffline");//VOffline = offline of vision
	spSynchronizer->registerEvent("VOffline->Behavior");
	//==========================================

	mdiArea=NULL;
	mdiArea= new QMdiArea;

	motionWindow=NULL;
	motionWindow=new MotionWindow(mdiArea);
	//mdiArea->addSubWindow(motionWindow);
	motionWindow->setVisible(false);

	connectWindow=NULL;
	connectWindow=new ConnectWindow(mdiArea);
	connectWindow->setVisible(false);
	//mdiArea->addSubWindow(connectWindow);

	pImageDialog=NULL;
	pImageDialog = new ImageDialog(tjRobotThread,spSynchronizer);
	pClassifyImageDialog=NULL;
	pClassifyImageDialog = new ClassifyImageDialog();

	tableWindow=NULL;
	tableWindow=new TableWindow(mdiArea);
	tableWindow->setVisible(false);

	visionResultWindow=NULL;
	visionResultWindow=new VResultWindow(mdiArea);                              
	visionResultWindow->setVisible(false);

	colorSpaceWindow=NULL;
	colorSpaceWindow=new ColorSpaceWindow(mdiArea,"");
	colorSpaceWindow->setVisible(false);


	

	subwindows1=NULL;
	subwindows1=new SubWindow(mdiArea,"Image");
	subwindows1->setWidget(pImageDialog);
	subwindows1->setGeometry(0,0,1140,600);//(0,0,1140,500);
	subwindows1->setWindowFlags(Qt::WindowMinimizeButtonHint);
	subwindows1->setVisible(false);
	mdiArea->addSubWindow(subwindows1);

	subwindows2 =  NULL;
	subwindows2 = new SubWindow(mdiArea,"Classify Image");
	subwindows2->setWidget(pClassifyImageDialog);
	subwindows2->setGeometry(0,0,700,1500);
	subwindows2->setWindowFlags(Qt::WindowMinimizeButtonHint);
	subwindows2->setVisible(false);
	mdiArea->addSubWindow(subwindows2);

	setCentralWidget(mdiArea);	

	tcpContorl=NULL;
	tcpContorl=new ManageTcp;

	IsConnect=false;
#ifdef DEBUG_NETWORK
	dialog=NULL;
	dialog=new ClientDialog(this);
	dialog->show();
#endif

	treeDockWidget=NULL;
 	 treeDockWidget =new Tree;
	 addDockWidget(Qt::LeftDockWidgetArea,treeDockWidget);


	 ImageToolBox=NULL;
 	 ImageToolBox =  new ClassifyImageToolBox;
 	 addDockWidget(Qt::LeftDockWidgetArea,ImageToolBox);
	 locatorWindow=NULL;
	 locatorWindow=new LocatorWindow(mdiArea);
	 locatorWindow->setVisible(false);

	 pMotionEngineWnd = NULL;
	 pBehaviorDataWnd = NULL;
	 signalControlWnd = NULL;
	 signalControlWnd = new SignalControlWnd(mdiArea);
	 signalControlWnd->setWindowFlags(Qt::WindowMinimizeButtonHint);//在一次调试中只被初始一次，所以不能被关闭
	 signalControlWnd->setVisible(false);
     locOnlineControlWnd=NULL;
	 locOnlineControlWnd=new LocOnlineControlWnd(mdiArea);
	 locOnlineControlWnd->setVisible(false);
 
	CreatConnect();
	getInitInfo();
	//=========ADD KS PROVIDERS AND REQUIRES HERE!!=================//
	buildModules();

	const char* test = MotionCmd::getMotionNameByID(MotionCmd::Walk);
	std::string cmp("Walk");
	const char* test2 = cmp.c_str();
	int result = strcmp(cmp.c_str(), MotionCmd::getMotionNameByID(MotionCmd::Walk));


//	actuatorThread->start();
// 	SystemCall::sleep(2000);
// 	actuatorThread->stop();
// 	SystemCall::sleep(2000);
// 	actuatorThread->start();
	//ADD_PROVIDES(VisionSimulation,Percept)
	OpenSimulatorWindow();
	//OpenMotionEngineWindow();
	
	createSaTools();
	createSensorWidget();
	//StartBehaviorThread();//just for vision debug ,can not be upload 

}


 MainWindow::~MainWindow()
 {
	 tcpContorl->Dodiscon();
	 if (selfMenuBar!=NULL)
	 {
		 delete selfMenuBar;
		 selfMenuBar=NULL;
	 }
	 if (selfToolBar!=NULL)
	 {
		 delete selfToolBar;
		 selfToolBar=NULL;
	 }
	 if(signalControlWnd != NULL)
	 {
		 delete signalControlWnd;
		 signalControlWnd = NULL;
	 }
	 if(locOnlineControlWnd!=NULL)
	 {
		 delete locOnlineControlWnd;
		 locOnlineControlWnd=NULL;
	 }
	 if (motionWindow!=NULL)
	 {
		 delete motionWindow;
		 motionWindow=NULL;
	 }
	 if (connectWindow!=NULL)
	 {
		 delete connectWindow;
		 connectWindow=NULL;
	 }
	 if (pImageDialog!=NULL)
	 {
		 delete pImageDialog;
		 pImageDialog=NULL;
	 }
	 if (subwindows1!=NULL)
	 {
		 delete subwindows1;
		 subwindows1=NULL;
	 }
	 if (pClassifyImageDialog!=NULL)
	 {
		 delete pClassifyImageDialog;
		 pClassifyImageDialog =  NULL;
	 }

	if (subwindows2!=NULL)
	{
		delete subwindows2;
		subwindows2 = NULL;
	}

	if (mdiArea!=NULL)
	{
		delete mdiArea;
		mdiArea=NULL;
	}
	if (tcpContorl!=NULL)
	{
		delete tcpContorl;
		tcpContorl=NULL;
	}
	if(tableWindow!=NULL)
	{
		delete tableWindow;
		tableWindow=NULL;
	}
	if(visionResultWindow!=NULL)
	{
		delete visionResultWindow;
		visionResultWindow=NULL;
	}

	if (colorSpaceWindow!=NULL)
	{
		delete colorSpaceWindow;
		colorSpaceWindow=NULL;
	}
	if (locatorWindow!=NULL)
	{
		delete locatorWindow;
		locatorWindow = NULL;
	}
#ifdef DEBUG_NETWORK
	if (dialog!=NULL)
	{
		delete dialog;
		dialog=NULL;
	}
#endif

	 if (treeDockWidget!=NULL)
	 {
		 delete treeDockWidget;
		 treeDockWidget=NULL;
	 }
	 if (ImageToolBox!=NULL)
	 {
		 delete ImageToolBox;
		 ImageToolBox =  NULL;
	 }

	 if(actuatorThread)
	 {
		 delete actuatorThread;
		 actuatorThread = NULL;
	 }

	 if(tjRobotThread)
	 {
		 delete tjRobotThread;
		 tjRobotThread = NULL;
	 }
	 if(document)
	 {
		 document->closeSlot();
	 }

	/* if(pMotionEngineWnd)
	 {
		delete pMotionEngineWnd;
		pMotionEngineWnd = NULL;
	 }*/
	 //if(pBehaviorDataWnd)
	 //{
		// delete pBehaviorDataWnd;
		// pBehaviorDataWnd = NULL;
	 //}

 }
 void MainWindow::closeEvent(QCloseEvent* event)
 {
	 if(tjRobotThread->isRunning())
	 {
		 QMessageBox::warning(this,tr("Forget Close Behavior Thread!"),
			 tr("Before you close MainWindow, please remember to stop behavior thread!"));
		 tjRobotThread->stop();
	 }
	 if(actuatorThread->isRunning())
	 {
		 QMessageBox::warning(this,tr("Forget Close Actuator Thread!"),
			 tr("Before you close MainWindow, please remember to stop actuator thread!"));
		 actuatorThread->stop();
	 }
	 setVisible(false); // optical close speedup
	 document->closeSlot();

	 QMainWindow::closeEvent(event);
 }
void MainWindow::buildModules()
{
	//=========ADD KS PROVIDERS AND REQUIRES HERE!!=================//

	//MAKE_KS(Sensor)
	ADD_REQUIRES(Sensor,SafeJointData)
	ADD_REQUIRES(Sensor,SafeSensorData)
	ADD_PROVIDES(Sensor,NaoStructure)
	ADD_PROVIDES(Sensor,FallDownState)

	//MAKE_KS(NaoVisionConfigProvider)
	ADD_PROVIDES(NaoVisionConfigProvider, NaoConfig)
	ADD_PROVIDES(NaoVisionConfigProvider, CameraConfig)

	//MAKE_KS(CameraMatrixProvider)
	ADD_REQUIRES(CameraMatrixProvider, NaoStructure)
	ADD_REQUIRES(CameraMatrixProvider,CameraConfig)
	ADD_PROVIDES(CameraMatrixProvider,CameraMatrix)
	//ADD_PROVIDES(CameraMatrixProvider, CameraTopMatrix)
	//ADD_PROVIDES(CameraMatrixProvider, CameraMatrix)

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
	//ADD_REQUIRES(GoalSpecialist,CameraMatrix)
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
  
    //MAKE_KS(RobotSpecialist)
	ADD_REQUIRES(RobotSpecialist,BlobPercept)
	ADD_REQUIRES(RobotSpecialist,TJImage)
	ADD_REQUIRES(RobotSpecialist,CameraMatrix)
	ADD_PROVIDES(RobotSpecialist,RobotPercept)

	//MAKE_KS(VisionSimulation)
	ADD_PROVIDES(VisionSimulation,TJImage)
	ADD_PROVIDES(VisionSimulation,FrameInfo)
	ADD_PROVIDES(VisionSimulation,SafeJointData)
	ADD_PROVIDES(VisionSimulation,SafeSensorData)
	
	ADD_REQUIRES(VisionProvider,TJImage)
	ADD_REQUIRES(VisionProvider,SafeJointData)
	ADD_REQUIRES(VisionProvider,SafeSensorData)
	ADD_PROVIDES(VisionProvider,FrameInfo)

	//MAKE_KS(ActuatorSimProvider)
	ADD_REQUIRES(ActuatorSimProvider, JointCmd);
	ADD_PROVIDES(ActuatorSimProvider, SensoryJointData);
	ADD_PROVIDES(ActuatorSimProvider, SafeJointData);
	ADD_PROVIDES(ActuatorSimProvider, MotionCmd);
	ADD_PROVIDES(ActuatorSimProvider, SensorData);
	ADD_PROVIDES(ActuatorSimProvider, SafeSensorData);
    ADD_PROVIDES(ActuatorSimProvider, ActuatorFrameInfo);
    
	//MAKE_KS(NaoActuatorConfigProvider)
	ADD_PROVIDES(NaoActuatorConfigProvider, NaoConfig)

   //MAKE_KS( OdometryDataProvider)
   ADD_REQUIRES(OdometryDataProvider,NaoStructure)
   ADD_REQUIRES(OdometryDataProvider,SafeSensorData)
   //ADD_REQUIRES(OdometryDataProvider,MotionPlan)
   //ADD_PROVIDES(OdometryDataProvider,InertiaMatrix)
   ADD_PROVIDES(OdometryDataProvider,OdometryData)
   //MAKE_KS(RobotStateProvider)
   ADD_REQUIRES(RobotStateProvider, NaoStructure)
   //REQUIRES(SensoryJointData)
   ADD_REQUIRES(RobotStateProvider, SafeSensorData)
   ADD_PROVIDES(RobotStateProvider, RobotState)

	//MAKE_KS(MotionEngine)
	ADD_REQUIRES(MotionEngine, MotionCmd)
	ADD_REQUIRES(MotionEngine, SafeJointData)
	ADD_REQUIRES(MotionEngine, WalkingEngineOutput)
	ADD_REQUIRES(MotionEngine, SpecActionOutput)
	ADD_PROVIDES(MotionEngine, MotionPlan)
	ADD_PROVIDES(MotionEngine, JointCmd)

	//MAKE_KS(WalkingEngine)
	ADD_REQUIRES(WalkingEngine, MotionPlan)
	ADD_REQUIRES(WalkingEngine, NaoStructure)
	ADD_PROVIDES(WalkingEngine, WalkingEngineOutput)
	/*ADD_REQUIRES(OdometryDataProvider, NaoStructure)
	ADD_REQUIRES(OdometryDataProvider, SafeSensorData)
	ADD_PROVIDES(OdometryDataProvider, InertiaMatrix)
	ADD_PROVIDES(OdometryDataProvider, OdometryData)*/

	//SpecActionEngine
	ADD_REQUIRES(SpecActionEngine,MotionPlan)
	ADD_REQUIRES(SpecActionEngine,SensoryJointData)
	ADD_REQUIRES(SpecActionEngine,SafeJointData)
	ADD_PROVIDES(SpecActionEngine,SpecActionOutput)

	//MAKE_KS(BehaviorProvider)
	ADD_REQUIRES(BehaviorSimProvider,GoalPercept)
	ADD_REQUIRES(BehaviorSimProvider,BallPercept)
	ADD_REQUIRES(BehaviorSimProvider,RobotPercept)
	ADD_REQUIRES(BehaviorSimProvider,RobotPose)
	ADD_REQUIRES(BehaviorSimProvider,BallModel)
	ADD_REQUIRES(BehaviorSimProvider,FreePartOfOpponentGoalModel)
	//ADD_REQUIRES(BehaviorSimProvider,TeamMateInfo)
	ADD_REQUIRES(BehaviorSimProvider,OutBehaviorData)
	ADD_REQUIRES(BehaviorSimProvider,RoboCupGameControlData)
	ADD_PROVIDES(BehaviorSimProvider,BehaviorData)
	ADD_PROVIDES(BehaviorSimProvider,MotionCmd)
	ADD_PROVIDES(BehaviorSimProvider,RoboCupGameControlData)
	ADD_PROVIDES(BehaviorSimProvider,TeamCfgInfo)
	ADD_PROVIDES(BehaviorSimProvider,RobotInfo)
	
	//Behavior
	ADD_PROVIDES(ODEData,SensoryJointData)
	ADD_PROVIDES(ODEData,SafeSensorData)
	ADD_PROVIDES(ODEData,OdometryData)
	ADD_PROVIDES(ODEData,NaoStructure)
	ADD_REQUIRES(ODEData,CameraMatrix)
	//ADD_PROVIDES(ODECameraProvider,CameraTopMatrix)
	//ADD_PROVIDES(ODECameraProvider,CameraMatrix)
	ADD_PROVIDES(ODECameraProvider,CameraMatrix)
	ADD_REQUIRES(ODECameraProvider,NaoStructure)
	ADD_PROVIDES(ODEOutBehaviorProvider,OutBehaviorData)
	ADD_REQUIRES(ODEOutBehaviorProvider,FallDownState)
	ADD_REQUIRES(ODEOutBehaviorProvider,NaoStructure)
	/*ADD_REQUIRES(SimOutBehaviorProvider,KeyStates)*/
	ADD_REQUIRES(ODEOutBehaviorProvider,SensoryJointData)

	//MAKE_KS(LogPercept)
	ADD_PROVIDES(LogPercept,GoalPercept)
	ADD_PROVIDES(LogPercept,LinesPercept)
	ADD_PROVIDES(LogPercept,OdometryData)
	ADD_PROVIDES(LogPercept,RoboCupGameControlData)
	ADD_PROVIDES(LogPercept,RobotInfo)
	//ADD_PROVIDES(LogPercept,TeamCfgInfo)
	ADD_PROVIDES(LogPercept,FrameInfo)
	ADD_PROVIDES(LogPercept,FieldDimensions)
	ADD_PROVIDES(LogPercept,CameraMatrix)
	ADD_PROVIDES(LogPercept,BallPercept)
	ADD_PROVIDES(LogPercept,MotionCmd)

	//MAKE_KS(SelfLocator)
	ADD_PROVIDES(SelfLocator,RobotPose)
	ADD_REQUIRES(SelfLocator,OdometryData)
	ADD_REQUIRES(SelfLocator,GoalPercept)
	ADD_REQUIRES(SelfLocator,LinesPercept)
	ADD_REQUIRES(SelfLocator, BallPercept)
	ADD_REQUIRES(SelfLocator,RoboCupGameControlData)
	ADD_REQUIRES(SelfLocator,RobotInfo)
	//ADD_REQUIRES(SelfLocator,TeamCfgInfo)
	ADD_REQUIRES(SelfLocator,FrameInfo)
	ADD_REQUIRES(SelfLocator,FieldDimensions)
	ADD_REQUIRES(SelfLocator,CameraMatrix)
    
    //MAKE_KS(RobotPoseValidator)
	ADD_REQUIRES(RobotPoseValidator,RobotPose)
	ADD_REQUIRES(RobotPoseValidator,CameraMatrix)
	ADD_REQUIRES(RobotPoseValidator,FieldDimensions)
	ADD_REQUIRES(RobotPoseValidator,LinesPercept)
	ADD_REQUIRES(RobotPoseValidator,OdometryData)
	ADD_REQUIRES(RobotPoseValidator,BallPercept)
	ADD_REQUIRES(RobotPoseValidator,TeamCfgInfo)
	ADD_REQUIRES(RobotPoseValidator,GoalPercept)
	ADD_REQUIRES(RobotPoseValidator,FallDownState)
	ADD_REQUIRES(RobotPoseValidator,FrameInfo)
    ADD_PROVIDES(RobotPoseValidator,RobotPose)


	//MAKE_KS(KalBallLocator)
	ADD_PROVIDES(BallLocator,BallModel)
	//ADD_PROVIDES(BallLocator,BallLocatorSampleSet)
	ADD_REQUIRES(BallLocator,RobotPose)
	ADD_REQUIRES(BallLocator,BallPercept)
	ADD_REQUIRES(BallLocator,OdometryData)
	ADD_REQUIRES(BallLocator,CameraMatrix)
	ADD_REQUIRES(BallLocator,FieldDimensions)
	ADD_REQUIRES(BallLocator,FrameInfo)
	//ADD_REQUIRES(BallLocator,MotionCmd)
     
	//FreePartOfOpponentGoalProvider
	ADD_PROVIDES(FreePartOfOpponentGoalProvider,FreePartOfOpponentGoalModel)
	ADD_PROVIDES(FreePartOfOpponentGoalProvider,FreePartData);
	ADD_REQUIRES(FreePartOfOpponentGoalProvider,RobotPercept)
	ADD_REQUIRES(FreePartOfOpponentGoalProvider,CameraMatrix)
    ADD_REQUIRES(FreePartOfOpponentGoalProvider,RobotPose)
	ADD_REQUIRES(FreePartOfOpponentGoalProvider,BallModel)
	ADD_REQUIRES(FreePartOfOpponentGoalProvider,FieldDimensions)
	

}

void MainWindow::CreatConnect()
{
	
	/*********************image*********************/
	connect(selfMenuBar->saToolsAct,SIGNAL(triggered()),		this,SLOT(openSaTools())	);
	connect(selfMenuBar->sensorWidgetAct,SIGNAL(triggered()),		this,SLOT(openSensorWidget())	);
	connect(selfMenuBar->imageRaw,SIGNAL(triggered()),this,SLOT(OpenRawImageWnd()));
	connect(selfMenuBar->imageClassify,SIGNAL(triggered()),this,SLOT(OpenClassifyImageWnd()));
	connect(selfMenuBar->imageOpenPic,SIGNAL(triggered()),pImageDialog,SLOT(openPic()));
	connect(selfMenuBar->imageOpenLut,SIGNAL(triggered()),pImageDialog,SLOT(openLut()));
	connect(selfMenuBar->imageRun,SIGNAL(triggered()),pImageDialog,SLOT(run()));
	connect(selfMenuBar->imagePreImage,SIGNAL(triggered()),pImageDialog,SLOT(preImage()));
	connect(selfMenuBar->imageNextImage,SIGNAL(triggered()),pImageDialog,SLOT(nextImage()));
	connect(selfMenuBar->imageGenLut,SIGNAL(triggered()),pImageDialog,SLOT(genLut()));
	connect(selfMenuBar->imageSaveLut,SIGNAL(triggered()),pImageDialog,SLOT(saveLut()));
	// 	connect(selfMenuBar->imageOpenColorspace,SIGNAL(triggered()),pImageDialog,SLOT(openColorspace()));
	connect(selfMenuBar->imageGetPic,SIGNAL(triggered()),pImageDialog,SLOT(getNetPic()));
	//connect(selfMenuBar->imageGetSteamPic,SIGNAL(triggered()),pImageDialog,SLOT(getAndSaveStreamPic()));
 	//connect(selfMenuBar->simOpen, SIGNAL(triggered()), this, SLOT(openSimulation()));
 	//connect(selfMenuBar->simStart, SIGNAL(triggered()), this, SLOT(startSimulation()));
 	//connect(selfMenuBar->simStep, SIGNAL(triggered()), this, SLOT(stepSimulation()));
 	//connect(selfMenuBar->simReset, SIGNAL(triggered()), this, SLOT(resetSimulation()));
	//connect(selfMenuBar->simRecord, SIGNAL(toggled(bool)), this, SLOT(recordToggled(bool)));
	//connect(selfMenuBar->simScreenShot, SIGNAL(triggered()), this, SLOT(saveScreenShot()));
	//selfMenuBar->imageGetSteamPic->setChecked(pImageDialog->getStreamPicOn());
	connect(selfMenuBar->imageGetSteamPic,SIGNAL(toggled(bool)),pImageDialog,SLOT(getStreamPic(bool)));
	connect(selfMenuBar->imageSavePic,SIGNAL(triggered()),pImageDialog,SLOT(savePic()));
	selfMenuBar->odeImageOpenPic->setChecked(pImageDialog->getOdePicOn());
	connect(selfMenuBar->odeImageOpenPic,SIGNAL(toggled(bool)),pImageDialog,SLOT(startOdePic(bool)));
	connect(selfMenuBar->odeImageSavePic,SIGNAL(triggered()),pImageDialog,SLOT(saveODEPic()));
	connect(selfMenuBar->imageSaveAsPic,SIGNAL(triggered()),pImageDialog,SLOT(saveAsPic()));
	connect(pImageDialog,SIGNAL(EnableSaveLut(bool )),selfMenuBar->imageSaveLut,SLOT(setEnabled(bool)));
	connect(pImageDialog,SIGNAL(EnableSavePic(bool )),selfMenuBar->imageSavePic,SLOT(setEnabled(bool)));
	connect(pImageDialog,SIGNAL(EnableSaveAsPic(bool )),selfMenuBar->imageSaveAsPic,SLOT(setEnabled(bool)));
	connect(pImageDialog,SIGNAL(activeThisWindow()),subwindows1,SLOT(setFocus()));
	connect(pImageDialog,SIGNAL(IsConnectNaoqi(bool)),this,SLOT(IsConnectNaoqi(bool)));
	connect(pImageDialog,SIGNAL(updateWindowTitle(const QString &)),subwindows1,SLOT(setWindowTitle ( const QString & )));
	connect(pImageDialog,SIGNAL(ReceiveResultOk(char *)),this,SLOT(GetVisionResult(char *)));
	connect(pImageDialog,SIGNAL(LoadNewLut(unsigned char*)),colorSpaceWindow,SLOT(setLut(unsigned char* )));
	/***************camera config*************************/

	connect(selfMenuBar->imageCameraChange,SIGNAL(triggered()),pImageDialog,SLOT(cameraChange()));

	/******************* tcp ip to get classified pic ************************/
	connect(selfMenuBar->startNetClassified,SIGNAL(triggered()),this,SLOT(InNetClassified()));
   connect(selfMenuBar->stopNetClassified,SIGNAL(triggered()),this,SLOT(StopNetClassified()));
   connect(this,SIGNAL(EnablestartNetClassified(bool)),selfMenuBar->startNetClassified,SLOT(setEnabled(bool)));
	connect(this,SIGNAL(EnablestopNetClassified(bool)),selfMenuBar->stopNetClassified,SLOT(setEnabled(bool)));
	
	
	/*********************tcp ip to get locdata**************************/
    	//打开在线定位调试工具
	connect(selfMenuBar->startOnlineLoc,SIGNAL(triggered()),this,SLOT(OpenLonOnlineControlWnd()));
	connect(this,SIGNAL(EnablestartOnLineLoc(bool)),locOnlineControlWnd->startLoc,SLOT(setEnabled(bool)));
	connect(this,SIGNAL(EnablestopOnLineLoc(bool)),locOnlineControlWnd->stopLoc,SLOT(setEnabled(bool)));
	connect(locOnlineControlWnd,SIGNAL(StartSendLocSample()),this,SLOT(INOnLineLoc()));
	connect(locOnlineControlWnd,SIGNAL(StopSendLocSample()),this,SLOT(StopOnLineLoc()));
    connect(locOnlineControlWnd,SIGNAL( RobotSampleTodraw(int)),locatorWindow,SLOT(getRobotsampleflag(int)));
	connect(locOnlineControlWnd,SIGNAL( BallSampleTodraw(int)),locatorWindow,SLOT(getBallsampleflag(int)));






	/*****************classify image**************************/
	//toolbox -------------->classifyImageDialog
	connect(ImageToolBox->LoadLut,SIGNAL(clicked()),pClassifyImageDialog,SLOT(openLut()));
	connect(ImageToolBox->Loadimage,SIGNAL(clicked()),pClassifyImageDialog,SLOT(openPic()));
	connect(ImageToolBox->PreImage,SIGNAL(clicked()),pClassifyImageDialog,SLOT(preImage()));
	connect(ImageToolBox->NextImage,SIGNAL(clicked()),pClassifyImageDialog,SLOT(nextImage()));
	connect(ImageToolBox->SaveAsLut,SIGNAL(clicked()),pClassifyImageDialog,SLOT(saveAsLut()));
	connect(ImageToolBox->SaveLut,SIGNAL(clicked()),pClassifyImageDialog,SLOT(saveLut()));
	connect(ImageToolBox->UnDo,SIGNAL(clicked()),pClassifyImageDialog,SLOT(pushUndo()));
	connect(ImageToolBox->y,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(updateYSensitivity(const QString &)));
	connect(ImageToolBox->u,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(updateUSensitivity(const QString &)));
	connect(ImageToolBox->v,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(updateVSensitivity(const QString &)));
	connect(ImageToolBox->selectColor,SIGNAL(currentIndexChanged(int)),pClassifyImageDialog,SLOT(updateCurrentcolor(int)));
	connect(ImageToolBox->NotReplace,SIGNAL(toggled(bool)),pClassifyImageDialog,SLOT(updateDontReplace(bool)));
	connect(ImageToolBox->Replace,SIGNAL(toggled(bool)),pClassifyImageDialog,SLOT(updateReplace(bool)));
	connect(ImageToolBox->Delete,SIGNAL(toggled(bool)),pClassifyImageDialog,SLOT(updateDelete(bool)));
	connect(ImageToolBox->HotKey,SIGNAL(toggled(bool)),pClassifyImageDialog,SLOT(updateHotkey(bool)));
	connect(ImageToolBox->HSI,SIGNAL(toggled(bool)),pClassifyImageDialog,SLOT(updateHsiEnabled(bool)));
	connect(ImageToolBox,SIGNAL(updateHSIRange(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)),pClassifyImageDialog,SLOT(updateHSIRange(const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &,const QString &)));
	// 	connect(ImageToolBox->R_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_r_min(const QString &)));
	// 	connect(ImageToolBox->R_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_r_max(const QString &)));
	// 	connect(ImageToolBox->O_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_o_min(const QString &)));
	// 	connect(ImageToolBox->O_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_o_max(const QString &)));
	// 	connect(ImageToolBox->Y_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_y_min(const QString &)));
	// 	connect(ImageToolBox->Y_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_y_max(const QString &)));
	// 	connect(ImageToolBox->F_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_f_min(const QString &)));
	// 	connect(ImageToolBox->F_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_f_max(const QString &)));
	// 	connect(ImageToolBox->C_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_c_min(const QString &)));
	// 	connect(ImageToolBox->C_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_c_max(const QString &)));
	// 	connect(ImageToolBox->W_y_min,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_w_min_Y(const QString &)));
	// 	connect(ImageToolBox->W_y_max,SIGNAL(textChanged(const QString & )),pClassifyImageDialog,SLOT(update_w_max_Y(const QString &)));

	//classifyImageDialog -------------->toolbox
	connect(pClassifyImageDialog,SIGNAL(enableSave(bool )),ImageToolBox->SaveLut,SLOT(setEnabled(bool)));
	connect(pClassifyImageDialog,SIGNAL(enableUnDo(bool )),ImageToolBox->UnDo,SLOT(setEnabled(bool)));
	connect(pClassifyImageDialog,SIGNAL(activeThisWindow()),subwindows2,SLOT(setFocus()));
	connect(pClassifyImageDialog,SIGNAL(updatePath(const QString &)),ImageToolBox->PathName,SLOT(setText ( const QString &)));
	connect(pClassifyImageDialog,SIGNAL(sendCurrentColorToToolBox(int )),ImageToolBox->selectColor,SLOT(setCurrentIndex(int)));
	connect(pClassifyImageDialog,SIGNAL(updateWindowTitle(const QString &)),subwindows2,SLOT(setWindowTitle(const QString &)));
	////////////////////////////华丽的分割线//////////////////////////////////////
	/*connect(treeDockWidget,SIGNAL(MotionSeen(QString )),motionWindow,SLOT(MotionSeen(QString )));
	connect(treeDockWidget,SIGNAL(MotionSeen(QString )),this,SLOT(ShowMotionWin()));*/
	connect(treeDockWidget,SIGNAL(MotionSeen(QString )),this,SLOT(TreeShowWin(QString)));
	connect(selfMenuBar->windowMotionAction,SIGNAL(triggered()),this,SLOT(ShowMotionWin()));
	connect(selfMenuBar->windowSensorAction,SIGNAL(triggered()),this,SLOT(ShowSensorWin()));
	connect(selfMenuBar->windowImageAction,SIGNAL(triggered()),this,SLOT(ShowImageViewWin()));
	connect(selfMenuBar->windowColorSpaceAction,SIGNAL(triggered()),this,SLOT(ShowColorSpaceWin()));
	connect(selfMenuBar->windowConnectAction,SIGNAL(triggered()),this,SLOT(ShowConnectWin()));
	connect(selfMenuBar->windowCascadeAction,SIGNAL(triggered()),mdiArea,SLOT(cascadeSubWindows()));
	connect(selfMenuBar->connectAction,SIGNAL(triggered()),this,SLOT(Doconnect()));
	connect(selfMenuBar->disconnectAction,SIGNAL(triggered()),this,SLOT(DoDisconnect()));
	connect(selfMenuBar->getSensorData,SIGNAL(triggered()),this,SLOT(GetSensorData()));

	connect(selfMenuBar->motionEngineAction,SIGNAL(triggered()),this,SLOT(OpenMotionEngineWindow()));
	connect(selfMenuBar->behaviorDataAction,SIGNAL(triggered()),this,SLOT(OpenBehaviorDataWindow()));
	connect(selfMenuBar->signalAction,SIGNAL(triggered()),this,SLOT(OpenSignalSwitchWnd()));
	connect(tcpContorl,SIGNAL(ConnectOk()),this,SLOT(ConnectOk()));
	connect(tcpContorl,SIGNAL(ConnectError()),this,SLOT(ConnectError()));
	connect(tcpContorl,SIGNAL(ReceiveOk(char *,const WDataInfo& )),this,SLOT(ProcessData(char *,const WDataInfo& )));
	connect(tcpContorl,SIGNAL(ReceiveLocOk(char*,const WDataInfo&)),this,SLOT(LocShow(char*,const WDataInfo&)));
	//connect(tcpContorl,SIGNAL(ReceiveResultOk(char *)),this,SLOT(GetVisionResult(char *)));
	connect(connectWindow,SIGNAL(ConfigInfo(ConnectInfo&)),this,SLOT(ReceiveConfig(ConnectInfo&)));
	connect(motionWindow,SIGNAL(SendToMain()),this,SLOT(SendToImageDialog()));
	connect(signalControlWnd,SIGNAL(updateBehaviorThread()),this,SLOT(StartBehaviorThread()));

	
}

void MainWindow::StartBehaviorThread()
{
 tjRobotThread->start();
}

void MainWindow::ShowMotionWin()
{
	motionWindow->Show();

}
void MainWindow::ShowConnectWin()
{
	connectWindow->Show();

}
void MainWindow::TreeShowWin(QString treeStr)
{
	if(treeStr=="Head"||treeStr=="LeftLeg"||treeStr=="LeftArm"||treeStr=="RightLeg"||treeStr=="RightArm"||treeStr=="NaoStartUp")
	{
		motionWindow->MotionSeen(treeStr);
		ShowMotionWin();
	}
	if(treeStr=="Sensor")
	{
		ShowSensorWin();
	}
	if (treeStr=="Image")
	{
		ShowImageViewWin();
	}
	if(treeStr == "WorldState")
	{
		ShowLocatorWin();
	}
}
void MainWindow::ShowSensorWin()
{
	tableWindow->Show();
}

void MainWindow::ShowImageViewWin()
{
	visionResultWindow->show();
}
void MainWindow::ShowColorSpaceWin()
{
	colorSpaceWindow->show();
}
void MainWindow::ShowLocatorWin()
{
	locatorWindow->Show();
}
void MainWindow::Doconnect()
{
#ifdef TCPCONNECT
	emit tcpContorl->TryConnect(guiRobotinfo);
#endif
	emit pImageDialog->connectNaoqi((guiRobotinfo.ipstring).toStdString());
	//Motion Engine
	if(pMotionEngineWnd)
		pMotionEngineWnd->doConnect((guiRobotinfo.ipstring).toStdString());
}
void MainWindow::DoDisconnect()
{
	IsConnect=false;
	UpdateToolBar();
	//	tcpContorl->Dodiscon();
	pImageDialog->closeNaoqi();
	//Motion Engine
	if(pMotionEngineWnd)
		pMotionEngineWnd->doDisconnect();
}
void MainWindow::GetSensorData()
{
	//NetCommandMsg tempMsg(idRawImage,0,0,0);

	//if(tempMsg.RS_COMMAND==idRawImage)//use the naocam proxy to get image
	//{
	int normalSize = 240*320*3; 
	//	pImageDialog->getNetPic();

	tableWindow->setSensorData(pImageDialog->getSensoryJointData());
	//double sencer=0;
	////memcpy(&sencer,buffer/*+(normalSize/6)*/,sizeof(double));
	//}
	//else if(tempMsg.RS_COMMAND==idJointMove)//use  the dcm proxy to control joint
	//{

	//}
	//else//some data that cannot control by nao API 
	//{
	//	emit tcpContorl->SendData(tempMsg);
	//}



}
void MainWindow::ConnectOk()
{
	IsConnect=true;

	UpdateToolBar();
}
void MainWindow::ConnectError()
{
	QMessageBox::warning(this,tr("ConnectError"),tr("Can't Connect to the robot %1").
		arg(guiRobotinfo.ipstring));
}
void MainWindow::UpdateToolBar()
{
	selfMenuBar->connectAction->setEnabled(!IsConnect);
	selfMenuBar->disconnectAction->setEnabled(IsConnect);
	selfMenuBar->imageGetPic->setEnabled(IsConnect);
	selfMenuBar->imageGetSteamPic->setEnabled(IsConnect);
	selfMenuBar->getSensorData->setEnabled(IsConnect);
}
void MainWindow::ReceiveConfig(ConnectInfo &robotinfo)
{
	guiRobotinfo=robotinfo;
}
void MainWindow::ProcessData(char *buffer,const WDataInfo& wdatainfo)
{
	pImageDialog->ShowNetClassified(buffer,pImageDialog->getclassifiedImage());
	//std::cout<<"nihao,ni hao "<<std::endl;

}
void MainWindow::GetVisionResult(char *buffer)
{
	//std::cout<<"we are in GetVisionResult"<<std::endl;
	visionResultWindow->getVisionResult(buffer);
}
void MainWindow::IsConnectNaoqi(bool isConnectNaoqi)
{
	if(isConnectNaoqi==false)
		QMessageBox::warning(this,tr("ConnectError"),tr("Can't Connect to the robot %1").
		arg(guiRobotinfo.ipstring));
	else 
	{
		IsConnect=true;
		UpdateToolBar();
		motionWindow->initJointData(pImageDialog->getSensoryJointData());
	}

}
void MainWindow::getInitInfo()
{
	guiRobotinfo=connectWindow->GetCurrentInfo();
}
void MainWindow::SendToImageDialog()
{
	if(IsConnect)
		pImageDialog->ReceiveJointData();
}

void MainWindow::updateObjectWindowSlot()
{
	if(objectWindow)
	{
		SubWindow* subWindow = qobject_cast<SubWindow*>(objectWindow);
		subWindow->updateObject();
	}
}

void MainWindow::startSimulation()
{
	document->simStartSlot();
	if(objectWindow)
	{
		objectWindow->update();
		objectWindow->show();
		objectWindow->raise();
	}
}
void MainWindow::stepSimulation()
{
	document->simStepSlot();
}
void MainWindow::resetSimulation()
{
	document->simResetSlot();
}
void MainWindow::openSimulation()
{
	//OpenSimulatorWindow();
	document->openSlot();

	if(objectWindow)
		objectWindow->setVisible(true);
	//document->simStartSlot();
}
void MainWindow::recordToggled(bool checked)
{
	if(document && document->isOpened() && selfMenuBar->simRecord)
	{
		if(checked)
		{
			document->startRecord();
			selfMenuBar->simRecord->setIcon(QIcon(":/icons/recording.png"));
		}else
		{
			document->endRecord();
			selfMenuBar->simRecord->setIcon(QIcon(":/icons/record.png"));
		}
	}
}
void MainWindow::saveScreenShot()
{
	if(document && document->isOpened()&& selfMenuBar->simScreenShot)
	{
		document->saveScreenShot();
	}
}
void MainWindow::shootObject()
{
	document->shootObjectSlot();
// 	if(objectWindow)
// 		objectWindow->
}
void MainWindow::signalActuatorThread()
{
	spSynchronizer->getEventbyName("ODESimulation->Actuator")->signal();
}
void MainWindow::OpenSimulatorWindow()
{
	if(objectWindow == NULL){
		document = new Document(this,actuatorThread,  spSynchronizer);
		if(!document->isOpened())
			document->newSlot();
		objectWindow = new ObjectWindow(mdiArea, "");
		if(objectWindow)
		{
			objectWindow->setAttribute(Qt::WA_DeleteOnClose);
			mdiArea->addSubWindow(objectWindow);
			objectWindow->show();
			objectWindow->setVisible(false);
		}
		//=========connec ODE Simulation Related Signals============//
			//=========connec ODE Simulation Related Signals============//
		connect(document, SIGNAL(updateObjectWindowSignal()), this, SLOT(updateObjectWindowSlot()));
		connect(document, SIGNAL(updateActuatorThreadSignal()), this, SLOT(signalActuatorThread()));
		connect(selfMenuBar->simOpen, SIGNAL(triggered()), this, SLOT(openSimulation()));
		connect(selfMenuBar->simStart, SIGNAL(triggered()), this, SLOT(startSimulation()));
		connect(selfMenuBar->simStep, SIGNAL(triggered()), this, SLOT(stepSimulation()));
		connect(selfMenuBar->simReset, SIGNAL(triggered()), this, SLOT(resetSimulation()));
		//connect(selfMenuBar->fileExitAction, SIGNAL(triggered()), this, SLOT(shootObject()));
		connect(selfMenuBar->simRecord, SIGNAL(toggled(bool)), this, SLOT(recordToggled(bool)));
		connect(selfMenuBar->simScreenShot, SIGNAL(triggered()), this, SLOT(saveScreenShot()));
		selfMenuBar->behaviorDataAction->setEnabled(true);
	}
	
}
void MainWindow::OpenMotionEngineWindow()
{
	if(pMotionEngineWnd == NULL)
	{
		pMotionEngineWnd = new MotionEngineWindow(*actuatorThread,mdiArea);
		//pMotionEngineWnd->show();
	 	//pMotionEngineWnd = new MotionEngineWindow(*actuatorThread, "MotionEngineWindow", pMotionEngineWnd);
	 	//pMotionEngineWnd->setAllowedAreas(Qt::TopDockWidgetArea| Qt::BottomDockWidgetArea);
	 	//addDockWidget(Qt::TopDockWidgetArea,pMotionEngineWnd);
		//pMotionEngineWnd->show();
		//pMotionEngineWnd->raise();
	}else
	{
		if(pMotionEngineWnd->isHidden())
		{
			pMotionEngineWnd->show();
		}
	}

}

void MainWindow::OpenBehaviorDataWindow()
{
	pBehaviorDataWnd = new BehaviorDataWindow(*tjRobotThread, mdiArea);
	pBehaviorDataWnd->show();
}

void MainWindow::OpenSignalSwitchWnd()
{
	signalControlWnd->setVisible(true);
	selfMenuBar->signalAction->setEnabled(false);
}
void MainWindow::OpenLonOnlineControlWnd()
{
	locOnlineControlWnd->Show();
}

void MainWindow::OpenRawImageWnd()
{
	subwindows1->setVisible(true);
	selfMenuBar->odeImageOpenPic->setEnabled(true);
	selfMenuBar->imageRaw->setEnabled(false);
}

void MainWindow::OpenClassifyImageWnd()
{
	subwindows2->setVisible(true);
	selfMenuBar->imageClassify->setEnabled(false);
}

void MainWindow::InNetClassified()
{
	EnablestartNetClassified(false);
	EnablestopNetClassified(true);
	//NetworkMsg tempMsg(idClassifidImage,0,0,0);
	//tcpContorl->tcpclient->receiveThread.recbuffer;
	emit tcpContorl->TryConnect(guiRobotinfo);
	std::cout<<"in InNetClassified "<<std::endl;
	NetPictimerid=startTimer(300);

	//bool running=true;
	//NetworkMsg tempMsg(idClassifidImage,0,0,0);
	//tcpContorl->SendData(tempMsg);

}

void MainWindow::timerEvent(QTimerEvent *event)
{
	//qDebug() << "Timer ID:" << event->timerId();
      if(NetPictimerid==event->timerId())
	  { 
		  std::cout<<"in timerEvent NetPic"<<std::endl;
	//InNetClassified();
	     bool running=true;
	     NetworkMsg tempMsg(idClassifidImage,0,0,0);
	     tcpContorl->SendData(tempMsg);
	  }
	  if(Loctimerid==event->timerId())
	  {
           std::cout<<"in timerEvent Loc"<<std::endl;
		   bool running=true;
		   NetworkMsg tempMsg(idLocatorData,0,0,0);
		   tcpContorl->SendData(tempMsg);
	  }

}

void MainWindow::StopNetClassified()
{
	EnablestartNetClassified(true);
	EnablestopNetClassified(false);
	killTimer(NetPictimerid);
	tcpContorl->Dodiscon();
}


void MainWindow::openSaTools()
{
	bool static openSaToolsFlag = false;
	if(!openSaToolsFlag)
	{
		saToolsDockWidget ->show();
		mySaWidget->show();
		openSaToolsFlag = true;
	}
	else
	{
		saToolsDockWidget->hide();
		mySaWidget->hide();
		openSaToolsFlag = false;
	}
}

void MainWindow::openSensorWidget()
{
	bool static openSaToolsFlag = false;
	if(!openSaToolsFlag)
	{
		sensorDockWidget ->show();
		mySensorWidget->show();
		openSaToolsFlag = true;
	}
	else
	{
		sensorDockWidget->hide();
		mySensorWidget->hide();
		openSaToolsFlag = false;
	}
}

void MainWindow::createSaTools()
{
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//x
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);//x

	mySaWidget = new SaWidget(this);
	mySaWidget->hide();
	saToolsDockWidget = new QDockWidget(tr("SaTools"),this);
	saToolsDockWidget->setWindowIcon(QIcon(tr(":/sa/icons/saTools/GT.png")));
	saToolsDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
	saToolsDockWidget->setWidget(mySaWidget);
	//addDockWidget(Qt::RightDockWidgetArea,saToolsDockWidget);
	QSize minSize = saToolsDockWidget->minimumSizeHint();
	saToolsDockWidget->setGeometry(122,222,minSize.width(),minSize.height());
	//QEvent * temLuan = new QEvent(QEvent::MouseButtonDblClick);
	//QCoreApplication::sendEvent(saToolsDockWidget,temLuan);

	saToolsDockWidget->hide();
}

void MainWindow::createSensorWidget()
{
	mySensorWidget = new SensorWidget(this);
	sensorDockWidget =  new QDockWidget(tr("Sensor Data"),this);
	sensorDockWidget->setWindowIcon(QIcon(tr(":/sa/icons/saTools/SensorWidget/Sensor.png")));
	sensorDockWidget->setWidget(mySensorWidget);
	addDockWidget(Qt::RightDockWidgetArea,sensorDockWidget);
	sensorDockWidget->hide();

	connect(mySensorWidget,SIGNAL(dragPlaneChangedSignal(int)),	this,SLOT(dragPlaneChangedSlot(int))	);
}

void MainWindow::dragPlaneChangedSlot(int type)
{
	switch (type)
	{
	case 1:
		objectWindow->setDragPlaneSlot(0);
		break;
	case 2:
		objectWindow->setDragPlaneSlot(1);
		break;
	case 3:
		objectWindow->setDragPlaneSlot(2);
		break;
	}
}
void MainWindow::LocShow(char *buffer,const WDataInfo&wdatainfo)
{
	locatorWindow->getLocResult(buffer,wdatainfo.size);
}
void MainWindow::INOnLineLoc()
{
	
		EnablestartOnLineLoc(false);
		EnablestopOnLineLoc(true);
		//NetworkMsg tempMsg(idClassifidImage,0,0,0);
		//tcpContorl->tcpclient->receiveThread.recbuffer;
		emit tcpContorl->TryConnect(guiRobotinfo);
		std::cout<<"in OnlineLoc "<<std::endl;
		Loctimerid=startTimer(200);

}
void MainWindow::StopOnLineLoc()
{
	EnablestartOnLineLoc(true);
	EnablestopOnLineLoc(false);
	killTimer(Loctimerid);
	tcpContorl->Dodiscon();
}