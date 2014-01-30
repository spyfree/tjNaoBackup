#include "TableWindow.h"
TableWindow::TableWindow(QMdiArea *mdiArea,QString nameWindow):SubWindow(mdiArea,nameWindow)
{
	widget=new QWidget;
	layout=new QHBoxLayout;
	checkBoxLay=new QVBoxLayout;
	tableWidget=new QTableWidget(5,5,this);
	layout->addWidget(tableWidget);
	tableWidget->setFixedHeight(700);
	tableWidget->setFixedWidth(230);
	initSensorData();
	layout->addLayout(checkBoxLay);
	widget->setLayout(layout);
	setWidget(widget);
	
}
void TableWindow::initSensorData()
{
	tableWidget->setColumnCount(1);
	tableWidget->setRowCount(JointData::numOfJoints-1);
	QStringList headlist;
	headlist<<"Value";
	QStringList Verlist;
	for(int i=(JointData::HeadYaw);i<JointData::numOfJoints;i++)
	{
	Verlist<<JointData::getJointName(JointData::Joint(i));
	}

	tableWidget->setHorizontalHeaderLabels(headlist);
	tableWidget->setVerticalHeaderLabels(Verlist);
	for(int i=0;i<JointData::numOfJoints-1;i++)
	{
		tableItem[i]=new QTableWidgetItem();
		tableWidget->setItem(i,0,tableItem[i]);
	}
	for(int i=0;i<JointData::numOfJoints-1;i++)
	{
		sensorCheckBox[i]=new QCheckBox();
		checkBoxLay->addWidget(sensorCheckBox[i]);
	}
}

void TableWindow::setSensorData(SafeJointData theSafeJointData)
{
	for(int i=0;i<JointData::numOfJoints-1;i++)
	{

		tableItem[i]->setText(QString::number(theSafeJointData.angles[i+1]*180/3.14159));

	}
}
void TableWindow::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}

}