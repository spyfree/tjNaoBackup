#include "SensorWidget.h"
#include <QtGui>
//--
#include "Tools/Math/Common.h"
#include "Simulation/saTools/SimNaoProvider/simNaoProvider.h"
#include "Platform/SimuQt/LocalRobot.h"
#include "Simulation/ODESimulation/SimulationCore/Simulation/Simulation.h"
#include "Simulation/ODESimulation/SimulationGUI/Document.h"
//==============Functions
SensorWidget::SensorWidget(QWidget *parent)
	: QWidget(parent)
{	
	//-----Flags
	startFlag = false;
	entityFlag = false;
	timerIntervalMsec = 1000;
	//-----Components
	createView();
	createDragPlaneButtons();
		//---Timer
	freshDataTimer = new QTimer(this);
	freshDataTimer->start(timerIntervalMsec);	//unit: msec
	connect(freshDataTimer,SIGNAL( timeout() ),		this,SLOT( freshData() )	);
		//---control Group Box
	controlGroupBox = new QGroupBox(this);

	timerIntervalSpinBox = new QSpinBox(this);
	timerIntervalSpinBox->setRange(0,10000);
	timerIntervalSpinBox->setValue(timerIntervalMsec);
	connect(timerIntervalSpinBox,SIGNAL(valueChanged(int)),		this,SLOT( updateTimerInterval(int) )	);

	autoFreshCheckBox = new QCheckBox(tr("Auto Fresh?"),this);
	autoFreshCheckBox->setCheckState(Qt::Checked);
	connect(autoFreshCheckBox,SIGNAL( stateChanged(int) ),		this,SLOT( autoFreshSwitch(int) )	);

	sensorWidgetStartCheckBox = new QCheckBox(tr("Start?"),this);
	sensorWidgetStartCheckBox->setCheckState(Qt::Unchecked);
	connect(sensorWidgetStartCheckBox,SIGNAL( stateChanged(int)),		this,SLOT( startFlagSwitch(int))	);

	manualFreshPushButton = new QPushButton(tr("Fresh"),this);
	manualFreshPushButton->setShortcut(QKeySequence::Refresh);
	connect(manualFreshPushButton,SIGNAL(clicked()),		this,SLOT( freshData())	);

	getEntityDataCheckBox = new QCheckBox(tr("Entity?"), this);
	getEntityDataCheckBox->setCheckState(Qt::Unchecked);
	connect(getEntityDataCheckBox,SIGNAL(stateChanged(int)),		this,SLOT( entityFlagSwitch(int))	);

	//-----
	QGridLayout * controlGridLayout = new QGridLayout;
	QHBoxLayout * hLayout_1 = new QHBoxLayout;
	hLayout_1->addWidget(timerIntervalSpinBox);
	hLayout_1->addWidget(autoFreshCheckBox);
	QHBoxLayout * hLayout_2 = new QHBoxLayout;
	hLayout_2->addWidget(sensorWidgetStartCheckBox);
	hLayout_2->addWidget(getEntityDataCheckBox);
	hLayout_2->addWidget(manualFreshPushButton);
	QHBoxLayout * hLayout_3 = new QHBoxLayout;	//--Drag Plane 
	hLayout_3->addWidget(xyPushbutton);
	hLayout_3->addWidget(xzPushbutton);
	hLayout_3->addWidget(yzPushbutton);
	
	controlGridLayout->addLayout(hLayout_1,0,0);
	controlGridLayout->addLayout(hLayout_2,1,0);
	controlGridLayout->addLayout(hLayout_3,2,0);
	controlGroupBox->setLayout(controlGridLayout);
	//-----Layouts		@todo:移植的同时完成布局，试试splitter
 	QGridLayout  * mainLayout = new QGridLayout;

	mainSplitter = new QSplitter(Qt::Vertical,this);
	mainSplitter->addWidget(sourceView);
	mainSplitter->addWidget(controlGroupBox);
	mainSplitter->addWidget(proxyView);
	mainSplitter->setStretchFactor(0,1);
	mainSplitter->setStretchFactor(2,1);

	mainLayout->addWidget(mainSplitter,0,0);
	setLayout(mainLayout);
}

SensorWidget::~SensorWidget()
{

}

void SensorWidget::createView()
{
	//------Proxy Model
	proxySortFilterModel = new QSortFilterProxyModel(this);
	proxySortFilterModel->setDynamicSortFilter(true);
	//------Source View
	sourceView = new QTreeView(this);
	sourceView->setRootIsDecorated(true);
	sourceView->setAlternatingRowColors(true);
	//------Proxy View
	proxyView = new QTreeView(this);
	proxyView->setRootIsDecorated(true);
	proxyView->setAlternatingRowColors(true);
	proxyView->setModel(proxySortFilterModel);
	proxyView->setSortingEnabled(true);
	proxyView->sortByColumn(0, Qt::AscendingOrder);
	//------Set Model
	setJointModel(mySensorModel.JointsSensorModel);
	setNaoSensorModel(mySensorModel.NaoSensorModel);
}

void SensorWidget::setNaoSensorModel(QStandardItemModel *model)
{
	proxySortFilterModel->setSourceModel(model);
}

void SensorWidget::setJointModel(QStandardItemModel *model)
{
	sourceView->setModel(model);
}

//=========SLOTS
void SensorWidget::freshData()
{
	if(startFlag && !entityFlag)		//@todo:区分实体和仿真
	{
		Simulation *sim = Document::document->getSimulation();
		//--------Joint Angle
		double angleValue;
		for (int i = 0;i<JointsData::numOfJoints;i++)
		{
			sim->getSensorportValue(LocalRobot::mySpJoints[i+1],angleValue);	//两边的JointsData差一格
			mySensorModel.setJointSensorDataItem(JointsData::Joint(i),JointsData::angle,angleValue);
		}
		//--------Nao Sensors
		float sensorValue;
		for (int i=0;i<SensorData::numOfSensors;i++)
		{
			sensorValue = LocalRobot::mySensorData.data[i];
			mySensorModel.setNaoSensorDataItem(SensorData::Sensor(i), sensorValue);
		}
	}

	if(startFlag && entityFlag)
	{
		//-------Joint Angle
		vector<float> angleVector;
		angleVector = SimNaoProvider::getInstance()->getAllAnglesMotion();

		for (int i = 0;i<JointsData::numOfJoints;i++)
		{
			mySensorModel.setJointSensorDataItem(JointsData::JointMap[i],JointsData::angle,
				rad2deg(angleVector[i]));
		}
		//--------Joint Stiffness
		vector<float> stiffnessVector;
		stiffnessVector = SimNaoProvider::getInstance()->getAllStiffnessMotion();

		for (int i=0;i<JointsData::numOfJoints;i++)
		{
			mySensorModel.setJointSensorDataItem(JointsData::JointMap[i],JointsData::stiffness,stiffnessVector[i]);
		}
		//---------Nao Sensors
		//@todo:可以考虑用新版naoqi的api
		//@通过SimNaoProvider来获取Sensor的信息，用一个sensordata装一下，然后在这里做一个函数，校正sensorData
		//@sensorsData.h里面没有data这个成员，我这里用了两套sensorData，可能需要考虑统一下，也就是说把sensorsData.h补全，
		//@然后完全在这些gui里面，不去使用SensorData,而改用SensorsData这个类
		
		vector<float> sensorValues = SimNaoProvider::getInstance()->getAllSensorValueMemory();
		for (int i=0;i<sensorValues.size();i++)
		{
			mySensorModel.setNaoSensorDataItem(i,sensorValues[i]);
		}
	}
}

void SensorWidget::updateTimerInterval(int interval)
{
	timerIntervalMsec = interval;
	freshDataTimer->setInterval(timerIntervalMsec);
}

void SensorWidget::autoFreshSwitch(int flag)
{
	if (flag)
	{
		freshDataTimer->start(timerIntervalMsec);
	}
	else
	{
		freshDataTimer->stop();
	}
}

void SensorWidget::startFlagSwitch(int flag)
{
	if (flag)
	{
		startFlag=true;
	}
	else
	{
		startFlag=false;
	}
}

void SensorWidget::entityFlagSwitch(int flag)
{
	if (flag)
	{
		entityFlag=true;
	}
	else
	{
		entityFlag=false;
	}
}

//-------Drag Plane Button
void SensorWidget::createDragPlaneButtons()
{
	xyPushbutton = new QPushButton(tr("X/Y"),this);
	xyPushbutton->setShortcut(QKeySequence(tr("Ctrl+1")));
	xzPushbutton = new QPushButton(tr("X/Z"),this);
	xzPushbutton->setShortcut(QKeySequence(tr("Ctrl+2")));
	yzPushbutton = new QPushButton(tr("Y/Z"),this);
	yzPushbutton->setShortcut(QKeySequence(tr("Ctrl+3")));

	connect(xyPushbutton,SIGNAL(clicked()),		this,SLOT(xyDragPlaneButton()));
	connect(xzPushbutton,SIGNAL(clicked()),		this,SLOT(xzDragPlaneButton()));
	connect(yzPushbutton,SIGNAL(clicked()),		this,SLOT(yzDragPlaneButton()));
}

void SensorWidget::xyDragPlaneButton()
{
	emit	dragPlaneChangedSignal(1);
}

void SensorWidget::xzDragPlaneButton()
{
	emit	dragPlaneChangedSignal(2);
}

void SensorWidget::yzDragPlaneButton()
{
	emit	dragPlaneChangedSignal(3);
}