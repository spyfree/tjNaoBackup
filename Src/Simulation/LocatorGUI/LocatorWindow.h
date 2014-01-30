#ifndef LocatorWindow_H
#define LocatorWindow_H

#include "Simulation/SubWindow.h"
#include "Simulation/LocatorGUI/FieldDrawing.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Modeling/SelfLocator/SelfLocator.h"
#include "Module/Modeling/ReadLogData.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"
#include <QtGui>
#include "Tools/Range.h"
class LocatorWindow : public SubWindow,public BaseProvider
{
	Q_OBJECT
public:
	LocatorWindow(QMdiArea *mdiArea = 0);
	~LocatorWindow();
	void Show();
	void getLocResult(char*buffer,int size);
	void drawRobotSamples(Robotsamplebuffer ss);
	void drawBallSamples(Ballsamplebuffer ss);
	void LocatorWindow::drawFreePart(FreePartData freepartdata);
	DrawManager* theDrawManager;
public:
	FieldDrawing* theFieldDrawing;
private slots:
	void updateLocView();
	void speedSliderChanged();
	void getRobotsampleflag(int flag);
	void getBallsampleflag(int flag);
public:
	int speedValue;
private:
	void createWindow();
	void createConnect();
	QWidget *widget;
	QSlider* speedSlider;
	QLineEdit* speedEdit;
	QLabel* speedLabel;
	QTimer *timer;
	bool FlagOfRobotSample;
	bool FlagOfBallSample;
};

#endif