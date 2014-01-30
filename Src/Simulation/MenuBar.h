#ifndef MENUBAR_H
#define MENUBAR_H
#include <QAction>
#include <QMenu>
#include <QMenuBar>


class MenuBar :public QMenuBar
{
public:
	MenuBar(QWidget* parent);
	static MenuBar *menuBar;
private:
	void CreatAction();
	void CreatMenu();
	void CreatToolsMenu();
	void CreatImageMenu();
	void createSimulationMenu();
	void CreatHelpMenu();
	void CreatWindowMenu();
public:
	static MenuBar* getInstance();

	//about Action
	QAction *helpAboutQtAction;
	//ImageAction
	QAction *imageRun;
	QAction *imageOpenPic;
	QAction *imageOpenLut;
	QAction *imageOpenColorspace;
	QAction *imagePreImage;
	QAction *imageNextImage;
	QAction *imageGetPic;
	QAction *imageGetSteamPic;
	QAction *imageSavePic;
	QAction *odeImageOpenPic;
	QAction *odeImageSavePic;
	QAction *imageSaveAsPic;
	QAction *imageGenLut;
	QAction *imageSaveLut;
	QAction *imageRaw;
	QAction *imageClassify;

	//Simulation Action
	QAction *simStart;
	QAction *simStep;
	QAction *simReset;
	QAction *simOpen;
	QAction *simRecord;
	QAction *simScreenShot;

	//tool Action
	//QAction *simulatorWndAction;
	QAction *motionEngineAction;
	QAction *behaviorDataAction;
	QAction *imageCameraChange;
	QAction *startNetClassified;
	QAction *stopNetClassified;
	QAction *signalAction;
    QAction *startOnlineLoc;
	//QAction *stopOnlineLoc;

	//QAction *loadLocLogAction;
	//windowAction
	QAction *windowMotionAction;
	QAction *windowImageAction;
	QAction *windowConnectAction;
	QAction *windowCascadeAction;
	QAction *windowSensorAction;
	QAction *windowColorSpaceAction;
	//connectAction
	QAction *connectAction;
	QAction *disconnectAction;
	QAction *getSensorData;
	//menu
	QMenu *toolsMenu;
	QMenu *imageMenu;
	QMenu *simMenu;	
	QMenu *windowMenu;
	QMenu *helpMenu;
	
	QAction * saToolsAct;	//saTools
	QAction * sensorWidgetAct;
};

#endif