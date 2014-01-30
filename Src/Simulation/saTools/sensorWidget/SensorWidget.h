#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H
//----
#include <QWidget>
#include "../jointsData.h"
#include "SensorDataModel.h"
//----
class QGroupBox;
class QTreeView;
class QStandardItemModel;
class QSortFilterProxyModel;
class QTimer;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QSplitter;
//========Class
class SensorWidget : public QWidget
{
	Q_OBJECT

		private slots:
			void freshData();
			void updateTimerInterval(int);
			void autoFreshSwitch(int);
			void startFlagSwitch(int);
			void entityFlagSwitch(int);
			//--Drag Plane Buttons
			signals:
				void dragPlaneChangedSignal(int);
			private slots:
				void xyDragPlaneButton();
				void xzDragPlaneButton();
				void yzDragPlaneButton();
	

public:
	SensorWidget(QWidget *parent );
	~SensorWidget();

public:
	void setJointModel(QStandardItemModel *model);
	void setNaoSensorModel(QStandardItemModel *model);

private:
	void createView();
	void createDragPlaneButtons();

private:	//dragPlane Buttons
	QPushButton  *  xyPushbutton;
	QPushButton  *  xzPushbutton;
	QPushButton  *  yzPushbutton;

private:
	QGroupBox * sensorGroupBox;
	QTreeView * sourceView;
	QTreeView * proxyView;
	QSortFilterProxyModel * proxySortFilterModel;

	QTimer * freshDataTimer;
	int timerIntervalMsec;
	QSplitter * mainSplitter;
	QGroupBox * controlGroupBox;
	QSpinBox * timerIntervalSpinBox;
	QCheckBox * autoFreshCheckBox;
	QCheckBox * sensorWidgetStartCheckBox;
	bool startFlag;
	QPushButton * manualFreshPushButton;
	QCheckBox * getEntityDataCheckBox;
	bool entityFlag;

	SensorDataModel  mySensorModel;
};

#endif // SENSORWIDGET_H
