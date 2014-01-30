#include "BehaviorDataWindow.h"
#include "Platform/SystemCall.h"
#include "Simulation/SignalUnconflict.h"
#include <math.h>

BehaviorDataWindow::BehaviorDataWindow(TJSimRobot& _tjRobot,QMdiArea* mdiArea)
:SubWindow(mdiArea,"BehaviorData"),tjRobot(_tjRobot),
naoName("Nao")
{
	createWindow();
	createConnect();
	startBehaviorButton->setDisabled(false);
	holdBehaviorButton->setDisabled(true);
	stopBehaviorButton->setDisabled(false);
}

BehaviorDataWindow::~BehaviorDataWindow()
{

}

void BehaviorDataWindow::createWindow()
{
	startBehaviorButton = new QPushButton(QIcon(":/icons/media-play.png"), tr("Start"));
	startBehaviorButton->setCheckable(true);
	holdBehaviorButton = new QPushButton(QIcon(":/icons/media-pause.png"), tr("Hold"));
	holdBehaviorButton->setCheckable(true);
	stopBehaviorButton = new QPushButton(QIcon(":/icons/media-stop.png"), tr("Stop"));
	QVBoxLayout* leftLayout = new QVBoxLayout();
	leftLayout->addWidget(startBehaviorButton);
	leftLayout->addWidget(holdBehaviorButton);
	leftLayout->addWidget(stopBehaviorButton);
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addLayout(leftLayout);
	widget = new QWidget;
	widget->setLayout(mainLayout);
	setWidget(widget);
	setFixedSize(sizeHint());
	setVisible(true);
}

void BehaviorDataWindow::createConnect()
{
	connect(startBehaviorButton,SIGNAL(toggled(bool)),this,SLOT(onStartBehavior(bool)));
	connect(holdBehaviorButton,SIGNAL(toggled(bool)),this,SLOT(onHoldBehavior(bool)));
	connect(stopBehaviorButton,SIGNAL(clicked(bool)),this,SLOT(onStopBehavior()));
}

void BehaviorDataWindow::Show()
{
	if(!isVisible())
	{
		setVisible(!isVisible());
		widget->show();
	}
}

void BehaviorDataWindow::onStartBehavior(bool checked)
{
	if(checked)
	{
		startBehaviorButton->setDisabled(true);
		holdBehaviorButton->setDisabled(true);
		stopBehaviorButton->setDisabled(false);
		holdBehaviorButton->setChecked(false);
		holdBehaviorButton->setText("Hold");
		tjRobot.start();
	}		
}

void BehaviorDataWindow::onHoldBehavior(bool checked)
{
	if(checked)
	{
		holdBehaviorButton->setText("Continue");
	}
	else
	{
		holdBehaviorButton->setText("Hold");
	}
}

void BehaviorDataWindow::onStopBehavior()
{
	if(tjRobot.isRunning())
	{
		tjRobot.stop();
	}
	startBehaviorButton->setChecked(false);
	holdBehaviorButton->setChecked(false);
	holdBehaviorButton->setDisabled(true);
	startBehaviorButton->setDisabled(true);
	stopBehaviorButton->setDisabled(true);
	holdBehaviorButton->setText("Hold");
}