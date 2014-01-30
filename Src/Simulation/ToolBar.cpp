#include "ToolBar.h"
ToolBar::ToolBar(QWidget *parent/* =0 */):QToolBar(parent)
{
	setWindowTitle(tr("Toolbar"));
	CreatAction();
}
void ToolBar::CreatAction()
{
	MenuBar *menuBar=MenuBar::menuBar;
	
	addAction(menuBar->signalAction);

	addAction(menuBar->imageRaw);
	addAction(menuBar->imageClassify);
	
	addAction(menuBar->odeImageOpenPic);
	addAction(menuBar->odeImageSavePic);

	addAction(menuBar->imageOpenPic);
	addAction(menuBar->imageOpenLut);
	addAction(menuBar->imagePreImage);
	addAction(menuBar->imageNextImage);
	addAction(menuBar->imageCameraChange);
	addAction(menuBar->startNetClassified);
	addAction(menuBar->stopNetClassified);
	addAction(menuBar->startOnlineLoc);
	//addAction(menuBar->stopOnlineLoc);
	//addAction(menuBar->startOnLineLoc);
	addAction(menuBar->imageGetPic);
	addAction(menuBar->imageSavePic);
	addAction(menuBar->imageGetSteamPic);
	addAction(menuBar->imageSaveAsPic);
	addAction(menuBar->imageRun);
	addAction(menuBar->imageOpenColorspace);
	addAction(menuBar->imageGenLut);
	addAction(menuBar->imageSaveLut);
	addSeparator();
	addAction(menuBar->connectAction);
	addAction(menuBar->disconnectAction);
	addAction(menuBar->getSensorData);
	/*addSeparator();
	addAction(menuBar->loadLocLogAction);*/
	addSeparator();
	/*simMenu = new QMenu("Scene",this);
	simMenu->setIcon(QIcon(":/icons/sim_open.png"));
	simMenu->addAction(menuBar->simOpen);
	simMenu->addAction(menuBar->simStart);
	simMenu->addAction(menuBar->simStep);
	simMenu->addAction(menuBar->simReset);
	simMenu->addAction(menuBar->simRecord);
	simMenu->addAction(menuBar->simScreenShot);
	addAction(simMenu->menuAction());*/
	addAction(menuBar->simOpen);
	addAction(menuBar->simStart);
	addAction(menuBar->simStep);
	addAction(menuBar->simReset);
	addAction(menuBar->simRecord);
	addAction(menuBar->simScreenShot);
	addSeparator();
	addAction(menuBar->motionEngineAction);
	addAction(menuBar->behaviorDataAction);
	addSeparator();
	/*netMenu = new QMenu("NetPic",this);
	netMenu->setIcon(QIcon(":/icons/startNetClassified.png"));
	netMenu->addAction(menuBar->imageCameraChange);
	netMenu->addAction(menuBar->startNetClassified);
	netMenu->addAction(menuBar->stopNetClassified);
	netMenu->addAction(menuBar->imageGetPic);
	netMenu->addAction(menuBar->imageGetSteamPic);
	netMenu->addAction(menuBar->imageSaveAsPic);
	netMenu->addAction(menuBar->imageSavePic);
	addAction(netMenu->menuAction());*/
	addSeparator();
	addAction(menuBar->saToolsAct);
	addAction(menuBar->sensorWidgetAct);
}