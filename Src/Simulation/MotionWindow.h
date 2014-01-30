#ifndef MOTIONWINDOW_H
#define MOTIONWINDOW_H
#include "SubWindow.h"
#include <QDialog>
#include <QPushButton>
#include "MotionWidget.h"
class QGroupBox;
class QLineEdit;
class MotionWindow :public SubWindow
{
	Q_OBJECT
public:
	MotionWindow(QMdiArea* mdiArea=0);
	void Show();
	void initJointData(SafeJointData theSafeJointData);
signals:
	void SendToMain();
public slots:
	void MotionSeen(QString str);
private:
	MotionGroupBox *motionGroupBox;
	void CreatConnect();
	void CreatLegWidget();
	void CreatHeadWidget();
	QGroupBox *legWidget;
	QGroupBox *headWidget;
	//left leg
	QPushButton *headButton;
	QHBoxLayout * leftwidget;
	QLineEdit *lineedit;
	QWidget *widget;
	QHBoxLayout *legLayout;
	QHBoxLayout *mainLayout;

};

#endif
