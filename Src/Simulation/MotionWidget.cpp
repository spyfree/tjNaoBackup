#include "MotionWidget.h"
MotionWidget::MotionWidget(QWidget *parent/* =0 */,const QString &str,int  min,int max):QFrame(parent)
{
	widgetName=str;
	maxValue=max;
	minValue=min;
	curValue=min;
	IsInit=false;
	CreatWidget();
	CreatConnect();
	selfSimulationData=SimulationData::getInstance();

}
void MotionWidget::CreatConnect()
{

	connect(checkBox,SIGNAL(stateChanged(int)),this,SLOT(LineValueChanged()));
	connect(checkBoxPower,SIGNAL(stateChanged(int)),this,SLOT(LineValueChanged()));
   
}
void MotionWidget::CreatWidget()
{
	label=new QLabel;
	slider=new QSlider(Qt::Horizontal);
	spinBox =new QSpinBox;
	layout=new QGridLayout;
	checkBox=new QCheckBox;
	checkLabel=new QLabel("SetValue");
	checkBoxPower=new QCheckBox;
	checkLabelPower=new QLabel("SetPower");
	spinBox->setRange(minValue,maxValue);
	slider->setRange(minValue,maxValue);
	slider->setValue(minValue);
	label->setText(widgetName);
	layout->addWidget(label,0,0,1,2);
	layout->addWidget(spinBox,0,2,1,2);
	layout->addWidget(slider,1,0,1,4);	
	layout->addWidget(checkLabel,2,0);
	layout->addWidget(checkBox,2,1);
	layout->addWidget(checkLabelPower,2,2);
	layout->addWidget(checkBoxPower,2,3);
	//layout->setRowStretch(1,0);
	setLayout(layout);

}

void MotionWidget::LineValueChanged()
{

	curValue=slider->value();
	spinBox->setValue(curValue);
	int id=-1;
	for(int i=0;i<JointData::numOfJoints;i++)
	{
		if(widgetName==JointData::getJointName(JointData::Joint(i)))
		{
			id=i;
			break;
		}
		
	}

	updateState(id);
}
void MotionWidget::SpinValueChanged()
{
	curValue=spinBox->value();
	slider->setValue(curValue);
	int id=-1;
	for(int i=0;i<JointData::numOfJoints;i++)
	{
		if(widgetName==JointData::getJointName(JointData::Joint(i)))
		{
			id=i;
			break;
		}

	}

	updateState(id);
	
}
void MotionWidget::SetCurValue(int *value)
{
	curValue =*value;
}
int  MotionWidget::GetValue()
{
	return curValue;
}
bool MotionWidget::IsChecked()
{
	if(checkBox->checkState()==Qt::Unchecked)
		return false;
	if(checkBox->checkState()==Qt::Checked)
		return true;
}
bool MotionWidget::IsPower()
{
	if(checkBoxPower->checkState()==Qt::Unchecked)
		return false;
	if(checkBoxPower->checkState()==Qt::Checked)
		return true;
}
void MotionWidget::updateState(int portID)
{
	if(IsChecked())
	{
		if(IsPower())
		{
			selfSimulationData->SetJointData(portID,(double)curValue*3.14159/180);

		}
		else 
		{
			selfSimulationData->SetJointData(portID,JointData::off);
		}
	}
	else 
	{
		selfSimulationData->SetJointData(portID,JointData::ignore);
	}
	
	emit SendValue();

}

void MotionWidget::SetinitValue(float value)
{

    curValue = value;
	slider->setValue(curValue);
	spinBox->setValue(curValue);
	connect(slider, SIGNAL(valueChanged(int)),this,SLOT(LineValueChanged()));
	connect(spinBox,SIGNAL(valueChanged(int )),this,SLOT(SpinValueChanged()));
	
}

MotionWidget_Startup::MotionWidget_Startup(QWidget *parent/* =0 *//*,const QString &str*/):QFrame(parent)
{
	CreatWidget();
	CreatConnect();
	double StartJoints_1[JointData::numOfJoints] = {0, 0, 0,
		100, -30, 90, 90,
		100, 30, -90, -90,
		0, 0, -40, 80, -40, 0,
		0, 0, -40, 80, -40, 0};
	for (int i=0;i<JointData::numOfJoints;i++)
	{
		StartJoints[i] = StartJoints_1[i];
	}
	StartselfSimulationData=SimulationData::getInstance();
}

void MotionWidget_Startup::CreatConnect()
{
	connect(checkBox,SIGNAL(stateChanged(int)),this,SLOT(NaoStartup()));
}

void MotionWidget_Startup::CreatWidget()
{
	layout=new QGridLayout;
	checkBox=new QCheckBox;
	checkLabel=new QLabel("isNaoStartup");
	layout->addWidget(checkLabel,2,0);
	layout->addWidget(checkBox,2,1);
	setLayout(layout);
}

void MotionWidget_Startup::NaoStartup()
{
	if(IsChecked())
	{
		for (int i=0;i<JointData::numOfJoints;i++)
		{
			StartselfSimulationData->SetJointData(i,StartJoints[i]*3.14159/180);
		}
	}
	else
	{
		for (int i=0;i<JointData::numOfJoints;i++)
		{
			StartselfSimulationData->SetJointData(i,JointData::off);
		}
	}
	emit SendValue();
}
bool MotionWidget_Startup::IsChecked()
{
	if(checkBox->checkState()==Qt::Unchecked)
		return false;
	if(checkBox->checkState()==Qt::Checked)
		return true;
}



MotionGroupBox ::MotionGroupBox(QWidget *parent /* =0 */):QWidget(parent)
{
	headGroup=new QGroupBox;
	leftLegGroup=new QGroupBox;
	leftHandGroup=new QGroupBox;
	rightLegGroup=new QGroupBox;
	rightHandGroup=new QGroupBox;
	naoStartupGroup=new QGroupBox;  //newly added
	headGroup->setTitle("Head");
	leftLegGroup->setTitle("LeftLeg");
	leftHandGroup->setTitle("LeftHand");
	rightLegGroup->setTitle("RightLeg");
	rightHandGroup->setTitle("RightHand");
	naoStartupGroup->setTitle("NaoStartup");   //newly added
	headLayout=new QHBoxLayout;
	leftHandLay=new QVBoxLayout;
	rightHandLay=new QVBoxLayout;
	leftLegLay =new QVBoxLayout;
	rightLegLay=new QVBoxLayout;
	boddyLayout=new QHBoxLayout; 
	naostartopLayout=new QHBoxLayout;    //newly added

	///*headYaw=new MotionWidget(this,"HeadYaw",-120,120);
	//headPitch=new MotionWidget(this,"HeadYaw"-45,45);
	//LShoulderPitch=new MotionWidget(this,"LShoulderPitch",-120,120);
	//LShoulderRoll=new MotionWidget(this,"LShoulderRoll",0,95);
	//LElbowYaw=new MotionWidget(this,"LElbowYaw",-120,120);
	//LElbowRoll=new MotionWidget(this,"LElbowRoll",-90,0);
	//LWristYaw=new MotionWidget(this,"LWristYaw",-105,105);
	//RShoulderPitch=new MotionWidget(this,"RShoulderPitch",-120,120);
	//RShoulderRoll=new MotionWidget(this,"RShoulderRoll",0,95);
	//RElbowYaw=new MotionWidget(this,"RElbowYaw",-120,120);
	//RElbowRoll=new MotionWidget(this,"RElbowRoll",-90,0);
	//RWristYaw=new MotionWidget(this,"RWristYaw",-105,105);
	//LHipYawPitch=new MotionWidget(this,"LHipYawPitch",-90,0);
	//LHipRoll=new MotionWidget(this,"LHipRoll",-25,45);
	//LHipPitch=new MotionWidget(this,"LHipPitch",-100,25);
	//LKneePitch=new MotionWidget(this,"LKneePitch",0,130);
	//LAnklePitch=new MotionWidget(this,"LAnklePitch",-75,45);
	//LAnkleRoll=new MotionWidget(this,"LAnkleRoll",-45,25);
	//RHipYawPitch=new MotionWidget(this,"RHipYawPitch",-90,0);
	//RHipRoll=new MotionWidget(this,"RHipRoll",-25,45);
	//RHipPitch=new MotionWidget(this,"RHipPitch",-100,25);
	//RKneePitch=new MotionWidget(this,"RKneePitch",0,130);
	//RAnklePitch=new MotionWidget(this,"RAnklePitch",-75,45);
	//RAnkleRoll=new MotionWidget(this,"RAnkleRoll",-45,25);*/
    
  double jointmaxvalues[JointData::numOfJoints] = {0, 120, 45, 120,95, 120,  0, 120,95, 120,  0,  0, 45,  25,130, 45, 45,  0, 45,  25,130, 45, 45};
  double jointminvalues[JointData::numOfJoints] = {0,-120,-45,-120, 0,-120,-90,-120, 0,-120,-90,-90,-25,-100,  0,-75,-45,-90,-25,-100,  0,-75,-45};


	for(int i=(JointData::HeadYaw);i<JointData::numOfJoints;i++)
	{  

		JointWidget[i]=new MotionWidget(this,JointData::getJointName(JointData::Joint(i)),jointminvalues[i],jointmaxvalues[i]);
		connect(JointWidget[i],SIGNAL(SendValue()),this,SIGNAL(SendToWin()));
		
	}
	StartupWidget = new MotionWidget_Startup(this/*,"NaoStartUp"*/);   //newly add
	connect(StartupWidget,SIGNAL(SendValue()),this,SIGNAL(SendToWin()));

	CreatHeadGroup();
	CreatLeftHandGroup();
	CreatLeftLegGroup();
	CreatRightHandGroup();
	CreatRightLegGroup();
	CreatStartupGroup();   //newly add
	boddyLayout->addWidget(leftHandGroup);
	boddyLayout->addWidget(leftLegGroup);
	boddyLayout->addWidget(rightHandGroup);
	boddyLayout->addWidget(rightLegGroup);
	setLayout(boddyLayout);
	
}

void MotionGroupBox::CreatHeadGroup()
{
	for(int i=(JointData::HeadYaw);i<=(JointData::HeadPitch);i++)
	{
	headLayout->addWidget(JointWidget[i]);
	}

	headGroup->setLayout(headLayout);
	headGroup->setVisible(false);
}
void MotionGroupBox::CreatLeftHandGroup()
{
	for(int  i=(JointData::LShoulderPitch);i<=(JointData::LElbowRoll);i++)
	{
		leftHandLay->addWidget(JointWidget[i]);
	}

	leftHandGroup->setLayout(leftHandLay);
	leftHandGroup->setVisible(false);
}
void MotionGroupBox::CreatRightHandGroup()
{
	for(int  i=(JointData::RShoulderPitch);i<=(JointData::RElbowRoll);i++)
	{
		rightHandLay->addWidget(JointWidget[i]);
	}

	rightHandGroup->setLayout(rightHandLay);
	rightHandGroup->setVisible(false);

}
void MotionGroupBox::CreatLeftLegGroup()
{
	for(int  i=(JointData::LHipYawPitch);i<=(JointData::LAnkleRoll);i++)
	{
		leftLegLay->addWidget(JointWidget[i]);
	}

	leftLegGroup->setLayout(leftLegLay);
	
	
}
void MotionGroupBox::CreatRightLegGroup()
{
	for(int  i=(JointData::RHipYawPitch);i<=(JointData::RAnkleRoll);i++)
	{
		rightLegLay->addWidget(JointWidget[i]);
	}
	
	rightLegGroup->setLayout(rightLegLay);
	
}
void MotionGroupBox::CreatStartupGroup()  //newly add
{
	naostartopLayout->addWidget(StartupWidget);
	naoStartupGroup->setLayout(naostartopLayout);
	naoStartupGroup->setVisible(false);
}
void MotionGroupBox::MotionSeen(QString str)
{
	if(str=="Head")
	headGroup->setVisible(!headGroup->isVisible());
	else if(str=="LeftLeg")
	leftLegGroup->setVisible(!leftLegGroup->isVisible());
	else if(str=="LeftArm")
	leftHandGroup->setVisible(!leftHandGroup->isVisible());
	else if(str=="RightLeg")
	rightLegGroup->setVisible(!rightLegGroup->isVisible());
	else if(str=="RightArm")
	rightHandGroup->setVisible(!rightHandGroup->isVisible());
	else if (str=="NaoStartUp") //newly add
	naoStartupGroup->setVisible(!naoStartupGroup->isVisible());
	adjustSize();

}
void MotionGroupBox::initJointData(SafeJointData theSafeJointData)
{
	for(int i=(JointData::HeadYaw);i<JointData::numOfJoints;i++)
	{  

		JointWidget[i]->SetinitValue(theSafeJointData.angles[i]*180/3.14);

	}
}