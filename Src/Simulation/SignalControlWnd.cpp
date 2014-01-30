#include "SignalControlWnd.h"

SignalControlWnd::SignalControlWnd(QMdiArea *mdiArea ):
SubWindow(mdiArea,"SignalControlWnd")
{
	createWindow();
	createConnect();
}

SignalControlWnd::~SignalControlWnd()
{

}

void SignalControlWnd::createWindow()
{
	visionChooseBox = new QComboBox();
	for(int i = 0; i < SignalControl::NumberVisionNames; ++i)
	{
		visionChooseBox->addItem(SignalControl::getInstance()->getVisionName(i));
	}
	visionLabel = new QLabel("visionSignal : ");
	locatorChooseBox = new QComboBox();
	for(int i = 0; i < SignalControl::NumberLocatorNames; ++i)
	{
		locatorChooseBox->addItem(SignalControl::getInstance()->getLocatorName(i));
	}
	locatorLabel = new QLabel("locatorSignal : ");
	actuatorChooseBox = new QComboBox();
	for(int i = 0; i < SignalControl::NumberActuatorNames; ++i)
	{
		actuatorChooseBox->addItem(SignalControl::getInstance()->getActuatorName(i));
	}
	actuatorLabel = new QLabel("actuatorSignal : ");
	QGridLayout* signalLayout = new QGridLayout();
	signalLayout->addWidget(visionLabel,0,0);
	signalLayout->addWidget(visionChooseBox,0,1);
	signalLayout->addWidget(locatorLabel,1,0);
	signalLayout->addWidget(locatorChooseBox,1,1);
	signalLayout->addWidget(actuatorLabel,2,0);
	signalLayout->addWidget(actuatorChooseBox,2,1);
	sendButton = new QPushButton(tr("Send"));
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(signalLayout);
	mainLayout->addWidget(sendButton);
	widget = new QWidget();
	widget->setLayout(mainLayout);
	setWidget(widget);
	setVisible(true);
}

void SignalControlWnd::createConnect()
{
	connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(onSend()));
}

void SignalControlWnd::onSend()
{
	SignalControl::getInstance()->visionSignal = visionChooseBox->currentIndex();
	SignalControl::getInstance()->locatorSignal = locatorChooseBox->currentIndex();
	SignalControl::getInstance()->actuatorSignal = actuatorChooseBox->currentIndex();
	if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
		emit updateBehaviorThread();
	sendButton->setText("NextSend");
}