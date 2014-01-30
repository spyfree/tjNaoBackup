#include "MenuBar.h"

MenuBar *MenuBar::menuBar=0;
MenuBar::MenuBar(QWidget*parent):QMenuBar(parent)
{
	CreatAction();
	CreatMenu();
	menuBar=this;
}
MenuBar* MenuBar::getInstance()
{
	return menuBar;
}
void MenuBar:: CreatAction()
{   
	toolsMenu=new QMenu(tr("&Tools"));
	imageMenu=new QMenu(tr("&Image"));
	simMenu = new QMenu(tr("&Simulation"));
	windowMenu=new QMenu(tr("&Window"));
	helpMenu=new QMenu(tr("&Help"));
	//toolAction
	motionEngineAction = new QAction(tr("&MotionEngineWindow"),this);
	//imageAction
	imageRaw = new QAction(QIcon(":/icons/rawImage.png"),tr("rawImage"),this);
	imageClassify = new QAction(QIcon(":/icons/classifyImage.png"),tr("classifyImage"),this);
	imageRun=new QAction(QIcon(":/icons/Run.png"),tr("&Run"),this);
	imageOpenPic=new QAction(QIcon(":/icons/ColorImage.png"),tr("&OpenPic"),this);
	imageOpenLut=new QAction(QIcon(":/icons/OpenLut.png"),tr("OpenLut"),this);
	imageOpenColorspace=new QAction(QIcon(":/icons/ColorSpace.png"),tr("&OpenCoSpace"),this);
	imagePreImage=new QAction(QIcon(":/icons/Previous.png"),tr("&PreImage"),this);
	imageNextImage=new QAction(QIcon(":/icons/Next.png"),tr("&NextImage"),this);
	imageGetPic=new QAction(QIcon(":/icons/getPic.png"),tr("&GetPic"),this);
	imageGetPic->setEnabled(false);
	imageGetSteamPic=new QAction(QIcon(":/icons/getSteamPic.png"),tr("&getStreamPic"),this);
	imageGetSteamPic->setCheckable(true);
	//imageGetSteamPic->setChecked();
    imageGetSteamPic->setEnabled(false);
	imageSavePic=new QAction(QIcon(":/icons/Save.png"),tr("&SavePic"),this);
	imageSavePic->setEnabled(false);
	odeImageSavePic=new QAction(QIcon(":/icons/ODESave.png"),tr("&ODESavePic"),this);
	odeImageSavePic->setEnabled(true);
	odeImageOpenPic=new QAction(QIcon(":/icons/ODEOpen.png"),tr("&ODEOpenPic"),this);
	odeImageOpenPic->setCheckable(true);
	odeImageOpenPic->setEnabled(false);
	imageSaveAsPic=new QAction(QIcon(":/icons/SaveAs.png"),tr("&SaveAsPic"),this);
	imageSaveAsPic->setEnabled(false);
// 	imageGenLut=new QAction(tr("&GenLut"),this);
// 	imageGenLut->setIcon(QIcon(":/icons/genlut.png"));
// 	imageSaveLut=new QAction(tr("&SaveLut"),this);
// 	imageSaveLut->setIcon(QIcon(":/icons/savelut.png"));
	imageGenLut=new QAction(QIcon(":/icons/genlut.png"),tr("&GenLut"),this);
	imageSaveLut=new QAction(QIcon(":/icons/savelut.png"),tr("&SaveLut"),this);
	imageCameraChange=new QAction(QIcon(":/icons/CameraChange.png"),tr("&CameraChange"),this);
	startNetClassified=new QAction(QIcon(":/icons/startNetClassified.png"),tr("&startNetClassified"),this);
	stopNetClassified=new QAction(QIcon(":/icons/stopNetClassified.png"),tr("&stopNetClassified"),this);
	//stopNetClassified->setEnabled(false);

	//Simulation Actions
	simOpen = new QAction(tr("&Open"), this);
	simOpen->setIcon(QIcon(":/icons/sim_open.png"));
	simStart = new QAction(tr("&Start"), this);
	simStart->setIcon(QIcon(":/icons/media-play.png"));
	simStep = new QAction(tr("&GoStep"), this);
	simStep->setIcon(QIcon(":/icons/step.png"));
	simReset = new QAction(tr("&Reset"), this);
	simReset->setIcon(QIcon(":/icons/reset.png"));
	simRecord = new QAction(tr("&Recorde"), this);
	simRecord->setIcon(QIcon(":/icons/record.png"));
	simRecord->setCheckable(true);
	simScreenShot = new QAction(tr("&ScreenShot"), this);
	simScreenShot->setIcon(QIcon(":/icons/screen-shot.png"));

	//saTools
	saToolsAct = new QAction( tr("S&aTools"),this	);
	saToolsAct->setIcon( QIcon(":/sa/icons/saTools/GT.png")	);
	saToolsAct->setShortcut(	tr("Ctrl+S") );
	//sensorWidget
	sensorWidgetAct = new QAction(tr("Sensor&Data"),this);
	sensorWidgetAct->setIcon( QIcon(":/sensor/icons/saTools/SensorWidget/Sensor.png") );
	sensorWidgetAct->setShortcut( tr("Ctrl+D") );

	//tool Action
	//simulatorWndAction = new QAction(tr("&SimulatorWnd"),this);
	motionEngineAction = new QAction(QIcon(":/icons/actuator.png"),tr("&MotionEngine"),this);
	behaviorDataAction = new QAction(QIcon(":/icons/behavior.png"),tr("&BehaviorData"),this);
	behaviorDataAction->setEnabled(false);
	signalAction = new QAction(QIcon(":/icons/signal.png"),tr("SignalSwitch"),this);
	//loadLocLogAction = new QAction(QIcon(":/icons/loadLog.png"),tr("&LoadLocLog"),this);


	imageGenLut=new QAction(QIcon(":/icons/genlut.png"),tr("&GenLut"),this);
	imageSaveLut=new QAction(QIcon(":/icons/savelut.png"),tr("&SaveLut"),this);
	imageCameraChange=new QAction(QIcon(":/icons/CameraChange.png"),tr("&CameraChange"),this);
	startNetClassified=new QAction(QIcon(":/icons/startNetClassified.png"),tr("&startNetClassified"),this);
	stopNetClassified=new QAction(QIcon(":/icons/stopNetClassified.png"),tr("&stopNetClassified"),this);
	startOnlineLoc=new QAction(QIcon(":/icons/layers.png"),tr("&startOnlineLoc"),this);
   // stopOnlineLoc=new QAction(QIcon(":/icons/stop.png"),tr("&stopOnlineLoc"),this);
	//startOnLineLoc=new QAction(QIcon(":/icons/startNetClassified.png"),tr("&preImage"),this);
	//startOnLineLoc->setIcon(QIcon(":/icons/startOnLineLoc.png"));
	//stopNetClassified->setEnabled(false);

	//window Action
	windowMotionAction=new QAction(tr("&Motion"),this);
	windowSensorAction=new QAction(tr("&Sensor"),this);
	windowImageAction=new QAction(tr("&Image"),this);
	windowConnectAction=new QAction(tr("&Connect"),this);
	windowCascadeAction =new QAction(tr("&Cascade"),this);
	windowColorSpaceAction=new QAction(tr("&ColorSpace"),this);
	//about Action
	helpAboutQtAction=new QAction(tr("&AboutPragram"),this);
	//connectAction
	connectAction=new QAction(QIcon(":/icons/connect.png"),tr("Connect"),this);

	disconnectAction=new QAction(QIcon(":/icons/disconnect.png"),tr("Disconnect"),this);
	disconnectAction->setEnabled(false);
	getSensorData=new QAction(QIcon(":/icons/Sensor.png"),tr("SensorData"),this);
	getSensorData->setEnabled(false);

}
void MenuBar::CreatMenu()
{
	CreatImageMenu();
	addMenu(imageMenu);
	createSimulationMenu();
	addMenu(simMenu);
	CreatToolsMenu();
	addMenu(toolsMenu);
	CreatWindowMenu();
	addMenu(windowMenu);
	CreatHelpMenu();
	addMenu(helpMenu);

	
}

void MenuBar::CreatToolsMenu()
{
	//toolsMenu->addAction(simulatorWndAction);
	toolsMenu->addAction(motionEngineAction);
	toolsMenu->addAction(behaviorDataAction);
	toolsMenu->addAction(signalAction);
	//toolsMenu->addAction(loadLocLogAction);
}
void MenuBar::CreatImageMenu()
{
	imageMenu->addAction(imageRaw);
	imageMenu->addAction(imageClassify);
	imageMenu->addAction(imageCameraChange);
	imageMenu->addAction(startNetClassified);
	imageMenu->addAction(stopNetClassified);
	imageMenu->addAction(imageGetPic);
	imageMenu->addAction(imageGetSteamPic);
	imageMenu->addAction(imageSaveAsPic);
	imageMenu->addAction(imageSavePic);
	imageMenu->addAction(odeImageOpenPic);
	imageMenu->addAction(odeImageSavePic);
	imageMenu->addAction(imageOpenPic);
	imageMenu->addAction(imageOpenLut);
	imageMenu->addAction(imageNextImage);
	imageMenu->addAction(imagePreImage);
	imageMenu->addAction(imageRun);
	imageMenu->addAction(imageOpenColorspace);
	imageMenu->addAction(imageGenLut);
	imageMenu->addAction(imageSaveLut);
}

void MenuBar::createSimulationMenu()
{
	simMenu->addAction(simStart);
	simMenu->addAction(simStep);
	simMenu->addAction(simReset);
	simMenu->addAction(simOpen);
	simMenu->addAction(simRecord);
	simMenu->addAction(simScreenShot);
}
void MenuBar::CreatHelpMenu()
{
	helpMenu->addAction(helpAboutQtAction);
}
void MenuBar::CreatWindowMenu()
{
	windowMenu->addAction(windowMotionAction);
	windowMenu->addAction(windowImageAction);
	windowMenu->addAction(windowConnectAction);
	windowMenu->addAction(windowSensorAction);
	windowMenu->addAction(windowColorSpaceAction);
	windowMenu->addSeparator();
	windowMenu->addAction(windowCascadeAction);
}