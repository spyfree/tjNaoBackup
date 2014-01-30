#ifndef VRESULTWINDOW_H
#define  VRESULTWINDOW_H
#include "../SubWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QVector>
#include <QStringList>
//#include "Module/Vision/GoalSpecialist.h"
#include "Module/Representations/Perception/VisionInterface.h"
#include "Tools/Math/Common.h"
//class robotViewObject
//{
//public:
//	QString objectname;
//	double x;
//	double y;
//	double dist;
//};
class robotViewObject
{
public:
	QString objectname;
	double x;
	double y;
	double dist;
};
class VResultWindow: public SubWindow
{
	//Q_OBJECT;
public:
	VResultWindow(QMdiArea *mdiArea,QString nameWindow="Vision Result ");
	QVector<robotViewObject> robotViewObjectList;
	void getVisionResult(char *buffer);
	void show();
private:
    void initTable();
//using the table method
    QWidget *widget;
	QTableWidget* tableWidget;
	QHBoxLayout *layout;

};

#endif