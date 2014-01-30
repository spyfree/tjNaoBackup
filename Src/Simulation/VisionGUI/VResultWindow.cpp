#include "VResultWindow.h"
#include <iostream>
VResultWindow::VResultWindow(QMdiArea *mdiArea,QString nameWindow):SubWindow(mdiArea,nameWindow)
{
   widget = new QWidget;
   layout = new QHBoxLayout;//?? no use
   tableWidget = new QTableWidget(5,5,this);
   layout->addWidget(tableWidget);
   tableWidget->setFixedHeight(240);
   tableWidget->setFixedWidth(420);
   initTable();
   widget->setLayout(layout);
   setWidget(widget);
}

void VResultWindow::initTable()
{
	tableWidget->setColumnCount(4);
	tableWidget->setRowCount(8);
	QStringList headlist;
    headlist<<"Name"<<"X(mm) or Left"<<"Y(mm) or Right"<<"Dist(mm)";
	tableWidget->setHorizontalHeaderLabels(headlist);
 //   QstringList *vertnamelist;
	//vertnamelist<<"Ball"<<"Yellow goal"<<"Cyan goal";
	//tableWidget->setVerticalHeaderLabels(vertnamelist);
}
void VResultWindow::show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}

void VResultWindow::getVisionResult(char *buffer)
{
	//std::cout<<"we are in VResultWindow getVisionResult"<<std::endl;
	netVisionInterface visionResult;
	memcpy(&visionResult,buffer,sizeof(netVisionInterface));

	if (robotViewObjectList.size()>0)
	{
		robotViewObjectList.clear();
	}

    if (visionResult.ballSeen)
    {
		robotViewObject ballView;
		ballView.objectname = "Ball";
		ballView.x = visionResult.ballx/100.0;
		ballView.y = visionResult.bally/100.0;
		ballView.dist = visionResult.balldis/100.0;
		robotViewObjectList.push_back(ballView);
    }
	if (visionResult.ownleftgoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "OwnLeftGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.ownleftGoalX/100.0;
		goalView.y=visionResult.ownleftGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
        robotViewObjectList.push_back(goalView);
	}
	if (visionResult.ownrightgoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "OwnRightGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.ownrightGoalX/100.0;
		goalView.y=visionResult.ownrightGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
		robotViewObjectList.push_back(goalView);
	}
	if (visionResult.unknowowngoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "UnknownOwnGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.unknowownGoalX/100.0;
		goalView.y=visionResult.unknowownGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
		robotViewObjectList.push_back(goalView);
	}
	if (visionResult.oppleftgoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "OppLeftGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.oppleftGoalX/100.0;
		goalView.y=visionResult.oppleftGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
		robotViewObjectList.push_back(goalView);
	}
	if (visionResult.opprightgoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "OppRightGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.opprightGoalX/100.0;
		goalView.y=visionResult.opprightGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
		robotViewObjectList.push_back(goalView);
	}
	if (visionResult.unknowoppgoalSeen)
	{
		robotViewObject goalView;
		goalView.objectname = "UnknownOppGoal";
		//goalYView.x = rad2deg(visionResult.leftGoalY/100.0);
		//goalYView.y = rad2deg(visionResult.rightGoalY/100.0);
		//goalYView.dist = visionResult.distGoalY/100.0;
		goalView.x=visionResult.unknowoppGoalX/100.0;
		goalView.y=visionResult.unknowoppGoalY/100.0;
		goalView.dist=sqrt(goalView.x*goalView.x+goalView.y*goalView.y);
		robotViewObjectList.push_back(goalView);
	}
   if(visionResult.CircleSeen)
   {
	   robotViewObject CircleView;
	   CircleView.objectname="CenterCircle";
	   CircleView.x=visionResult.CircleCenterX/100.0;
	   CircleView.y=visionResult.CircleCenterY/100.0;
	   CircleView.dist=sqrt(CircleView.x*CircleView.x+CircleView.y*CircleView.y);
	   robotViewObjectList.push_back(CircleView);
   }

    tableWidget->setRowCount(0);
	int newRowCount=0;
	newRowCount=robotViewObjectList.size();
    tableWidget->setRowCount(newRowCount);

    const int columnCount = tableWidget->currentColumn();
	for (int i = 0; i < newRowCount; ++i) 
	{

			QTableWidgetItem *item[4];//magic number , means the column number.attention
			item[0] = new QTableWidgetItem;
            item[0]->setText(robotViewObjectList[i].objectname);
			tableWidget->setItem(i,0,item[0]);

			item[1] = new QTableWidgetItem;
			item[1]->setText(QString::number(robotViewObjectList[i].x));
			tableWidget->setItem(i,1,item[1]);

			item[2] = new QTableWidgetItem;
			item[2]->setText(QString::number(robotViewObjectList[i].y));
			tableWidget->setItem(i,2,item[2]);

			item[3] = new QTableWidgetItem;
			item[3]->setText(QString::number(robotViewObjectList[i].dist));
			tableWidget->setItem(i,3,item[3]);
   
	}

}