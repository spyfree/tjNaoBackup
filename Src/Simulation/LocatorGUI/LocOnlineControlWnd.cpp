#include "LocOnlineControlWnd.h"

LocOnlineControlWnd::LocOnlineControlWnd(QMdiArea *mdiArea ):
SubWindow(mdiArea,"LocOnlineControlWnd")
{
	createWindow();
	createConnect();
}

LocOnlineControlWnd::~LocOnlineControlWnd()
{

}

void LocOnlineControlWnd::createWindow()
{
	RobotSamples = new QCheckBox(tr("DrawRobotSamples?"), this);
	RobotSamples->setCheckState(Qt::Unchecked);
	BallSamples = new QCheckBox(tr("DrawBallSamples?"), this);
	BallSamples->setCheckState(Qt::Unchecked);
	startLoc= new QPushButton(tr("StartLoc"),this);
	stopLoc=new QPushButton(tr("StopLoc"),this);
	QVBoxLayout* vlayout1=new QVBoxLayout();
	vlayout1->addWidget(RobotSamples);
	vlayout1->addWidget(BallSamples);
	QVBoxLayout* vlayout2=new QVBoxLayout();
	vlayout2->addWidget(startLoc);
	vlayout2->addWidget(stopLoc);
	QHBoxLayout* hlayout=new QHBoxLayout();
	hlayout->addLayout(vlayout1);
	hlayout->addLayout(vlayout2);
	widget = new QWidget();
	widget->setLayout(hlayout);
	setWidget(widget);
	setVisible(true);
}

void LocOnlineControlWnd::createConnect()
{
	connect(startLoc, SIGNAL(clicked()), this, SIGNAL(StartSendLocSample()));
	connect(stopLoc,SIGNAL(clicked()),this,SIGNAL( StopSendLocSample()));
	connect(RobotSamples,SIGNAL(stateChanged(int)),this,SIGNAL( RobotSampleTodraw(int)));
	connect(BallSamples,SIGNAL(stateChanged(int)),this,SIGNAL(BallSampleTodraw(int)));
}
void LocOnlineControlWnd::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}
