#ifndef  SAWIDGET_H
#define SAWIDGET_H

#include <QWidget>

#include "basicJoint.h"
#include "jointsData.h"
#include "connectWidget/ConnectWidget.h"
#include "saPlayerWidget/SaPlayerWidget.h"
//================
class QGridLayout;
class QHBoxLayout;

class QDialog;
class QDialogButtonBox;
class QListWidget;

class QMenuBar;
class QMenu;
class QAction;

class QGroupBox;
class QCheckBox;
class QPushButton;
class QSpinBox;
class QComboBox;
class QDoubleSpinBox;
//=============
class SaWidget : public QWidget
{
	Q_OBJECT

signals:
	void sendAngleValueToMainWindow(int portId,  int angleValue);	//@
	void sendOnOffFlag(int);	//@

	private slots:	//@todo: public??
		//--Power Limb Check
		void headPowerCheck(int);
		void leftArmPowerCheck(int);
		void rightArmPowerCheck(int);
		void leftLegPowerCheck(int);
		void rightLegPowerCheck(int);
		//--plane check
		void headPanelCheck(int state);
		void leftArmPanelCheck(int);
		void rightArmPanelCheck(int);
		void leftLegPanelCheck(int);
		void rightLegPanelCheck(int);
		//--set JointsData->jointsAngle
		void setAngleValue(int portId, int angleValue);
		//--set SaTools On/Off Flag
		void setOnOffFlag(int	flag);
		//--pushBotton
		void allJointsToZero(bool);
		void allJointsPowerOn(bool);
		void allJointsPowerOff(bool);
		void goToInitialStateButton();

		private slots:
			void aboutSaTools();
			void changeDecimalNum();	//@
			void setCurrentRobotName(int currentRobotNumber);

		private slots:		//Entity SLOTS
			void toEntityFlagSwitch(int);
			void myStiffnessValueChange(double);
			void setStiffnessValue(int,int);
			//------SaPlayer SLOTS
			void saPlayerAction(int,int,QVector<JointsData>);

public:
	SaWidget(QWidget *parent);
	~SaWidget();
	void addRobotsName();

private:
	void createMenu();
	void createHelpDialog();
	//--control plane
	void createControlGroupBox();
	void createEntityNaoControlGroupBox();
	//---limb plane
	void createHeadGroupBox();
	void createLeftArmGroupBox();
	void createRightArmGroupBox();
	void createLeftLegGroupBox();
	void createRightLegGroupBox();
	//--style sheet
	void useStyleSheet();
	void freshSize();

private:
	JointsData * myJointsData;

private:
	//------new added to control box
	QCheckBox * headLimbPowerCheckBox;
	QCheckBox * leftArmLimbPowerCheckBox;
	QCheckBox * rightArmLimbPowerCheckBox;
	QCheckBox * leftLegLimbPowerCheckBox;
	QCheckBox * rightLegLimbPowerCheckBox;

private:
	//------helpDialog
	QDialog * helpDialog;
	QDialogButtonBox * buttonBox;
	QListWidget * directionListWidget;

private:
	//--------QSpinBox for precision
	QSpinBox * precisionSpinBox;
	QComboBox *currentRobotNameComboBox;
	//--------Layout
	QHBoxLayout * saWidgetLayout;

	//--------Menu
	QMenuBar *menuBar;
	QMenu *fileMenu;
	QMenu *helpMenu;
	//--------QAction
	QAction *exitAction;
	QAction *aboutQtAct;
	QAction *aboutSaToolsAct;
	//--------control Group Box
	QGroupBox * controlGroupBox;

	QCheckBox *  headPanelCheckBox;
	QCheckBox *  leftArmPanelCheckBox;
	QCheckBox *  rightArmPanelCheckBox;
	QCheckBox *  leftLegPanelCheckBox;
	QCheckBox *  rightLegPanelCheckBox;

	QCheckBox * saToolsSwitchCheckBox;
	QCheckBox * jointsPowerListCheckBox;

	QPushButton * allJointsToZeroButton;
	QPushButton * allPowerOnButton;
	QPushButton * allPowerOffButton;
	QPushButton * gotoInitialStatePushButton;

	//--------basicJoint
	BasicJoint * basicJointUnits[JointsData::numOfJoints];	

	//--------limbs joint Group Box
	QGroupBox * headJointsGroupBox;
	QGroupBox * leftArmJointsGroupBox;
	QGroupBox * rightArmJointsGroupBox;
	QGroupBox * leftLegJointsGroupBox;
	QGroupBox * rightLegJointsGroupBox;

	//--------Entity Nao Control Group Box
	QGroupBox * entityNaoControlGroupBox;
	ConnectWidget * onlyConnectWidget;
	QCheckBox * toEntityCheckBox;
	float myStiffness;
	QDoubleSpinBox * myStiffnessSpinBox;
	//--------SaPlayer Plane
	SaPlayerWidget * mySaPlayerWidget;

private:
	bool	toEntityFlag;
};

#endif	//SAWIDGET_H