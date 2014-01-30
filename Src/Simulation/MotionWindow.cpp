#include "MotionWindow.h"
MotionWindow::MotionWindow(QMdiArea* mdiArea):SubWindow(mdiArea,"Motion")
{
	motionGroupBox=new MotionGroupBox;
	 legLayout=new QHBoxLayout();
	legLayout->addWidget(motionGroupBox);
	mainLayout=new QHBoxLayout();
	widget=new QWidget;
	mainLayout->addLayout(legLayout);
	widget->setLayout(mainLayout);
	setWidget(widget);
	setVisible(true);
	CreatConnect();
	
}
void MotionWindow::CreatConnect()
{
connect(motionGroupBox,SIGNAL(SendToWin()),this,SIGNAL(SendToMain()));	
	
}
void MotionWindow::MotionSeen(QString str)
{
	
	motionGroupBox->MotionSeen(str);
	adjustSize();
}
void MotionWindow::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}

}
void MotionWindow::initJointData(SafeJointData theSafeJointData)
{
	motionGroupBox->initJointData(theSafeJointData);
}