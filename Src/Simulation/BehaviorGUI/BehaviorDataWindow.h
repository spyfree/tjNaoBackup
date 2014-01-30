#ifndef BehaviorDataWindow_H
#define BehaviorDataWindow_H
#include <QtGui>
#include "Module/Behavior/TJSimRobot.h"
#include "Simulation/SubWindow.h"
#include "Module/Representations//BaseProvider.h"
#include "Tools/Math/Common.h"
#include <Python.h>
class BehaviorDataWindow: public SubWindow,public BaseProvider
{
	Q_OBJECT

public:
	BehaviorDataWindow(TJSimRobot& _tjRobot, QMdiArea* mdiArea = 0);
	~BehaviorDataWindow();
private slots:
	void onStartBehavior(bool checked);
	void onHoldBehavior(bool checked);
	void onStopBehavior();
private:
	void createWindow();
	void createConnect();
	void Show();
private:
	std::string ipAddress;
	std::string naoName;
	TJSimRobot& tjRobot;
	QPushButton* startBehaviorButton;
	QPushButton* holdBehaviorButton;
	QPushButton* stopBehaviorButton;
	QPushButton* getVisionDataButton;
	QPushButton* getMotionDataButton;
	QWidget *widget;
};

#endif