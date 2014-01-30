#include <QtGui>

#include "basicJoint.h"
//=================static members
int BasicJoint::numOfBasicJoint = 0;

//=================SLOTS
void BasicJoint::angleChangedSignalEmitter( int angleValue)
{
	emit angleChanged(myId,angleValue);
}

void BasicJoint::setJointStiffness(int stiffness)
{
	if(stiffness)
		jointStiffness=1;
	else
		jointStiffness=0;

	emit  stiffnessChanged(myId,stiffness);
}

//=================Functions
BasicJoint::BasicJoint(const QString & jointName, int id, int minJ, int maxJ,QWidget *parent)
: QWidget(parent)
{
	jointStiffness = 1.0;
	//--------- need params
	basicJointGroupBox = new  QGroupBox(  jointName  ,this);	

	maxJoint = maxJ;  
	minJoint = minJ;  
	//----------components
	basicJointSlider = new QSlider(Qt::Horizontal,this);
	basicJointSlider->setFocusPolicy(Qt::StrongFocus);		//这里满写一下比较稳健，在slider比较多的时候可以保证focus有效
	basicJointSlider->setTickPosition(QSlider::TicksAbove);		// display Tick Mark

	basicJointSpinBox = new QSpinBox(this);
	basicJointPowerCheckBox = new QCheckBox( tr("Power") ,this);		//@todo: 显示效果要改改
	basicJointPowerCheckBox->setFont( QFont("Helvetica", 10, QFont::Bold) );
	basicJointPowerCheckBox->setCheckState(Qt::Checked);

	setJointRange();
	//-----------connects
	connect(basicJointSlider,SIGNAL(valueChanged(int)),		basicJointSpinBox,SLOT(setValue(int))	);
	connect(basicJointSpinBox,SIGNAL(valueChanged(int)),		basicJointSlider,SLOT(setValue(int))	);
	basicJointSlider->setValue(0);	//初值设置为0

	connect(basicJointSlider,SIGNAL(valueChanged(int)),		this,SLOT(angleChangedSignalEmitter(int))	);

	connect(basicJointPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(setJointStiffness(int))	);
	//-----------layout assignment
	QGridLayout * basicGroupLayout = new QGridLayout;
	basicGroupLayout->addWidget(basicJointSlider,0,0,1,2);		//row-0,column-0
	basicJointSpinBox->setFixedWidth(43);
	basicGroupLayout->addWidget(basicJointSpinBox,1,0);
	basicJointPowerCheckBox->setFixedWidth(62);
	basicGroupLayout->addWidget(basicJointPowerCheckBox,1,1);
	basicGroupLayout->setContentsMargins(0,0,0,0);
	basicGroupLayout->setSpacing(0);

	basicJointGroupBox->setLayout(basicGroupLayout);
	basicJointGroupBox->setContentsMargins(0,11,0,0);

	myId = id;
	numOfBasicJoint++;
}

void BasicJoint::setJointRange()
{
	basicJointSlider->setRange(minJoint,maxJoint);
	basicJointSpinBox->setRange(minJoint,maxJoint);		//两者既然关联了，那么range也应该统一
}

void BasicJoint::setAngleValue(int angleValue)
{
	basicJointSlider->setValue(angleValue);
}

void BasicJoint::setPower(bool onOff)
{
	if(onOff)	//on
		basicJointPowerCheckBox->setCheckState(Qt::Checked);
	else	//off
		basicJointPowerCheckBox->setCheckState(Qt::Unchecked);
}