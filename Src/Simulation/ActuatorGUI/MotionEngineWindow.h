#ifndef __MotionEngineWindow_h_
#define __MotionEngineWindow_h_
#include <QtGui>
#include "Module/Actuator/Actuator.h"
#include "Simulation/SubWindow.h"
#include "SImulation/DockWidget.h"
#include "Module/Common/MotionCmd.h"
#include "Module/Representations/BaseProvider.h"

//namespace AL 
//{
//
//	class ALProxy;
//	class ALValue;
//	class ALBroker;
//	class DCMProxy;	
//};

//class MotionEngineWindow: public DockWidget, public BaseProvider
class MotionEngineWindow: public SubWindow, public BaseProvider
{
	Q_OBJECT

public:
	MotionEngineWindow(Actuator& _actuator, QMdiArea* mdiArea=0);
	//MotionEngineWindow(Actuator& _actuator, const QString & title, QWidget * parent /* = 0 */);
	~MotionEngineWindow();
	void doConnect(const std::string ipAddress_);
	void doDisconnect();
	void setConnected(std::string ipAddress_, std::string naoName_);
	void setDisConnected();
private slots:
	void onFowardSliderChanged();
	void onLeftSliderChanged();
	void onTurnSliderChanged();
	void onFowardEditChanged();
	void onLeftEditChanged();
	void onTurnEditChanged();
	void onStartActuator(bool checked);
	void onHoldActuator(bool checked);
	void onStopActuator();
	void onSend();
	void onStopRobot();
	void onEmergency();
	void onCmdSending(bool checked);
	void onJointsSending(bool checked);
	void onSaveMDebug(bool checked);
	void onConnectRemoteActuator(bool checked);
private:
	void createWindow();
	void creadConnect();
	void Show();
	void resetWalkCmd();
	void setControls2Cmd(const MotionCmd& motionCmd_);
	void sendMotionCmd(MotionCmd& motionCmd_);
	void closeEvent(QCloseEvent *event);
private:
	enum{
		eWalkID = 0,
		eSepcialID
	};
	std::string ipAddress;
	std::string naoName;
	Actuator& actuator;
	bool connected;
	//walk cmd
	MotionCmd motionCmd;
	//======Controls===============//
	//Motion Command
	QRadioButton* walkRadioButton;
	QRadioButton* specialActionRadioButton;
	QButtonGroup* motionRadioButtonGroup;
	QComboBox* walkTypeComboBox;
	QComboBox* specialComboBox;
	//Walk Command
	QSlider* forwardSlider;
	QSlider* leftSlider;
	QSlider* turnSlider;
	QLineEdit* forwardEdit;
	QLineEdit* leftEdit;
	QLineEdit* turnEdit;
	QDoubleValidator* forwardValidator;
	QDoubleValidator* leftValidator;
	QIntValidator* turnValidator;
	QGroupBox* walkGroup;
	QPushButton* sendCmdButton;
	QPushButton* stopButton;
	QPushButton* emergencyStopButton;
	// TODO: Add a coordinate-like controls
	QPushButton* connectRemoteActuatorButton;
	// Actuator Thread Run
	QPushButton* startActuatorButton;
	QPushButton* holdActuatorButton;
	QPushButton* stopActuatorButton;
	QCheckBox* cmdSending2Robot;;
	QCheckBox* jointsSending2Robot;
	QCheckBox* saveMDebug;
	//Layout...	
	QWidget *widget;

	bool sendtimex,sendtimey,sendtimez;//indicate if the status(forward, left, or turn) has been changed
 private:
 	//AL::ALProxy *remoteActuatorProxy;
	bool remoteActuatorConnected;
};

#endif //__MotionEngineWindow_h_

