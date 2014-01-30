#include "SaPlayerWidget.h"
#include "../SimNaoProvider/simNaoProvider.h"
#include <string>
#include <QtGui>
//=========SLOTS
void SaPlayerWidget::recordNowJointsDataButton()
{
	QString path = saConfigFilePathString+ "/SpecActions/" + "SaveNow.txt";
	QFile file(path);
	file.open(QIODevice::ReadWrite | QIODevice::Append);	//@todo: read & write
	QTextStream textStream(&file);
	//QDataStream dataStream(&file);
	//dataStream.setVersion(10);

	std::vector<float> tempJoints = SimNaoProvider::getInstance()->getAllAnglesMotion();
	for (int i=0;i<tempJoints.size();i++)
	{
		if (i==2|i==6|i==10|i==16)
		{
			textStream<<"			";
		}
		textStream<<tempJoints[JointsData::MotionProxyToSaMap[i]]*180/3.1415926<<"  ";
	}
	textStream<<"\r\n";

	for (int i=0;i<tempJoints.size();i++)
	{
		textStream<<tempJoints[i]<<",";
	}
	textStream<<"\r\n"<<"//====================="<<"\r\n";

	file.close();
}
void SaPlayerWidget::freshDocumentFilesButton()
{
	freshDocumentFile();
}
void SaPlayerWidget::openSaConfigFileButton()
{
	QString comman = "start  ";
	QString path = saConfigFilePathString + "/SpecActions/";	
	QString command = comman +path +namesOfFile[currentFileNum];
	system( command.toAscii() );
}

void SaPlayerWidget::openFloderButton()
{
	QString comman = "explorer  ";
	QString path = saConfigFilePathString + "/SpecActions";	  //@todo: path
	QString command = comman +path;
	command.replace("/","\\");
	system( command.toAscii() );
}

void SaPlayerWidget::activeButton()
{
	static int flag =0;
	//if(flag<0)
	//{
	//	infoDisplayScreenTreeView->show();
	//	flag=1;
	//	return;
	//}
	//----------
	if(flag==0)
	{
		infoDisplayScreenTreeView->show();
		flag=1;
	}
	else if(flag==1)
	{
		infoDisplayScreenTreeView->hide();
		flag=0;
	}
}
void SaPlayerWidget::currentFileNumChange(int index)
{
	currentFileNum = index;
	freshCurrentFile();
}
void SaPlayerWidget::currentStepChange(int curStep)
{
	currentStep = curStep;
	freshCurrentStep();
}
void SaPlayerWidget::startStepChange(int num)
{
	startStep = num;
}
void SaPlayerWidget::endStepChange(int num)
{
	endStep = num;
}
void SaPlayerWidget::playCurrentStepButton()
{
	emit saPlayerCallSaToolsSignal(SaPlayerWidget::playSingleStep, currentStep, jointValueVector);
}
void SaPlayerWidget::playAllStepsButton()
{
	emit saPlayerCallSaToolsSignal( SaPlayerWidget::playAllStep, currentStep, jointValueVector);
}
void SaPlayerWidget::playRangeStepsButton()
{
	emit saPlayerCallSaToolsSignal( SaPlayerWidget::playRangeStep, currentStep, jointValueVector);
}
//=========Functions
SaPlayerWidget::SaPlayerWidget(QWidget *parent)
: QWidget(parent)
{		
		saConfigFilePathString = QDir::currentPath()  + "/../../../Config";
		saConfigFilePathString = QDir::cleanPath(saConfigFilePathString);
		//----------
		createDirModel();
		createFileList();
		readFileData();
		processFileData();
		createInfoDisplayScreen();
		createControlPlane();
		createPlayerPlane();
		//-------Layouts
		QHBoxLayout * hLayout_1 = new QHBoxLayout;
		hLayout_1->addWidget(saFilesComboBox);
		hLayout_1->addWidget(freshDocumentFilesPushButton);
		hLayout_1->addWidget(openSaConfigFilePushButton);
		
		QHBoxLayout * hLayout_2 = new QHBoxLayout;
		hLayout_2->addWidget(openContainerPushButton);
		hLayout_2->addWidget(activePushButton);
		hLayout_2->addWidget(currentStepSpinBox);

		QVBoxLayout * vLayout_1 = new QVBoxLayout;
		vLayout_1->addLayout(hLayout_1);
		vLayout_1->addWidget(recordNowJointsDataPushButton);
		vLayout_1->addLayout(hLayout_2);
		vLayout_1->addWidget(playPlaneGroupBox);
		vLayout_1->addWidget(infoDisplayScreenTreeView);
		//vLayout_1->addWidget(cfgTextEdit);
		setLayout(vLayout_1);
}

void SaPlayerWidget::createDirModel()
{
	fileDirModel = new QDirModel;
	saConfigPathModelIndex = fileDirModel->index(saConfigFilePathString + "/SpecActions");
	//-----------
	QModelIndex child;
	namesOfFile.clear();
	int tempNumOfRows = fileDirModel->rowCount(saConfigPathModelIndex);
	for (int i=0;i<tempNumOfRows;i++)
	{
		child = saConfigPathModelIndex.child(i,0);	//??
		if(!fileDirModel->isDir(child) )
		{
			QString tempName =  fileDirModel->data(child).toString();
			if(tempName.contains(".act", Qt::CaseInsensitive) && (!tempName.contains(".bak", Qt::CaseInsensitive)))
				namesOfFile.push_back(tempName);
		}
	}
	delete fileDirModel;
	fileDirModel = NULL;
}

void SaPlayerWidget::createFileList()
{
	//--------
	saFilesComboBox = new QComboBox(this);
	for (int i=0;i<namesOfFile.size();i++)
	{
		saFilesComboBox->addItem(QIcon(":/saPlayer/icons/saTools/saPlayerWidget/Uedit32.png"),namesOfFile[i]);
	}
	currentFileNum = saFilesComboBox->currentIndex();		//init currentFileNum
	connect(saFilesComboBox,SIGNAL( currentIndexChanged (int) ),		this,SLOT(currentFileNumChange(int))	);
}
void SaPlayerWidget::freshDocumentFile()
{
	qWarning("This Button is useless now!!");
	QMessageBox::warning(this, tr("Caution!!"),
		tr("<h2>SaTools 1.2</h2>""<p>This button is useless now") );

// 	//=====fresh Document Files
// 	createDirModel();
// 	//=====fresh ComboBox
// 	saFilesComboBox->clear();	//@todo:这里有问题，clear了以后，没有项了，就会报错
// 	for (int i=0;i<namesOfFile.size();i++)
// 	{
// 		saFilesComboBox->addItem(QIcon(":/saPlayer/icons/saTools/saPlayerWidget/Uedit32.png"),namesOfFile[i]);
// 	}
// 	currentFileNum = saFilesComboBox->currentIndex();		//init currentFileNum
}
void SaPlayerWidget::readFileData()
{
	QString path = saConfigFilePathString+ "/SpecActions/" + namesOfFile[currentFileNum];	
	QFile file(path);
	file.open(QIODevice::ReadOnly);	//@todo: read & write
	QTextStream stream(&file);
	fileDataString.clear();
	while(!stream.atEnd())
	{
		fileDataString.push_back(stream.readLine());
	}
	file.close();
}

void SaPlayerWidget::processFileData()
{
	QVector<QStringList> tempFile;
	for (int i=0;i<fileDataString.size();i++)
	{
		if( fileDataString[i][0]!=QChar('#') )		//judge whether current line is effective
			if(!fileDataString[i].isEmpty())			//judge whether current line is EnterLine
			{
				QStringList tempStringList = fileDataString[i].split(" ",QString::SkipEmptyParts);
				tempStringList.removeAll("*");
				tempFile.push_back( tempStringList );
			}
	}
	//----------
	JointsData tempJointData;
	jointValueVector.clear();
	for (int i=0;i<tempFile.size();i++)
	{
		if( !tempFile[i].isEmpty() )
		{
			if(tempFile[i].size()==1)
				numOfStepToAct = tempFile[i][0].toInt();
			else
			{
				for (int j=0;j<tempFile[i].size();j++)
				{
					if(j<JointsData::numOfJoints)
						tempJointData.jointsAngle[j] = tempFile[i][j].toDouble();	//Be Caution;
					if(j==JointsData::numOfJoints)
						tempJointData.numOfCycle = tempFile[i][j].toInt();
					if(j==JointsData::numOfJoints+1)
						tempJointData.timeOfAction = tempFile[i][j].toInt();
				}
				jointValueVector.push_back(tempJointData);
			}
		}
	}
	//----------
}
void SaPlayerWidget::createInfoDisplayScreen()
{
	currentStep=1;
	//--------Model
	infoDisplayModel = new QStandardItemModel(0,2,this);
	infoDisplayModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Title"));
	infoDisplayModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Value"));

	for (int i=0;i<JointsData::numOfJoints;i++)
	{
		addItemToModel(infoDisplayModel,tr(JointsData::getJointName(JointsData::Joint(JointsData::numOfJoints-i-1))),
			jointValueVector[currentStep-1].jointsAngle[JointsData::numOfJoints-i-1]	);		//@todo:隐射表
	}
	addItemToModel(infoDisplayModel,tr("numCycle"),jointValueVector[currentStep-1].numOfCycle);
	addItemToModel(infoDisplayModel,tr("time(ms)"),jointValueVector[currentStep-1].timeOfAction);
	addItemToModel(infoDisplayModel,tr("to Act:"),numOfStepToAct);
	addItemToModel(infoDisplayModel,tr("Total Steps:"),jointValueVector.size());
	//----------View
	infoDisplayScreenTreeView = new QTreeView(this);
	infoDisplayScreenTreeView->setRootIsDecorated(true);
	infoDisplayScreenTreeView->setAlternatingRowColors(true);
	infoDisplayScreenTreeView->setModel(infoDisplayModel);
	//infoDisplayScreenTreeView->setRootIndex(infoDisplayModel->index(0,0));
}

void SaPlayerWidget::addItemToModel(QAbstractItemModel *model, QString & name, QVariant value)
{
	model->insertRow(0); 
	model->setData( infoDisplayModel->index(0, 0), name );
	model->setData( infoDisplayModel->index(0,1), value );
}

void SaPlayerWidget::freshCurrentFile()
{
	readFileData();
	processFileData();
	currentStepSpinBox->setValue(1);
	freshModelData();
	currentStepSpinBox->setRange(1,jointValueVector.size());
}

void SaPlayerWidget::freshModelData()
{
	if(currentStep<=jointValueVector.size())		//@todo:因为Model写的太简单，所以使用起来就比较麻烦了，可以考虑改进
	{
		setItemOfModel(infoDisplayModel,3 ,jointValueVector[currentStep-1].numOfCycle);
		setItemOfModel(infoDisplayModel,2 ,jointValueVector[currentStep-1].timeOfAction);
		setItemOfModel(infoDisplayModel,1 ,numOfStepToAct);
		setItemOfModel(infoDisplayModel,0 ,jointValueVector.size());
		for (int i=0;i<JointsData::numOfJoints;i++)
		{
			setItemOfModel(infoDisplayModel,i+4,jointValueVector[currentStep-1].jointsAngle[i]	);		//@todo:隐射表
		}
	}
}

void SaPlayerWidget::setItemOfModel(QAbstractItemModel *model,int row, QVariant value)
{
	model->setData( infoDisplayModel->index(row,1), value );
}

void SaPlayerWidget::createControlPlane()
{
	recordNowJointsDataPushButton = new QPushButton(QIcon("/sa/icons/saTools/GT.png"),tr("Save"),this);
	//setShortCut()
	connect(recordNowJointsDataPushButton,SIGNAL(clicked()),		this,SLOT(recordNowJointsDataButton()));
	//---
	freshDocumentFilesPushButton = new QPushButton(tr("Fresh"),this);
	freshDocumentFilesPushButton->setShortcut(QKeySequence::Find);
	connect(freshDocumentFilesPushButton,SIGNAL(clicked()),	this,SLOT(freshDocumentFilesButton())	);
	//-------
	openSaConfigFilePushButton = new QPushButton(tr("Open"),this);
	openSaConfigFilePushButton->setShortcut(QKeySequence::Open);
	connect(openSaConfigFilePushButton,SIGNAL(clicked()),		this,SLOT(openSaConfigFileButton()));
	//-------
	openContainerPushButton = new QPushButton(tr("Open Folder"),this);
	connect(openContainerPushButton,SIGNAL(clicked()),		this,SLOT(openFloderButton())	);
	//-------
	currentStepSpinBox = new QSpinBox(this);
	currentStepSpinBox->setRange(1,jointValueVector.size());//@todo:每次fresh后都要修正这里的range
	currentStepSpinBox->setValue(currentStep);
	currentStepSpinBox->setPrefix("Current Step:");
	connect(currentStepSpinBox,SIGNAL(valueChanged(int)),		this,SLOT(currentStepChange(int))	);
	//--------
	activePushButton = new QPushButton("Active?",this);
	connect(activePushButton,SIGNAL(clicked()),		this,SLOT(activeButton())	);
}

void SaPlayerWidget::freshCurrentStep()
{
	freshModelData();
}

void SaPlayerWidget::createPlayerPlane()
{
	//------Init Params & Flags
	startStep = 1;
	endStep = 1;
	//------Component
	playCurrentStepPushButton = new QPushButton(QIcon(":/saPlayer/icons/saTools/saPlayerWidget/playCurrentStep.png"),tr(""),this);
	playCurrentStepPushButton->setShortcut(QKeySequence::Print);
	playAllStepsPushButton = new QPushButton(QIcon(":/saPlayer/icons/saTools/saPlayerWidget/playAllSteps.png"),tr(""),this);
	playAllStepsPushButton->setShortcut(QKeySequence::SelectAll);
	playRangeStepsPushButton = new QPushButton(QIcon(":/saPlayer/icons/saTools/saPlayerWidget/playRangeSteps.png"),tr(""),this);
	playRangeStepsPushButton->setShortcut(tr("Ctrl+R"));
	startStepSpinBox = new QSpinBox(this);
	startStepSpinBox->setRange(1,99);
	//startStepSpinBox->setValue(1);
	startStepSpinBox->setPrefix(tr("Start:"));
	endStepSpinBox = new QSpinBox(this);
	endStepSpinBox->setRange(1,99);
	//endStepSpinBox->setValue(1);
	endStepSpinBox->setPrefix(tr("End:"));
	//-------Connect
	connect(startStepSpinBox,SIGNAL(valueChanged(int)),		this,SLOT(startStepChange(int))	);
	connect(endStepSpinBox,SIGNAL(valueChanged(int)),		this,SLOT(endStepChange(int))	);

	connect(playCurrentStepPushButton,SIGNAL(clicked()),		this,SLOT(playCurrentStepButton())	);
	connect(playAllStepsPushButton,SIGNAL(clicked()),		this,SLOT(playAllStepsButton())	);
	connect(playRangeStepsPushButton,SIGNAL(clicked()),		this,SLOT(playRangeStepsButton())	);
	//-------Layout
	QHBoxLayout * hLayout_1 = new QHBoxLayout;
	//hLayout_1->setSpacing(0);
	hLayout_1->addWidget(playCurrentStepPushButton);
	hLayout_1->addWidget(playAllStepsPushButton);
	hLayout_1->addWidget(startStepSpinBox);
	hLayout_1->addWidget(endStepSpinBox);
	hLayout_1->addWidget(playRangeStepsPushButton);

	playPlaneGroupBox = new QGroupBox(this);
	playPlaneGroupBox->setContentsMargins(0,0,0,0);
	playPlaneGroupBox->setLayout(hLayout_1);
}