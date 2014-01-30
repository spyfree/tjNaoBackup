#include "ConnectWidget.h"
#include <QtGui>
#include "Simulation/saTools/SimNaoProvider/simNaoProvider.h"
//==========SLOTS
void ConnectWidget::saveEditInfo()
{
	if(robotListInfo.size())
	{
	//-----refresh  vector<ConnectInformation>
	currentRobotConnectInfo.masterNameString = robotNameLineEdit->text();
	currentRobotConnectInfo.ipString = ipLineEdit->text();
	currentRobotConnectInfo.portString = portLineEdit->text();
	robotListInfo[robotListComboBox->currentIndex()] = currentRobotConnectInfo;
	//-----refresh  robotListComboBox
	robotListComboBox->setItemText(robotListComboBox->currentIndex(),currentRobotConnectInfo.masterNameString);
	}
}

void ConnectWidget::robotListBox(int index)
{
	if(index>=0)
	{
		refreshInfo(robotListInfo[index]);
		currentRobotConnectInfo = robotListInfo[index];		//keep that: comboItem==vector<Info>==curInfo
	}
}

void ConnectWidget::connectButton(bool)
{
	connectPushButton->setEnabled(0);
	disConnectPushButton->setEnabled(1);
	SimNaoProvider::getInstance()->doConnect(currentRobotConnectInfo.ipString.toStdString());
}

void ConnectWidget::disConnectButton(bool)
{
	connectPushButton->setEnabled(1);
	disConnectPushButton->setEnabled(0);
}

void ConnectWidget::addRobotInfoButton()
{
	ConnectInformation newConnectInfo;
	newConnectInfo.masterNameString="newNao";
	
	robotListInfo.push_back(newConnectInfo);
	robotListComboBox->addItem(QIcon(":/robot/icons/saTools/Robots/Red.png"), newConnectInfo.masterNameString );
	
	if(robotListComboBox->count()>1)
		robotListComboBox->setCurrentIndex(robotListComboBox->currentIndex()+1);
	else
		robotListComboBox->setCurrentIndex(0);
}

void ConnectWidget::deleRobotInfoButton()
{
	if(robotListInfo.size()>0)
	{
		robotListInfo.pop_back();
		robotListComboBox->currentIndex();
		robotListComboBox->removeItem(robotListComboBox->currentIndex());
	}
}

void ConnectWidget::editRobotInfoButton()
{
	static bool editWidgetFlag = 0;
	if(editWidgetFlag)
		{
			robotInfoEditWidget->hide();
			editWidgetFlag = 0;
		}
	else
		{
			robotInfoEditWidget->show();
			editWidgetFlag = 1;
		}
	resizeConnectWidget();
}
//==========Functions
ConnectWidget::ConnectWidget(QWidget *parent)
: QWidget(parent)
{
	createRobotInfoEditWidget();
	//-----Components
	robotListComboBox = new QComboBox;

	connectPushButton = new QPushButton(QIcon(":/sa/connect/icons/saTools/ConnectWidget/connect.png"),tr("Connect"),this);
	disConnectPushButton = new QPushButton(QIcon(":/sa/connect/icons/saTools/ConnectWidget/disconnect"),tr("Disconnect"),this);
	disConnectPushButton->setEnabled(0);

	addRobotInfoPushButton = new QPushButton(QIcon(":/sa/connect/icons/saTools/ConnectWidget/add.png"),tr(""),this);
	deleRobotInfoPushButton = new QPushButton(QIcon(":/sa/connect/icons/saTools/ConnectWidget/delete.png"),tr(""),this);
	editRobotInfoPushButton = new QPushButton(QIcon(":/sa/connect/icons/saTools/ConnectWidget/edit.png"),tr(""),this);
	//-----Connects
	connect(connectPushButton,SIGNAL(clicked(bool)),		this,SLOT(connectButton(bool))		);
	connect(disConnectPushButton,SIGNAL(clicked(bool)),		this,SLOT(disConnectButton(bool))	);

	connect(addRobotInfoPushButton,SIGNAL(clicked()),		this,SLOT(addRobotInfoButton())	);
	connect(deleRobotInfoPushButton,SIGNAL(clicked()),	this,SLOT(deleRobotInfoButton())	);
	connect(editRobotInfoPushButton,SIGNAL(clicked()),		this,SLOT(editRobotInfoButton())	);

	connect(robotListComboBox,SIGNAL(currentIndexChanged(int)),		this,SLOT(robotListBox(int))	);
	//-----Layout
	QGridLayout  * mainLayout = new QGridLayout;
	QHBoxLayout  * hLayout = new QHBoxLayout;
	hLayout->addWidget(editRobotInfoPushButton);
	hLayout->addWidget(addRobotInfoPushButton);
	hLayout->addWidget(deleRobotInfoPushButton);
	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(robotListComboBox);
	vLayout->addLayout(hLayout);

	mainLayout->addLayout(vLayout,0,0,1,2);
	mainLayout->addWidget(robotInfoEditWidget,1,0,1,2);
	mainLayout->addWidget(connectPushButton,2,0,1,1);
	mainLayout->addWidget(disConnectPushButton,2,1,1,1);

	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);

	setLayout(mainLayout);
	//-------InitList
	readInfo();
}

ConnectWidget::~ConnectWidget()
{
	writeInfo(robotListInfo);
}

void ConnectWidget::createRobotInfoEditWidget()
{
	robotInfoEditWidget = new QWidget;
	robotNameLineEdit = new QLineEdit(tr(""),robotInfoEditWidget);
	ipLineEdit = new QLineEdit(tr(""),robotInfoEditWidget);
	portLineEdit = new QLineEdit(tr(""),robotInfoEditWidget); 
	//------Connects
	connect(robotNameLineEdit,SIGNAL(editingFinished()),		this,SLOT(saveEditInfo())	);
	connect(ipLineEdit,SIGNAL(editingFinished()),		this,SLOT(saveEditInfo())	);
	connect(portLineEdit,SIGNAL(editingFinished()),		this,SLOT(saveEditInfo())	);
	//------Layout
	QGridLayout  * layout = new QGridLayout;
	layout->addWidget(robotNameLineEdit,0,1,1,1);
	layout->addWidget(ipLineEdit,1,1,1,1);
	layout->addWidget(portLineEdit,2,1,1,1);

	QLabel * tempLabel_1 = new QLabel( tr("Robot Name:") );
	layout->addWidget(tempLabel_1,0,0);
	QLabel * tempLabel_2 = new QLabel( tr("IP:") );
	layout->addWidget(tempLabel_2,1,0);
	QLabel * tempLabel_3 = new QLabel( tr("Port:") );
	layout->addWidget(tempLabel_3,2,0);

	robotInfoEditWidget->setLayout(layout);
	robotInfoEditWidget->hide();
}

void ConnectWidget::refreshInfo(ConnectInformation  curConnectInfo)
{
	robotNameLineEdit->setText(curConnectInfo.masterNameString);
	ipLineEdit->setText(curConnectInfo.ipString);
	portLineEdit->setText(curConnectInfo.portString);
}

void ConnectWidget::resizeConnectWidget()
{
	QSize * tempSize = new QSize;
	//*tempSize = minimumSize();	
	*tempSize = minimumSizeHint();
	setMinimumSize(*tempSize);
	resize(*tempSize);
}
//===========
void ConnectWidget::writeInfo(vector<ConnectInformation>  robotVector)
{
	std::string path=std::string(GetSetting::GetConfigDir())+"/ipAddress.cfg";
	QFile file(path.c_str());//QFile file(path.c_str());
	file.open(QIODevice::ReadWrite|QIODevice::Truncate);	//??
	QTextStream stream(&file);

	for(int i=0;i<robotListInfo.size();i++)
	{
		//if(i!=m)
		{
			if(!robotListInfo[i].masterNameString.isEmpty())
				stream<<robotListInfo[i].masterNameString;
			else
				stream<<"Null";
			stream<<"\t";

			if(!robotListInfo[i].ipString.isEmpty())
				stream<<robotListInfo[i].ipString;
			else
				stream<<"0";
			stream<<"\t";

			if(!robotListInfo[i].portString.isEmpty())
				stream<<robotListInfo[i].portString;
			else
				stream<<"0";
			stream<<"\n";
		}
	}
	file.close();
}

void ConnectWidget::readInfo()
{
	//std::string path = "ipAddress.cfg";   //for debug
	std::string path=std::string(GetSetting::GetConfigDir())+"/ipAddress.cfg";
	QFile file(tr(path.c_str()));
	file.open(QIODevice::ReadWrite);
	QTextStream stream(&file);
//@todo:解决空格问题
	while(!stream.atEnd())
	{
		ConnectInformation info;
		stream>>info.masterNameString;
		stream>>info.ipString;
		stream>>info.portString;

		if(info.masterNameString != NULL)
			robotListInfo.push_back(info);
	}

	for(int i=0;i<robotListInfo.size();i++)
	{
		currentRobotConnectInfo=robotListInfo[i];
		robotListComboBox->addItem(QIcon(":/robot/icons/saTools/Robots/Turquoise.png"),currentRobotConnectInfo.masterNameString);
		robotListComboBox->setCurrentIndex(i);
	}

	file.close();
}