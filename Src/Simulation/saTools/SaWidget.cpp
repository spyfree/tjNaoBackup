#include <QtGui>
#include "SaWidget.h"
//-------
#include "Simulation/saTools/SimNaoProvider/simNaoProvider.h"
#include "Platform/SimuQt/RoboCupCtrl.h"
#include "Platform/SimuQt/LocalRobot.h"
#include "Simulation/ODESimulation/SimulationCore/Simulation/Simulation.h"
#include "Simulation/ODESimulation/SimulationGUI/Document.h"
//@todo:  show/hide 机制或许可以再改进改进？
//@todo:  窗口大小需要调整，面板有点太大，在hide了以后应该及时调整，把窗口压到最小的效果

float basicState[22] = {0.00456004,0.00302604,2.08313,0.269942,-1.40825,-1.38209,-0.00149204,4.19617e-05,-0.523052,1.04768,-0.523136,4.19617e-05,-0.00149204,4.19617e-05,-0.523136,1.04623,-0.524586,4.19617e-05,2.07248,-0.268492,1.40203,1.38218};
//========SLOTS
//-------SaPlayer SLOTS
void SaWidget::saPlayerAction(int playType,int curStep,QVector<JointsData> comInfo)
{
	curStep=curStep-1;
	//--------
	switch (playType)
	{
	case SaPlayerWidget::playSingleStep:
		if(LocalRobot::saToolsSwitch)
		for (int i=0;i<JointsData::numOfJoints;i++)
		{
			basicJointUnits[i]->setAngleValue(comInfo[curStep].jointsAngle[i]); 
		}
		
		if(toEntityFlag)
		{
			int start=curStep;
			int end=curStep;
			SimNaoProvider::getInstance()->setAllJointForSaPlayerDCM(start,end,comInfo);
		}
		break;
	//---------
	case SaPlayerWidget::playAllStep:
		if(LocalRobot::saToolsSwitch)
		{
			
		}

		if(toEntityFlag)
		{
			int start=0;
			int end=comInfo.size()-1;
			SimNaoProvider::getInstance()->setAllJointForSaPlayerDCM(start,end,comInfo);
		}
		break;
	//---------
	case SaPlayerWidget::playRangeStep:
		if(LocalRobot::saToolsSwitch)
		{

		}

		if(toEntityFlag)
		{
			int start = mySaPlayerWidget->startStep-1;
			int end = mySaPlayerWidget->endStep-1;
			if(start<=end)
				SimNaoProvider::getInstance()->setAllJointForSaPlayerDCM(start,end,comInfo);
			else
			{
				QMessageBox::about(this, tr("Error!!"),
					tr("<h2>SaTools 1.2</h2>"
					"<p><font color=red>Range Error</font></p> "
					) );
			}
		}
		break;
	}
}
//-------Entity SLOTS
void SaWidget::setStiffnessValue(int portId,int flag)
{
	if (toEntityFlag)
	{
		if(flag)
			SimNaoProvider::getInstance()->setJointStiffnessMotion(string(JointsData::getJointName(JointsData::Joint(portId))),  myStiffness);
		else
			SimNaoProvider::getInstance()->setJointStiffnessMotion(string(JointsData::getJointName(JointsData::Joint(portId))),	0);
	}
}

void SaWidget::myStiffnessValueChange(double stiffness)
{
	myStiffness = stiffness;
	//@todo:是否要在修改了硬度之后，附带一个全关节赋值呢？
}
void SaWidget::toEntityFlagSwitch(int flag)
{
	if (flag)
		toEntityFlag = true;
	else
		toEntityFlag = false;
}
//-------new added
void SaWidget::headPowerCheck(int powerflag)
{
	for (int i = JointsData::HeadYaw; i<=JointsData::HeadPitch;i++)
	{
		basicJointUnits[i]->setPower(powerflag);
	}

	if (toEntityFlag)
	{
		if (powerflag)
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("Head",myStiffness);
		}
		else
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("Head",0);
		}
	}
}

void SaWidget::leftArmPowerCheck(int powerflag)
{
	for(int i = JointsData::LShoulderPitch; i<=JointsData::LElbowRoll;i++)
	{
		basicJointUnits[i]->setPower(powerflag);
	}

	if (toEntityFlag)
	{
		if (powerflag)
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("LArm",myStiffness);
		}
		else
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("LArm",0);
		}
	}
}

void SaWidget::rightArmPowerCheck(int powerflag)
{
	for (int i = JointsData::RShoulderPitch;i<= JointsData::RElbowRoll;i++)
	{
		basicJointUnits[i]->setPower(powerflag);
	}

	if (toEntityFlag)
	{
		if (powerflag)
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("RArm",myStiffness);
		}
		else
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("RArm",0);
		}
	}
}

void SaWidget::leftLegPowerCheck(int powerflag)
{
	for (int i = JointsData::LHipYawPitch; i<=JointsData::LAnkleRoll;i++)
	{
		basicJointUnits[i]->setPower(powerflag);
	}

	if (toEntityFlag)
	{
		if (powerflag)
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("LLeg",myStiffness);
		}
		else
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("LLeg",0);
		}
	}
}

void SaWidget::rightLegPowerCheck(int powerflag)
{
	for (int i = JointsData::RHipYawPitch; i<=JointsData::RAnkleRoll;i++)
	{
		basicJointUnits[i]->setPower(powerflag);
	}

	if (toEntityFlag)
	{
		if (powerflag)
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("RLeg",myStiffness);
		}
		else
		{
			SimNaoProvider::getInstance()->setChainStiffnessMotion("RLeg",0);
		}
	}
}

//--------
void SaWidget::setAngleValue(int portId, int angleValue)		// this->slot  <make jointsData>
{
	myJointsData->jointsAngle[portId] = angleValue;	//just a copy

	if(LocalRobot::saToolsSwitch)
		if(basicJointUnits[portId]->jointStiffness)
		{
			Simulation *sim = Document::document->getSimulation();
			sim->setActuatorport(LocalRobot::myApJoints[portId+1], double(angleValue) );
		}

	if(toEntityFlag)
		if(basicJointUnits[portId]->jointStiffness)
	{
		SimNaoProvider::getInstance()->setJointAngleMotion(string(JointsData::getJointName(JointsData::Joint(portId))),float(angleValue));
	}
}

void SaWidget::setOnOffFlag(int flag)
{
		LocalRobot::saToolsSwitch = bool(flag);
}

void SaWidget::setCurrentRobotName(int currentRobotNumber)
{
	RoboCupCtrl::currentRobotNum = currentRobotNumber+1;	
}

void SaWidget::allJointsToZero(bool)
{
	for (int i=0;i<JointsData::numOfJoints;i++)	
	{
		myJointsData->jointsAngle[i] = 0;
		basicJointUnits[i]->setAngleValue(0);
		setAngleValue(i,0);
		//@todo: 有没有必要将myJointsData->jointsAngle[i]的变化和slider连接起来呢？这样直接控制slider会导致两边的值不一致哟
	}
}

void SaWidget::allJointsPowerOn(bool)
{
	for (int i=0;i<JointsData::numOfJoints;i++)
	{
		basicJointUnits[i]->setPower(1);		// 1->on	//@todo:在power机制完善后这里要添加一些，完善一下
	}
	headLimbPowerCheckBox->setCheckState(Qt::Checked);
	leftArmLimbPowerCheckBox->setCheckState(Qt::Checked);
	rightArmLimbPowerCheckBox->setCheckState(Qt::Checked);
	leftLegLimbPowerCheckBox->setCheckState(Qt::Checked);
	rightLegLimbPowerCheckBox->setCheckState(Qt::Checked);

	if (toEntityFlag)
	{
		SimNaoProvider::getInstance()->setStiffnessMotion(myStiffness);
	}
}

void SaWidget::allJointsPowerOff(bool)
{
	for(int i=0;i<JointsData::numOfJoints;i++)
	{
		basicJointUnits[i]->setPower(0);		// 0->off
	}
	headLimbPowerCheckBox->setCheckState(Qt::Unchecked);
	leftArmLimbPowerCheckBox->setCheckState(Qt::Unchecked);
	rightArmLimbPowerCheckBox->setCheckState(Qt::Unchecked);
	leftLegLimbPowerCheckBox->setCheckState(Qt::Unchecked);
	rightLegLimbPowerCheckBox->setCheckState(Qt::Unchecked);

	if (toEntityFlag)
	{
		SimNaoProvider::getInstance()->setStiffnessMotion(0);
	}
}
void SaWidget::goToInitialStateButton()
{
	RoboCupCtrl::goToInitialStateFlag = true;	

	if (toEntityFlag)
	{
		std::vector<float> basicS;
		for (int i=0;i<22;i++)
		{
			basicS.push_back(basicState[i]);
		}
		SimNaoProvider::getInstance()->setManyJointsAnglesMotion("Body", basicS);
	}
}
//---------
void SaWidget::headPanelCheck(int state)
{
	if(state>0)
		headJointsGroupBox->show();
	else
		headJointsGroupBox->setHidden(1);

	freshSize();
}

void SaWidget::leftArmPanelCheck(int state)
{
	if(state>0)
	{
		leftArmJointsGroupBox->show();
	}
	else
	{
		leftArmJointsGroupBox->close();
	}

	freshSize();
}

void SaWidget::rightArmPanelCheck(int state)
{
	if(state>0)
		rightArmJointsGroupBox->show();
	else
		rightArmJointsGroupBox->close();

	freshSize();
}

void SaWidget::leftLegPanelCheck(int state)
{
	if(state>0)
		leftLegJointsGroupBox->show();
	else
		leftLegJointsGroupBox->close();

	freshSize();
}

void SaWidget::rightLegPanelCheck(int state)
{
	if(state>0)
		rightLegJointsGroupBox->show();
	else
		rightLegJointsGroupBox->close();

	freshSize();
}

void SaWidget::aboutSaTools()
{
	//helpDialog->show();
	QMessageBox::about(this, tr("About SaTools"),
		tr("<h2>SaTools 1.2</h2>"
		"<p>Copyright &copy; 2010 Tjark"
		"<p>Author: Xjasura"
		"<hr>"
		"<p> < font color=red> Direction:  </font> </p>"
		"<hr>"
		) );
}

void SaWidget::changeDecimalNum()
{
	//x
}
//===============Functions
SaWidget::~SaWidget()
{
	delete myJointsData;
	myJointsData = NULL;

	for (int i=0;i<BasicJoint::numOfBasicJoint;i++)
	{
		delete basicJointUnits[i];
		basicJointUnits[i] = NULL;
	}
}
SaWidget::SaWidget(QWidget *parent)
: QWidget(parent)
{
	myJointsData = new JointsData;	
	myStiffness = 0.8;
	//-------------------
	createMenu();
	createControlGroupBox();
	createHelpDialog();
	createHeadGroupBox();
	createLeftArmGroupBox();
	createRightArmGroupBox();
	//---------
	QGridLayout * tempLayout = new QGridLayout;
	tempLayout->addWidget(headJointsGroupBox,0,0,1,2);
	tempLayout->addWidget(leftArmJointsGroupBox,1,0,1,1);
	tempLayout->addWidget(rightArmJointsGroupBox,1,1,1,1);

	createLeftLegGroupBox();
	createRightLegGroupBox();
	//---------SaPlayer
 	mySaPlayerWidget = new SaPlayerWidget(this);
	connect(mySaPlayerWidget,SIGNAL(saPlayerCallSaToolsSignal(int,int,QVector<JointsData>)),		this,SLOT(saPlayerAction(int,int,QVector<JointsData>))	);
	
	//========Layout
	saWidgetLayout =new QHBoxLayout(this);
	saWidgetLayout->setMenuBar(menuBar);
	QVBoxLayout * vLayout_1 = new QVBoxLayout;
	vLayout_1->addWidget(controlGroupBox);
	//vLayout_1->addWidget(mySaPlayerWidget);
	saWidgetLayout->addWidget(mySaPlayerWidget);
	saWidgetLayout->addLayout(vLayout_1);
	saWidgetLayout->addLayout(tempLayout);
	saWidgetLayout->addWidget(leftLegJointsGroupBox);
	saWidgetLayout->addWidget(rightLegJointsGroupBox);

	setLayout(saWidgetLayout);
	setWindowTitle( tr("Special Action Tools") );
	setWindowIcon(QIcon(":/sa/icons/saTools/GT.png"));
}

void SaWidget::createMenu()
{
	menuBar = new QMenuBar(this);		// MenuBar=>Menu=>action
	//-------file Menu Bar
	fileMenu = new QMenu(tr("&File"), this);
	exitAction = fileMenu->addAction(tr("E&xit (x)"));
	connect( exitAction, SIGNAL(triggered()), this, SLOT(close()) );  //??
	//----------help Menu Bar	
	aboutQtAct = new QAction( tr("About &Qt (Q)"),this );
	aboutQtAct->setIcon(	QIcon(tr(":/sa/icons/saTools/Qt.png"))	);
	connect(aboutQtAct,SIGNAL(triggered()),		qApp,SLOT(aboutQt())	);

	aboutSaToolsAct= new QAction(tr("About &This (T)"),this);
	aboutSaToolsAct->setIcon( QIcon(tr(":/sa/icons/saTools/GT.png")) );
	connect(aboutSaToolsAct,SIGNAL(triggered()),		this,SLOT(aboutSaTools())	);

	helpMenu = new QMenu(tr("&Help"),this);
	helpMenu->addAction(aboutQtAct);
	helpMenu->addAction(aboutSaToolsAct);
	//-----------
	menuBar->addMenu(fileMenu);
	menuBar->addMenu(helpMenu);
}

void SaWidget::createControlGroupBox()
{
	controlGroupBox = new QGroupBox( tr("Control Group"),this );
	//------check box
	headPanelCheckBox = new QCheckBox( tr("Head") ,this);
	headPanelCheckBox->setCheckState(Qt::Checked);
	leftArmPanelCheckBox = new QCheckBox( tr("Left Arm") ,this);
	leftArmPanelCheckBox->setCheckState(Qt::Checked);
	rightArmPanelCheckBox = new QCheckBox( tr("Right Arm") ,this);
	rightArmPanelCheckBox->setCheckState(Qt::Checked);
	leftLegPanelCheckBox = new QCheckBox( tr("Left Leg") ,this);
	leftLegPanelCheckBox->setCheckState(Qt::Checked);
	rightLegPanelCheckBox = new QCheckBox( tr("Right Leg") ,this);
	rightLegPanelCheckBox->setCheckState(Qt::Checked);

	saToolsSwitchCheckBox = new QCheckBox( tr("Sa Switch") ,this);
	saToolsSwitchCheckBox->setCheckState(Qt::Unchecked);
	//jointsPowerListCheckBox = new QCheckBox( tr("Joints Power List"),this );	//@todo:尚未装上
	
	//------buttons
	allJointsToZeroButton = new QPushButton( tr("All Joints to 0"),this );
	allPowerOnButton = new QPushButton( tr("All Power On"),this );
	allPowerOffButton = new QPushButton( tr("All Power Off"),this );
	gotoInitialStatePushButton = new QPushButton(tr("Recover"),this);
	gotoInitialStatePushButton->setShortcut(tr("Ctrl+Z"));
	gotoInitialStatePushButton->setToolTip(tr("Ctrl+Z"));
	//------Precisioin spinBox 尚未使用
	//precisionSpinBox = new QSpinBox(this);//( tr("Precision")	);
	//precisionSpinBox->setRange(0,12);
	//precisionSpinBox->setValue(1);
	//connect( precisionSpinBox,SIGNAL(valueChanged(int)),		this,SLOT(changeDecimalNum(int)) ); 

	//----comboBox
	currentRobotNameComboBox = new QComboBox(this);
	for (int i=1;i<=6;i++)  //@todo:这里的数量预先给几个凑合着用
	{
		currentRobotNameComboBox->addItem(	  tr("Robot_%1").arg(i)	);
	}
	connect(currentRobotNameComboBox,SIGNAL(currentIndexChanged(int)),
					this,SLOT(setCurrentRobotName(int))	);
	//------connects
	connect(headPanelCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(headPanelCheck(int )) 	);
	connect(leftArmPanelCheckBox,SIGNAL(stateChanged(int)),	  this,SLOT(leftArmPanelCheck(int))	);
	connect(rightArmPanelCheckBox,SIGNAL(stateChanged(int)),	this,SLOT(rightArmPanelCheck(int))	);
	connect(leftLegPanelCheckBox,SIGNAL(stateChanged(int)),	  this,SLOT(leftLegPanelCheck(int))	);
	connect(rightLegPanelCheckBox,SIGNAL(stateChanged(int)),	this,SLOT(rightLegPanelCheck(int))	);

	//connect(saToolsSwitchCheckBox,SIGNAL(stateChanged(int)),		this,SIGNAL(sendOnOffFlag(int))	);
	connect(saToolsSwitchCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(setOnOffFlag(int))	);

	connect(allJointsToZeroButton,SIGNAL(clicked(bool)),	this,SLOT(allJointsToZero(bool))		);
	connect(allPowerOnButton,SIGNAL(clicked(bool)),	this,SLOT(allJointsPowerOn(bool))	);
	connect(allPowerOffButton,SIGNAL(clicked(bool)),	this,SLOT(allJointsPowerOff(bool))	);
	connect(gotoInitialStatePushButton,SIGNAL(clicked(bool)),	this,SLOT(goToInitialStateButton())	);

	//==========Layout
	QVBoxLayout * planeCheckLayout = new QVBoxLayout;
	planeCheckLayout->addWidget(headPanelCheckBox);
	planeCheckLayout->addWidget(leftArmPanelCheckBox);
	planeCheckLayout->addWidget(rightArmPanelCheckBox);
	planeCheckLayout->addWidget(leftLegPanelCheckBox);
	planeCheckLayout->addWidget(rightLegPanelCheckBox);
	planeCheckLayout->setContentsMargins(0,0,0,0);
	planeCheckLayout->setSpacing(0);

	QGridLayout * pushButtonLayout = new QGridLayout;
	pushButtonLayout->addWidget(saToolsSwitchCheckBox,0,0);
	pushButtonLayout->addWidget(currentRobotNameComboBox,5,0);//comboBox
	pushButtonLayout->addWidget(allJointsToZeroButton,1,0);
	pushButtonLayout->addWidget(allPowerOnButton,2,0);
	pushButtonLayout->addWidget(allPowerOffButton,3,0);
	pushButtonLayout->addWidget(gotoInitialStatePushButton,4,0);

	//-----new added to control box(Power Control)
	//QGroupBox * limbPowerGroupBox = new QGroupBox( tr("Limb Power") );
	headLimbPowerCheckBox = new QCheckBox( tr("Head Limb") ,this);
	headLimbPowerCheckBox->setCheckState(Qt::Checked);
	leftArmLimbPowerCheckBox = new QCheckBox( tr("Left Arm Limb") ,this);
	leftArmLimbPowerCheckBox->setCheckState(Qt::Checked);
	rightArmLimbPowerCheckBox = new QCheckBox( tr("Right Arm Limb") ,this);
	rightArmLimbPowerCheckBox->setCheckState(Qt::Checked);
	leftLegLimbPowerCheckBox = new QCheckBox( tr("Left Leg Limb") ,this);
	leftLegLimbPowerCheckBox->setCheckState(Qt::Checked);
	rightLegLimbPowerCheckBox = new QCheckBox( tr("Right Leg Limb") ,this);
	rightLegLimbPowerCheckBox->setCheckState(Qt::Checked);

	connect(headLimbPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(headPowerCheck(int))	);
	connect(leftArmLimbPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(leftArmPowerCheck(int))		);
	connect(rightArmLimbPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(rightArmPowerCheck(int))	);
	connect(leftLegLimbPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(leftLegPowerCheck(int))	);
	connect(rightLegLimbPowerCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(rightLegPowerCheck(int))	);

	QVBoxLayout * limbPowerLayout = new QVBoxLayout;
	limbPowerLayout->setContentsMargins(0,0,0,0);
	limbPowerLayout->setSpacing(0);
	limbPowerLayout->addWidget(headLimbPowerCheckBox);
	limbPowerLayout->addWidget(leftArmLimbPowerCheckBox);
	limbPowerLayout->addWidget(rightArmLimbPowerCheckBox);
	limbPowerLayout->addWidget(leftLegLimbPowerCheckBox);
	limbPowerLayout->addWidget(rightLegLimbPowerCheckBox);
	//limbPowerGroupBox->setLayout(limbPowerLayout);
	//-----Entity Control Group
	createEntityNaoControlGroupBox();
	
	//-----
	QHBoxLayout * hLayout_2 = new QHBoxLayout;
	hLayout_2->addLayout(planeCheckLayout);		// row-0  column-0
	hLayout_2->addLayout(limbPowerLayout);

	QHBoxLayout * hLayout_1 = new QHBoxLayout;
	hLayout_1->addLayout(pushButtonLayout);
	hLayout_1->addWidget(entityNaoControlGroupBox);

	QVBoxLayout * controlBoxLayout =new QVBoxLayout;
	controlBoxLayout->setContentsMargins(0,0,0,0);
	controlBoxLayout->setSpacing(0);
	controlBoxLayout->addLayout(hLayout_2);
	controlBoxLayout->addLayout(hLayout_1);
	controlBoxLayout->addStretch(2);

	controlGroupBox->setLayout(controlBoxLayout);
	//----style sheet
	useStyleSheet();
}

void SaWidget::createEntityNaoControlGroupBox()
{
	entityNaoControlGroupBox = new QGroupBox(tr("Entity Control"),this);
	onlyConnectWidget = new ConnectWidget(this);

	toEntityCheckBox = new QCheckBox(tr("Entity?"),this);
	toEntityFlag = false;
	connect(toEntityCheckBox,SIGNAL(stateChanged(int)),		this,SLOT(toEntityFlagSwitch(int))	);

	myStiffnessSpinBox = new QDoubleSpinBox(this);
	myStiffnessSpinBox->setPrefix("Stiffness:");
	myStiffnessSpinBox->setRange(0.0,1.0);
	myStiffnessSpinBox->setSingleStep(0.1);
	myStiffnessSpinBox->setValue(myStiffness);
	connect(myStiffnessSpinBox,SIGNAL(valueChanged(double)),		this,SLOT(myStiffnessValueChange(double))	);

	QVBoxLayout * layout_1 = new QVBoxLayout;
	layout_1->addWidget(onlyConnectWidget);
	QHBoxLayout * hLayout_1 = new QHBoxLayout;
	hLayout_1->addWidget(toEntityCheckBox);
	hLayout_1->addWidget(myStiffnessSpinBox);
	layout_1->addLayout(hLayout_1);
	entityNaoControlGroupBox->setLayout(layout_1);
	entityNaoControlGroupBox->setContentsMargins(0,0,0,0);
}
void SaWidget::createHeadGroupBox()
{
	headJointsGroupBox = new QGroupBox(tr("Head Limb"),this);

	//-------layout assignment
	QHBoxLayout * headJointsGroupBoxLayout = new QHBoxLayout;

	//for (int i = 0; i<=1;i++)
	for(int i = JointsData::HeadYaw; i<=JointsData::HeadPitch;i++)
	{
		basicJointUnits[i] = new BasicJoint(JointsData::getJointName(JointsData::Joint(i)),  i ,
			JointsData::jointsMinArray[i], JointsData::jointsMaxArray[i],this);
		//@todo: connect to setJointsAngles
		headJointsGroupBoxLayout->addWidget(basicJointUnits[i]->basicJointGroupBox);

		connect(basicJointUnits[i],SIGNAL(angleChanged(int, int)),		this,SLOT(setAngleValue(int,  int))	);
		connect(basicJointUnits[i],SIGNAL(stiffnessChanged(int, int)),		this,SLOT(setStiffnessValue(int, int))	);
	}

	headJointsGroupBox->setLayout(headJointsGroupBoxLayout);
	headJointsGroupBox->setContentsMargins(0,0,0,0);
	headJointsGroupBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void SaWidget::createLeftArmGroupBox()
{
	leftArmJointsGroupBox = new QGroupBox(tr("Left Arm Limb"),this);

	//-------layout assignment
	QVBoxLayout * leftArmJointsGroupBoxLayout = new QVBoxLayout;

	//for (int i = 6; i<=9;i++)
	for(int i = JointsData::LShoulderPitch; i<=JointsData::LElbowRoll;i++)
	{
		basicJointUnits[i] = new BasicJoint(JointsData::getJointName(JointsData::Joint(i)),  i ,
			JointsData::jointsMinArray[i], JointsData::jointsMaxArray[i],this);
		//@todo: connect to setJointsAngles
		leftArmJointsGroupBoxLayout->addWidget(basicJointUnits[i]->basicJointGroupBox);

		connect(basicJointUnits[i],SIGNAL(angleChanged(int, int)),		this,SLOT(setAngleValue(int,  int))	);
		connect(basicJointUnits[i],SIGNAL(stiffnessChanged(int, int)),		this,SLOT(setStiffnessValue(int, int))	);
	}

	leftArmJointsGroupBox->setLayout(leftArmJointsGroupBoxLayout);
	leftArmJointsGroupBox->setContentsMargins(0,0,0,0);
	leftArmJointsGroupBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
}

void SaWidget::createRightArmGroupBox()
{
	rightArmJointsGroupBox = new QGroupBox(tr("Right Arm Limb"),this);

	//-------layout assignment
	QVBoxLayout * rightArmJointsGroupBoxLayout = new QVBoxLayout;

	//for (int i = 2; i<=5;i++)
	for (int i = JointsData::RShoulderPitch;i<= JointsData::RElbowRoll;i++)
	{
		basicJointUnits[i] = new BasicJoint(JointsData::getJointName(JointsData::Joint(i)),  i ,
			JointsData::jointsMinArray[i], JointsData::jointsMaxArray[i],this);
		//@todo: connect to setJointsAngles
		rightArmJointsGroupBoxLayout->addWidget(basicJointUnits[i]->basicJointGroupBox);

		connect(basicJointUnits[i],SIGNAL(angleChanged(int, int)),		this,SLOT(setAngleValue(int,  int))	);
		connect(basicJointUnits[i],SIGNAL(stiffnessChanged(int, int)),		this,SLOT(setStiffnessValue(int, int))	);
	}

	rightArmJointsGroupBox->setLayout(rightArmJointsGroupBoxLayout);
	rightArmJointsGroupBox->setContentsMargins(0,0,0,0);
	rightArmJointsGroupBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void SaWidget::createLeftLegGroupBox()
{
	leftLegJointsGroupBox = new QGroupBox(tr("Left Leg Limb"),this);

	//-------layout assignment
	QVBoxLayout * leftLegJointsGroupBoxLayout = new QVBoxLayout;

	for (int i = JointsData::LHipYawPitch; i<=JointsData::LAnkleRoll;i++)
	{
		basicJointUnits[i] = new BasicJoint(JointsData::getJointName(JointsData::Joint(i)),  i ,
			JointsData::jointsMinArray[i], JointsData::jointsMaxArray[i],this);
		//@todo: connect to setJointsAngles
		leftLegJointsGroupBoxLayout->addWidget(basicJointUnits[i]->basicJointGroupBox);

		connect(basicJointUnits[i],SIGNAL(angleChanged(int, int)),		this,SLOT(setAngleValue(int,  int))	);
		connect(basicJointUnits[i],SIGNAL(stiffnessChanged(int, int)),		this,SLOT(setStiffnessValue(int, int))	);
	}

	leftLegJointsGroupBox->setLayout(leftLegJointsGroupBoxLayout);
	leftLegJointsGroupBox->setContentsMargins(0,0,0,0);
	leftLegJointsGroupBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void SaWidget::createRightLegGroupBox()
{
	rightLegJointsGroupBox = new QGroupBox(tr("Right Leg Limb"),this);

	//-------layout assignment
	QVBoxLayout * rightLegJointsGroupBoxLayout = new QVBoxLayout;

	for (int i = JointsData::RHipYawPitch; i<=JointsData::RAnkleRoll;i++)
	{
		basicJointUnits[i] = new BasicJoint(JointsData::getJointName(JointsData::Joint(i)),  i ,
			JointsData::jointsMinArray[i], JointsData::jointsMaxArray[i],this);
		//@todo: connect to setJointsAngles
		rightLegJointsGroupBoxLayout->addWidget(basicJointUnits[i]->basicJointGroupBox);

		connect(basicJointUnits[i],SIGNAL(angleChanged(int, int)),		this,SLOT(setAngleValue(int,  int))	);
		connect(basicJointUnits[i],SIGNAL(stiffnessChanged(int, int)),		this,SLOT(setStiffnessValue(int, int))	);
	}

	rightLegJointsGroupBox->setLayout(rightLegJointsGroupBoxLayout);
	rightLegJointsGroupBox->setContentsMargins(0,0,0,0);
	rightLegJointsGroupBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void SaWidget::createHelpDialog()
{
	//@todo:设计一个帮助文档面板(可以做的比较好看，记得要加上html效果)  (QListWidget用在这里效果似乎不太好)

	/*
	helpDialog = new QDialog;
	//---------component
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok	);
	connect(buttonBox, SIGNAL(accepted()), helpDialog, SLOT(accept()));

	directionListWidget = new QListWidget();
	directionListWidget->addItems(QStringList()
		<< "<i>Thank you for your payment which we have received today.</i>"
		<< "Your order has been dispatched and should be with you within 28 days.");
	//---------layout assignment
	QVBoxLayout * helpLayout = new QVBoxLayout;
	helpLayout->addWidget(directionListWidget);
	helpLayout->addWidget(buttonBox);

	helpDialog->setLayout(helpLayout);
	helpDialog->setWindowTitle( tr("Hi~~") );
	*/
}

void SaWidget::addRobotsName()  //暂未使用
{
	//for (int i=1;i<=RoboCupCtrl::robotNamesList.size();i++)  //这里的数量预先给几个凑合着用
	//{
	//	currentRobotNameComboBox->addItem("robot_1");
	//}
}

void SaWidget::useStyleSheet()
{
	setStyleSheet(" QMenuBar {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 lightgray, stop:1 darkgray);}"
		"QMenuBar::item {spacing: 3px; padding: 1px 4px;background: transparent;border-radius: 4px;}"
		"QMenuBar::item:selected { background: #a8a8a8;}"
		"QMenuBar::item:pressed {background: #888888;} ");
}

void SaWidget::freshSize()
{
	QSize  *minSize = new QSize;		
	*minSize = minimumSizeHint(); 
	setMinimumSize(*minSize);
	resize(*minSize);
}