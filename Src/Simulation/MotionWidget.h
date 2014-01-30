#ifndef MOTION_WIDGET
#define MOTION_WIDGET
#include <QSlider>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <Module/Common/JointData.h>
#include "SimulationData.h"


class MotionWidget:public QFrame
{
	Q_OBJECT
public:
	MotionWidget(QWidget *parent=0,const QString &str="",int min=0,int max=0);
	void SetCurValue(int *value);
	int GetValue();
	void SetinitValue(float value);  //added by qiang
  
signals:
	void SendValue();
private slots:
	void LineValueChanged();
	void SpinValueChanged();
	
private:
	void updateState(int portID);
	SimulationData *selfSimulationData;
	void CreatConnect();
	void CreatWidget();
	bool IsChecked();
	bool IsPower();
	QString widgetName;
	int maxValue;
	int minValue;
	int curValue;
	bool IsInit; 
	QGridLayout *layout;
	QLabel *label;
	QSlider *slider;
	QSpinBox  *spinBox;
	QCheckBox *checkBox;
	QLabel *checkLabel;
	QCheckBox *checkBoxPower;
	QLabel *checkLabelPower;
	

	
};
class MotionWidget_Startup:public QFrame   //newly add
{
	Q_OBJECT
public:
	MotionWidget_Startup(QWidget *parent=0/*,const QString &str=""*/);
	bool NaoReadyToStart;
	double StartJoints[JointData::numOfJoints];
signals:
	void SendValue();
private slots:
		void NaoStartup();  

private:
	SimulationData *StartselfSimulationData;
	void CreatConnect();
	void CreatWidget();
	bool IsChecked();
	QString widgetName;
	QGridLayout *layout;
	QCheckBox *checkBox;
	QLabel *checkLabel;

};
class MotionGroupBox:public QWidget
{
	Q_OBJECT
public:
	MotionGroupBox(QWidget *parent =0);
	void MotionSeen(QString str);
	void initJointData(SafeJointData theSafeJointData);
signals:
	void SendToWin();
private:
	void CreatHeadGroup();
	void CreatLeftLegGroup();
	void CreatLeftHandGroup();
	void CreatRightHandGroup();
	void CreatRightLegGroup();
	void CreatStartupGroup();   //newly add
	QWidget *widget;
	QGroupBox *headGroup;
	QGroupBox *leftLegGroup;
	QGroupBox *leftHandGroup;
	QGroupBox *rightLegGroup;
	QGroupBox *rightHandGroup;
	QGroupBox *naoStartupGroup; //newly add
	QHBoxLayout *boddyLayout;
	QHBoxLayout *headLayout;
	QVBoxLayout *leftLegLay;
	QVBoxLayout *rightLegLay;
	QVBoxLayout *leftHandLay;
	QVBoxLayout *rightHandLay;
	QHBoxLayout *naostartopLayout;  //newly add

	MotionWidget*JointWidget[JointData::numOfJoints];
	MotionWidget_Startup *StartupWidget;
    
	


   
}; 
#endif