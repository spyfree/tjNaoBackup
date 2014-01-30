#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDialog>
#include <QMainWindow>
#include <QMdiArea>
#include <QMessageBox>
#include "SubWindow.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "DockWidget.h"
#include "MotionWindow.h"
#include "TableWindow.h"
#include "network/ConnectDialog.h"
#include "Simulation/VisionGUI/ImageDialog.h"
#include "Simulation/VisionGUI/VResultWindow.h"
#include "Simulation/VisionGUI/ClassifyImageToolBox.h"
#include "Simulation/VisionGUI/ClassifyImageDialog.h"
#include "Simulation/ActuatorGUI/MotionEngineWindow.h"
#include "Simulation/BehaviorGUI/BehaviorDataWindow.h"
#include "Simulation/saTools/SaWidget.h"
#include "Simulation/saTools/sensorWidget/SensorWidget.h"
#include "Simulation/LocatorGUI/LocOnlineControlWnd.h"
#include "Simulation/VisionGUI/ColorSpaceDialog.h"
//#include "Simulation/VisionGUI/CameraSettingDialog.h"
#ifdef DEBUG_NETWORK
#include "network/client.h"
#endif

#include "network/TcpConnect.h"

//#include "Module/Behavior/RobotProvider.h"
#include "Module/Actuator/Actuator.h"
#include "Module/Behavior/TJSimRobot.h"
#include "Simulation/LocatorGUI/LocatorWindow.h"

class Document;
class ObjectWindow;
class SignalControlWnd;
class MainWindow :public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent=0);
	~MainWindow();
private slots:
	void ShowMotionWin();
	void ShowConnectWin();
	void ShowSensorWin();
	
	void ShowImageViewWin();
	void ShowLocatorWin();
	void ShowColorSpaceWin();
	
	
	//image slots
	//void LoadImage();
	//Simulation slot
	void startSimulation();
	void stepSimulation();
	void resetSimulation();
	void openSimulation();
	void recordToggled(bool);
	void saveScreenShot();
	void shootObject();
	void updateObjectWindowSlot();

	//connect slots
	void Doconnect();
	void DoDisconnect();
	void GetSensorData();
	void ConnectOk();
	void ConnectError();
	void ReceiveConfig(ConnectInfo &robotinfo);
	void ProcessData(char *buffer,const WDataInfo& wdatainfo);
	void LocShow(char *buffer,const WDataInfo&wdatainfo);
	void GetVisionResult(char *buffer);
	void IsConnectNaoqi(bool IsConnect);
	void TreeShowWin(QString treeStr);
	void SendToImageDialog();
	void OpenSimulatorWindow();
	void signalActuatorThread();
	void OpenMotionEngineWindow();
	void OpenBehaviorDataWindow();
	void OpenSignalSwitchWnd();
	//image windows
	void OpenRawImageWnd();
	void OpenClassifyImageWnd();
    //tcp ip net pic slot
	void InNetClassified();
	void StopNetClassified();
	void StartBehaviorThread();
	//void loadLog();
    //tcp ip to get loc data
	void INOnLineLoc();
	void StopOnLineLoc();
    void OpenLonOnlineControlWnd();
private:
	void closeEvent(QCloseEvent* event);
	void buildModules();
	void getInitInfo();
	void CreateStatusBar();
	void UpdateMenu();
	void UpdateToolBar();
	bool LoadFile(const QString &fileName);
	bool SaveFile(const QString &fileName);
	void SetCurrentFile(const QString &fileName);
	QString curFile;
	QMdiArea *mdiArea;
	void CreatConnect();
	void CreatMenus();
	MenuBar *selfMenuBar;
	MotionWindow *motionWindow;
	ConnectWindow *connectWindow;
	TableWindow *tableWindow;
	SubWindow *subwindows1;
	VResultWindow *visionResultWindow;
	ColorSpaceWindow* colorSpaceWindow;
	ManageTcp *tcpContorl;
	ToolBar *selfToolBar;
	DockWidget *treeDockWidget;
	ConnectInfo guiRobotinfo;
	NetworkMsg guiCommandMsg;
	bool IsConnect;
	ImageDialog* pImageDialog;
	ClassifyImageToolBox *ImageToolBox;
	ClassifyImageDialog *pClassifyImageDialog;
	SubWindow *subwindows2;
	//Actuator WIndows
	MotionEngineWindow* pMotionEngineWnd;
	BehaviorDataWindow* pBehaviorDataWnd;
	LocatorWindow* locatorWindow;
	SignalControlWnd* signalControlWnd;
    LocOnlineControlWnd*locOnlineControlWnd;

	//CameraSettingDialog *cameraSettingWindow;
	//NaoProvider *naoProvider;
	//ODE Simulation
	Document* document;
	ObjectWindow* objectWindow;
#ifdef DEBUG_NETWORK
	ClientDialog *dialog;
#endif

	Actuator* actuatorThread;
	TJSimRobot* tjRobotThread;
	SmartPtr<Synchronizer> spSynchronizer;

	void timerEvent(QTimerEvent *event);
	int NetPictimerid;
	int Loctimerid;

	//x
	private slots:
		void openSaTools();
	private:
		void createSaTools();
private:
	SaWidget * mySaWidget;
	QDockWidget * saToolsDockWidget;

	private slots:
		void openSensorWidget();
		void dragPlaneChangedSlot(int);
	private:
		void createSensorWidget();
	private:
		SensorWidget * mySensorWidget;
		QDockWidget * sensorDockWidget;
signals:
		void EnablestartNetClassified(bool startNetClassified);
		void EnablestopNetClassified(bool stopNetClassified);
		void EnablestartOnLineLoc(bool startOnLineLoc);
		void EnablestopOnLineLoc(bool stopOnLineLoc);
};



#endif
