#ifndef __Document_h_
#define __Document_h_
#include <QSettings>
#include <QStringList>
#include <vector>
#include "../SimulationCore/Simulation/APIDatatypes.h"
#include "../SimulationCore/Controller/Controller.h"
#include "Module/Core/Thread.h"
class QMainWindow;
class Simulation;
class Controller;
class Actuator;
class Document:public QObject
{
	Q_OBJECT
public:
	static Document* document;
	Document(QMainWindow* parent, Actuator* actuatorThread, SmartPtr<Synchronizer> _synchro);
	~Document();
	Simulation* getSimulation() const;
	bool isOpened() const;
	void setUpdateViewsDelay(unsigned int ms);
public slots:
	
	void newSlot();
	void closeSlot();
	void openFileSlot(const QString& filePath);
signals:
	void updateViewsSignal();
	void updateObjectWindowSignal();
	void updateActuatorThreadSignal();//signal to run actuator thread....
public slots:
	void openSlot();
	void simResetSlot();
	void simStartSlot();
	void simStepSlot();

	void startRecord();
	void endRecord();
	void saveScreenShot();
	void shootObjectSlot();

private:
	QString file;
	bool opened;
	Simulation* simulation;
	Controller* controller;
	SimulatorGui simulatorGui;
	bool running;
	bool performStep;
	bool updateViews;
	unsigned int lastUpdateViewsTime;
	unsigned int updateViewsDelay;
	QSettings settings;
	ErrorDescription errorDescription;
	bool recording;
	//std::vector<ObjectDescription> objectDescriptionTree;
	void timerEvent(QTimerEvent* event);
	void initDocument();
	void compile();
	void updateObjects();
	SmartPtr<Synchronizer> theSynchronizer;
	Actuator* actuatorThread;
};
#endif