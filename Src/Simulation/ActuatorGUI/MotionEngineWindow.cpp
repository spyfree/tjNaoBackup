#include "MotionEngineWindow.h"
#include "Module/Common/WalkCmd.h"
#include "Platform/SystemCall.h"
#include "Module/Actuator/MotionDebugging.h"
#include "Simulation/SignalUnconflict.h"
//#include "alvalue.h"
//#include "dcmproxy.h"
//#include "alproxy.h"

MotionEngineWindow::MotionEngineWindow(Actuator& _actuator, QMdiArea* mdiArea)
:SubWindow(mdiArea,"MotionEngine"), actuator(_actuator),
//MotionEngineWindow::MotionEngineWindow(Actuator& _actuator, const QString & title, QWidget * parent /* = 0 */)
//:DockWidget(title, parent), actuator(_actuator),
naoName("Nao"), /*remoteActuatorProxy(NULL),*/ remoteActuatorConnected(false)
{
	createWindow();
	resetWalkCmd();
	creadConnect();
	startActuatorButton->setDisabled(false);
	holdActuatorButton->setDisabled(true);
	stopActuatorButton->setDisabled(false);
	cmdSending2Robot->setDisabled(true);
	jointsSending2Robot->setDisabled(true);
// 	saveMDebug->setDisabled(true);
// 	sendCmdButton->setDisabled(true);
// 	stopButton->setDisabled(true);
	emergencyStopButton->setDisabled(true);
}
MotionEngineWindow::~MotionEngineWindow()
{

}
void MotionEngineWindow::closeEvent(QCloseEvent *event)
{
	hide();
}
void MotionEngineWindow::doConnect(const std::string ipAddress_)
{
	ipAddress = ipAddress_;
	if(actuator.doConnect(ipAddress_) || remoteActuatorConnected)
	{
		setConnected(ipAddress_, "NAO");
	}else
	{
		setDisConnected();
	}
}
void MotionEngineWindow::doDisconnect()
{
	actuator.disConnect();
	setDisConnected();
}
void MotionEngineWindow::setConnected(std::string ipAddress_, std::string naoName_)
{
	connected = true;
	ipAddress = ipAddress_;
	naoName = naoName_;
	// TODO: actuator.setIP...
	setWindowTitle(tr("MotionEngine - Connected... IP: ") + tr(ipAddress.c_str()) + tr("  || Name: ") + tr(naoName.c_str()));
	cmdSending2Robot->setDisabled(false);
	cmdSending2Robot->setChecked(false);
	jointsSending2Robot->setDisabled(false);
	jointsSending2Robot->setChecked(false);
	emergencyStopButton->setDisabled(false);
// 	saveMDebug->setDisabled(false);
// 	saveMDebug->setChecked(false);
}

void MotionEngineWindow::setDisConnected()
{
	connected = false;
	/*if(remoteActuatorProxy != NULL)
	{
		delete remoteActuatorProxy;
		remoteActuatorProxy = NULL;
	}*/
	setWindowTitle(tr("MotionEngine - Disconnected "));
	cmdSending2Robot->setDisabled(true);
	cmdSending2Robot->setChecked(false);
	jointsSending2Robot->setDisabled(true);
	jointsSending2Robot->setChecked(false);
// 	saveMDebug->setDisabled(true);
// 	saveMDebug->setChecked(false);
	emergencyStopButton->setDisabled(true);
}

void MotionEngineWindow::onConnectRemoteActuator(bool checked)
{
	/*if(checked){
		try {
			if(remoteActuatorProxy != NULL){
			delete remoteActuatorProxy;
			remoteActuatorProxy=NULL;
			}
			remoteActuatorProxy = new AL::ALProxy("MainModule", "192.168.1.102", 9559);
			remoteActuatorConnected = true;
			cmdSending2Robot->setDisabled(false);
			std::cout<<"remoteActuatorConnected"<<std::endl;
		} catch(AL::ALError &e) {
			remoteActuatorProxy = NULL;
			remoteActuatorConnected = false;
			setDisConnected();
			QMessageBox::warning(this,tr("Cmd Error!"),tr("Failed to Connect TJPro->MainModule"));
			std::cout<<e.toString()<<std::endl;
		}
	}*/
}
void MotionEngineWindow::createWindow()
{
	//Motion Command
	walkRadioButton = new QRadioButton("Walk");
	specialActionRadioButton = new QRadioButton("Special Action");
	motionRadioButtonGroup = new QButtonGroup();
	
	motionRadioButtonGroup->addButton(walkRadioButton);
	motionRadioButtonGroup->addButton(specialActionRadioButton);
	motionRadioButtonGroup->setId(walkRadioButton, eWalkID);
	motionRadioButtonGroup->setId(specialActionRadioButton, eSepcialID);
	walkRadioButton->setChecked(true);
	walkTypeComboBox = new QComboBox();
	// add items here
	for(int i = 0; i < WalkCmd::numWalkTypes; ++i)
	{
		walkTypeComboBox->addItem(WalkCmd::getWalkTypeName(i));
	}
	specialComboBox = new QComboBox();
	for(int i = 0; i < SpecActionCmd::numSpecActions;++i)
	{
		specialComboBox->addItem(SpecActionCmd::getSpecActionName(i));
	}
	QGridLayout* motionCmdLayout = new QGridLayout();
	motionCmdLayout->addWidget(walkRadioButton, 0,0);
	motionCmdLayout->addWidget(specialActionRadioButton, 1,0);
	motionCmdLayout->addWidget(walkTypeComboBox, 0,1);
	motionCmdLayout->addWidget(specialComboBox, 1,1);
	QGroupBox* motionCmdGroup = new QGroupBox("Motion Cmd");
	motionCmdGroup->setLayout(motionCmdLayout);
	//walk cmd
	QLabel* forwardLabel = new QLabel(tr("Forward(cm/s): "));
	QLabel* leftLabel = new QLabel(tr("Left(cm/s): "));
	QLabel* turnLabel = new QLabel(tr("TurnCCW(deg/s): "));
	forwardSlider = new QSlider();
	leftSlider = new QSlider();
	turnSlider = new QSlider();
	forwardSlider->setRange(-10, 10);
	forwardSlider->setSingleStep(1);
	forwardSlider->setTracking(true);
	forwardSlider->setOrientation(Qt::Horizontal);
	leftSlider->setRange(-10, 10);
	leftSlider->setSingleStep(1);
	leftSlider->setTracking(true);
	leftSlider->setOrientation(Qt::Horizontal);
	turnSlider->setRange(-10,10);
	turnSlider->setSingleStep(1);
	turnSlider->setTracking(true);
	turnSlider->setOrientation(Qt::Horizontal);
	forwardEdit = new QLineEdit();
	forwardValidator = new QDoubleValidator(-10.0, 10.0,1,this);
	forwardValidator->setNotation(QDoubleValidator::StandardNotation);
	forwardEdit->setValidator(forwardValidator);
	leftValidator = new QDoubleValidator(-5.0, 5.0,1,this);
	leftValidator->setNotation(QDoubleValidator::StandardNotation);
	leftEdit = new QLineEdit();
	leftEdit->setValidator(leftValidator);
	turnEdit = new QLineEdit();
	turnValidator=new QIntValidator(-20, 20, this);
	turnEdit->setValidator(turnValidator);
// 	forwardEdit->setMaxLength(5);
// 	forwardEdit->setFixedSize(forwardEdit->sizeHint());
// 	leftEdit->setMaxLength(5);
// 	leftEdit->setFixedSize(leftEdit->sizeHint());
// 	turnEdit->setMaxLength(5);
// 	turnEdit->setFixedSize(turnEdit->sizeHint());
	sendCmdButton = new QPushButton(QIcon(":/icons/send.png"), tr("Send"));
	stopButton = new QPushButton(QIcon(":/icons/stop.png"), tr("Stop"));
	emergencyStopButton = new QPushButton(QIcon(":/icons/emergency-stop.png"), tr("EMERGENCY STOP!"));
	//sendCmdButton->setShortcut(QKeySequence(Qt::Key_F));
	stopButton->setShortcut(QKeySequence(Qt::Key_Space));
	QGridLayout* walkLayout = new QGridLayout();
	walkLayout->addWidget(forwardLabel, 0, 0);
	walkLayout->addWidget(forwardSlider, 0, 1);
	walkLayout->addWidget(forwardEdit, 0,2);
	walkLayout->addWidget(leftLabel, 1,0);
	walkLayout->addWidget(leftSlider, 1, 1);
	walkLayout->addWidget(leftEdit, 1, 2);
	walkLayout->addWidget(turnLabel, 2, 0);
	walkLayout->addWidget(turnSlider, 2, 1);
	walkLayout->addWidget(turnEdit, 2, 2);
	walkGroup = new QGroupBox("Walk Command");
	walkGroup->setLayout(walkLayout);
	QHBoxLayout* sendButtonLayout = new QHBoxLayout();
	sendButtonLayout->addWidget(sendCmdButton);
	sendButtonLayout->addWidget(stopButton);
	sendButtonLayout->addWidget(emergencyStopButton);
	//====
	connectRemoteActuatorButton = new QPushButton(tr("Connect Remote Actuator"));
	connectRemoteActuatorButton->setCheckable(true);
	//Actuator Thread Control
	startActuatorButton = new QPushButton(QIcon(":/icons/media-play.png"), tr("Start"));
	startActuatorButton->setCheckable(true);
	holdActuatorButton = new QPushButton(QIcon(":/icons/media-pause.png"), tr("Hold"));
	holdActuatorButton->setCheckable(true);
	stopActuatorButton = new QPushButton(QIcon(":/icons/media-stop.png"), tr("Stop"));
	cmdSending2Robot = new QCheckBox(tr("Cmd Sending..."));
	//cmdSending2Robot->setCheckable(true);
	jointsSending2Robot = new QCheckBox(tr("Joints Sending..."));
	saveMDebug = new QCheckBox(tr("Save MDebug Data"));
	//jointsSending2Robot->setCheckable(true);
	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->addWidget(cmdSending2Robot);
	layout2->addWidget(jointsSending2Robot);
	QVBoxLayout* actuatorLayout = new QVBoxLayout();
	actuatorLayout->setAlignment(Qt::AlignTop);
	actuatorLayout->addWidget(connectRemoteActuatorButton);
	actuatorLayout->addWidget(startActuatorButton);
	actuatorLayout->addWidget(holdActuatorButton);
	actuatorLayout->addWidget(stopActuatorButton);
	actuatorLayout->addLayout(layout2);
	actuatorLayout->addWidget(saveMDebug);
	QVBoxLayout* rightLayout  = new QVBoxLayout();
	QVBoxLayout* midLayout = new QVBoxLayout();
	rightLayout->addWidget(motionCmdGroup);
	midLayout->addWidget(walkGroup);
	midLayout->addLayout(sendButtonLayout);
	QVBoxLayout* leftLayout = new QVBoxLayout();
	leftLayout->addLayout(actuatorLayout);
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addLayout(rightLayout);
	mainLayout->addLayout(midLayout);
	mainLayout->addLayout(leftLayout);
	widget = new QWidget;
	widget->setLayout(mainLayout);
	setWidget(widget);
	//setFixedSize(sizeHint());
	setVisible(true);
}

void MotionEngineWindow::creadConnect()
{
	connect(forwardSlider, SIGNAL(valueChanged(int)), this, SLOT(onFowardSliderChanged()));
	connect(leftSlider, SIGNAL(valueChanged(int)), this, SLOT(onLeftSliderChanged()));
	connect(turnSlider, SIGNAL(valueChanged(int)), this, SLOT(onTurnSliderChanged()));

	connect(forwardEdit, SIGNAL(editingFinished()), this, SLOT(onFowardEditChanged()));
	connect(leftEdit, SIGNAL(editingFinished()), this, SLOT(onLeftEditChanged()));
	connect(turnEdit, SIGNAL(editingFinished()), this, SLOT(onTurnEditChanged()));

	connect(startActuatorButton, SIGNAL(toggled(bool)), this, SLOT(onStartActuator(bool)));
	connect(holdActuatorButton, SIGNAL(toggled(bool)), this, SLOT(onHoldActuator(bool)));
	connect(stopActuatorButton, SIGNAL(clicked(bool)), this, SLOT(onStopActuator()));

	connect(cmdSending2Robot, SIGNAL(toggled(bool)), this, SLOT(onCmdSending(bool)));
	connect(jointsSending2Robot,SIGNAL(toggled(bool)), this, SLOT(onJointsSending(bool)));
	connect(saveMDebug, SIGNAL(toggled(bool)), this, SLOT(onSaveMDebug(bool)));
	connect(sendCmdButton, SIGNAL(clicked(bool)), this, SLOT(onSend()));
	connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(onStopRobot()));
	connect(emergencyStopButton, SIGNAL(clicked(bool)), this, SLOT(onEmergency()));

	connect(connectRemoteActuatorButton, SIGNAL(clicked(bool)), this, SLOT(onConnectRemoteActuator(bool)));
	
}

void MotionEngineWindow::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}

void MotionEngineWindow::resetWalkCmd()
{
	motionCmd.motion = MotionCmd::Walk;
	motionCmd.walkCmd.walkType = WalkCmd::eCartTableWalk;
	motionCmd.walkCmd.speed.translation.x = 0;
	motionCmd.walkCmd.speed.translation.y = 0;
	motionCmd.walkCmd.speed.rotation = 0;
	setControls2Cmd(motionCmd);
}
void MotionEngineWindow::setControls2Cmd(const MotionCmd& motionCmd_)
{	
	if(motionCmd_.motion == MotionCmd::Stand)
	{
		walkGroup->setDisabled(false);
		walkRadioButton->setChecked(true);
		walkTypeComboBox->setCurrentIndex(WalkCmd::eCartTableWalk);
		//specialComboBox
		//forward
		forwardSlider->setSliderPosition(0);
		forwardEdit->setText(QString::number(0.0, 'f',1));
		//left
		leftSlider->setSliderPosition(0);
		leftEdit->setText(QString::number(0.0, 'f',1));
		//turn
		turnSlider->setSliderPosition(0);
		turnEdit->setText(QString::number(0));
	}
	else if(motionCmd_.motion == MotionCmd::Walk)
	{
		walkGroup->setDisabled(false);
		walkRadioButton->setChecked(true);
		walkTypeComboBox->setCurrentIndex(motionCmd_.walkCmd.walkType);
		//specialComboBox
		double speedx = CLIP(motionCmd_.walkCmd.speed.translation.x, forwardValidator->bottom(), forwardValidator->top());
		//forward
		double ratio =(forwardSlider->maximum() - forwardSlider->minimum()) /(forwardValidator->top() -  forwardValidator->bottom());
		int pos = static_cast<int>(ratio * (speedx - forwardValidator->bottom()) + forwardSlider->minimum());
		forwardSlider->setSliderPosition(pos);
		forwardEdit->setText(QString::number(speedx, 'f',1));
		//left
		double speedy= CLIP(motionCmd_.walkCmd.speed.translation.y, leftValidator->bottom(), leftValidator->top());
		ratio =(leftSlider->maximum() - leftSlider->minimum())/ (leftValidator->top() -  leftValidator->bottom());
		pos = static_cast<int>(ratio * (speedy - leftValidator->bottom()) + leftSlider->minimum());
		leftSlider->setSliderPosition(pos);
		leftEdit->setText(QString::number(speedy, 'f',1));
		//turn
		int speedt = CLIP(motionCmd_.walkCmd.speed.rotation, (double)turnValidator->bottom(), (double)turnValidator->top());
		ratio =(double)(turnSlider->maximum() - turnSlider->minimum()) / (turnValidator->top() -  turnValidator->bottom());
		pos = static_cast<int>(ratio * (speedt - turnValidator->bottom()) + turnSlider->minimum());
		turnSlider->setSliderPosition(pos);
		turnEdit->setText(QString::number(static_cast<int>(speedt)));
	}
	else if(motionCmd_.motion == MotionCmd::SpecialAction)
	{
		specialActionRadioButton->setChecked(true);
		walkGroup->setDisabled(true);
	}
}


void MotionEngineWindow::onFowardSliderChanged()
{
		int pos = forwardSlider->sliderPosition();
		double ratio =(forwardValidator->top() -  forwardValidator->bottom())/(forwardSlider->maximum() - forwardSlider->minimum());
		double speedf = (pos -  forwardSlider->minimum()) * ratio + forwardValidator->bottom();
		forwardEdit->setText(QString::number(speedf, 'f',1));
		motionCmd.walkCmd.speed.translation.x = speedf;
		sendtimex=0;//the figure of forward has been changed
}
void MotionEngineWindow::onLeftSliderChanged()
{
	int pos = leftSlider->sliderPosition();
	double ratio =(leftValidator->top() -  leftValidator->bottom())/(leftSlider->maximum() - leftSlider->minimum());
	double speedl = (pos -  leftSlider->minimum()) * ratio + leftValidator->bottom();
	leftEdit->setText(QString::number(speedl, 'f',1));
	motionCmd.walkCmd.speed.translation.y = speedl;
	sendtimey=0;
}
void MotionEngineWindow::onTurnSliderChanged()
{
	int pos = turnSlider->sliderPosition();
	double ratio =(double)(turnValidator->top() -  turnValidator->bottom())/(turnSlider->maximum() - turnSlider->minimum());
	int  speedt=static_cast<int>((pos -  turnSlider->minimum()) * ratio + turnValidator->bottom());
	turnEdit->setText(QString::number(speedt));
	motionCmd.walkCmd.speed.rotation = speedt;
	sendtimez=0;
}
void MotionEngineWindow::onFowardEditChanged()
{
	double speedx = forwardEdit->text().toDouble();
	//forward
	double ratio =(forwardSlider->maximum() - forwardSlider->minimum()) /(forwardValidator->top() -  forwardValidator->bottom());
	int pos = static_cast<int>(ratio * (speedx - forwardValidator->bottom()) + forwardSlider->minimum());
	forwardSlider->setSliderPosition(pos);
	motionCmd.walkCmd.speed.translation.x = speedx;
}
void MotionEngineWindow::onLeftEditChanged()
{
	double speedy= leftEdit->text().toDouble();
	double ratio =(leftSlider->maximum() - leftSlider->minimum())/ (leftValidator->top() -  leftValidator->bottom());
	int pos = static_cast<int>(ratio * (speedy - leftValidator->bottom()) + leftSlider->minimum());
	leftSlider->setSliderPosition(pos);
	motionCmd.walkCmd.speed.translation.y = speedy;
}
void MotionEngineWindow::onTurnEditChanged()
{
	int speedt = turnEdit->text().toDouble();
	double ratio =(double)(turnSlider->maximum() - turnSlider->minimum()) / (turnValidator->top() -  turnValidator->bottom());
	int pos = static_cast<int>(ratio * (speedt - turnValidator->bottom()) + turnSlider->minimum());
	turnSlider->setSliderPosition(pos);
	motionCmd.walkCmd.speed.rotation = speedt;
}
void MotionEngineWindow::onStartActuator(bool checked)
{
	if(checked)
	{
		startActuatorButton->setDisabled(true);
		holdActuatorButton->setDisabled(false);
		stopActuatorButton->setDisabled(false);
		holdActuatorButton->setChecked(false);
		holdActuatorButton->setText("Hold");
		//Only when Actuator Stopped, can user change send state.
		cmdSending2Robot->setDisabled(true);
		jointsSending2Robot->setDisabled(true);
		saveMDebug->setDisabled(true);
		//if(cmdSending2Robot->isChecked() && remoteActuatorConnected){
		//	try{
		//		remoteActuatorProxy->callVoid("startActuator");//->startActuator();
		//	}catch(AL::ALError &e) {
		//		QMessageBox::warning(this,tr("Cmd Error!"),tr("Failed to call Remote startActuator"));
		//	}
		//}
		//else
			actuator.start();
	}
}
void MotionEngineWindow::onHoldActuator(bool checked)
{
	if(checked)
	{
		holdActuatorButton->setText("Continue");
		if(actuator.isRunning()){
			//actuator.hold();
		}
	}else
	{
		holdActuatorButton->setText("Hold");
		//actuator.resume()
	}
}
void MotionEngineWindow::onStopActuator()
{
	if(cmdSending2Robot->isChecked() && remoteActuatorConnected)
	{
		//try{
		//	remoteActuatorProxy->callVoid("stopActuator");//->startActuator();
		//}catch(AL::ALError &e) {
		//	QMessageBox::warning(this,tr("Cmd Error!"),tr("Failed to call Remote stopActuator"));
		//}
	}else
	{
		if(actuator.isRunning())
			actuator.stop();
	}
	
	//save MDebug data
	if(saveMDebug->isChecked())
	{
		QString filename = QFileDialog::getSaveFileName(this,tr("Saving Motion Debugging Data..."),tr("."),tr("MDebugData (*.mdata)"));
// 		int pos = -1;
// 		if( (pos = filename.indexOf(tr("."), -1)) != -1)
// 			filename.insert(pos, tr("_v1"));
		if(!filename.isEmpty())
			MDEBUG_FINISH_SAVE(filename.toLocal8Bit().data());
	}
	startActuatorButton->setChecked(false);
	holdActuatorButton->setChecked(false);
	holdActuatorButton->setDisabled(true);
	startActuatorButton->setDisabled(false);
	holdActuatorButton->setText("Hold");
	//Only when Actuator Stopped, can user change send state.
	cmdSending2Robot->setDisabled(false);
	jointsSending2Robot->setDisabled(false);
	saveMDebug->setDisabled(false);
}
void MotionEngineWindow::onSend()
{
	if(specialActionRadioButton->isChecked()){
		motionCmd.motion = MotionCmd::SpecialAction;
		motionCmd.specActionCmd.specAction = specialComboBox->currentIndex();
	}else{
		motionCmd.motion = MotionCmd::Walk;
	}
	
	sendMotionCmd(motionCmd);
	//SystemCall::sleep(500);
}
void MotionEngineWindow::onStopRobot()
{
	resetWalkCmd();
	sendMotionCmd(motionCmd);
}
void MotionEngineWindow::onEmergency()
{
	actuator.setEmergencyStop();
	onStopActuator();
}

void MotionEngineWindow::sendMotionCmd(MotionCmd& motionCmd_)
{
	
	if (sendtimex==0)// value has been changed, so cm/s => m/s
	{motionCmd_.walkCmd.speed.translation.x /= 100;//cm/s => m/s
	}
	if (sendtimey==0)
	{motionCmd_.walkCmd.speed.translation.y /= 100;
	}
	if (sendtimez==0)
	{motionCmd_.walkCmd.speed.rotation = deg2rad(motionCmd_.walkCmd.speed.rotation);
	}
	if(cmdSending2Robot->isChecked() && remoteActuatorConnected)
	{
		std::string miniType;
		std::vector<float> data;
		data.push_back(motionCmd_.headCmd.pan);
		data.push_back(motionCmd_.headCmd.tilt);
		data.push_back(motionCmd_.headCmd.speed);
		data.push_back(motionCmd_.walkCmd.speed.translation.x);
		data.push_back(motionCmd_.walkCmd.speed.translation.y);
		data.push_back(motionCmd_.walkCmd.speed.rotation);
		//try{
			if(motionCmd_.motion == MotionCmd::Walk || motionCmd_.motion == MotionCmd::Stand)
				miniType = WalkCmd::getWalkTypeName(motionCmd_.walkCmd.walkType);
			else
				miniType = SpecActionCmd::getSpecActionName(motionCmd_.specActionCmd.specAction);

			//remoteActuatorProxy->callVoid< std::string, std::string, std::vector<float> >("setMotionCmd", MotionCmd::getMotionNameByID(motionCmd_.motion), 
			//		miniType, data);//->startActuator();
			
		   // }
		/*catch(AL::ALError &e) {
			QMessageBox::warning(this,tr("Cmd Error!"),tr("Failed to call Remote setMotionCmd"));
		}*/
	}
	else{
		if(actuator.isRunning() /*|| remoteActuatorIsRunning*/)
		{
			selfMessageQueue->SetMessageData(idMotionCmd,idMotionEngineWindow,idActuatorThread,
				sizeof(MotionCmd), (char*)(&motionCmd_));
		}
	}
	sendtimex=1;sendtimey=1;sendtimez=1;
	}

void MotionEngineWindow::onCmdSending(bool checked)
{
	if(checked){
		jointsSending2Robot->setChecked(false);
	}
}
void MotionEngineWindow::onJointsSending(bool checked)
{
	if(checked){
		cmdSending2Robot->setChecked(false);
		actuator.setSendingJoints(true);
	}
	else
		actuator.setSendingJoints(false);
}

void MotionEngineWindow::onSaveMDebug(bool checked)
{
	MDEBUG_OPEN_RECORD(checked);
}