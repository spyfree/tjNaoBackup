#ifndef SignalControlWnd_H
#define SignalControlWnd_H
#include <QComboBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "Simulation/SubWindow.h"
#include "SignalControl.h"

class SignalControlWnd: public SubWindow
{
	Q_OBJECT
public:
	SignalControlWnd(QMdiArea* mdiArea = 0);
	~SignalControlWnd();
private:
	void createWindow();
	void createConnect();
signals:
	void updateBehaviorThread();
private slots:
	void onSend();
private:
	QComboBox* visionChooseBox;
	QLabel* visionLabel;
	QComboBox* locatorChooseBox;
	QLabel* locatorLabel;
	QComboBox* actuatorChooseBox;
	QLabel* actuatorLabel;
	QPushButton* sendButton;
	QWidget* widget;
};
#endif