#ifndef TableWindow_H
#define TableWindow_H
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QString>
#include <QCheckBox>
#include "Module/Common/JointData.h"
#include "SubWindow.h"
class TableWindow:public SubWindow
{
public:	
	TableWindow(QMdiArea *mdiArea,QString nameWindow="Sensor");
	void Show();
	void setSensorData(SafeJointData theSafeJointData);
private:
	void initSensorData();
	QWidget *widget;
	QTableWidget* tableWidget;
	QHBoxLayout *layout;
	QVBoxLayout *checkBoxLay;
	QTableWidgetItem *tableItem[JointData::numOfJoints];
	QCheckBox *sensorCheckBox[JointData::numOfJoints];

};
#endif