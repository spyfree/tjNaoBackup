#ifndef LocOnlineControlWnd_H
#define LocOnlineControlWnd_H
#include <QCheckBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>t
#include <QPushButton>
#include <QLabel>

#include "Simulation/SubWindow.h"

class LocOnlineControlWnd: public SubWindow
{
	Q_OBJECT
public:
	LocOnlineControlWnd(QMdiArea* mdiArea = 0);
	~LocOnlineControlWnd();
	void Show();
private:
	void createWindow();
	void createConnect();
	
signals:
	/*void startOnLineLoc();
	void stopOnLineLoc();*/
	void RobotSampleTodraw(int flag);
	void BallSampleTodraw(int flag);
	void StartSendLocSample();
	void StopSendLocSample();
private:
	QCheckBox* RobotSamples;
	QCheckBox* BallSamples;
    QWidget* widget;
public:
	QPushButton* startLoc;
	QPushButton* stopLoc;
};
#endif




